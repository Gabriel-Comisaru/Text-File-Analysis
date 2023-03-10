/* Comisaru Gabriel-Cornel - 311CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Functii.h"


int codHash(void *element) /*functie ce transforma cuvantul in cod hash
                            luand ambele cazuri cand prima litera a cuvantului
                            este scrisa ori cu litera mica ori cu majuscula*/
{
    char *nume = (char*)element;
    int afisare = 0;
    if (isupper(nume[0]))
        afisare = (nume[0] - 'A');
    else if (islower(nume[0]))
        afisare = (nume[0] - 'a');
    return afisare;
}

/* functie de inlaturarea semnelor de punctuatie ce primeste ca parametrii
string-ul respectiv si semnele de punctuatie pe care dorim sa le inlaturam.
se va verifica daca in string este prezent unul din semnele de punctuatie si
se va inlocui acel semn cu '\0' caracter null ce va determina sfaristul
string-ului*/
void RemovePunctuation(char *str, const char *strRemove)
{
    int Remove[256] = {0};
    if (NULL == str || NULL == strRemove)
        return;
    while (*strRemove)
    {
        Remove[*strRemove++] = 1;
    }
    char *strDest = str;
    while (*str)
    {
        if (!Remove[*str])
            *strDest++ = *str;
        str++;
    }
    *strDest = '\0';
}

void printCuv(void *cuv, void *size)
{
    TCuvant *cuvant = (TCuvant *)malloc(sizeof(TCuvant));
    cuvant = (TCuvant *)cuv;
    printf("%s/%d", cuvant->nume, cuvant->nraparitii);
}
/* functia de inserare primeste ca si parametrii tabela noastra hash creata in main
precum si cuvantul ce trebuie inserat si functia de comparare ce este distribuita
pe doua cazuri in functie de ce se insereaza*/
void InserareTH(HashTable *t, void *cuv, int (*compare)(void *, void *, int))
{
    TCuvant *cuvant = (TCuvant *)malloc(sizeof(TCuvant));
    cuvant->nume = cuv;
    cuvant->nraparitii = 1;
    int cH = codHash(cuv);
    List *l = (List *)malloc(sizeof(List));
    l = t->v[cH];
    int spion = 0;
    Node *n = (Node *)malloc(sizeof(Node));
    int contor = 1;
    RemovePunctuation(cuv, ".,!"); /* functie de inlaturare a punctuatiei
    (folosita si la tema2 sem1 pclp) cu exceptia "-"*/
    int dim = strlen(cuvant->nume);
    for (n = l->head; n != NULL; n = n->next)
    {
        if (compare(n->value, &dim, contor) == 0)
        {
            n = l->head;
            spion = 1;
            break;
        }
    }
    if (spion == 0) {
        addValueInOrder(l, &dim, compare, contor);
        for (n = l->head; n->next != NULL; n = n->next)
        {
            if (compare(n->value, &dim, contor) == 0)
                break;
        }
        cuvant->nume = cuv;
        if (t->v[cH]->head->next == NULL)
            n = l->head;
        n->cuvinte = CreateList();
    }
    for (n = l->head; n->next != NULL; n = n->next)
    {
        if (compare(n->value, &dim, contor) == 0)
            break;
    }
    cuvant->nume = cuv;
    List *l2 = (List *)malloc(sizeof(List));
    l2 = n->cuvinte;
    Node *n2 = (Node *)malloc(sizeof(Node));
    n2 = l2->head;
    spion = 0;
    for (; n2 != NULL; n2 = n2->next)
    {
        contor = 0;
        if (compare(n2->value, cuvant, contor) == 0)
        {
            TCuvant *c = (TCuvant *)n2->value;
            c->nraparitii++;
            spion = 1;
            break;
        }
    }
    if (spion == 0)
        addValueInOrderCuv(l2, cuvant, compare, contor);
}
/* functie ce ordoneaza cuvintele din lista dupa nr-ul de aparitii,
ele fiind deja ordonate lexicografic prin functia de AddValueInOrder*/
void InOrderApp(List *l)
{
    Node *n;
    TCuvant *aux = malloc(sizeof(TCuvant));
    int spion = 0;
    while (spion == 0)
    {
        spion = 1;
        for (n = l->head; n->next != NULL; n= n->next)
        {

            if (((TCuvant *)(n->value))->nraparitii < ((TCuvant *)n->next->value)->nraparitii)
            {
                aux = n->value;
                n->value = n->next->value;
                n->next->value = aux;
                spion = 0;
            }
        }
    }
}
void PrintTH(HashTable *t, void (*printList)(List *, int *,
void (*)(void *, void *)), void (*printElement)(void *, void *))
{
    for (int i = 0; i < t->size; i++)
    {
        List *l1 = (List *)malloc(sizeof(List));
        l1 = t->v[i];
        Node *n1 = (Node *)malloc(sizeof(Node));
        n1 = l1->head;
        int dim = 0;
        if (l1->head != 0)
        {
            printf("pos %d: ", t->v[i]->size);
            for (; n1 != 0; n1 = n1->next)
            {
                TCuvant *cuv = (TCuvant *)malloc(sizeof(TCuvant));
                cuv = (TCuvant *)(n1->cuvinte->head->value);
                dim = strlen(cuv->nume);
                InOrderApp(n1->cuvinte);
                printList(n1->cuvinte, &dim, printElement);
            }
            printf("\n");
        }
    }
}

