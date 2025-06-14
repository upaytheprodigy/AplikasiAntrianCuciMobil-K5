#ifndef MOBIL_H
#define MOBIL_H

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
    int durasiCuci, durasiBilas, durasiKering; // durasi total
    int statusProses;       // 0 = antri, 1 = cuci, 2 = bilas, 3 = selesai
} Mobil; // Struktur data untuk menyimpan informasi mobil

void tambahMobil(); // Fungsi untuk input dan proses data mobil

#endif