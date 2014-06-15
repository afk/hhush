#include<stdio.h>

int main() {
    char input[256];
    while (1) {
      fgets(input, sizeof(input), stdin);

      printf ("Input: %s\n", input);
    }
    
    return 0;
}