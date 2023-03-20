/*Ethan Dessner UID: 117919219 DID: edessner*/

#include <stdio.h>
#include "command.h"
#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sysexits.h>
#include <err.h>
#include <fcntl.h>
#include <sys/wait.h>
#define FILE_PERMISSIONS 0644

static void print_tree(struct tree *t);
static int recurse_tree(struct tree *t, int in_fd, int out_fd);

int execute(struct tree *t) {
   if(t != NULL){
      return recurse_tree(t, -1, -1);
   }
   print_tree(t->left);
   print_tree(t->right);
   return 0;
}

static int recurse_tree(struct tree *t, int in_fd, int out_fd){
   pid_t pid;
   int status, status2;

   /*Handles NONE*/
   if(t->conjunction == NONE){
      if(!strcmp(t->argv[0], "cd")){
         chdir(t->argv[1]);
      }else if(!strcmp(t->argv[0], "exit")){
         exit(0);
      }else{
         if((pid = fork()) < 0){
            err(EX_OSERR, "Fork error\n");
         }else{
            if(pid != 0){ /*Parent Process*/
               wait(&status);
               return status;
            }
            if(pid == 0){ /*Child Process*/
               if(t->input){
                  if((in_fd = open(t->input, O_RDONLY)) < 0){
                     err(EX_OSERR, "Input file could not be opened.\n");
                  }
                  if(dup2(in_fd, STDIN_FILENO) < 0){
                     err(EX_OSERR, "dup2 input failed.\n");
                  }
                  close(in_fd);
               }
               if(t->output){
                  if((out_fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMISSIONS)) < 0){
                     err(EX_OSERR, "Output file couldn't be opened.\n");
                  }
                  if(dup2(out_fd, STDIN_FILENO) < 0){
                     err(EX_OSERR, "dup2 output failed.\n");
                  }
                  close(out_fd);
               }
            execvp(t->argv[0], t->argv);
            fprintf(stderr, "Failed to execute%s\n", t->argv[0]);
            exit(-1);
            }
         }
      }
      /*Handles AND*/
   }else if(t->conjunction == AND){
      if(t->input){
         if((in_fd = open(t->input, O_RDONLY) < 0)){
            err(EX_OSERR, "Input file could not be opened.\n");
         }
      }
      if(t->output){
         if((out_fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMISSIONS)) < 0){
            err(EX_OSERR, "Output file could not be opened.\n");
         }
      }
      status2 = recurse_tree(t->left, in_fd, out_fd);
      if(status2 == 0){ /*status2 is 0 if left side executes correctly*/
         recurse_tree(t->right, in_fd, out_fd);
      }
   /*Handles PIPE*/
   } else if(t->conjunction == PIPE){   
      pid_t child_pid_one, child_pid_two;
      int pipe_fd[2]; /* pipe */
      if(t->left->output){
         printf("Ambiguous output redirect.\n");
         fflush(stdout);
         return -1;
       } else if(t->right->input){
            printf("Ambiguous output redirect.\n");
            fflush(stdin);
            return -1;
      }
         /* Before the fork, we need to create the pipe */ 
         /* (otherwise no sharing of pipe) */
      if (pipe(pipe_fd) < 0) {
         err(EX_OSERR, "Pipe error");
      }

      if ((child_pid_one = fork()) < 0) {
         err(EX_OSERR, "Fork error");
      }

      if (child_pid_one == 0)  { /* CHILD #1's code */
         close(pipe_fd[0]); /* we don't need pipe's read end */

         /* Redirecting standard output to pipe write end */
         if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
            err(EX_OSERR, "dup2 error");
         }
         /* Releasing resource */     
         close(pipe_fd[1]);
   
         recurse_tree(t->left, in_fd, out_fd);
         err(EX_OSERR, "exec error");
      }  else { /* parent's code */
   
         /* Creating second child */
         if ((child_pid_two = fork()) < 0) {
            err(EX_OSERR, "Fork error");
         } 

         if (child_pid_two == 0)  { /* CHILD #2's code  */
            close(pipe_fd[1]); /* we don't need pipe's write end */

            /* Redirecting standard input to pipe read end */
            if (dup2(pipe_fd[0], STDIN_FILENO) < 0) {
               err(EX_OSERR, "dup2 error");
            }
            /* Releasing resource */
            close(pipe_fd[0]);

            recurse_tree(t->right, in_fd, out_fd);
            err(EX_OSERR, "exec error");
         } else {
            /* Parent has no need for the pipe */
            close(pipe_fd[0]);
            close(pipe_fd[1]);

            /* Reaping each child */
            wait(NULL); 
            wait(NULL);
         }
      }
      /*Handles SUBSHELL*/

   }else if(t->conjunction == SUBSHELL){

      if(t->input){
         if((in_fd = open(t->input, O_RDONLY)) < 0){
            err(EX_OSERR, "SUBSHELL - input file couldn't be opened\n");
         }
      }
      if(t->output){
         if((in_fd = open(t->output, O_RDONLY)) < 0){
            err(EX_OSERR, "SUBSHELL - output file couldn't be opened\n");
         }
      }
      if((pid = fork()) < 0){
         err(EX_OSERR, "fork");
      }else{
         if(pid != 0){
            wait(NULL);
         }
         if(pid == 0){
            recurse_tree(t->left, in_fd, out_fd);
            exit(0);
         }
      }
   }
   return 0;
}

static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}