void Print2TH(HashTable *t, FILE* in, char* c, void (*printList)(List *, int *,
void (*)(void *, void *)), void (*printElement)(void *, void *),
int (*compare)(void *, void *, int))
{
    int cH = codHash(c);
    int* n = calloc(1, sizeof(int));
    fscanf(in, "%d", n);
    List* l = t->v[cH];
    Node* nl = l->head;
    int contor = 1;
    if (*n >= 3) {
        while (nl != NULL) {
            if (compare(nl->value, n, contor) == 0) {
                List* lc = nl->cuvinte;
                printList(lc, n, printElement);
            }
            nl = nl->next;
        }
        printf("\n");
    }
}

void Print3TH(HashTable *t, void (*printList)(List *, int, int *,
void (*)(void *, void *)), void (*printElement)(void *, void *), int n)
{
    int spion = 0;
    for (int i = 0; i < t->size; i++)
    {
        List *l1 = (List *)malloc(sizeof(List));
        l1 = t->v[i];
        Node *n1 = (Node *)malloc(sizeof(Node));
        n1 = l1->head;
        int dim = 0;
        while (n1 != NULL) {
            for (Node* n2 = n1->cuvinte->head; n2 != NULL; n2 = n2->next) {
                TCuvant* c = (TCuvant*)(n2->value);
                int nr = c->nraparitii;
                if (nr <= n) {
                    printf("pos%d: ", i);
                    spion = 1;
                }
                if (spion == 1)
                    break;
            }
            if (spion == 1)
                break;
            n1 = n1->next;
        }
        if (l1->head != 0)
        {
            for (n1 = l1->head; n1 != 0; n1 = n1->next)
            {
                TCuvant *cuv = (TCuvant *)malloc(sizeof(TCuvant));
                cuv = (TCuvant *)(n1->cuvinte->head->value);
                dim = strlen(cuv->nume);
                InOrderApp(n1->cuvinte);
                printList(n1->cuvinte, n, &dim, printElement);
            }
            if (spion == 1)
            printf("\n");
        }
        spion = 0;
    }
}

int main(int argc, char* argv[])
{
    FILE * in; /*main primeste ca si parametrii
    argumentele procesate de la tastatura si anume
    numele fisierului care este ulterior folosit
    in deschiderea fisierului prin fopen*/
    in = fopen(argv[1], "r");
    if (in == NULL)
        return 1;
    size_t n = 26; /* dimensiunea maxima pe care o
                    pe care o poate avea tabela Hash 
                    (27 de litere in alfabet)*/
    HashTable *t = CreateTable(n, codHash);
    char *comanda, *cuvant;
    comanda = malloc(sizeof(char));
    cuvant = malloc(sizeof(char));
    /* am incercat inlocuirea goto-urilor insa n-am reusit
    rezultatul fiind un seg fault si am zis sa las deoarece
    structura este si ampla cu destule if-uri si else-uri totusi
    sper ca se poate descifra ce am facut*/
    while (fscanf(in, "%s", comanda) != EOF)
    {
        if (strcmp(comanda, "insert") == 0)
        {
        insert:
            while (fscanf(in, "%s", cuvant) == 1)
            {
                if (strcmp(cuvant, "insert") == 0)
                {
                    goto insert;
                }
                if (strcmp(cuvant, "print") == 0)
                    goto print;
                if (strlen(cuvant) >= 3 && isdigit(cuvant[0]) == 0)
                {
                    InserareTH(t, cuvant, Cmp);
                    cuvant = malloc(sizeof(char));
                }
            }
        }
    /* in cazul citirii cuvantului "print" se va verifica daca dupa acesta
    mai exista vreun cuvant si se ia in considerare fiecare caz in parte*/
    print:
        if (fscanf(in, "%s", comanda) == 1)
        {
            if (strcmp(comanda, "insert") == 0)
            {
                PrintTH(t, printList, printCuv);
                goto insert;
            }
            if (strcmp(comanda, "print") == 0) {
                PrintTH(t, printList, printCuv);
                printagain:
                if (fscanf(in, "%s", comanda) == 1) {
                    if (strcmp(comanda, "print") == 0) {
                        PrintTH(t, printList, printCuv);
                        goto printagain;
                    }
                    if (isalpha(comanda[0]) != 0) {
                        Print2TH(t, in, comanda, printList, printCuv, Cmp);
                    }
                    if (isdigit(comanda[0]) != 0) {
                        int n = atoi(comanda);
                        Print3TH(t, printList2, printCuv, n);
                    }
                } else
                    PrintTH(t, printList, printCuv);
            } else
                if (isalpha(comanda[0]) != 0)
                    Print2TH(t, in, comanda, printList, printCuv, Cmp);
                else if (isdigit(comanda[0]) != 0) {
                    int n = atoi(comanda);
                    Print3TH(t, printList2, printCuv, n);
                }
        }
        else
            PrintTH(t, printList, printCuv);
    }
    free(cuvant);
    free(comanda);
    fclose(in);
    return 0;
}
