#include "LinkedList.h"

void push(ListNode **head, int data)
{
    ListNode *newNode = malloc(sizeof(ListNode));
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
}

ListNode *sortLinkedList(ListNode *head)
{
    ListNode *i = head;
    while(i->next != NULL)
    {
        ListNode *j = i->next;
        ListNode *smallest = i;
        if(j->data < i->data)
        {
            smallest = j;
        }

        while(j != NULL)
        {
	        if(j->data < smallest->data)
            {
                smallest = j;
            }
	        j = j->next;
        }
        ListNode *curr = i;
	    head = swapNodes(i, smallest, head);
        i = smallest->next;
    }
    return head;
}

ListNode *swapNodes(ListNode *i, ListNode *smallest, ListNode *head)
{
    if(smallest != i)
    {
        ListNode *currNode = head; 
        ListNode *beforeI = head;
        while(currNode->next != smallest)
        {
            if(currNode->next == i)
            {
                beforeI = currNode;
            }
            currNode = currNode->next;
        }

        if(head == i)
        {   
            head = smallest;
        }
        else
        {
            beforeI->next = smallest;
        }

        if(i->next == smallest)
        {
            i->next = smallest->next;
            smallest->next = i;
        }
        else
        {
            currNode->next = i;
            ListNode *temp = smallest->next;
            smallest->next = i->next;
            i->next = temp;
        }
    }

    return head;
}

void printLinkedList(ListNode *head)
{
    int i = 1;
    ListNode *list = head;
    while(list != NULL)
    {
        printf("Element %d: %d\n", i, list->data);
        list = list->next;
        i++;
    }
    printf("\n");
}


