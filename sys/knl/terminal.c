/*
    terminal.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Terminal
*/

#include <sys/knl/terminal.h>

uint32_t largestUseableMem = 0;

void userInput(char* input) {
  if(strcmp(input, "node") == 0) {
    node_t* src = createNode("src", 777, DIR, root);
    node_t* bruh = createNode("bruh", 777, DIR, src);
    printf(root->children[0]->children[0]->path);
  } else {
    printf("Invalid Command\n");
  }
  printf("\nuser@flameOS # ");
}