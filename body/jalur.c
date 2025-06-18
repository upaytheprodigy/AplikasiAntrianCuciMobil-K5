/*
Program        : Jalur Pencucian Mobil
Deskripsi     : Program ini mengelola jalur pencucian mobil VIP dan Reguler
                serta memantau status setiap jalur pencucian yang tersedia
Pembuat        : Revaldi Ardhi Prasetyo
NIM           : 241511057
*/

#include "../header/jalur.h"
#include "../header/mobil.h"
#include "../header/antrian.h"
#include "../header/riwayat.h"
#include "../header/kupon.h"
#include "../header/treewaktu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))

extern time_t waktuMulaiSimulasi;
extern TreeWaktu* rootTreeWaktu;
int modePesanProses = 0;

// ===================== FUNGSI PEMBANTU =====================

// Mutex untuk antrian dan jalur
pthread_mutex_t antrianVIPMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t antrianRegulerMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t jalurCuciMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t jalurBilasMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t jalurKeringMutex = PTHREAD_MUTEX_INITIALIZER;

// Cek tipe jalur
int isVIP(const char* jalur) {
    return strcasecmp(jalur, "VIP") == 0;
}
int isReguler(const char* jalur) {
    return strcasecmp(jalur, "Reguler") == 0;
}

// ===================== INISIALISASI JALUR =====================

void inisialisasiJalur(JalurCuci jalurCuci[], JalurCuci jalurBilas[], JalurCuci jalurKering[]) {
    for (int i = 0; i < TOTAL_JALUR; i++) {
        jalurCuci[i].sedangDigunakan = 0;
        jalurCuci[i].idJalur = i;
        jalurCuci[i].mobilSedangDicuci.id = -1;
        if (i < 2) {
            strcpy(jalurCuci[i].tipe, "Reguler");
        } else {
            strcpy(jalurCuci[i].tipe, "VIP");
        }
    }
    for (int i = 0; i < 2; i++) {
        jalurBilas[i].sedangDigunakan = 0;
        jalurBilas[i].idJalur = i;
        jalurBilas[i].mobilSedangDicuci.id = -1;
        if (i == 0) {
            strcpy(jalurBilas[i].tipe, "Reguler");
        } else {
            strcpy(jalurBilas[i].tipe, "VIP");
        }
    }
    for (int i = 0; i < 2; i++) {
        jalurKering[i].sedangDigunakan = 0;
        jalurKering[i].idJalur = i;
        jalurKering[i].mobilSedangDicuci.id = -1;
        if (i == 0) {
            strcpy(jalurKering[i].tipe, "Reguler");
        } else {
            strcpy(jalurKering[i].tipe, "VIP");
        }
    }
}

// Cari jalur kosong berdasarkan tipe (untuk cuci/bilas/kering)
int cariJalurKosong(JalurCuci jalur[], int jumlah, const char* tipe) {
    for (int i = 0; i < jumlah; i++) {
        if (!jalur[i].sedangDigunakan && strcmp(jalur[i].tipe, tipe) == 0) {
            return i;
        }
    }
    return -1; // Tidak ada jalur kosong
}

// ===================== PROSES UTAMA (THREAD) =====================

