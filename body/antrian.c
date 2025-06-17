#include "../header/antrian.h"
#include "../header/jalur.h"
#include "../header/pembatalan.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))

// Inisialisasi antrian global (VIP & Reguler)
extern NodeAntrian* antrianVIP;
extern NodeAntrian* antrianReguler;
extern NodeAntrian* antrianPembilasanVIP;
extern NodeAntrian* antrianPembilasanReguler;
extern NodeAntrian* antrianPengeringanVIP;
extern NodeAntrian* antrianPengeringanReguler;

extern int modePesanProses;

// ===================== ENQUEUE =====================
void enqueue(NodeAntrian** front, Mobil data) {
    NodeAntrian* newNode = (NodeAntrian*)malloc(sizeof(NodeAntrian));
    newNode->data = data;
    newNode->next = NULL;
    if (*front == NULL) {
        *front = newNode;
    } else {
        NodeAntrian* temp = *front;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// ===================== PRINT QUEUE =====================
void printQueue(NodeAntrian* front, const char* jenis) {
    printf("\n===== ANTRIAN %s =====\n", jenis);
    if (front == NULL) {
        printf("Antrian kosong.\n");
        return;
    }
    NodeAntrian* temp = front;
    while (temp != NULL) {
        printf("ID: %d | Nama: %s | Jenis: %s | Plat: %s\n",
            temp->data.id,
            temp->data.nama,
            temp->data.jenisMobil,
            temp->data.platNomor
        );
        temp = temp->next;
    }
}

// ===================== FIND MOBIL =====================
Mobil* findMobil(NodeAntrian* front, int mode, const char* keyword) {
    NodeAntrian* temp = front;
    while (temp != NULL) {
        switch (mode) {
            case 1: // Cari berdasarkan nama
                if (strcmp(temp->data.nama, keyword) == 0) {
                    return &(temp->data);
                }
                break;
            case 2: // Cari berdasarkan jenis mobil
                if (strcmp(temp->data.jenisMobil, keyword) == 0) {
                    return &(temp->data);
                }
                break;
            case 3: // Cari berdasarkan plat nomor
                if (strcmp(temp->data.platNomor, keyword) == 0) {
                    return &(temp->data);
                }
                break;
            default:
                return NULL; // Mode tidak valid
        }
        temp = temp->next;
    }
    return NULL;
}

// ===================== BATALKAN ANTRIAN =====================
void batalkanAntrian(int mode, const char* keyword) {
    Mobil* mobil = NULL;
    NodeAntrian* prev = NULL;
    NodeAntrian* current = NULL;
    NodeAntrian** antrian = NULL;

    if (mode == 0) {
        printf("Mode tidak valid.\n");
        return;
    }

    // Cari di antrian VIP
    pthread_mutex_lock(&antrianVIPMutex);
    mobil = findMobil(antrianVIP, mode, keyword);
    if (mobil != NULL) {
        antrian = &antrianVIP;
    }
    pthread_mutex_unlock(&antrianVIPMutex);

    // Jika tidak ditemukan di antrian VIP, cari di antrian Reguler
    if (mobil == NULL) {
        pthread_mutex_lock(&antrianRegulerMutex);
        mobil = findMobil(antrianReguler, mode, keyword);
        if (mobil != NULL) {
            antrian = &antrianReguler;
        }
        pthread_mutex_unlock(&antrianRegulerMutex);
    }

    if (mobil != NULL) {
        printf("\nData Mobil yang Ditemukan:\n");
        printf("ID: %d\n", mobil->id);
        printf("Nama: %s\n", mobil->nama);
        printf("Jenis Mobil: %s\n", mobil->jenisMobil);
        printf("Plat Nomor: %s\n", mobil->platNomor);

        char konfirmasi;
        printf("\nApakah Anda yakin ingin membatalkan antrian mobil ini? (y/n): ");
        scanf(" %c", &konfirmasi);
        getchar();

        if (konfirmasi == 'y' || konfirmasi == 'Y') {
            pthread_mutex_lock((antrian == &antrianVIP) ? &antrianVIPMutex : &antrianRegulerMutex);
            current = *antrian;
            prev = NULL;
            while (current != NULL && &(current->data) != mobil) {
                prev = current;
                current = current->next;
            }
            if (current != NULL) {
                if (prev == NULL) {
                    *antrian = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current);
            }
            pthread_mutex_unlock((antrian == &antrianVIP) ? &antrianVIPMutex : &antrianRegulerMutex);

            pushPembatalan(&stackPembatalan, *mobil);
            printf("Mobil berhasil dibatalkan dan dicatat ke riwayat pembatalan.\n");
        } else {
            printf("Pembatalan dibatalkan.\n");
        }
    } else {
        printf("Mobil dengan kriteria tersebut tidak ditemukan dalam antrian cuci. Pembatalan tidak dapat dilakukan.\n");
    }
}

// ===================== PROSES ANTRIAN OTOMATIS =====================
void prosesAntrianOtomatis(JalurCuci jalurCuci[], JalurCuci jalurBilas[], JalurCuci jalurKering[], int tampilPesan) {
    // Proses antrian cuci
    for (int i = 0; i < TOTAL_JALUR; i++) {
        if (!jalurCuci[i].sedangDigunakan) {
            NodeAntrian* mobilNode = NULL;
            if (strcmp(jalurCuci[i].tipe, "VIP") == 0) {
                pthread_mutex_lock(&antrianVIPMutex);
                if (antrianVIP != NULL) {
                    mobilNode = antrianVIP;
                    antrianVIP = antrianVIP->next;
                }
                pthread_mutex_unlock(&antrianVIPMutex);
            } else {
                pthread_mutex_lock(&antrianRegulerMutex);
                if (antrianReguler != NULL) {
                    mobilNode = antrianReguler;
                    antrianReguler = antrianReguler->next;
                }
                pthread_mutex_unlock(&antrianRegulerMutex);
            }

            if (mobilNode != NULL) {
                jalurCuci[i].sedangDigunakan = 1;
                jalurCuci[i].mobilSedangDicuci = mobilNode->data;
                pthread_create(&jalurCuci[i].threadId, NULL, prosesCuci, &jalurCuci[i]);
                if (tampilPesan) {
                    printf("Mobil %d masuk jalur cuci %s\n", jalurCuci[i].mobilSedangDicuci.id, jalurCuci[i].tipe);
                }
                free(mobilNode);
            }
        }
    }

    // Proses antrian bilas
    for (int i = 0; i < 2; i++) {
        if (!jalurBilas[i].sedangDigunakan) {
            NodeAntrian* mobilNode = NULL;
            if (strcmp(jalurBilas[i].tipe, "VIP") == 0) {
                pthread_mutex_lock(&antrianVIPMutex);
                if (antrianPembilasanVIP != NULL) {
                    mobilNode = antrianPembilasanVIP;
                    antrianPembilasanVIP = antrianPembilasanVIP->next;
                }
                pthread_mutex_unlock(&antrianVIPMutex);
            } else {
                pthread_mutex_lock(&antrianRegulerMutex);
                if (antrianPembilasanReguler != NULL) {
                    mobilNode = antrianPembilasanReguler;
                    antrianPembilasanReguler = antrianPembilasanReguler->next;
                }
                pthread_mutex_unlock(&antrianRegulerMutex);
            }

            if (mobilNode != NULL) {
                jalurBilas[i].sedangDigunakan = 1;
                jalurBilas[i].mobilSedangDicuci = mobilNode->data;
                pthread_create(&jalurBilas[i].threadId, NULL, prosesBilas, &jalurBilas[i]);
                if (tampilPesan) {
                    printf("Mobil %d masuk jalur bilas %s\n", jalurBilas[i].mobilSedangDicuci.id, jalurBilas[i].tipe);
                }
                free(mobilNode);
            }
        }
    }

    // Proses antrian kering
    for (int i = 0; i < 2; i++) {
        if (!jalurKering[i].sedangDigunakan) {
            NodeAntrian* mobilNode = NULL;
            if (strcmp(jalurKering[i].tipe, "VIP") == 0) {
                pthread_mutex_lock(&antrianVIPMutex);
                if (antrianPengeringanVIP != NULL) {
                    mobilNode = antrianPengeringanVIP;
                    antrianPengeringanVIP = antrianPengeringanVIP->next;
                }
                pthread_mutex_unlock(&antrianVIPMutex);
            } else {
                pthread_mutex_lock(&antrianRegulerMutex);
                if (antrianPengeringanReguler != NULL) {
                    mobilNode = antrianPengeringanReguler;
                    antrianPengeringanReguler = antrianPengeringanReguler->next;
                }
                pthread_mutex_unlock(&antrianRegulerMutex);
            }

            if (mobilNode != NULL) {
                jalurKering[i].sedangDigunakan = 1;
                jalurKering[i].mobilSedangDicuci = mobilNode->data;
                pthread_create(&jalurKering[i].threadId, NULL, prosesKering, &jalurKering[i]);
                if (tampilPesan) {
                    printf("Mobil %d masuk jalur kering %s\n", jalurKering[i].mobilSedangDicuci.id, jalurKering[i].tipe);
                }
                free(mobilNode);
            }
        }
    }
}

// ===================== LOOP PROSES ANTRIAN (THREAD) =====================
void* loopProsesAntrian(void* arg) {
    JalurCuci* jalurCuci = ((JalurCuci**)arg)[0];
    JalurCuci* jalurBilas = ((JalurCuci**)arg)[1];
    JalurCuci* jalurKering = ((JalurCuci**)arg)[2];
    while (1) {
        modePesanProses = 0;
        prosesAntrianOtomatis(jalurCuci, jalurBilas, jalurKering, 0);
        sleep(1);
    }
    return NULL;
}

// ===================== TAMPILKAN SEMUA JALUR =====================
void tampilkanSemuaJalur(JalurCuci* jalurCuci, int nCuci, JalurCuci* jalurBilas, int nBilas, JalurCuci* jalurKering, int nKering) {
    printQueue(antrianVIP, "VIP");
    printQueue(antrianReguler, "Reguler");

    printf("===== JALUR CUCI =====\n");
    for (int i = 0; i < nCuci; i++) {
        if (jalurCuci[i].sedangDigunakan && jalurCuci[i].mobilSedangDicuci.id != -1)
            printf("[%s]: Mobil %d (%s, %s)\n", jalurCuci[i].tipe, 
                jalurCuci[i].mobilSedangDicuci.id, 
                jalurCuci[i].mobilSedangDicuci.nama, 
                jalurCuci[i].mobilSedangDicuci.platNomor);
        else
            printf("[%s]: Kosong\n", jalurCuci[i].tipe);
    }

    printf("===== JALUR BILAS =====\n");
    for (int i = 0; i < nBilas; i++) {
        if (jalurBilas[i].sedangDigunakan && jalurBilas[i].mobilSedangDicuci.id != -1)
            printf("[%s]: Mobil %d (%s, %s)\n", jalurBilas[i].tipe, 
                jalurBilas[i].mobilSedangDicuci.id, 
                jalurBilas[i].mobilSedangDicuci.nama, 
                jalurBilas[i].mobilSedangDicuci.platNomor);
        else
            printf("[%s]: Kosong\n", jalurBilas[i].tipe);
    }

    printf("===== JALUR KERING =====\n");
    for (int i = 0; i < nKering; i++) {
        if (jalurKering[i].sedangDigunakan && jalurKering[i].mobilSedangDicuci.id != -1)
            printf("[%s]: Mobil %d (%s, %s)\n", jalurKering[i].tipe, 
                jalurKering[i].mobilSedangDicuci.id, 
                jalurKering[i].mobilSedangDicuci.nama, 
                jalurKering[i].mobilSedangDicuci.platNomor);
        else
            printf("[%s]: Kosong\n", jalurKering[i].tipe);
    }
}