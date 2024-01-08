#include <stdio.h>
#include <stdlib.h>

// Definizione della struttura del nodo
struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};

struct DoublyLinkedList {
    struct Node* head;
    struct Node* tail;
};

struct Node* createNode(int data)
 {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void append(struct DoublyLinkedList* list, int data) {
    struct Node* newNode = createNode(data);
    
    if (list->head == NULL) 
    {
        list->head = newNode;
        list->tail = newNode;
    } else 
    {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

void append_before_item(struct DoublyLinkedList* list, struct Node* node, int data) {

    if (node == NULL)
    {
       return;
    }

    struct Node* newNode = createNode(data);

    //metto come predente del nuovo node, il precedente del nodo passatogli
    newNode->prev = node->prev;

    //al nodo successivo assegno effettivamente il nodo che gli viene passato
    newNode->next = node;

    if (node->prev != NULL)
    {   //diciamo al nodo successivo del nuovo nodo, che il suo precedente sarà il nuovo nodo, in modo da mantenere collegate le due liste
        node->prev->next = newNode;
    }
    else
    {
        list->head = newNode;
    }

    node->prev = newNode;
   
}


void append_after_item(struct DoublyLinkedList* list, struct Node* node, int data) {
    if (node == NULL) 
    {
        return;
    }

    struct Node* newNode = createNode(data);

    newNode->prev = node;
    newNode->next = node->next;

    if (node->next != NULL) 
    {
        node->next->prev = newNode;
    } else 
    {
        list->tail = newNode;
    }

    node->next = newNode;
}


void removeNode(struct DoublyLinkedList* list, int data) {

    struct Node* current = list->head;

    //controllo che il nodo current non sia nullo
    while (current != NULL) 
    {   
        //controllo che il dato che ho nei data corrisponda con quello da rimuovere
        if (current->data == data) 
        {
            //controllo se ho un nodo precendente
            if (current->prev != NULL) 
            {
                //se si imposto il nodo precedente al successivo
                current->prev->next = current->next;
            } else 
            { //altrimenti imposto la testa della lista al nodo successivo a quello corrente
                list->head = current->next;
            }
            // controllo se il nodo nel quali mi trovo ha un successivo
            if (current->next != NULL) 
            {   //se si imposto il puntatore precedente al nodo successivo al nodo precedente di quello corrente
                current->next->prev = current->prev;
            } else
             {
                //altrimenti se il nodo corrente è la coda della lista imposto la coda della lista al nodo precedente del nodo corrente
                list->tail = current->prev;
            }

            //libero la memoria
            free(current); 
            return; 
        }
    //se il nodo non è quello da rimuovere passo avanti
        current = current->next;
    }

    printf("L'elemento %d non e' presente nella lista.\n", data);
}

void printList(struct Node* head) {

    struct Node* current = head;

    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {

    struct DoublyLinkedList my_list = {NULL, NULL};

    append(&my_list, 1);
    append(&my_list, 3);
    append(&my_list, 5);


    printf("Parte 1 dell'esercizio delle doubly:\n ");

    printf("Lista originale:\n");
    printList(my_list.head);

    int elemento_da_rimuovere = 5;
    removeNode(&my_list, elemento_da_rimuovere);
    printf("Lista dopo la rimozione dell'elemento %d:\n", elemento_da_rimuovere);
    printList(my_list.head);
    printf("\n");
    append(&my_list, 5);

    printf("Parte 2 e 3 dell'esericizio delle doubly:\n");
    printf("Lista originale:\n");
    printList(my_list.head);

    append_before_item(&my_list, my_list.head->next, 2);

    printf("Lista con inserimento prima del 3: ");
    printList(my_list.head);

    append_after_item(&my_list, my_list.head->next->next, 4);

    printf("Lista dopo l'inserimento dopo 3: ");
    printList(my_list.head);


    return 0;
}