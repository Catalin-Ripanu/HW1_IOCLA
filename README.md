# HW1_IOCLA

Minimal Linux File System Manager written in C.

## Implementation

### Day 1
- Implemented basic functions (init, touch, mkdir)
- Used `while(1)` and `fgets` to obtain data
- Used `strtok` to store command arguments in a `char*`
- Implemented 'ls' and 'stop' commands

### Day 2
- Implemented rm, rmdir, and pwd operations
- Encountered most obstacles on this day
- Deletion operations generated many errors due to traversing `head_children_files` and `head_children_dirs` lists
- Spent several hours debugging
- Used valgrind and GDB (Valgrind reported the most errors)

## Main Idea

The designed program follows this hierarchy:

```
                 ....             ........
                  ^                ^   ^
                   \               |  /
                   d1_1 d2_1      f3_1
                     ^  ^         ^
                      \ |        /
                       d1   d2  d3   f1  f2
                        ^    ^   ^   ^   ^
                         \   |   |   |  /
                           home(directory)
```

Represented as:

```
                   |d1_1   
                |d1|d2_1
            home|d2
                |d3|f3_1
                |f1
                |f2
```

## Data Structure Meanings

### struct Dir
- `next`: Move to the neighboring directory
- `head_children_dirs`: First element in the list of subdirectories
- `parent`: Parent of subdirectories (home directory is the root)

Example:
- In directory d1_1:
  - `d1_1->next` leads to d2_1
  - `d1_1->parent` is d1
  - `d1->head_children_dirs` is d1_1

### struct File
- `next`: Move to the neighboring file
- `parent`: Parent of files in the current directory

Example:
- When pwd returns /home:
  - `f1->parent` is home
  - `f1->next` is f2

## Implementation Details

### touch and mkdir
- Created `create_condition` function to check for file/directory existence
- `search` traverses file list, `find` traverses subdirectory list
- Returns 1 only if file/directory not found in lists
- Allocates memory and adds to end of respective list using while() traversal

### ls
- Displays subdirectories list first, then files list
- Uses `aux1_helper` for subdirectories and `aux2_helper` for files
- Checks for end of lists with `if(...!=NULL)`

### cd
- Verifies existence of target directory
- Uses sequences `*target = find` or `*target = (*target)->head_children_dirs`
- For "..", changes current directory to its parent

### stop
- Uses recursion to deallocate memory
- Takes root of hierarchy (home directory) as argument
- `aux1_helper` frees memory for file list in a subdirectory
- `aux2_helper` uses recursion to traverse subdirectories

### rm and rmdir
- Traverses respective lists to find and delete items
- `rmdir` calls `stop` function if directory contains subdirectories/files

### pwd
- Uses helper variable to build path string
- Traverses up to root, concatenating directory names

### tree
- Implemented recursively
- Uses `aux_iterator` to move towards last-level subdirectory
- Displays tree form using `printf("%*s\n",4*level+strlen(string),string)`

### mv
- Checks conditions: oldname must exist, newname must not
- Moves file/directory to end of list under specified conditions
- Handles all cases: item at beginning, middle, or end of list

## Main Function
- Uses `fgets` to read input line
- Uses `strtok` to obtain command arguments
- Uses `strcspn` to handle newline character from `fgets`
- Implements command execution in a `while(1)` loop
- Program ends on 'stop' command
