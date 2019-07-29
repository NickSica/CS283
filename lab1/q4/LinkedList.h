#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct ListNode;
typedef struct ListNode ListNode;
struct ListNode {
    int data;
    struct ListNode *next;
};

void push(ListNode **head, int data);
ListNode *sortLinkedList(ListNode *head);
ListNode *swapNodes(ListNode *i, ListNode *smallest, ListNode *head);
void printLinkedList(ListNode *head);

#endif
