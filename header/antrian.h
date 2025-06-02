#ifndef ANTRIAN_H
#define ANTRIAN_H
#include "mobil.h"

typedef struct NodeAntrian {
    Mobil data;
    struct NodeAntrian* next;
} NodeAntrian;

// Prototipe fungsi
void enqueue(NodeAntrian** front, Mobil data); // Menambahkan mobil ke antrian
Mobil dequeue(NodeAntrian** front); // Mengambil mobil dari antrian
void printQueue(NodeAntrian* front, const char* jenis); // Menampilkan antrian berdasarkan jenis (VIP/Reguler)
int countQueue(NodeAntrian* front);  // OPSIONAL, untuk membandingkan reguler 1 dan 2
Mobil* findMobil(NodeAntrian* front, int id);  // untuk pembatalan
void deleteMobil(NodeAntrian** front, int id); // untuk menghapus mobil dari queue saat dibatalkan
void selesaikanAntrian(); // Proses bertahap: cuci → bilas → kering → riwayat
void tampilAntrian(); // Tampilkan semua antrian: VIP, Reguler, Pembilasan, Pengeringan

#endif
