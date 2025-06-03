#include "../header/antrian.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Variabel antrian global
extern NodeAntrian* antrianVIP;
extern NodeAntrian* antrianReguler;
extern NodeAntrian* antrianBilas;
extern NodeAntrian* antrianKering;
extern NodeAntrian* riwayat;

// Tambah mobil ke antrian (belakang)
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

// Hapus mobil dari depan antrian dan kembalikan datanya
Mobil dequeue(NodeAntrian** front) {
    Mobil kosong = {-1}; // return nilai default jika kosong
    if (*front == NULL) return kosong;

    NodeAntrian* temp = *front;
    Mobil data = temp->data;
    *front = temp->next;
    free(temp);
    return data;
}

// Tampilkan semua isi antrian dengan format
void printQueue(NodeAntrian* front, const char* jenis) {
    if (front == NULL) {
        printf("Antrian %s kosong.\n", jenis);
        return;
    }
    int i = 1;
    NodeAntrian* temp = front;
    while (temp != NULL) {
        printf("%d. ID: %d | Nama: %s | Jenis: %s | Plat: %s\n",
            i, temp->data.id, temp->data.nama,
            temp->data.jenisMobil, temp->data.platNomor);
        temp = temp->next;
        i++;
    }
}

// Hitung jumlah elemen dalam antrian
int countQueue(NodeAntrian* front) {
    int count = 0;
    while (front != NULL) {
        count++;
        front = front->next;
    }
    return count;
}

// Cari mobil berdasarkan ID
Mobil* findMobil(NodeAntrian* front, int id) {
    while (front != NULL) {
        if (front->data.id == id)
            return &front->data;
        front = front->next;
    }
    return NULL;
}

// Hapus mobil dari antrian berdasarkan ID
void deleteMobil(NodeAntrian** front, int id) {
    NodeAntrian* temp = *front;
    NodeAntrian* prev = NULL;

    while (temp != NULL) {
        if (temp->data.id == id) {
            if (prev == NULL) {
                *front = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            printf("Mobil ID %d berhasil dibatalkan dan dihapus dari antrian.\n", id);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Mobil dengan ID %d tidak ditemukan dalam antrian.\n", id);
}

// Proses satu mobil: Cuci → Bilas → Kering → Selesai
void selesaikanAntrian() {
    Mobil m;
    // Prioritaskan antrian VIP dulu
    if (antrianVIP != NULL) {
        m = dequeue(&antrianVIP);
    } else if (antrianReguler != NULL) {
        m = dequeue(&antrianReguler);
    } else if (antrianBilas != NULL) {
        m = dequeue(&antrianBilas);
        printf("Mobil ID %d selesai tahap Bilas → lanjut Kering.\n", m.id);
        enqueue(&antrianKering, m);
        return;
    } else if (antrianKering != NULL) {
        m = dequeue(&antrianKering);
        printf("Mobil ID %d selesai tahap Kering → masuk Riwayat.\n", m.id);
        enqueue(&riwayat, m);
        return;
    } else {
        printf("Tidak ada antrian yang sedang diproses.\n");
        return;
    }

    // Masuk ke tahap bilas
    printf("Mobil ID %d selesai Cuci → lanjut Bilas.\n", m.id);
    enqueue(&antrianBilas, m);
}

// Menampilkan semua antrian secara menyeluruh
void tampilAntrian() {
    printf("\n======= ANTRIAN VIP =======\n");
    printQueue(antrianVIP, "VIP");

    printf("\n======= ANTRIAN REGULER =======\n");
    printQueue(antrianReguler, "Reguler");

    printf("\n======= PROSES BILAS =======\n");
    printQueue(antrianBilas, "Bilas");

    printf("\n======= PROSES KERING =======\n");
    printQueue(antrianKering, "Kering");

    printf("\n======= SELESAI (RIWAYAT) =======\n");
    printQueue(riwayat, "Riwayat");
}