// Fungsi thread untuk proses cuci
void* prosesCuci(void* arg) {
    JalurCuci* jalur = (JalurCuci*)arg;
    // Simpan waktu mulai cuci (real time)
    time_t mulai = time(NULL);
    struct tm *localMulai = localtime(&mulai);
    strftime(jalur->mobilSedangDicuci.waktuMulaiCuciStr, sizeof(jalur->mobilSedangDicuci.waktuMulaiCuciStr), "%H:%M:%S", localMulai);

    sleep(jalur->mobilSedangDicuci.durasiCuci);

    // --- PINDAHKAN DULU KE ANTRIAN BILAS ---
    if (isVIP(jalur->mobilSedangDicuci.jalur)) {
        pthread_mutex_lock(&antrianVIPMutex);
        enqueue(&antrianPembilasanVIP, jalur->mobilSedangDicuci);
        pthread_mutex_unlock(&antrianVIPMutex);
        if (modePesanProses)
            printf("Mobil %d (VIP) selesai cuci, masuk antrian bilas VIP\n", jalur->mobilSedangDicuci.id);
    } else if (isReguler(jalur->mobilSedangDicuci.jalur)) {
        pthread_mutex_lock(&antrianRegulerMutex);
        enqueue(&antrianPembilasanReguler, jalur->mobilSedangDicuci);
        pthread_mutex_unlock(&antrianRegulerMutex);
        if (modePesanProses)
            printf("Mobil %d (Reguler) selesai cuci, masuk antrian bilas Reguler\n", jalur->mobilSedangDicuci.id);
    }

    // --- BARU RESET DATA JALUR CUCI ---
    pthread_mutex_lock(&jalurCuciMutex);
    jalur->sedangDigunakan = 0;
    jalur->mobilSedangDicuci.id = -1;
    pthread_mutex_unlock(&jalurCuciMutex);

    pthread_exit(NULL);
    return NULL;
}

// Fungsi thread untuk proses bilas
void* prosesBilas(void* arg) {
    JalurCuci* jalur = (JalurCuci*)arg;
    sleep(jalur->mobilSedangDicuci.durasiBilas);

    // --- PINDAHKAN DULU KE ANTRIAN KERING ---
    if (isVIP(jalur->mobilSedangDicuci.jalur)) {
        pthread_mutex_lock(&antrianVIPMutex);
        enqueue(&antrianPengeringanVIP, jalur->mobilSedangDicuci);
        pthread_mutex_unlock(&antrianVIPMutex);
        if (modePesanProses)
            printf("Mobil %d (VIP) selesai bilas, masuk antrian kering VIP\n", jalur->mobilSedangDicuci.id);
    } else if (isReguler(jalur->mobilSedangDicuci.jalur)) {
        pthread_mutex_lock(&antrianRegulerMutex);
        enqueue(&antrianPengeringanReguler, jalur->mobilSedangDicuci);
        pthread_mutex_unlock(&antrianRegulerMutex);
        if (modePesanProses)
            printf("Mobil %d (Reguler) selesai bilas, masuk antrian kering Reguler\n", jalur->mobilSedangDicuci.id);
    }

    // --- BARU RESET DATA JALUR BILAS ---
    pthread_mutex_lock(&jalurBilasMutex);
    jalur->sedangDigunakan = 0;
    jalur->mobilSedangDicuci.id = -1;
    pthread_mutex_unlock(&jalurBilasMutex);

    pthread_exit(NULL);
    return NULL;
}

// Fungsi thread untuk proses kering
void* prosesKering(void* arg) {
    JalurCuci* jalur = (JalurCuci*)arg;
    sleep(jalur->mobilSedangDicuci.durasiKering);

    // Simpan waktu selesai (real time)
    time_t selesai = time(NULL);
    jalur->mobilSedangDicuci.waktuSelesaiEpoch = selesai;
    struct tm *localSelesai = localtime(&selesai);
    strftime(jalur->mobilSedangDicuci.waktuSelesaiStr, sizeof(jalur->mobilSedangDicuci.waktuSelesaiStr), "%H:%M:%S", localSelesai);

    insertRiwayat(&riwayat, jalur->mobilSedangDicuci);

    tambahKuponPlat(
        jalur->mobilSedangDicuci.platNomor,
        jalur->mobilSedangDicuci.jenisMobil,
        jalur->mobilSedangDicuci.waktuSelesaiStr
    );

    if (modePesanProses)
        printf("Mobil %d selesai dikeringkan, masuk riwayat\n", jalur->mobilSedangDicuci.id);

    // --- RESET DATA JALUR KERING PALING AKHIR ---
    pthread_mutex_lock(&jalurKeringMutex);
    jalur->sedangDigunakan = 0;
    jalur->mobilSedangDicuci.id = -1;
    pthread_mutex_unlock(&jalurKeringMutex);

    pthread_exit(NULL);
    return NULL;
}