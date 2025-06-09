#include "../header/antrian.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Variabel antrian global
extern NodeAntrian* antrianVIP;
extern NodeAntrian* antrianReguler;
extern NodeAntrian* antrianPembilasanVIP;
extern NodeAntrian* antrianPembilasanReguler;
extern NodeAntrian* antrianPengeringanVIP;
extern NodeAntrian* antrianPengeringanReguler;

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

// Proses satu mobil: Cuci -> Bilas -> Kering -> Selesai
Mobil selesaikanAntrian() {
    Mobil m = {-1}; // default jika tidak ada proses

    // 1. Proses dari antrian cuci ke pembilasan
    if (antrianVIP != NULL) {
        m = dequeue(&antrianVIP);
        printf("Mobil ID %d selesai Cuci VIP -> lanjut Bilas VIP.\n", m.id);
        enqueue(&antrianPembilasanVIP, m);
        m.id = -1; // Jangan dikirim ke riwayat
        return m;
    }
    if (antrianReguler != NULL) {
        m = dequeue(&antrianReguler);
        printf("Mobil ID %d selesai Cuci Reguler -> lanjut Bilas Reguler.\n", m.id);
        enqueue(&antrianPembilasanReguler, m);
        m.id = -1;
        return m;
    }
    // 2. Proses dari pembilasan ke pengeringan
    if (antrianPembilasanVIP != NULL) {
        m = dequeue(&antrianPembilasanVIP);
        printf("Mobil ID %d selesai Bilas VIP -> lanjut Pengeringan VIP.\n", m.id);
        enqueue(&antrianPengeringanVIP, m);
        m.id = -1;
        return m;
    }
    if (antrianPembilasanReguler != NULL) {
        m = dequeue(&antrianPembilasanReguler);
        printf("Mobil ID %d selesai Bilas Reguler -> lanjut Pengeringan Reguler.\n", m.id);
        enqueue(&antrianPengeringanReguler, m);
        m.id = -1;
        return m;
    }
    // 3. Proses dari pengeringan
    if (antrianPengeringanVIP != NULL) {
        m = dequeue(&antrianPengeringanVIP);
        printf("Mobil ID %d selesai Pengeringan VIP -> masuk Riwayat.\n", m.id);
        return m;
    }
    if (antrianPengeringanReguler != NULL) {
        m = dequeue(&antrianPengeringanReguler);
        printf("Mobil ID %d selesai Pengeringan Reguler -> masuk Riwayat.\n", m.id);
        return m;
    }
    printf("Tidak ada antrian yang sedang diproses.\n");
    return m;
}

// Menampilkan semua antrian secara menyeluruh
void tampilAntrian() {
    printf("\n======= ANTRIAN VIP =======\n");
    printQueue(antrianVIP, "VIP");

    printf("\n======= ANTRIAN REGULER =======\n");
    printQueue(antrianReguler, "Reguler");

    printf("\n======= PEMBILASAN VIP =======\n");
    printQueue(antrianPembilasanVIP, "Pembilasan VIP");

    printf("\n======= PEMBILASAN REGULER =======\n");
    printQueue(antrianPembilasanReguler, "Pembilasan Reguler");

    printf("\n======= PENGERINGAN VIP =======\n");
    printQueue(antrianPengeringanVIP, "Pengeringan VIP");

    printf("\n======= PENGERINGAN REGULER =======\n");
    printQueue(antrianPengeringanReguler, "Pengeringan Reguler");
}