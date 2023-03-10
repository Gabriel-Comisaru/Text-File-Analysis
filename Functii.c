/* Comisaru Gabriel-Cornel - 311CB */
#include "Functii.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

HashTable* CreateTable(int size, int (*hashCode)(void*))
{
    HashTable* table = malloc(sizeof(HashTable));
    table->size = size;
    table->hashCode = hashCode;

    table->v = malloc(size*sizeof(List*));
    for(int i = 0; i < size; i++) {
        table->v[i] = CreateList();
        table->v[i]->size = i;
    }
    return table;
}

/**
 * next - pointer catre nodul urmator
 * value - pointer catre valoarea ce trebuie retinuta in nod
 *
 * Functia creeaza si returneaza un nou nod avand elementele initializate cu cele date ca parametru
 *
 **/
Node *CreateNode(Node *next, void *value)
{
    Node *newNode = (Node *)malloc(sizeof(struct _Node)); /// cream un nod nou
    if (newNode == NULL)                                  /// verificam daca alocarea a dat fail (aka lipsa de memorie)
        return NULL;                                      /// daca nu a mers, returnam NULL

    newNode->next = next;   /// facem legatura cu urmatorul nod
    newNode->value = value; /// facem legatura intre
    newNode->cuvinte = NULL;

    return newNode; /// returnam noul nod creat
}
/**
 * Functia aloca memorie pentru o noua lista si initializeaza campurile astfel incat aceasta sa fie goala
 **/
List *CreateList()
{
    List *list = (List *)malloc(sizeof(struct _List)); /// aloc memorie pentru o lista
    list->head = NULL;                                 /// marchez faptul ca este goala punand capul de lista NNULL
    list->size = 0;                                    /// marchez dimensiunea 0 (o lista goala nu are elemente)
    return list;
}
/**
 * list - pointer catre lista in care adaug
 * freeValue - pointer catre fucntia pe care o folosim sa eliberam memoria alocata unui element
 *
 * Functia sterge rand pe rand fiecare nod din lista si valorile corespunzatoare, mai apoi eliberand si memoria alocata listei
 *
 **/
void deleteValue(List *list, int (*condition)(void *), void (*f)(void *))
{
    if (condition(list->head->value) == 1)
    {
        Node *aux = list->head->next;
        f(list->head->value);
        free(list->head);
        list->head = aux;
        return;
    }
    Node *n = list->head;
    while (n->next != NULL)
    {
        if (condition(n->next->value) == 1)
        {
            Node *aux = n->next->next;
            f(n->next->value);
            free(n->next);
            n->next = aux;
        }
        else
            n = n->next;
    }
}
void freeList(List **list, void (*freeValue)(void *))
{
    Node *current = (*list)->head; /// luam nodul de inceput
    Node *next;

    while (current != NULL) /// cat timp mai avem elemente
    {
        next = current->next;      /// salvam nodul urmator
        freeValue(current->value); /// eliberam memoria alocata elementului
        free(current);             /// eliberam nodul curent
        current = next;            /// trecem mai departe
    }

    free(*list); /// eliberam memoria alocata listei
    *list = NULL;
}
/* 
    Functie de comparare a valorilor generic in functie de ce contor primeste functia , in caz
    ca primeste contorul 1, comparatia se face intre doua int-uri si in caz contrar
    comparatia se face intre doua structuri de cuvinte de unde se extrage string-ul de cuvant si
    se compara.
*/
int Cmp(void *v1, void *v2, int contor)
{
    if (contor == 1)
    {
        if (*(int *)v1 > *(int *)v2)
            return 1;
        else if (*(int *)v1 < *(int *)v2)
            return -1;
        else if (*(int *)v1 == *(int *)v2)
            return 0;
    }
    else
    {
        TCuvant *c1 = (TCuvant *)v1;
        TCuvant *c2 = (TCuvant *)v2;
        char *cuvant1 = (char *)(c1->nume);
        char *cuvant2 = (char *)(c2->nume);
        if (strcmp(cuvant1, cuvant2) < 0)
            return -1;
        else if (strcmp(cuvant1, cuvant2) > 0)
            return 1;
        else
            return 0;
    }
    return 0;
}

