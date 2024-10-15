// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed: Nathaniel O'Brien     Date: 10/14/22

// 3460:426 Lab 1 - Basic C shell rev. 9/10/2020

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

#define MAX_ARGS 64
#define MAX_ARG_LEN 16
#define MAX_LINE_LEN 80
#define WHITESPACE " ,\t\n"

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
int interpretCommand(struct command_t *);

int main(int argc, char *argv[]) {
   int pid;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;

   while (1)
   {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
      parseCommand(cmdLine, &command);
      command.argv[command.argc] = NULL;

      // If the command is Q, exit the shell
      if (strcmp(command.name, "Q") == 0)
      {
         break;
      }

      /* Create a child process to execute the command */
      if ((pid = fork()) == 0) {
         /* Child executing command */

         // See if it is a recognized command, if not, just use execvp
         int err = interpretCommand(&command);
         if (err)
         {
            int e = execvp(command.name, command.argv);
            if (e != 0)
            {
               printf("\033[0;31m"); // Set the text color to red
               printf("Error running command. Error code %i\n", err);
               printf("\033[0m"); // Reset the text color to default
            }
         }
      }
      /* Wait for the child to terminate */
      wait(&status); /* EDIT THIS LINE */
   }

   /* Shell termination */
   printf("\n\n shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);
   return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - Nutt pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */

   char *user = getenv("USER"); // Get uanet ID from env $USER variable
   printf("linux (%s) | > ", user);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */

int interpretCommand(struct command_t *cmd)
{
   int err = 0;
   if (strcmp(cmd->name, "C") == 0)
   {
      // Attempt to run copy command
      if (cmd->argc == 3)
      {
         // Copy file if correct num args
         char *argv[] = {"cp", cmd->argv[1], cmd->argv[2], NULL};
         err = execvp("cp", argv);
      }
      else
      {
         // Show error message if wrong num args
         printf("\033[0;31m"); // Set the text color to red
         printf("ERROR:\tIncorrect number of arguments.\nSyntax:\tC file1 file2\n");
         printf("\033[0m"); // Reset the text color to default
      }
   }
   else if (strcmp(cmd->name, "D") == 0)
   {
      // Attempt to run delete command
      if (cmd->argc == 2)
      {
         // Delete file if corrent num args
         char *argv[] = {"rm", cmd->argv[1], NULL};
         err = execvp("rm", argv);
      }
      else
      {
         // Show error message is wrong num args
         printf("\033[0;31m"); // Set the text color to red
         printf("ERROR:\tIncorrect number of arguments.\nSyntax:\tD file\n");
         printf("\033[0m"); // Reset the text color to default
      }
   }
   else if (strcmp(cmd->name, "E") == 0)
   {
      // Echo the comment (all other args except 0)
      for (int i = 1; i < cmd->argc; i++)
      {
         printf("%s ", cmd->argv[i]);
      }
      printf("\n");
   }
   else if (strcmp(cmd->name, "H") == 0)
   {
      // Display help
      printf("Help:\n");
      printf("C file1 file2 - Copy; create file2, copy all bytes of file1 to file2 without deleting file1 (exactly 2 arguments required).\n");
      printf("D file - Delete the named file (exactly 1 argument required).\n");
      printf("E comment - Echo; display comment on screen (any number of arguments).\n");
      printf("H - Help (no arguments).\n");
      printf("L - List the contents of the current directory (no arguments).\n");
      printf("M file - Make; create the named text file and launching a text editor (exactly 1 argument required).\n");
      printf("P file - Print; display the contents of the named file on screen (exactly 1 argument required).\n");
      printf("Q - Quit the shell (no arguments).\n");
      printf("W - Wipe; clear the screen (no arguments).\n");
      printf("X program arguments - Execute the named program and pass arguments if wanted (any number of arguments).\n");
   }
   else if (strcmp(cmd->name, "L") == 0)
   {
      // List current directory and subdirectories
      printf("\n");
      system("pwd");
      printf("\n");
      system("ls -l");
   }
   else if (strcmp(cmd->name, "M") == 0)
   {
      // Attempt to create file
      if (cmd->argc == 2)
      {
         // Open text editor: nano file if corrent num args
         char *argv[] = {"nano", cmd->argv[1], NULL};
         err = execvp("nano", argv);
      }
      else
      {
         // Show error message is wrong num args
         printf("\033[0;31m"); // Set the text color to red
         printf("ERROR:\tIncorrect number of arguments.\nSyntax:\tM file\n");
         printf("\033[0m"); // Reset the text color to default
      }
   }
   else if (strcmp(cmd->name, "P") == 0)
   {
      // Attempt to print file
      if (cmd->argc == 2)
      {
         // Print file: more file if corrent num args
         char *argv[] = {"more", cmd->argv[1], NULL};
         err = execvp("more", argv);
      }
      else
      {
         // Show error message is wrong num args
         printf("\033[0;31m"); // Set the text color to red
         printf("ERROR:\tIncorrect number of arguments.\nSyntax:\tP file\n");
         printf("\033[0m"); // Reset the text color to default
      }
   }
   else if (strcmp(cmd->name, "W") == 0)
   {
      // Clear screen
      char *argv[] = {"clear", NULL};
      err = execvp("clear", argv);
   }
   else if (strcmp(cmd->name, "X") == 0)
   {
      // Build argv[] as every arg but X, then execute the named program
      char argv[cmd->argc];
      for (int i = 1; i < cmd->argc; i++)
      {
         argv[i - 1] = cmd->argv[i];
      }
      argv[cmd->argc - 1] = NULL;
      err = execvp(argv[0], argv);
   }
   else
   {
      return 1; // Returns 1 error code if no command recognized
   }

   if (err != 0)
   {
      printf("\033[0;31m"); // Set the text color to red
      printf("Error running command. Error code %i\n", err);
      printf("\033[0m"); // Reset the text color to default
   }

   return 0; // Returns 9 if a command was found, even if it wasn't success
}