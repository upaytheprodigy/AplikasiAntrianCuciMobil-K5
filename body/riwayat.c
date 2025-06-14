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

void simpanRiwayatKeFile(NodeRiwayat* head) {
    FILE* file = fopen("../riwayat/riwayat.txt", "w");
    if (!file) {
        printf("Gagal membuka file riwayat/riwayat.txt\n");
        return;
    }
    NodeRiwayat* temp = head;
    while (temp != NULL) {
        fprintf(file, "ID: %d | Nama: %s | Jenis: %s | Plat: %s\n",
            temp->data.id,
            temp->data.nama,
            temp->data.jenisMobil,
            temp->data.platNomor
        );
        temp = temp->next;
    }
    fclose(file);
}