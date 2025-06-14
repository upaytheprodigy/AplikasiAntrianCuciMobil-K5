#include "jalur.h"
#include "mobil.h"
#include <stdio.h>
#include <string.h>

// Inisialisasi semua jalur (cuci, bilas, kering)
void inisialisasiJalur(JalurCuci jalurCuci[], JalurCuci jalurBilas[], JalurCuci jalurKering[]) {
    // Jalur Cuci: 4 (2 Reguler, 2 VIP)
    for (int i = 0; i < 4; i++) {
        jalurCuci[i].sedangDigunakan = 0;
        jalurCuci[i].idJalur = i;
        jalurCuci[i].waktuSelesai = 0;
        snprintf(jalurCuci[i].tipe, sizeof(jalurCuci[i].tipe), (i < 2) ? "Reguler" : "VIP");
    }
    // Jalur Bilas: 2 (0: Reguler, 1: VIP)
    for (int i = 0; i < 2; i++) {
        jalurBilas[i].sedangDigunakan = 0;
        jalurBilas[i].idJalur = i;
        jalurBilas[i].waktuSelesai = 0;
        snprintf(jalurBilas[i].tipe, sizeof(jalurBilas[i].tipe), (i == 0) ? "Reguler" : "VIP");
    }
    // Jalur Kering: 2 (0: Reguler, 1: VIP)
    for (int i = 0; i < 2; i++) {
        jalurKering[i].sedangDigunakan = 0;
        jalurKering[i].idJalur = i;
        jalurKering[i].waktuSelesai = 0;
        snprintf(jalurKering[i].tipe, sizeof(jalurKering[i].tipe), (i == 0) ? "Reguler" : "VIP");
    }
}

// Cari jalur kosong berdasarkan tipe (untuk cuci/bilas/kering)
int cariJalurKosong(JalurCuci jalur[], int jumlah, const char* tipe) {
    for (int i = 0; i < jumlah; i++) {
        if (jalur[i].sedangDigunakan == 0 && strcmp(jalur[i].tipe, tipe) == 0) {
            return i;
        }
    }
    return -1;
}

// Set jalur dengan mobil dan waktu selesai
bool setJalur(JalurCuci* jalur, Mobil m, int waktuSelesai) {
    if (jalur->sedangDigunakan == 0) {
        jalur->mobilSedangDicuci = m;
        jalur->waktuSelesai = waktuSelesai;
        jalur->sedangDigunakan = 1;
        return true;
    }
    return false;
}

// Kosongkan jalur
void kosongkanJalur(JalurCuci* jalur) {
    jalur->sedangDigunakan = 0;
    jalur->waktuSelesai = 0;
}

// Tampilkan status semua jalur
void tampilkanAntrian(JalurCuci jalurCuci[], JalurCuci jalurBilas[], JalurCuci jalurKering[]) {
    printf("\n=== Status Jalur Cuci ===\n");
    for (int i = 0; i < 4; i++) {
        printf("Cuci %d (%s): %s\n", i + 1, jalurCuci[i].tipe,
               jalurCuci[i].sedangDigunakan ? "Sedang Digunakan" : "Kosong");
        if (jalurCuci[i].sedangDigunakan) {
            printf("  Mobil: %s | Plat: %s | Waktu Selesai: %d menit\n",
                   jalurCuci[i].mobilSedangDicuci.nama, jalurCuci[i].mobilSedangDicuci.platNomor,
                   jalurCuci[i].waktuSelesai);
        }
    }
    printf("\n=== Status Jalur Bilas ===\n");
    for (int i = 0; i < 2; i++) {
        printf("Bilas %d (%s): %s\n", i + 1, jalurBilas[i].tipe,
               jalurBilas[i].sedangDigunakan ? "Sedang Digunakan" : "Kosong");
        if (jalurBilas[i].sedangDigunakan) {
            printf("  Mobil: %s | Plat: %s | Waktu Selesai: %d menit\n",
                   jalurBilas[i].mobilSedangDicuci.nama, jalurBilas[i].mobilSedangDicuci.platNomor,
                   jalurBilas[i].waktuSelesai);
        }
    }
    printf("\n=== Status Jalur Kering ===\n");
    for (int i = 0; i < 2; i++) {
        printf("Kering %d (%s): %s\n", i + 1, jalurKering[i].tipe,
               jalurKering[i].sedangDigunakan ? "Sedang Digunakan" : "Kosong");
        if (jalurKering[i].sedangDigunakan) {
            printf("  Mobil: %s | Plat: %s | Waktu Selesai: %d menit\n",
                   jalurKering[i].mobilSedangDicuci.nama, jalurKering[i].mobilSedangDicuci.platNomor,
                   jalurKering[i].waktuSelesai);
        }
    }
}

// Cek apakah semua jalur penuh (untuk cuci/bilas/kering)
bool semuaJalurPenuh(JalurCuci jalur[], int jumlah, const char* tipe) {
    for (int i = 0; i < jumlah; i++) {
        if (jalur[i].sedangDigunakan == 0 && strcmp(jalur[i].tipe, tipe) == 0) {
            return false;
        }
    }
    return true;
}
