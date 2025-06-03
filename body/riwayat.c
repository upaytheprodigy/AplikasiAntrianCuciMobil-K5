#include "../header/riwayat.h"
#include <stdio.h>
#include <stdlib.h>

// Fungsi insert ke riwayat (doubly linked list, di akhir)
void insertRiwayat(NodeRiwayat** head, Mobil data) {
    NodeRiwayat* newNode = (NodeRiwayat*)malloc(sizeof(NodeRiwayat));
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        NodeRiwayat* temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
        newNode->prev = temp;
    }
}