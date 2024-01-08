#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define AIV_LIST(x) &(x.list_item)

struct aiv_list_item
{
    struct aiv_list_item* next;
    unsigned int count;
};

struct aiv_int_item
{
    struct aiv_list_item list_item;
    int value;
};
struct aiv_list_item* aiv_list_get_tail(struct aiv_list_item* head)
{
    if (!head)
    {
        return NULL;
    }

    struct aiv_list_item* current_item = head;
    struct aiv_list_item* last_item = head;
    while(current_item)
    {
        last_item = current_item;
        current_item = current_item->next;
    }

    return last_item;  
    
};

struct aiv_list_item* aiv_list_append(struct aiv_list_item** head, struct aiv_list_item* item)
{
    struct aiv_list_item* tail = aiv_list_get_tail(*head);
    if (!tail)
    {
        *head = item;
        (*head)->count = 1;
    }
    else
    {
        tail->next = item;
        (*head)->count++;
    }

    item->next = NULL;

    return item;
};

void removebyIndex(struct aiv_list_item** head, int index) {
    if (*head == NULL)
    { //se la lista non comprende nodi segnalo che è vuota
        printf("La lista e' vuota.\n");
        return;
    }

    struct aiv_list_item* temp = *head;

    // Se l'indice è 0, rimuovo il primo nodo
    if (index == 0) {
        *head = temp->next;
        return;
    }

    // Trova il nodo precedente al nodo da rimuovere
    for (int i = 0; temp != NULL && i < index - 1; i++) {
        temp = temp->next;
    }

    // Se l'indice è fuori dalla dimensione della lista stampo che l'indice è OutOfRange
    if (temp == NULL || temp->next == NULL) {
        printf("Indice non disponibile.\n");
        return;
    }
    // aggiungo il nodo precedente a quello successivo
    struct aiv_list_item* next_node = temp->next->next;
    temp->next = next_node;
}

void printList(struct aiv_list_item* head) {
    while (head != NULL) 
    { 
        printf("%d\n", head->count);
        head = head->next;
    }
    printf("\n");
}



int main(int argc, char** argv)
{
    struct aiv_list_item* head = NULL;

    struct aiv_int_item int_item;
    int_item.value = 100;
    aiv_list_append(&head, AIV_LIST(int_item));

    struct aiv_int_item int_item2;
    int_item2.value = 101;
    aiv_list_append(&head, AIV_LIST(int_item2));

   struct aiv_int_item int_item3;
    int_item3.value = 1222;
    aiv_list_append(&head, AIV_LIST(int_item3));

    printf("Lista prima della rimozione dall'indice: \n");
    printList(head);

    int indexToRemove = 4;
    removebyIndex(&head, indexToRemove);
    printf("\n");
    printf("Lista dopo la rimozione all'indice %d: \n", indexToRemove);
    printList(head);

    return 0;
}