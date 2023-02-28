#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char username[20];
  char pass[20];
  int status;
} account;

typedef account elementtype;
typedef struct Node
{
  elementtype ele;
  struct Node *next;
  struct Node *prev;
} Node;
Node *root = NULL, *cur = NULL, *tail = NULL;

void printData(Node *p);
elementtype getData();

Node *makeNewNode(elementtype ele)
{
  Node *new = (Node *)malloc(sizeof(Node));
  new->ele = ele;
  new->next = NULL;
  new->prev = NULL;
  return new;
}

void traverseList(Node *root)
{
  Node *curr;
  for (curr = root; curr != NULL; curr = curr->next)
  {
    printData(curr);
  }
}

void insertAtFirst(elementtype ele) // insertAtHead
{
  Node *new = makeNewNode(ele);
  new->next = root;
  if (root != NULL)
  {
    root->prev = new;
  }
  else
  {
    tail = new;
  }

  root = new;
  cur = new;
}

void insertAfterCurrentPos(elementtype ele)
{
  Node *new = makeNewNode(ele);
  if (root == NULL)
  {
    root = new;
    cur = new;
    tail = new;
  }
  else
  {
    new->next = cur->next;
    cur->next = new;
    if (cur->next->next != NULL)
    {
      cur->next->next->prev = new;
    }
    new->prev = cur;
    cur = new;
  }
}

void insertBeforeCurrentPos(elementtype ele)
{
  /* Node tmp = root;
     while(tmp != NULL && tmp->next != cur && cur != NULL)
     {
        tmp = tmp->next;
     }
     prev = tmp;
   */

  Node *new = makeNewNode(ele);
  if (root == NULL)
  {
    root = new;
    cur = new;
    tail = new;
  }
  else
  {
    new->next = cur;
    new->prev = cur->prev;
    if (cur == root)
    {
      root = new;
    }
    else
    {
      cur->prev->next = new;
    }
    cur->prev = new;
    cur = new;
  }
}

void append(elementtype ele)
{
  Node *new = makeNewNode(ele);

  if (root == NULL)
  {
    root = new;
    cur = new;
    tail = new;
  }
  else
  {
    tail->next = new;
    new->prev = tail;
    tail = new;
    cur = tail;
  }
}

void Free(Node *root)
{
  Node *to_free = root;
  while (to_free != NULL)
  {
    root = root->next;
    free(to_free);
    to_free = root;
  }
}

void deleteFirstElement()
{
  if (root == NULL)
  {
    printf("Con tro root NULL\n");
    return;
  }

  Node *tmp;
  tmp = root;
  root = root->next;
  cur = root;
  free(tmp);
}

void deleteCurrentElement()
{
  if (cur == NULL)
    return;
  if (cur == root)
  {
    deleteFirstElement();
  }
  else
  {
    cur->prev->next = cur->next;
    if (cur->next != NULL)
      cur->next->prev = cur->prev;
    free(cur);
    if (cur == NULL)
      cur = root;
    /*  cur = root; */
  }
}

void deleteLastElement()
{
  Node *tmp;
  if (tail == NULL)
  {
    printf("Con tro tail NULL");
    return;
  }

  cur = tail;
  if (root == tail)
  {
    deleteFirstElement();
  }
  else
  {
    tail->prev->next = NULL;
    tmp = tail->prev;
    free(tail);
    tail = tmp;
  }
}

// Node *list_reverse()
// {
//   Node *tmp = NULL;
//   if (root == NULL)
//   {
//     return root;
//   }
//   int i;

//   tail = root;
//   for (*cur = *root; *cur != NULL; *cur = (*cur)->prev)
//   {
//     tmp = (*cur)->next;
//     (*cur)->next = (*cur)->prev;
//     (*cur)->prev = tmp;
//   }

//   for (*cur = *tail; *cur != NULL; *cur = (*cur)->prev)
//   {
//     tmp = *cur;
//   }

//   *root = tmp;
//   return *root;
// }

Node *insertAtPosition(elementtype ad, int n)
{
  if (root == NULL)
  {
    return root;
  }

  cur = root;
  for (int i = 0; i < n; i++)
  {
    cur = cur->next;
  }
  if (cur == root)
  {
    insertAtFirst(ad);
  }
  else if (cur == tail)
  {
    append(ad);
  }
  else
  {
    insertAfterCurrentPos(ad);
  }

  return root;
}

Node *deleteAtPosition(int n)
{
  if (root == NULL)
    return root;

  cur = root;
  for (int i = 0; i < n; i++)
  {
    cur = cur->next;
  }

  if (cur == root)
  {
    deleteFirstElement();
    return root;
  }
  else if (cur == tail)
  {
    deleteLastElement();
  }
  else
  {
    deleteCurrentElement();
  }
  return root;
}



// void searchAndUpdate(Node **root, Node **cur, Node **tail)
// {
//   printf("\t SEARCH AND UPDATE \n");
//   char username[20];
//   int i = 0;
//   printf("Search by Model: ");
//   scanf("%s%*c", username);
//   for (*cur = *root; *cur != NULL; *cur = (*cur)->next)
//   {
//     i++;
//     if (strcmp(((*cur)->ele).username, model) == 0)
//     {
//       printf(">>> UPDATE ELEMENT thu %d <<<\n", i);
//       printf("Model: ");
//       scanf("%s%*c", ((*cur)->ele).model);
//       printf("Memory: ");
//       scanf("%d%*c", &(((*cur)->ele).memory));
//       printf("Space: ");
//       scanf("%d%*c", &(((*cur)->ele).space));
//       printf("Screen size: ");
//       scanf("%lf%*c", &(((*cur)->ele).screen_size));
//       printf("Price: ");
//       scanf("%d%*c", &(((*cur)->ele).price));
//     }
//   }
//   printf("\t DONE! \n");
// }

// void saveListToDat(Node **root, Node **cur, Node **tail, char *filename)
// {
//   printf("\t SAVE TO DAT \n");
//   FILE *f = fopen(filename, "wb");
//   if (f == NULL)
//   {
//     printf("Khong the mo file nen khong the luu file.\n");
//     return;
//   }

//   for (*cur = *root; *cur != NULL; *cur = (*cur)->next)
//   {
//     fwrite(&((*cur)->ele), sizeof(elementtype), 1, f);
//   }
//   printf("Da luu file thanh cong!\n");
//   fclose(f);
// }