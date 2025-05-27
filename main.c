#include <stdio.h>
#include <stdlib.h>
#include "antrian.h"

void enqueue(NodeAntrian** front, Mobil data) {
    NodeAntrian* newNode = (NodeAntrian*)malloc(sizeof(NodeAntrian));
    newNode->data = data;
    newNode->next = NULL;

    if (*front == NULL) {
        *front = newNode;
    } else {
        NodeAntrian* temp = *front;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
}

Mobil dequeue(NodeAntrian** front) {
    Mobil m = (*front)->data;
    NodeAntrian* temp = *front;
    *front = (*front)->next;
    free(temp);
    return m;
}

void printQueue(NodeAntrian* front) {
    while (front != NULL) {
        printf("%s (%s) -> ", front->data.nama, front->data.platNomor);
        front = front->next;
    }
    printf("NULL\n");
}
