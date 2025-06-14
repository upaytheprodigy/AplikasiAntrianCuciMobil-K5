#include "../header/riwayat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void printRiwayat(NodeRiwayat* head) {
    printf("\n===== RIWAYAT CUCI MOBIL =====\n");
    if (head == NULL) {
        printf("Belum ada riwayat.\n");
        return;
    }
    NodeRiwayat* temp = head;
    while (temp != NULL) {
        printf("ID: %d | Nama: %s | Jenis: %s | Plat: %s\n",
            temp->data.id,
            temp->data.nama,
            temp->data.jenisMobil,
            temp->data.platNomor
        );
        temp = temp->next;
    }
}

void printRiwayatTerbalik(NodeRiwayat* tail) {
    printf("\n===== RIWAYAT CUCI MOBIL (TERBALIK) =====\n");
    if (tail == NULL) {
        printf("Belum ada riwayat.\n");
        return;
    }
    NodeRiwayat* temp = tail;
    while (temp->next != NULL) {
        temp = temp->next; // cari node paling akhir
    }
    while (temp != NULL) {
        printf("ID: %d | Nama: %s | Jenis: %s | Plat: %s\n",
            temp->data.id,
            temp->data.nama,
            temp->data.jenisMobil,
            temp->data.platNomor
        );
        temp = temp->prev;
    }
}

void cariRiwayatMobil(NodeRiwayat* head, int mode, const char* keyword) {
    int found = 0;
    NodeRiwayat* temp = head;
    while (temp != NULL) {
        int cocok = 0;
        switch (mode) {
            case 1: // Nama
                if (strstr(temp->data.nama, keyword)) cocok = 1;
                break;
            case 2: // Jenis Mobil
                if (strcmp(temp->data.jenisMobil, keyword) == 0) cocok = 1;
                break;
            case 3: // Plat Nomor
                if (strstr(temp->data.platNomor, keyword)) cocok = 1;
                break;
            case 4: // Jalur
                if (strcmp(temp->data.jalur, keyword) == 0) cocok = 1;
                break;
        }
        if (cocok) {
            printf("ID: %d | Nama: %s | Jenis: %s | Plat: %s | Jalur: %s\n",
                temp->data.id,
                temp->data.nama,
                temp->data.jenisMobil,
                temp->data.platNomor,
                temp->data.jalur
            );
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) {
        printf("Data tidak ditemukan.\n");
    }
}