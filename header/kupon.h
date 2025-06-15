#ifndef KUPON_H
#define KUPON_H

#define MAX_KUPON 200
#define MAX_RIWAYAT_KUPON 30

typedef struct {
    char platNomor[20];
    char jenisMobil[10];
    int jumlahKupon;
    char waktuCuci[MAX_RIWAYAT_KUPON][20]; // waktu cuci (format "HH:MM:SS")
    int riwayatCount;
} DataKupon;

extern DataKupon dataKupon[MAX_KUPON];
extern int jumlahDataKupon;

// Tambah kupon berdasarkan plat nomor
void tambahKuponPlat(const char* platNomor, const char* jenisMobil, const char* waktuCuci);

// Cari index kupon berdasarkan plat nomor
int cariKuponPlat(const char* platNomor);

// Cek dan tawarkan penggunaan kupon saat registrasi
int cekDanGunakanKupon(const char* platNomor);

// Tampilkan menu data kupon
void menuKupon();

#endif