# OS Lab 1 Nathaniel O'Brien

After extracting the zip, run these commands inside of the extracted folder to run the program.

```
make
./a.out
```

## How to use

On program startup, you can instantly start typing commands.
When you press enter, the command will be executed if correctly input. You can enter commands by closely following the syntax provided below. After each command is completed or errors, you will be prompted to enter a new command. The loop continues until you exit out of the program using the Q or ^C commands.
The commands include:

### Copy

Syntax:
```
C file1 file2
```

Description: Copies the contents of file1 into a new file, file2 inside the current directory.

### Delete

Syntax:
```
D file
```

Description: Deletes the specified file inside the current directory.

### Echo

Syntax:
```
E comment
```

Description: Prints the specified comment. 

### Help

Syntax:
```
H
```

Description: Lists every command with syntax and description.

### List

Syntax:
```
L
```

Description: Lists the directory you are currently inside of, and all of the contents inside current directory.

### Make

Syntax:
```
M file
```

Description: Creates a new file with the specified file name and opens a text editor with the new file.

### Print

Syntax:
```
P file
```

Description: Prints all contents the of specified file name.

### Quit

Syntax:
```
Q
```

Description: Quits the terminal and terminates the program.

### Wipe

Syntax:
```
W
```

Description: Clears the screen in the command prompt.

### Execute

Syntax:
```
X program arguments
```

Description: Executes the specified program and passes the arguments to it if wanted.

## References
* https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-getenv-get-value-environment-variables
* https://www.programiz.com/c-programming/library-function/string.h/strcmp
* https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus