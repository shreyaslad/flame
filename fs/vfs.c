/*
    vfs.c
    Copyright Shreyas Lad (PenetratingShot) 2020

    Virtual File System
*/

#include <fs/vfs.h>

node_t* root;
char* currentPath = "/";

void initVFS() {
  root = (node_t*)malloc(sizeof(node_t));

  root->name = "/";
  root->perms = 777;
  root->size = 0;
  root->creationtime = tick;
  root->rfunc = vfsRead;
  root->wfunc = vfsWrite;
  root->type = DIR;
  root->nchild = 0;
  root->parent = NULL;
  root->children = malloc(sizeof(root->children));
}

node_t* createNode(char* name, uint64_t perms, type_t type, node_t* parent) {
  node_t* node = (node_t*)malloc(sizeof(node_t));

  node->name = name;
  node->perms = perms;
  node->size = 0;
  node->creationtime = tick;
  node->rfunc = vfsRead;
  node->wfunc = vfsWrite;
  node->type = type;
  node->nchild = 0;
  node->parent = parent;
  node->children = NULL;

  parent->children =
      (node_t**)pmrealloc(parent->children, parent->nchild, parent->nchild + 1);
  memset(parent->children[parent->nchild], 0, sizeof(node_t*));
  parent->children[parent->nchild] = node;
  parent->nchild++;

  return node;
}

node_t* vfsResolve(char* filepath) {
  uint64_t items = strnchr(filepath, '/');

  const char* delim = "/";
  char* token = strtok(filepath, delim);
  node_t* current = root;

  for (uint64_t i = 0; i < items - 1; i++) {
    token = strtok(filepath, delim);
    for (uint64_t j = 0; j < current->nchild; j++) {
      if (strcmp(current->children[j]->name, token) == 0) {

        current = current->children[j];
        break;
      } else {
        return NULL;
      }
    }
  }
  return current;
}

// the item to be created is not in the path
void createNodeInPath(char* path, node_t* newNode) {
  uint64_t items = strnchr(path, '/');

  const char* delim = "/";
  char* token = strtok(path, delim);
  node_t* current = root;

  for (uint64_t i = 0; i < items - 1; i++) {
    token = strtok(path, delim);
    for (uint64_t j = 0; j < current->nchild; i++) {
      if (strcmp(current->children[j]->name, token) == 0) {
        current = current->children[j];

        if (strcmp(current->name, token)) {
          memset(current->children[current->nchild + 1], 0, sizeof(node_t*));
          current->children[current->nchild] = newNode;
        }
      }
    }
  }
}

char* vfsRead(node_t* node, char** buffer, uint64_t bytes) {
  if (node->type == DIR) {
    return NULL;
  }

  // call fs implementation
  return "a";
  UNUSED(bytes);
}

uint64_t vfsWrite(node_t* node, char* text) {
  if (node->type == DIR) {
    return 0;
  }

  node->size += strlen(text);

  // call fs implementation

  return 1;
}