void printList(List *list, int *dim, void (*printElement)(void *, void *))
{
    Node *current = (Node *)malloc(sizeof(Node));
    current = list->head;
    printf("(");
    int i = 0;
    int *dimensiune = malloc(sizeof(int));
    dimensiune = (int *)dim;
    while (current != NULL)
    {
        if (i == 0)
        {
            printf("%d:", *dimensiune);
        }
        printElement(current->value, dimensiune);
        if (current->next != NULL)
            printf(", ");
        current = current->next;
        i++;
    }
    printf(")");
}
/* am folosit o noua functie de printlist deoarece output-urile pentru
a 3-a variante de print sunt diferite pentru spatiu, virgule s.a.m.d.*/
void printList2(List *list, int n, int *dim, void (*printElement)(void *, void *))
{
    Node *current = (Node *)malloc(sizeof(Node));
    current = list->head; /// pornim de la inceputul listei
    int i = 0, spion = 0;
    int *dimensiune = malloc(sizeof(int));
    dimensiune = (int *)dim;
    while (current != NULL)
    {
        TCuvant* c = (TCuvant*)(current->value);
        int nr = c->nraparitii;
        if (nr <= n) {
            spion = 1;
            if (i == 0)
            {
                printf("(%d: ", *dimensiune);
                i++;
            }
                printElement(current->value, dimensiune);
            if (current->next != NULL)
                printf(", ");
        }
        current = current->next;
    }
    if (spion == 1)
        printf(")");
}

/* aici sunt functiile de addvalue in order. Cel mai probabil puteam sa mi
usurez treaba si sa nu fac 3 functii diferite pentru valoare, cuvinte si 
ordonare dupa nr de aparitii dar asa s-a ajuns si daca incerc sa modific
100% o sa dea seg faultde care nu o sa mai scap - multumesc de intelegere!
(si de rabdarea de a citi aceste comentarii)*/
void addValueInOrder(List *list, void *value, int (*compare)(void *, void *, int), int contor)
{
    contor = 1;
    int *x = malloc(sizeof(int));
    *x = *(int *)value;
    Node *n = malloc(sizeof(Node));
    n = list->head;
    if (list->head == NULL)
    {
        list->head = (Node *)malloc(sizeof(struct _Node));
        list->head->next = NULL;
        list->head->value = x;
        list->head->cuvinte = NULL;
        return;
    }
    if (compare(n->value, value, contor) > 0)
    {
        Node *aux = (Node *)malloc(sizeof(struct _Node));
        aux->value = x;
        aux->next = list->head;
        list->head = aux;
        return;
    }
    while (n->next != NULL && compare(n->next->value, value, contor) < 0)
    {
        n = n->next;
    }
    Node *aux = (Node *)malloc(sizeof(struct _Node));
    aux->value = x;
    aux->next = n->next;
    n->next = aux;
}

void addValueInOrderCuv(List *list, void *value, int (*compare)(void *, void *, int), int contor)
{
    contor = 0;
    Node *n = malloc(sizeof(Node));
    n = list->head;
    if (list->head == NULL)
    {
        list->head = (Node *)malloc(sizeof(struct _Node));
        list->head->next = NULL;
        list->head->value = value;
        list->head->cuvinte = NULL;
        return;
    }
    if (compare(n->value, value, contor) > 0)
    {
        Node *aux = (Node *)malloc(sizeof(struct _Node));
        aux->value = value;
        aux->next = list->head;
        list->head = aux;
        return;
    }
    while (n->next != NULL && compare(n->next->value, value, contor) < 0)
    {
        n = n->next;
    }
    Node *aux = (Node *)malloc(sizeof(struct _Node));
    aux->value = value;
    aux->next = n->next;
    n->next = aux;
}

void addValueInOrderApp(List *list, void *value, int (*compare)(void *, void *, int), int contor)
{
    contor = 1;
    TCuvant *c1 = malloc(sizeof(TCuvant));
    TCuvant *c2 = malloc(sizeof(TCuvant));
    Node *n = malloc(sizeof(Node));
    n = list->head;
    c1 = value;
    c2 = n->value;
    if (compare(&c2->nraparitii, &c1->nraparitii, contor) < 0)
    {
        Node *aux = (Node *)malloc(sizeof(struct _Node));
        aux->value = value;
        aux->next = list->head;
        list->head = aux;
        return;
    }
    if (n->next != NULL)
        c2 = n->next->value;
    while (n->next != NULL && compare(&c2->nraparitii, &c1->nraparitii, contor) > 0)
    {
        n = n->next;
        c2 = n->value;
    }
    Node *aux = (Node *)malloc(sizeof(struct _Node));
    aux->value = value;
    aux->next = n->next;
    n->next = aux;
}
