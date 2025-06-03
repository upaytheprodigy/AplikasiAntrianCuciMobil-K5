#include "../header/antrian.h"
#include <stdio.h>
#include <stdlib.h>

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

void printQueue(NodeAntrian* front, const char* jenis) {
    if (front == NULL) {
        printf("Antrian %s kosong.\n", jenis);
        return;
    }
    int i = 1;
    NodeAntrian* temp = front;
    while (temp != NULL) {
        printf("%d. ID: %d | Nama: %s | Jenis: %s | Plat: %s\n", i, temp->data.id, temp->data.nama, temp->data.jenisMobil, temp->data.platNomor);
        temp = temp->next;
        i++;
    }
}
