#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LinkedList.h"

int main()
{
    srand(time(0));
    ListNode *head = NULL;
    push(&head, rand());
    push(&head, rand());
    push(&head, rand());
    push(&head, rand());
    push(&head, rand());
    push(&head, rand());
    push(&head, rand());

    printf("Unsorted:\n");
    printLinkedList(head);
    head = sortLinkedList(head);
    printf("Sorted:\n");
    printLinkedList(head);

    while(head != NULL)
    {
        ListNode *temp = head;
        head = head->next;
        free(temp);
    }
}
