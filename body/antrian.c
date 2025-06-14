#include "../header/antrian.h"
#include "../header/jalur.h"
#include "../header/pembatalan.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// Inisialisasi antrian global (VIP & Reguler)
extern NodeAntrian* antrianVIP;
extern NodeAntrian* antrianReguler;
extern NodeAntrian* antrianPembilasanVIP;
extern NodeAntrian* antrianPembilasanReguler;
extern NodeAntrian* antrianPengeringanVIP;
extern NodeAntrian* antrianPengeringanReguler;

extern int modePesanProses;

// Fungsi untuk memulai proses cuci secara otomatis
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

// Fungsi untuk menjalankan proses antrian secara otomatis dalam thread
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

// Fungsi untuk menambahkan mobil ke antrian
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

// Fungsi untuk mengambil mobil dari antrian
Mobil dequeue(NodeAntrian** front) {
    if (*front == NULL) {
        Mobil emptyMobil = {-1, "", "", "", "", 0, 0, 0, 0, 0, 0};
        return emptyMobil; // Antrian kosong
    }
    NodeAntrian* temp = *front;
    Mobil data = temp->data;
    *front = (*front)->next;
    free(temp);
    return data;
}

// Fungsi untuk menampilkan antrian berdasarkan jenis (VIP/Reguler)
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

// Fungsi untuk menghitung jumlah mobil dalam antrian
int countQueue(NodeAntrian* front) {
    int count = 0;
    NodeAntrian* temp = front;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Fungsi untuk mencari mobil dalam antrian berdasarkan ID
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

// Fungsi untuk menghapus mobil dari antrian saat dibatalkan
void deleteMobil(NodeAntrian** front, int id) {
    NodeAntrian* current = *front;
    NodeAntrian* prev = NULL;

    // Jika mobil yang akan dihapus adalah head
    if (current != NULL && current->data.id == id) {
        *front = current->next;
        free(current);
        return;
    }

    // Cari mobil yang akan dihapus
    while (current != NULL && current->data.id != id) {
        prev = current;
        current = current->next;
    }

    // Jika mobil tidak ditemukan
    if (current == NULL) {
        return;
    }

    // Hapus mobil dari antrian
    prev->next = current->next;
    free(current);
}

// Fungsi untuk membatalkan antrian dan memindahkan mobil ke stack pembatalan
void batalkanAntrian(int mode, const char* keyword) {
    Mobil* mobil = NULL;
    NodeAntrian* prev = NULL;
    NodeAntrian* current = NULL;
    NodeAntrian** antrian = NULL;

    // Tentukan antrian yang akan dicari berdasarkan mode
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

    // Jika mobil ditemukan, tampilkan data mobil dan minta konfirmasi pembatalan
    if (mobil != NULL) {
        printf("\nData Mobil yang Ditemukan:\n");
        printf("ID: %d\n", mobil->id);
        printf("Nama: %s\n", mobil->nama);
        printf("Jenis Mobil: %s\n", mobil->jenisMobil);
        printf("Plat Nomor: %s\n", mobil->platNomor);

        char konfirmasi;
        printf("\nApakah Anda yakin ingin membatalkan antrian mobil ini? (y/n): ");
        scanf(" %c", &konfirmasi);
        getchar(); // Consume newline

        if (konfirmasi == 'y' || konfirmasi == 'Y') {
            // Hapus mobil dari antrian
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

// Fungsi untuk menampilkan semua antrian: VIP, Reguler, Pembilasan, Pengeringan
void tampilAntrian() {
    printQueue(antrianVIP, "VIP");
    printQueue(antrianReguler, "Reguler");
    printQueue(antrianPembilasanVIP, "Pembilasan VIP");
    printQueue(antrianPembilasanReguler, "Pembilasan Reguler");
    printQueue(antrianPengeringanVIP, "Pengeringan VIP");
    printQueue(antrianPengeringanReguler, "Pengeringan Reguler");
}