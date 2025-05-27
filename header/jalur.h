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

void inisialisasiJalur(JalurCuci jalur[]);
int cariJalurKosong(JalurCuci jalur[], const char* tipe);
bool setJalur(JalurCuci* jalur, Mobil m, int waktuSelesai);
void kosongkanJalur(JalurCuci* jalur);
void tampilkanStatusJalur(JalurCuci jalur[]);
bool semuaJalurPenuh(JalurCuci jalur[], const char* tipe);


#endif
