/*
Program         : Antrian Cuci Mobil
Deskripsi       : Program ini mengelola antrian cuci mobil dengan menggunakan 
                  struktur data Queue untuk menangani antrian mobil VIP dan
                  Reguler, serta proses perpindahan antar tahap pencucian
Pembuat         : Rina Permata Dewi
NIM             : 241511061
*/

#ifndef ANTRIAN_H
#define ANTRIAN_H
#include "mobil.h"
#include "jalur.h"

typedef struct NodeAntrian {
    Mobil data;
    struct NodeAntrian* next;
} NodeAntrian;

// Fungsi antrian
void enqueue(NodeAntrian** front, Mobil data);
void printQueue(NodeAntrian* front, const char* jenis);
Mobil* findMobil(NodeAntrian* front, int mode, const char* keyword);
void batalkanAntrian(int mode, const char* keyword);
void prosesAntrianOtomatis(JalurCuci jalurCuci[], JalurCuci jalurBilas[], JalurCuci jalurKering[], int tampilPesan);
void* loopProsesAntrian(void* arg);
void tampilkanSemuaJalur(JalurCuci* jalurCuci, int nCuci, JalurCuci* jalurBilas, int nBilas, JalurCuci* jalurKering, int nKering);

// Global antrian
extern NodeAntrian* antrianVIP;
extern NodeAntrian* antrianReguler;
extern NodeAntrian* antrianPembilasanVIP;
extern NodeAntrian* antrianPembilasanReguler;
extern NodeAntrian* antrianPengeringanVIP;
extern NodeAntrian* antrianPengeringanReguler;

#endif