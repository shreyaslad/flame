/*
    vfs.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Virtual File System
*/

#include <fs/vfs.h>

node_t root = {"/", "/", 777, 0, 0, 0, 0, NULL, NULL, NULL, NULL};
char* currentPath = NULL;

void initDirPath() { currentPath = root.name; }