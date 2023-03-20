#include <stdio.h>
#include <stdint.h>

extern uint32_t fib(uint32_t n);

int main() {
  uint32_t inputs[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 23, 24, 0 };
  uint32_t i;

  for(i = 0; inputs[i]; i++) {
    printf("%d -> %d\n", inputs[i], fibonacci(inputs[i]));
  }
}

