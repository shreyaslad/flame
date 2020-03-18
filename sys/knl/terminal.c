/*
    terminal.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Terminal
*/

#include <sys/knl/terminal.h>

uint32_t largestUseableMem = 0;

void userInput(char* input) {
  if (strcmp(input, "clear") == 0) {
    clear();
    formatTime();
    printf("flame\nIt is %s\nuser@flame ~$ ", format);
  } else {
    printf("Invalid Command\n");
    printf("user@flameOS # ");
  }
}