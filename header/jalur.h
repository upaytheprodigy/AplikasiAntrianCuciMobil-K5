#ifndef JALUR_H
#define JALUR_H

#include "mobil.h"
#include <stdbool.h>
#include <pthread.h>

// Jumlah total jalur cuci
#define TOTAL_JALUR 4

// Indeks jalur untuk referensi
#define REGULER_1 0
#define REGULER_2 1
#define VIP_1     2
#define VIP_2     3

typedef struct {
    int sedangDigunakan;
    char tipe[10];
    int idJalur;
    Mobil mobilSedangDicuci;
    int waktuSelesai;
    pthread_t threadId; // ID thread
} JalurCuci;

// Mutex untuk sinkronisasi
extern pthread_mutex_t antrianVIPMutex;
extern pthread_mutex_t antrianRegulerMutex;
extern pthread_mutex_t jalurCuciMutex;
extern pthread_mutex_t jalurBilasMutex;
extern pthread_mutex_t jalurKeringMutex;

void inisialisasiJalur(JalurCuci jalurCuci[], JalurCuci jalurBilas[], JalurCuci jalurKering[]);
int cariJalurKosong(JalurCuci jalur[], int jumlah, const char* tipe);

void* prosesCuci(void* arg);
void* prosesBilas(void* arg);
void* prosesKering(void* arg);
#endif