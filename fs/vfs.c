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

  root->name = "root";
  root->path = "/";
  root->perms = 777;
  root->size = 0;
  root->creationtime = tick;
  root->rfunc = vfsread;
  root->wfunc = vfswrite;
  root->type = DIR;
  root->nchild = 0;
  root->parent = NULL;
  root->children = malloc(sizeof(root->children));
}

node_t* createNode(char* name, uint64_t perms, type_t type, node_t* parent) {
  node_t* node = (node_t*)malloc(sizeof(node_t));
  char* path = (char*)malloc(sizeof(char) * strlen(name) + 2);
  if (type == DIR) {
    path = strcat(parent->path, strcat(name, "/"));
  } else {
    path = strcat(parent->path, name);
  }

  node->name = name;
  node->path = path;
  node->perms = perms;
  node->size = 0;
  node->creationtime = tick;
  node->rfunc = vfsread;
  node->wfunc = vfswrite;
  node->type = type;
  node->nchild = 0;
  node->parent = NULL;
  node->children = NULL;
  return node;
}

void addChild(node_t* parent, node_t* child) {
  parent->children =
      (node_t**)pmrealloc(parent->children, parent->nchild, parent->nchild + 1);
  memset(parent->children[parent->nchild], 0, sizeof(node_t*));
  parent->children[parent->nchild] = child;
  parent->nchild++;
}

node_t* vfsresolve(char* filepath) {
  uint64_t items = strnchr(filepath, "/");

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
  uint64_t items = strnchr(path, "/");

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

char* vfsread(node_t* node, uint64_t bytes) {
  if (node->type == DIR) {
    return NULL;
  }

  // call fs implementation
  return "a";
  UNNUSED(bytes);
}

uint64_t vfswrite(node_t* node, char* text) {
  if (node->type == DIR) {
    return NULL;
  }

  node->size += strlen(text);

  // call fs implementation

  return 1;
}