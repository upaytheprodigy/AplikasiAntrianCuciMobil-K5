/*
Program        : Manajemen Data Mobil
Deskripsi      : Program ini mengelola data mobil yang akan dicuci termasuk
                input data mobil baru dan pengecekan status mobil dalam sistem
Pembuat        : Rina Permata Dewi
NIM            : 241511061
*/

#ifndef MOBIL_H
#define MOBIL_H

#include <time.h>

typedef struct {
    int id;
    char nama[50];
    char jenisMobil[10];
    char platNomor[20];
    char jalur[10];         // VIP / Reguler
    int waktuDatang;
    int waktuMulaiCuci;
    int waktuSelesai;
    int estimasiSelesai;
    char waktuDatangStr[20];  
    char waktuMulaiCuciStr[20]; 
    char waktuSelesaiStr[20]; 
    char estimasiSelesaiStr[20];
    time_t waktuSelesaiEpoch;
    int durasiCuci, durasiBilas, durasiKering; // durasi total
    int statusProses;       // 0 = antri, 1 = cuci, 2 = bilas, 3 = selesai
} Mobil;

// Fungsi utama pendaftaran mobil
void tambahMobil();

#endif