## ObsidianSync
Synchronizer with remote repository for Obsidian.
 
![Qt6.5.3](https://img.shields.io/badge/Qt-6.5.3-green.svg) 
![Cpp]( https://img.shields.io/badge/-C++-blue?logo=cplusplus)  

The program automatically sends data to the specified folder every 5 minutes.

### Program
When you launch it for the first time, the program opens in this form;

![prog](https://github.com/Jomart02/ObsidianSync/blob/master/gitMd/program.png)

#### Input:
 - Path - path to the folder that will be synchronized;
 - Remote link - link to remote Git repository;
 - Branch - branch in the repository (master by default).

#### Buttons:
  -  Obsidian path - opens the menu for selecting the desired folder;
  -  StartSync - starts synchronization;
  -  StopSync - stops synchronization.

#### StartSync

When synchronization starts, the local Git repository is initialized and linked to the remote, .gitignore is added.

The program goes to the tray and is added to autorun. Stop button removes from tray and autorun.

To store settings in the user's root folder, a syncObsidian folder is created; if you want to create a new synchronizer, you need to delete or clear the files in this folder

### Building
```bash
  #win/linux

  git clone https://github.com/Jomart02/ObsidianSync
  cd ObsidianSync
  mkdir build && cd build 
  cmake .. && cmake --build .
```

### Roadmap
- Add file input for .gitignore;
- Add the ability to add several synchronization tasks on one PC (settings must be entered differently so that data does not overlap).
