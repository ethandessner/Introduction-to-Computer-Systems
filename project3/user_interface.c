#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sysexits.h>
#include "document.h"
#define MAX 1024

static int check_command(Document *doc, char *command);
static int run_add_paragraph_after(Document *doc, char *command);
static int run_add_line_after(Document *doc, char *command);
static int run_print_document(Document *doc, char *command);
static int run_append_line(Document *doc, char *command);
static int run_load_file(Document *doc, char *command);
static int run_remove_line(Document *doc, char *command);
static int run_replace_text(Document *doc, char *command);
static int run_highlight_text(Document *doc, char *command);
static int run_remove_text(Document *doc, char *command);
static int run_reset_doc(Document *doc, char *command);
static int run_save_document(Document *doc, char *command);
static char *remove_quotes(char *command, char *no_quotes);

/*If there is no file specified when the program is started, the program reads 
  its data from standard input. The program displays a prompt (>) after which 
  commands will be entered. If a file is named, however, the program reads its 
  data from that file; in this case no prompt will be used.

  In case of an error opening the file, your program should print (to stderr) 
  the message "FILENAME cannot be opened." where FILENAME represents the file 
  name. The program then exits with the exit code EX_OSERR. Upon starting 
  execution  the program initializes a document with the name "main_document", 
  and performs operations on that document as instructed by the commands the 
  program reads. If the word "quit" or "exit" is input into the command line, 
  the program terminates. This is done inside the main method instead of 
  creating separate methods for both "quit" and "exit". */
int main(int argc, char *argv[]){
    FILE *file;
    Document doc;
    const char *doc_name = "main_document";
    char line[MAX_STR_SIZE + 1], input[MAX_STR_SIZE + 1], symbol;
    int num_args;
    
    if(init_document(&doc, doc_name) == FAILURE){
        printf("Initialization failed\n");
    }else if(argc == 1){    
        file = stdin;
        printf("> ");
        fgets(line, MAX_STR_SIZE + 1, stdin);
        while(strcmp(line, "quit\n") && strcmp(line, "exit\n")){
            sscanf(line, " %c%s", &symbol, input);
            if(strcmp(line,"\n") != 0 && symbol != '#'){
                check_command(&doc, line);
            }
            printf("> ");
            fgets(line, MAX_STR_SIZE + 1, stdin);
        }
    }else if(argc == 2){
        file = fopen(argv[1], "r");
        if(file == NULL){
            fprintf(stderr, "%s cannot be opened. \n", argv[1]);
            exit(EX_OSERR);
        }else{
            while(fgets(line, MAX_STR_SIZE + 1, file)){
                if(strcmp(line, "quit\n") && strcmp(line, "exit\n")){
                    num_args = sscanf(line, " %c%s", &symbol, input);
                    if(num_args > 0 && symbol != '#'){
                        check_command(&doc, line);
                    }
                }
            }
        }
    }else{
        fprintf(stderr, "Usage: user_interface\n");
        fprintf(stderr, "Usage: user_interface <filename>\n");
        exit(EX_USAGE);
    }
    fclose(file);
    exit(EXIT_SUCCESS);
}

/*Checks command line to see if appropriate arguments are passed in when 
  calling add_paragraph_after. Funciton fails and prints "Invalid Command" 
  if paragraph number does not represent a number, paragraph number is a 
  negative value, paragraph number is missing, or if additional 
  information is provided after the paragraph number. */
