/*
Program        : Manajemen Kupon Cuci Mobil
Deskripsi      : Program ini mengelola sistem kupon untuk pelanggan cuci mobil
                 termasuk pembuatan, penggunaan, dan validasi kupon
Pembuat        : Revaldi Ardhi Prasetyo
NIM            : 241511057
*/

#ifndef KUPON_H
#define KUPON_H

#define MAX_KUPON 200
#define MAX_RIWAYAT_KUPON 30

typedef struct {
    char platNomor[20];
    char jenisMobil[10];
    int jumlahKupon;
    char waktuCuci[MAX_RIWAYAT_KUPON][20];
    int riwayatCount;
} DataKupon;

extern DataKupon dataKupon[MAX_KUPON];
extern int jumlahDataKupon;

// Fungsi kupon
void normalisasiPlat(const char* src, char* dst);
int cariKuponPlat(const char* platNomor);
void loadKuponDariFile();
void tambahKuponPlat(const char* platNomor, const char* jenisMobil, const char* waktuCuci);
int cekDanGunakanKupon(const char* platNomor);
void simpanKuponKeFile();
void menuKupon();

#endif