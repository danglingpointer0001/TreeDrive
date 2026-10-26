#include <iostream>
#include <string>
using namespace std;

enum EntryType
{
    FOLDER,
    TEXT_FILE
};

struct Entry
{
    int id;
    string name;
    EntryType type;

    Entry* parent;
    Entry* firstChild;
    Entry* nextSibling;
};

struct RecentNode
{
    string fileName;
    string filePath;

    RecentNode* prev;
    RecentNode* next;
};

Entry* root = nullptr;
Entry* currentDir = nullptr;
RecentNode* recentHead = nullptr;
RecentNode* recentTail = nullptr;

int nextId = 1;
int recentCount = 0;
const int RECENT_LIMIT = 10;

Entry* makeEntry(string name, EntryType type, Entry* parent)
{
    Entry* item = new Entry;

    item->id = nextId++;
    item->name = name;
    item->type = type;
    item->parent = parent;
    item->firstChild = nullptr;
    item->nextSibling = nullptr;

    return item;
}

Entry* findChild(Entry* folder, string name)
{
    Entry* temp = folder->firstChild;

    while (temp != nullptr)
    {
        if (temp->name == name)
            return temp;

        temp = temp->nextSibling;
    }

    return nullptr;
}

void addEntry(Entry* folder, string name, EntryType type)
{
    if (name.empty())
    {
        cout << "Name cannot be empty.\n";
        return;
    }

    if (name.find('/') != string::npos)
    {
        cout << "Name cannot contain '/'.\n";
        return;
    }

    if (findChild(folder, name) != nullptr)
    {
        cout << "An entry with that name already exists here.\n";
        return;
    }

    Entry* item = makeEntry(name, type, folder);

    if (folder->firstChild == nullptr)
    {
        folder->firstChild = item;
    }
    else
    {
        Entry* temp = folder->firstChild;

        while (temp->nextSibling != nullptr)
            temp = temp->nextSibling;

        temp->nextSibling = item;
    }

    cout << (type == FOLDER ? "Folder" : "Text file")
         << " created: " << name << "\n";
}

string getPath(Entry* item)
{
    if (item == root)
        return "/";

    string path = "";
    Entry* temp = item;

    while (temp != nullptr && temp != root)
    {
        path = "/" + temp->name + path;
        temp = temp->parent;
    }

    return path;
}

void displayTree(Entry* folder, int depth)
{
    Entry* temp = folder->firstChild;

    while (temp != nullptr)
    {
        for (int i = 0; i < depth; i++)
            cout << "  ";

        if (temp->type == FOLDER)
            cout << "[Folder] ";
        else
            cout << "[File]   ";

        cout << temp->name << " (ID: " << temp->id << ")\n";

        if (temp->type == FOLDER)
            displayTree(temp, depth + 1);

        temp = temp->nextSibling;
    }
}

void addToRecent(string fileName, string filePath)
{
    // If this file is already in the list, remove its old node first.
    RecentNode* temp = recentHead;

    while (temp != nullptr)
    {
        if (temp->filePath == filePath)
        {
            if (temp->prev != nullptr)
                temp->prev->next = temp->next;
            else
                recentHead = temp->next;

            if (temp->next != nullptr)
                temp->next->prev = temp->prev;
            else
                recentTail = temp->prev;

            delete temp;
            recentCount--;
            break;
        }

        temp = temp->next;
    }

    RecentNode* node = new RecentNode;
    node->fileName = fileName;
    node->filePath = filePath;
    node->prev = nullptr;
    node->next = recentHead;

    if (recentHead != nullptr)
        recentHead->prev = node;
    else
        recentTail = node;

    recentHead = node;
    recentCount++;

    // Keep only the 10 most recently opened files.
    if (recentCount > RECENT_LIMIT)
    {
        RecentNode* oldTail = recentTail;
        recentTail = recentTail->prev;
        recentTail->next = nullptr;
        delete oldTail;
        recentCount--;
    }
}

void showRecentFiles()
{
    if (recentHead == nullptr)
    {
        cout << "No files have been opened yet.\n";
        return;
    }

    cout << "\nRecently opened files (newest first):\n";

    RecentNode* temp = recentHead;
    int number = 1;

    while (temp != nullptr)
    {
        cout << number << ". " << temp->filePath << "\n";
        temp = temp->next;
        number++;
    }
}

