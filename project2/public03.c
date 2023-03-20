#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Exercise Description";
   int paragraph_number, line_number;

   init_document(&doc, doc_name);

   /* Adding first paragraph */
   paragraph_number = 0;
   add_paragraph_after(&doc, paragraph_number);

   line_number = 0;
   paragraph_number = 1;
   add_line_after(&doc, paragraph_number, line_number, "First Paragraph, First line");
   add_line_after(&doc, paragraph_number, line_number + 1, "First Paragraph, Second line");
   add_line_after(&doc, paragraph_number, line_number + 2, "First Paragraph, Third line");

   /* Adding second paragraph */
   paragraph_number = 1;
   add_paragraph_after(&doc, paragraph_number);

   line_number = 0;
   paragraph_number = 2;
   add_line_after(&doc, paragraph_number, line_number, "Second Paragraph, First line");
   add_line_after(&doc, paragraph_number, line_number + 1, "Second Paragraph, Second line");
   add_line_after(&doc, paragraph_number, line_number + 2, "Second Paragraph, Third line");

   print_document(&doc);

   return 0;
}
