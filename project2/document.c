#include <stdio.h>
#include <string.h>
#include "document.h"


int init_document(Document *doc, const char *name){
    if(doc != NULL && name != NULL && strlen(name) <= MAX_STR_SIZE){
        strcpy(doc->name, name);
        doc->number_of_paragraphs = 0;
        return SUCCESS;
    }
    return FAILURE;
}

int reset_document(Document *doc){
    if(doc != NULL){
        doc->number_of_paragraphs = 0;
        return SUCCESS;
    }
    return FAILURE;
}

int print_document(Document *doc){
    int x, y = 0;

    if(doc != NULL){
        printf("Document name: \"%s\"\n", doc->name);
        printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
        for(x = 0; x < doc->number_of_paragraphs; x++){
            if(doc->paragraphs[x].number_of_lines > 0){
                for(y = 0; y < doc->paragraphs[x].number_of_lines; y++){
                    printf("%s\n", doc->paragraphs[x].lines[y]);
                }
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

int add_paragraph_after(Document *doc, int paragraph_number){
    int end_para = 0;

    if(doc != NULL && doc->number_of_paragraphs < MAX_PARAGRAPHS && 
       paragraph_number <= doc->number_of_paragraphs){
        if(paragraph_number == 0){
            doc->paragraphs[paragraph_number].number_of_lines = 0;
            doc->number_of_paragraphs++;
            return SUCCESS;
        }
        end_para = doc->number_of_paragraphs - 1;
        for(end_para; end_para >= paragraph_number; end_para--){
            doc->paragraphs[end_para + 1] = doc->paragraphs[end_para];
        }
        doc->paragraphs[paragraph_number].number_of_lines = 0;
        doc->number_of_paragraphs++;
        return SUCCESS;
    }
    return FAILURE;
}

int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line){
    int x, lines_in_target = 0;
    char *ptr, *str1, *str2;

    if(doc != NULL && paragraph_number <= doc->number_of_paragraphs){
        lines_in_target = doc->paragraphs[paragraph_number - 1].number_of_lines;
        if(lines_in_target < MAX_PARAGRAPH_LINES && line_number <= lines_in_target){
            if(line_number == 0){
                ptr = doc->paragraphs[paragraph_number - 1].lines[0];
                strcpy(ptr, new_line);
                doc->paragraphs[paragraph_number - 1].number_of_lines += 1;
                return SUCCESS;
            }
            if(line_number == lines_in_target){
                ptr = doc->paragraphs[paragraph_number - 1].lines[line_number];
                strcpy(ptr, new_line);
                doc->paragraphs[paragraph_number - 1].number_of_lines += 1;
                return SUCCESS;
            }
            if(line_number != 0 && line_number != lines_in_target){
                for(x = lines_in_target; x >= line_number; x--){
                    str1 = doc->paragraphs[paragraph_number - 1].lines[x];
                    str2 = doc->paragraphs[paragraph_number - 1].lines[x+1];
                    strcpy(str2,str1);
                }
                ptr = doc->paragraphs[paragraph_number - 1].lines[line_number];
                strcpy(ptr, new_line);
            }
            doc->paragraphs[paragraph_number - 1].number_of_lines += 1;
            return SUCCESS;
        }
    }
    return FAILURE;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines){
    int lines = 0;

    if(doc != NULL && number_of_lines != NULL && paragraph_number <= doc->number_of_paragraphs){
        lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
        *number_of_lines = lines;
        return SUCCESS;
    }
    return FAILURE;
}

int append_line(Document *doc, int paragraph_number, const char *new_line){
   int lines_in_target = 0;

   if(doc != NULL && new_line != NULL){
      lines_in_target = doc->paragraphs[paragraph_number - 1].number_of_lines;
      if(paragraph_number <= MAX_PARAGRAPHS && lines_in_target < MAX_PARAGRAPH_LINES){
         add_line_after(doc, paragraph_number, lines_in_target, new_line);
         return SUCCESS;
      } 
    }
   return FAILURE;
}

int remove_line(Document *doc, int paragraph_number, int line_number){
    int x, y, lines_in_target = 0;
    char *str1, *str2;

    if(doc != NULL && paragraph_number <= doc->number_of_paragraphs){
        lines_in_target = doc->paragraphs[paragraph_number - 1].number_of_lines;
        if(line_number <= lines_in_target){
            if(line_number == doc->paragraphs[paragraph_number - 1].number_of_lines){
                strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number - 1], "");
                doc->paragraphs[paragraph_number - 1].number_of_lines--;
                return SUCCESS;
            }else{
                strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number - 1], "");
                for(x = line_number; x < doc->paragraphs[paragraph_number - 1].number_of_lines; x++){
                    str1 = doc->paragraphs[paragraph_number - 1].lines[x - 1];
                    str2 = doc->paragraphs[paragraph_number - 1].lines[x];
                    strcpy(str1, str2);
                }
                doc->paragraphs[paragraph_number - 1].number_of_lines--;
                return SUCCESS;
            }
        }
    }
    return FAILURE;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){
    int x = 0;

    if(doc != NULL && data != NULL && data_lines != 0){
       add_paragraph_after(doc, doc->number_of_paragraphs);
       for(x = 0; x < data_lines; x++){
          if(strcmp(data[x], "") == 0){
            add_paragraph_after(doc, doc->number_of_paragraphs);
          }else{
            append_line(doc, doc->number_of_paragraphs, data[x]);
          }
       }
       return SUCCESS;
    }
    return FAILURE;
}

