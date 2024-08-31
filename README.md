# HW1_IOCLA

Minimal Linux File System Manager written in C.

## Implementation
On the first day, I implemented the basic functions (init, touch, mkdir).
I used while(1) and fgets to obtain data. I also used strtok to store the command's argument(s) in a char*.
Also on the first day, I implemented the 'ls' and 'stop' commands.
On the second day, I performed the rm, rmdir, and pwd operations.
I encountered the most obstacles on this day.
The deletion operations generated many errors because it was necessary to traverse the head_children_files and head_children_dirs lists.
I spent several hours debugging.
I used valgrind and GDB. Valgrind reported the most errors.
The main idea of the designed program is:
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

I represented the following hierarchy:
```
                                      |d1_1   
                                   |d1|d2_1
                               home|d2
                                   |d3|f3_1
                                   |f1
                                   |f2
```
Meanings of the following fields:
From struct Dir:
next -> move to the neighboring directory
head_children_dirs -> the first element in the list of subdirectories. 
parent -> the parent of subdirectories. Note that the home directory is the parent/ancestor (root).
Example: I'm in directory d1_1
I will reach, after the operation d1_1->next, d2_1.
d1_1->parent is d1.
d1->head_children_dirs is d1_1.
From struct File:
next -> move to the neighboring file.
parent -> the parent of files in the current directory.
Example: pwd returns /home
f1->parent is home.
f1->next is f2.

## Ideas:
- For the touch and mkdir functions, a create_condition function was created to check for the existence of a file/directory in the subdirectory and file lists.
- The idea is simple, search traverses the file list, and find traverses the subdirectory list. The function returns 1 only if the file/directory is not found in the lists.
- The touch function allocates memory and adds a file to the end of the file list using a while() traversal.
- The mkdir function allocates memory and adds a directory to the end of the subdirectory list using a while() traversal.
- The ls function displays the list of subdirectories and the list of files, in this order.
- The idea is simple, aux1_helper traverses and displays the subdirectory list using a while(), similarly with aux2_helper (for the file list).
- Obviously, I make sure I haven't reached the end of the lists with an if(...!=NULL).
- The implementation of the cd function was a bit more delicate since the first argument was a Dir**
- The basic idea is to verify the existence of the directory to which the move is desired.
- A traversal is performed, if the element was found, then the sequences *target = find/*target = (*target)->head_children_dirs are executed.
- These sequences are executed because the address of the address needs to be changed.
- Obviously, if the string ".." is entered, then the current directory must be changed to its parent, no other traversal is necessary.
- Recursion was used to implement the stop function.
- Stop will deallocate memory 
- This function takes the root of the hierarchy (the home directory) as an argument.
- The aux1_helper variable frees all the memory allocated to the file list in a subdirectory.
- The interesting part is at aux2_helper because recursion intervenes.
- The list of subdirectories in the parent directory is traversed.
- If the current subdirectory contains other things (subdirectories, files), stop is called recursively.

## A helpful scheme:
Consider the following structure
Let dir_current be the directory where we are. 

```   
(after recursive call) dir_current -> d1_1 d2_1      f3_1
                                         ^  ^         ^
                                          \ |        /
                                           d1   d2  d3   f1  f2
                                            ^    ^   ^   ^   ^
                                             \   |   |   |  /
                                            home(directory) <- dir_current(when the stop function is called for the first time)
```
                                            
At the end of the function, the memory allocated to the root is deallocated.
The rm function is simple, the file list is traversed with the search variable. If strcmp(search,name) returns 0, the memory allocated to the searched file is freed.
Obviously, the counter variable receives the value 1 if the deletion was performed.

The rmdir function presents the same mechanism, except that the stop function is called if the respective directory contains other subdirectories and files.
I used a helper char* variable (helper). To get something of the form /home/dir1/dir2/..., the target parameter must reach the root (home).

Consider the following example
We are in the dir3 directory.
pwd should return the following string "/home/dir1/dir2/dir3".
target->name is "dir3".
At the first iteration:
helper -> "/dir3"
result -> "/dir3" (after executing strcpy(..,strcat(..)) ).
target->name is now "dir2"
At the second iteration
helper -> "/dir2"
result -> "/dir2/dir3" (after executing strcpy(..,strcat(..)) ).
target->name is now "dir1"

Finally, the allocated memory is freed and the string "/home/dir1/dir2/dir3" is obtained.
I implemented the tree function recursively.
A move is made towards the subdirectory on the last level using the aux_iterator variable.
To respect the condition in the requirement, the tree form is displayed using printf("%*s\n",4*level+strlen(string),string).

The same reasoning applies in the case of files.
Obviously, in the first part of the function, the conditions from the requirement are checked (oldname must exist, newname must not). I used condition_(something) variables.
Also, the condition_file_oldname and condition_directory_oldname variables cannot have the value 1 at the same time since touch and mkdir do not present errors.
In the second part, the file/directory is moved to the end of the list, under the mentioned conditions.

Helper_dir holds the address of the directory with the name oldname. Also, helper_file holds the address of the file with the name oldname.
All cases were treated using structures of the form if..else..else if.
The first case represents the presence of the directory/file with the name oldname at the beginning of the list.
The second case represents the presence of the directory/file with the name oldname in the list (not at the beginning/end).
The third case represents the presence of the directory/file with the name oldname at the end of the list.
If the conditions are not met, error messages will be displayed.
In the main function, fgets was used to read the line.

Also, strtok was used to obtain the argument.
The strcspn function was used because fgets adds a '\n' at the end.
The structure of the main function is simple, decision instructions were used in a while(1) repetitive instruction to be able to execute the commands.
The program ends when the stop command is entered.