static int run_add_paragraph_after(Document *doc, char *command){
    char input[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1];
    int num_args, para_num;

    num_args = sscanf(command, "%s%d%s", input, &para_num, additional);
    if(num_args == 2 && para_num >= 0){
        if(add_paragraph_after(doc, para_num) == FAILURE){
            printf("add_paragraph_after failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*Checks command line to see if appropriate arguments are passed in when 
  calling add_line_after. Funciton fails and prints "Invalid Command" if 
  paragraph number does not represent a number, paragraph number is a 
  negative value or 0, paragraph number is missing, line number does not 
  represent a number, line number is a negative value, line number is 
  missing, or if * is missing. */
static int run_add_line_after(Document *doc, char *command){
    char input[MAX_STR_SIZE + 1], line[MAX_STR_SIZE + 1], 
         new_line[MAX_STR_SIZE + 1];
    int num_args, para_num, line_num;

    num_args = sscanf(command, "%s%d%d%s", input, &para_num, 
                      &line_num, new_line);
    if(num_args == 4 && para_num > 0 && line_num >= 0){
        if(strchr(command, '*') != NULL){
            strcpy(line, strchr(command, '*') + 1);
            if(add_line_after(doc, para_num, line_num, line) == FAILURE){
                printf("add_line_after failed\n");
            }
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*Prints document information.*/
static int run_print_document(Document *doc, char *command){
    char input[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1];
    int num_args;

    num_args = sscanf(command, "%s%s", input, additional);
    if(num_args == 1){
        print_document(doc);
        return SUCCESS;
    }
    return FAILURE;
}

/*Appends a line to the specified paragraph. The line to add appears after the 
  * character. The "Invalid Command" message is generated if paragraph number 
  does not represent a number, paragraph number is a negative value or 0, 
  paragraph number is missing, or if * is missing. */
static int run_append_line(Document *doc, char *command){
    char input[MAX_STR_SIZE + 1], line[MAX_STR_SIZE + 1], 
         new_line[MAX_STR_SIZE + 1];
    int num_args, para_num;

    num_args = sscanf(command, "%s%d%s", input, &para_num, new_line);
    if(num_args == 3 && para_num > 0){
         if(strchr(command, '*') != NULL){
            strcpy(line, strchr(command, '*') + 1);
            if(append_line(doc, para_num, line) == FAILURE){
                printf("append_line failed\n");
            }
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*Checks command line to see if appropriate arguments are passed in when 
  calling remove_line. Funciton fails and prints "Invalid Command" if 
  paragraph number does not represent a number, paragraph number is a 
  negative value or 0, paragraph number is missing, line number does not 
  represent a number, line number is a negative value or 0, line number 
  is missing, or if any data appears after the line number. */
static int run_remove_line(Document *doc, char *command){
    char input[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1];
    int num_args, para_num, line_num;

    num_args = sscanf(command, "%s%d%d%s", input, &para_num, 
                      &line_num, additional);
    if(num_args == 3 && para_num > 0 && line_num > 0){
        if(remove_line(doc, para_num, line_num) == FAILURE){
            printf("remove_line failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*Replaces the string "TARGET" with "REPLACEMENT". The "Invalid Command" 
  message is generated if both "TARGET" and "REPLACEMENT" are missing, 
  or if only "TARGET" is provided. */
static int run_replace_text(Document *doc, char *command){
   char input[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1], 
        replacement[MAX_STR_SIZE + 1], *ptr1, *ptr2, *ptr3, *ptr4;
   int num_args;

   num_args = sscanf(command, "%s%s%s", input, target, replacement);

   if(num_args == 3 && strstr(command, "\"") != NULL){
        ptr1 = strstr(command, "\"");
        ptr2 = strstr(ptr1 + 1, "\"");
        if(ptr2 != NULL){
            ptr3 = strstr(ptr2 + 1, "\"");
            if(ptr3 != NULL){
                ptr4 = strstr(ptr3 + 1, "\"");
            }
            if(ptr4 == NULL){
                return FAILURE;
            }
        }
        strncpy(target, ptr1 + 1, ptr2 - ptr1);
        target[(ptr2 - (ptr1 + 1))] = '\0';

        strncpy(replacement, ptr3 + 1, ptr4 - ptr3);
        replacement[(ptr4 - (ptr3 + 1))] = '\0';

        if(replace_text(doc, target, replacement) == FAILURE){
            printf("replace_text failed\n");
        }
        return SUCCESS;
   }
    return FAILURE;
}

/*Removes the string "TARGET". The "Invalid Command" message is generated if 
  "TARGET" is missing. */
static int run_remove_text(Document *doc, char *command){
    char input[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1],
    *final;
    int num_args;
    
    num_args = sscanf(command, "%s%s", input, target);
    final = strstr(command, "\"");
    if(num_args == 2 && target != NULL && final != NULL){
        final = remove_quotes(final, target);
        if(final != NULL){
            remove_text(doc, final);
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*Highlights the string "TARGET". The "Invalid Command" message is generated if 
  "TARGET" is missing. */
static int run_highlight_text(Document *doc, char *command){
    char input[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1],
    *final;
    int num_args;

    num_args = sscanf(command, "%s%s", input, target);
    final = strstr(command, "\"");
    if(num_args == 2 && target != NULL && final != NULL){
        final = remove_quotes(final, target);
        if(final != NULL){
            highlight_text(doc, final);
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*Resets the curent document. The "Invalid Command" message is generated if 
  any data appears after reset_document. */
static int run_reset_doc(Document *doc, char *command){
    char input[MAX_STR_SIZE + 1], additional[MAX_STR_SIZE + 1];
    int num_args;

    num_args = sscanf(command, "%s%s", input, additional);
    if(num_args == 1){
        reset_document(doc);
        return SUCCESS;
    }
    return FAILURE;
}

/*Loads the specified file into the current document. The "Invalid Command" 
  message will be generated if FILENAME is missing or if any data appears 
  after FILENAME. */
static int run_load_file(Document *doc, char *command){
    char input[MAX_STR_SIZE + 1], file_name[MAX_STR_SIZE + 1], 
         additional[MAX_STR_SIZE + 1];
    int num_args;

    num_args = sscanf(command, "%s%s%s", input, file_name, additional);
    if(num_args == 2){
        if(load_file(doc, file_name) == FAILURE){
            printf("load_file failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*Saves the curent document to the specified file. The "Invalid Command"
  message is generated if: FILENAME is missing or if any data appears 
  after FILENAME. */
static int run_save_document(Document *doc, char *command){
    char input[MAX_STR_SIZE + 1], doc_name[MAX_STR_SIZE + 1], 
         additional[MAX_STR_SIZE + 1];
    int num_args;

    num_args = sscanf(command, "%s%s%s", input, doc_name, additional);
    if(num_args == 2){
        if(save_document(doc, doc_name) == FAILURE){
            printf("save_document failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}
/*Removes a single pair of quotes from a string.*/
static char *remove_quotes(char *command, char *no_quotes){
    char *ptr1, *ptr2;

    ptr1 = strstr(command, "\"");
    if(ptr1 != NULL){
        ptr2 = strstr(ptr1 + 1, "\"");
        if(ptr2 != NULL){
            strncpy(no_quotes, ptr1 + 1, ptr2 - ptr1);
            no_quotes[ptr2 - (ptr1 + 1)] = '\0';
            return no_quotes;
        }
    }
    return NULL;
    
}

/*Reads the command line to check if the proper argument name was typed in. 
  Runs the helper method called that is associated with the typed in command, 
  and checks to see if the helper method ran successfully. */
static int check_command(Document *doc, char *command){
    int result = FAILURE;

    if(strstr(command, "add_paragraph_after") != NULL 
        && run_add_paragraph_after(doc, command) != FAILURE){
            result = SUCCESS;
    }else if(strstr(command, "add_line_after") != NULL 
        && run_add_line_after(doc, command) != FAILURE){
            result = SUCCESS;
    }else if(strstr(command, "print_document") != NULL 
        && run_print_document(doc, command) != FAILURE){
            result = SUCCESS;
    }else if(strstr(command, "append_line") != NULL 
        && run_append_line(doc, command) != FAILURE){
            result = SUCCESS;
    }else if(strstr(command, "remove_line") != NULL
        && run_remove_line(doc, command) != FAILURE){
            result = SUCCESS;
    }else if(strstr(command, "replace_text") != NULL
        && run_replace_text(doc, command) != FAILURE){
            result = SUCCESS;
    }else if(strstr(command, "load_file") != NULL
        && run_load_file(doc, command) != FAILURE){
            result = SUCCESS;
    }else if(strstr(command, "remove_text") != NULL
        && run_remove_text(doc, command) != FAILURE){
            result = SUCCESS;
    }else if(strstr(command, "highlight_text") != NULL
        && run_highlight_text(doc, command) != FAILURE){
            result = SUCCESS;
    }else if(strstr(command, "save_document") != NULL
        && run_save_document(doc, command) != FAILURE){
            result = SUCCESS;
    }else if(strstr(command, "reset_document") != NULL
        && run_reset_doc(doc, command) != FAILURE){
            result = SUCCESS;
    }

    if(result == FAILURE){
        printf("Invalid Command\n");
    }
    return result;
}