int replace_text(Document *doc, const char *target, const char *replacement){
    int para_inc, line_inc, lines_in_target = 0;
    char *ptr, *check, curr_line[MAX_STR_SIZE + 1] = {0};

    if(doc != NULL && target != NULL && replacement != NULL){
        for(para_inc = 0; para_inc < doc->number_of_paragraphs; para_inc++){
            lines_in_target = doc->paragraphs[para_inc].number_of_lines;
            for(line_inc = 0; line_inc < lines_in_target; line_inc++){
                ptr = strstr(doc->paragraphs[para_inc].lines[line_inc], target);
                while(ptr != '\0'){
                    strcpy(curr_line, ptr);
                    strcpy(ptr, "");
                    strcat(ptr, replacement);
                    strcat(ptr + strlen(replacement), curr_line + strlen(target));
                    check = strstr(doc->paragraphs[para_inc].lines[line_inc], target);
                    if(check){
                        ptr = strstr(doc->paragraphs[para_inc].lines[line_inc], target);
                    }else{
                        ptr = strstr(ptr + strlen(target), target);
                    }
                }
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

int highlight_help(Document *doc, const char *target, const char *replacement){
    int para_inc, line_inc, lines_in_target = 0;
    char *ptr, *check, curr_line[MAX_STR_SIZE + 1] = {0};

    if(doc != NULL && target != NULL && replacement != NULL){
        for(para_inc = 0; para_inc < doc->number_of_paragraphs; para_inc++){
            lines_in_target = doc->paragraphs[para_inc].number_of_lines;
            for(line_inc = 0; line_inc < lines_in_target; line_inc++){
                ptr = strstr(doc->paragraphs[para_inc].lines[line_inc], target);
                while(ptr != '\0'){
                    strcpy(curr_line, ptr);
                    strcpy(ptr, "");
                    strcat(ptr, replacement);
                    strcat(ptr + strlen(replacement), curr_line + strlen(target));
                    ptr = strstr(ptr + strlen(target), target);
                }
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

int highlight_text(Document *doc, const char *target){
    char highlight[MAX_STR_SIZE + 1] = "";

    if(doc != NULL && target != NULL){
        strcat(highlight, HIGHLIGHT_START_STR);
        strcat(highlight, target);
        strcat(highlight, HIGHLIGHT_END_STR);
        highlight_help(doc, target, highlight);
        return SUCCESS;
    }
    return FAILURE;
}

int remove_text(Document *doc, const char *target){
     if(doc != NULL && target != NULL){
        replace_text(doc, target, "");
        return SUCCESS;
     }
     return FAILURE;
}
int load_file(Document *doc, const char *filename){
    int x = 0;
    FILE *input_stream;
    if(doc != NULL && filename != NULL && doc->number_of_paragraphs < MAX_PARAGRAPHS){
        input_stream = fopen(filename, "r");
        
        if(input_stream == NULL)
            return FAILURE;
        
        add_paragraph_after(doc, doc->number_of_paragraphs);
    }
    return FAILURE;
}