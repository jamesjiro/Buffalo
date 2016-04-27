#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Singly linked list for sentence numbers */
typedef struct entry {        
  unsigned int sent;              // A sentence number where the word appeared
  struct entry* next;             // A pointer to the next entry
} sentList;

/* Binary tree for concordance */
typedef struct node {
  char* word;
  unsigned int count;            // The count of occurrences for the word
  sentList* sNums;               // A list of sentence numbers for each occurrence of the word
  struct node* left;
  struct node* right;
} node;

/* These functions are described below with their implementations */
char* toLower (char* str);           
sentList* newList (int sNum);
sentList* insertSen (int sNum, sentList* list);
void freeList (sentList* list);
void printList (sentList* list);
node* newNode (char* word, unsigned int count, int sNum);
node* insertN (node* root, char* word, unsigned int count, int sNum);
void printNode (node* root);
void freeNode (node* root);
void printTree (node* root);
void freeTree (node* node);

int main (int argc, char* argv[]) {
  FILE* fp;
  int sNum;

  // test code
  int i;
  sentList* tList = newList (0);
  for (i = 1; i < 5; i++) {
    insertSen (i, tList);
  }

  printList (tList);
  freeList (tList);

  char word[] = "BALl";
  
  printf(" %s \n", toLower(word));
  char* word1 = (char*) malloc(5*sizeof(char));
  word1 = "ambi";
  char* word2 = (char*) malloc(3*sizeof(char));
  word2 = "ba";
  char* word3 = (char*) malloc(3*sizeof(char));
  word3 = "ae";
  char* word4 = (char*) malloc(3*sizeof(char));
  word4 = "ak";
  char* word5 = (char*) malloc(3*sizeof(char));
  word5 = "bi";
  char* word6 = (char*) malloc(3*sizeof(char));
  word6 = "bc";

  node* root =  newNode (word1, 1, 1);

  node* child = insertN (root, word1, 1, 1);

  node* child1 = insertN (root, word1, 1, 2);

  node* child2 = insertN (root, word2, 1, 2);

  node* child3 = insertN (root, word3, 1, 3);
  node* child4 = insertN (root, word4, 1, 3);
  node* child5 = insertN (root, word2, 1, 4);
  node* child6 = insertN (root, word5, 1, 4);
  node* child7 = insertN (root, word6, 1, 4);
  

  printTree (root);

  freeTree (root);

  // test code

  if (!(argv[1]) || argv[2]) {
    printf("Please include one filename argument.\n");
    exit(0);
  }

  if ((fp = fopen(argv[1], "r")) == NULL) {
    perror(argv[1]);
    exit(0);
  }

  fclose(fp);

  return 0;
}

/* 
Converts strings to lowercase for comparison with
strcmp.
*/
char* toLower (char* str) {  
  int i;
  
  for (i = 0; str[i]; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] += 32;
    }
  }

  return str;
}

/*
Allocates the head of the singly linked list.
*/
sentList* newList (int sNum) {  
  sentList* list = (sentList*) malloc(sizeof(sentList));
  list->sent = sNum;
  list->next = NULL;

  return(list);
}

/*
Inserts the next entry in the list of sentence numbers
*/
sentList* insertSen (int sNum, sentList* list) {
  sentList* curr = list;
  while ((curr->next)) {
    curr = curr->next;
  }
  curr->next = (sentList*) malloc(sizeof(sentList));
  curr->next->sent = sNum;
  curr->next->next = NULL;

  return list;
}

/*
Frees the list of sentence numbers 
*/
void freeList (sentList* list) {
  sentList* curr;
  while ((curr = list)) {
    list = list->next;
    free (curr);
  }
  return;
}

/*
Prints the list of sentence numbers 
*/
void printList (sentList* list) {
  sentList* curr = list;
  while ((curr->next)) {
    printf(" %d,", curr->sent);
    curr = curr->next;
  }
  printf(" %d}\n", curr->sent);

  return;
}   

/*
Creates the root node for the binary tree structure
*/
node* newNode (char* word, unsigned int count, int sNum) {
  node* root = (node*)malloc(sizeof(node));
  root->word = word;
  root->count = count;
  root->sNums = newList(sNum);               // Creates a new sentence number list
  root->left = NULL;
  root->right = NULL;

  return (root);
}

node* insertN (node* root, char* word, unsigned int count, int sNum) {
  if (root == NULL) {                  
    return (newNode(word, count, sNum));     // If the node is empty then a new node is created    
  }

  else {
    /* 
       If the new word precedes the word in the current node alphabetically, recurse 
       down the tree to the left child
    */
    if ((strcmp(word, root->word)) < 0) {
      root->left = insertN (root->left, word, count, sNum);     
    }

    /* 
      If the new word succeeds the current node, recurse down the tree to 
      the right
    */
    else if ((strcmp(word, root->word)) > 0) {
      root->right = insertN (root->right, word, count, sNum);
    }
    /* 
       If the new word is identical to the word in the current node, increment
       the count and insert the sentence number for the occurrence
    */
    else {
      root->count++;
      root->sNums = insertSen (sNum, root->sNums);
    }
    /*
      Recursively returns root to the previous call until the original root
      pointer is popped off the stack and returned
    */
    return (root);               
  }
}

/*
Prints the contents of a node
*/
void printNode (node* node) {
  char* str = node->word;
  unsigned int cnt = node->count;
  printf ("%s {%d:", str, cnt);
  printList(node->sNums);

  return;
}

/*
Frees a node
*/
void freeNode (node* node) {
  free (node->word);
  //freeList (node->sNums);
  free (node);

  return;
}
  
/*
Prints the concordance tree in alphabetical order
*/
void printTree (node* node) {
  if (node == NULL) {
    printf ("empty file!\n");
    exit(0);
  }
  if (node->left) {
    printTree (node->left);
  }

  printNode (node);
  
  if (node->right) {
    printTree (node->right);
  }
  
  return;                            
}

void freeTree (node* node) {
  if (node == NULL) {
    return;
  }

  else if (node->left) {
    freeTree (node->left);
  }
  else if (node->right) {
    freeTree (node->right);
  }
  else {
    freeNode (node);
    return;
  }
}
  




