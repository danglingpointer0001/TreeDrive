# TreeDrive
TreeDrive is actually a mini OS which will be implemeted in C++ showcasting the in depth core use of LLs and tree DSs

# TreeDrive — Mini File System

A small in-memory C++ file-system simulator built around data structures from the Data Structures course.

## Implemented features (3 core features) //temporary features

1. Tree feature — Create folders and text-file entries.- Entries are stored in a general directory tree using `firstChild`, `nextSibling`, and `parent` pointers.
2. Tree feature — Display the directory hierarchy.- A recursive traversal displays folders and files with indentation.
3. Doubly linked list feature — Recently opened files.** Opening a text-file entry moves it to the front of a doubly linked list. The list shows newest-first and keeps at most 10 entries.

Supporting menu operations are included so the three core features can be demonstrated: entering a folder, going to its parent, and listing the current folder.

## Important scope note

This is an -in-memory simulation**. It does not create real files on your computer, store file contents, or preserve the virtual directory tree after the program exits. Those can be future extensions.

## Compile and run (VS Code terminal, Windows PowerShell)

If your compiler is GCC/MinGW, the same source can also be compiled with warnings enabled:



## Suggested demo

1. Choose `1`, create folder `Documents`.
2. Choose `4`, enter `Documents`.
3. Choose `2`, create text file `notes.txt`.
4. Choose `7`, open `notes.txt`.
5. Choose `8`, show recently opened files.
6. Choose `5` to return to root.
7. Choose `1` and create `Projects`.
8. Choose `3` to display the full tree.

## Data structures used

- General tree: Each entry has `firstChild` (first child), `nextSibling` (next entry at the same level), and `parent` (containing folder).
- Doubly linked list: Each recent-file node has `prev` and `next`, allowing removal of an older occurrence and insertion at the front.
- Dynamic memory: Entries and recent-file nodes are allocated dynamically and released on exit.

## Limitations / future work

- File contents and persistence are not implemented.
- No delete/recycle-bin feature in this evaluation version.
- Recent-file list resets when the program exits.

