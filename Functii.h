/* Comisaru Gabriel-Cornel - 311CB */
#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED


typedef struct _Node
{
    void* value; ///pointer catre valoarea stocata in nod
    struct _Node* next; ///pointer catre nodul urmator
    struct _List* cuvinte;
}Node;

typedef struct _List
{
    struct _Node* head; ///pointer catre capul de lista
    unsigned int size; ///numarul de elemetne din lista
}List;

typedef struct _hashTable
{
    int size;
    int (*hashCode)(void*);
    List** v;
}HashTable;

typedef struct
{
    char* nume;
    int nraparitii;
}TCuvant;

void freeList(List** list, void (*freeValue)(void*));
HashTable* CreateTable(int size, int (*hashCode)(void*));
List* CreateList();
Node* CreateNode(Node* next, void* value);
void addValueInOrder(List* list, void* value, int (*compare)(void*, void*, int), int contor);
void addValueInOrderCuv(List* list, void* value, int (*compare)(void*, void*, int), int contor);
void addValueInOrderApp(List* list, void* value, int (*compare)(void*, void*, int), int contor);
void freeCuv(void* cuv);
void freeDim(void* dim);
int Cmp(void* v1, void* v2, int contor);
void printList(List* list, int* dim, void (*printElement)(void*, void*));
void printList2(List *list, int n, int *dim, void (*printElement)(void *, void *));



#endif // LINKEDLIST_H_INCLUDED