void openTextFile()
{
    string name;

    cout << "Enter text file name in the current folder: ";
    getline(cin, name);

    Entry* item = findChild(currentDir, name);

    if (item == nullptr)
    {
        cout << "No entry with that name exists in this folder.\n";
        return;
    }

    if (item->type != TEXT_FILE)
    {
        cout << "That entry is a folder, not a text file.\n";
        return;
    }

    string path = getPath(item);
    addToRecent(item->name, path);

    cout << "Opened text file: " << path << "\n";
    cout << "(This version tracks file opening; file contents are not stored yet.)\n";
}

void showCurrentFolder()
{
    cout << "\nCurrent path: " << getPath(currentDir) << "\n";

    if (currentDir->firstChild == nullptr)
    {
        cout << "This folder is empty.\n";
        return;
    }

    cout << "Contents:\n";
    Entry* temp = currentDir->firstChild;

    while (temp != nullptr)
    {
        cout << (temp->type == FOLDER ? "[Folder] " : "[File]   ")
             << temp->name << "\n";
        temp = temp->nextSibling;
    }
}

void freeTree(Entry* folder)
{
    Entry* temp = folder->firstChild;

    while (temp != nullptr)
    {
        Entry* next = temp->nextSibling;

        if (temp->type == FOLDER)
            freeTree(temp);

        delete temp;
        temp = next;
    }

    folder->firstChild = nullptr;
}

void freeRecentList()
{
    while (recentHead != nullptr)
    {
        RecentNode* temp = recentHead;
        recentHead = recentHead->next;
        delete temp;
    }

    recentTail = nullptr;
    recentCount = 0;
}

void showMenu()
{
    cout << "\n========== TreeDrive: Mini File System ==========\n";
    cout << "Current directory: " << getPath(currentDir) << "\n";
    cout << "1. Create folder\n";
    cout << "2. Create text file\n";
    cout << "3. Display full directory tree\n";
    cout << "4. Enter a folder\n";
    cout << "5. Go to parent folder\n";
    cout << "6. List current folder\n";
    cout << "7. Open a text file (add to recent list)\n";
    cout << "8. Show recently opened files\n";
    cout << "0. Exit\n";
    cout << "Choose: ";
}

int main()
{
    root = new Entry;
    root->id = 0;
    root->name = "/";
    root->type = FOLDER;
    root->parent = nullptr;
    root->firstChild = nullptr;
    root->nextSibling = nullptr;

    currentDir = root;

    int choice;

    do
    {
        showMenu();

        if (!(cin >> choice))
        {
            cout << "\nInvalid input. Exiting.\n";
            break;
        }

        cin.ignore(10000, '\n');

        string name;

        switch (choice)
        {
            case 1:
                cout << "Folder name: ";
                getline(cin, name);
                addEntry(currentDir, name, FOLDER);
                break;

            case 2:
                cout << "Text file name (for example, notes.txt): ";
                getline(cin, name);
                addEntry(currentDir, name, TEXT_FILE);
                break;

            case 3:
                cout << "\nDirectory tree rooted at /:\n";
                cout << "/\n";
                displayTree(root, 1);
                break;

            case 4:
                cout << "Folder name to enter: ";
                getline(cin, name);
                {
                    Entry* item = findChild(currentDir, name);

                    if (item == nullptr)
                        cout << "Folder not found in the current directory.\n";
                    else if (item->type != FOLDER)
                        cout << "That entry is a file, not a folder.\n";
                    else
                    {
                        currentDir = item;
                        cout << "Changed directory to " << getPath(currentDir) << "\n";
                    }
                }
                break;

            case 5:
                if (currentDir == root)
                    cout << "You are already at the root directory.\n";
                else
                {
                    currentDir = currentDir->parent;
                    cout << "Changed directory to " << getPath(currentDir) << "\n";
                }
                break;

            case 6:
                showCurrentFolder();
                break;

            case 7:
                openTextFile();
                break;

            case 8:
                showRecentFiles();
                break;

            case 0:
                cout << "Closing TreeDrive...\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    freeRecentList();
    freeTree(root);
    delete root;

    return 0;
}
