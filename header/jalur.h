#ifndef JALUR_H
#define JALUR_H

#include "mobil.h"
#include <stdbool.h>

// Jumlah total jalur cuci
#define TOTAL_JALUR 4

// Indeks jalur untuk referensi
#define REGULER_1 0
#define REGULER_2 1
#define VIP_1     2
#define VIP_2     3

typedef struct {
    int sedangDigunakan;// 0 = kosong, 1 = sibuk
    char tipe[10];// "VIP" atau "Reguler"
    int idJalur;// 0 s/d 3
    Mobil mobilSedangDicuci;// Mobil yang sedang diproses
    int waktuSelesai;// Waktu selesai pencucian (dalam menit)
} JalurCuci;

void inisialisasiJalur(JalurCuci jalurCuci[], JalurCuci jalurBilas[], JalurCuci jalurKering[]); // Inisialisasi jalur cuci
int cariJalurKosong(JalurCuci jalur[],int jumlah, const char* tipe); // Cari jalur kosong berdasarkan tipe (VIP/Reguler)
bool setJalur(JalurCuci* jalur, Mobil m, int waktuSelesai); // Set jalur cuci dengan mobil dan waktu selesai
void kosongkanJalur(JalurCuci* jalur); // Kosongkan jalur cuci
void tampilkanAntrian(JalurCuci jalur[], JalurCuci jalurBilas[], JalurCuci jalurKering[]); // Tampilkan status semua jalur cuci
bool semuaJalurPenuh(JalurCuci jalur[],int jumlah, const char* tipe); //OPSIONAL, untuk cek apakah semua jalur VIP atau Reguler sudah penuh
void selesaikanAntrian(JalurCuci jalur[], int idJalur); // Selesaikan antrian pada jalur tertentu
#endif
