#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Testing Document";
   int paragraph_number;

   init_document(&doc, doc_name);

   /* Adding first paragraph */
   add_paragraph_after(&doc, 0);

   paragraph_number = 1;
   append_line(&doc, paragraph_number, "First Paragraph, First line");
   append_line(&doc, paragraph_number, "First Paragraph, Second line");
   append_line(&doc, paragraph_number, "First Paragraph, Third line");
   append_line(&doc, paragraph_number, "First Paragraph, Fourth line");
   append_line(&doc, paragraph_number, "First Paragraph, Fifth line");

   print_document(&doc);

   return 0;
}
