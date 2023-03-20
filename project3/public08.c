#include <stdio.h>
#include <stdlib.h>
#include "document.h"

int main() {
  system("./user_interface public08.in");
  system("echo '***** SAVED FILE *****'; cat public08.file.tmp");

  return 0;
}
