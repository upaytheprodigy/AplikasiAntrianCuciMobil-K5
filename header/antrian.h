#ifndef ANTRIAN_H
#define ANTRIAN_H
#include "mobil.h"
#include "jalur.h"

typedef struct NodeAntrian {
    Mobil data;
    struct NodeAntrian* next;
} NodeAntrian;

// Prototipe fungsi
void enqueue(NodeAntrian** front, Mobil data); // Menambahkan mobil ke antrian
Mobil dequeue(NodeAntrian** front); // Mengambil mobil dari antrian
void printQueue(NodeAntrian* front, const char* jenis); // Menampilkan antrian berdasarkan jenis (VIP/Reguler)
int countQueue(NodeAntrian* front);  // OPSIONAL, untuk membandingkan reguler 1 dan 2
Mobil* findMobil(NodeAntrian* front, int mode, const char* keyword);  // untuk pembatalan
void deleteMobil(NodeAntrian** front, int id); // untuk menghapus mobil dari queue saat dibatalkan
void batalkanAntrian(int mode, const char* keyword); // untuk membatalkan dan memindahkan mobil ke stack pembatalan
void tampilAntrian(); // Tampilkan semua antrian: VIP, Reguler, Pembilasan, Pengeringan
void prosesAntrianOtomatis(JalurCuci jalurCuci[], JalurCuci jalurBilas[], JalurCuci jalurKering[], int tampilPesan);void* loopProsesAntrian(void* arg); // Thread untuk proses antrian otomatis

extern NodeAntrian* antrianVIP;
extern NodeAntrian* antrianReguler;
extern NodeAntrian* antrianPembilasanVIP;
extern NodeAntrian* antrianPembilasanReguler;
extern NodeAntrian* antrianPengeringanVIP;
extern NodeAntrian* antrianPengeringanReguler;
#endif
