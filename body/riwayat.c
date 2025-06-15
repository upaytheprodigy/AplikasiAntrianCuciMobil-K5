#include "../header/riwayat.h"
#include "../header/treewaktu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int jumlahDataBaruRiwayat = 0;

// Fungsi insert ke riwayat (doubly linked list, di akhir)
void insertRiwayat(NodeRiwayat** head, Mobil data) {
    NodeRiwayat* newNode = (NodeRiwayat*)malloc(sizeof(NodeRiwayat));
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        NodeRiwayat* temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
        newNode->prev = temp;
    }

    jumlahDataBaruRiwayat++;
    if (jumlahDataBaruRiwayat >= 2) {
        simpanRiwayatKeFile(*head);
        jumlahDataBaruRiwayat = 0;
    }
    rootTreeWaktu = insertTreeWaktu(rootTreeWaktu, data, data.waktuSelesaiEpoch);
}

void simpanRiwayatKeFile(NodeRiwayat* head) {
    FILE* file = fopen("riwayat/riwayat.txt", "w"); // mode tulis ulang agar tabel selalu rapi
    if (!file) {
        printf("Gagal membuka file riwayat/riwayat.txt\n");
        return;
    }

    // Tabel VIP
    fprintf(file, "====================================================================================================\n");
    fprintf(file, "| %-4s | %-20s | %-8s | %-10s | %-8s | %-8s | %-8s | %-8s |\n",
        "ID", "Nama", "Jenis", "Plat", "Masuk", "Mulai", "Estimasi", "Selesai");
    fprintf(file, "----------------------------------------------------------------------------------------------------\n");
    NodeRiwayat* temp = head;
    int adaVIP = 0;
    while (temp != NULL) {
        if (strcasecmp(temp->data.jalur, "VIP") == 0) {
            fprintf(file, "| %-4d | %-20s | %-8s | %-10s | %-8s | %-8s | %-8s | %-8s |\n",
                temp->data.id,
                temp->data.nama,
                temp->data.jenisMobil,
                temp->data.platNomor,
                temp->data.waktuDatangStr,
                temp->data.waktuMulaiCuciStr,
                temp->data.estimasiSelesaiStr,
                temp->data.waktuSelesaiStr
            );
            adaVIP = 1;
        }
        temp = temp->next;
    }
    if (!adaVIP) {
        fprintf(file, "| %-94s |\n", "Belum ada riwayat VIP.");
    }
    fprintf(file, "====================================================================================================\n\n");

    // Tabel Reguler
    fprintf(file, "====================================================================================================\n");
    fprintf(file, "| %-4s | %-20s | %-8s | %-10s | %-8s | %-8s | %-8s | %-8s |\n",
        "ID", "Nama", "Jenis", "Plat", "Masuk", "Mulai", "Estimasi", "Selesai");
    fprintf(file, "----------------------------------------------------------------------------------------------------\n");
    temp = head;
    int adaReguler = 0;
    while (temp != NULL) {
        if (strcasecmp(temp->data.jalur, "Reguler") == 0) {
            fprintf(file, "| %-4d | %-20s | %-8s | %-10s | %-8s | %-8s | %-8s | %-8s |\n",
                temp->data.id,
                temp->data.nama,
                temp->data.jenisMobil,
                temp->data.platNomor,
                temp->data.waktuDatangStr,
                temp->data.waktuMulaiCuciStr,
                temp->data.estimasiSelesaiStr,
                temp->data.waktuSelesaiStr
            );
            adaReguler = 1;
        }
        temp = temp->next;
    }
    if (!adaReguler) {
        fprintf(file, "| %-94s |\n", "Belum ada riwayat Reguler.");
    }
    fprintf(file, "====================================================================================================\n");

    fclose(file);
}

void printRiwayatFilter(NodeRiwayat* head, int mode, const char* keyword) {
    printf("\n====================================================================================================\n");
    printf("| %-5s | %-20s | %-8s | %-10s | %-8s | %-8s | %-8s | %-8s |\n",
    "ID", "Nama", "Jenis", "Plat", "Masuk", "Mulai", "Estimasi", "Selesai");
    printf("----------------------------------------------------------------------------------------------------\n");
    if (head == NULL) {
        printf("| %-94s |\n", "Belum ada riwayat.");
        printf("====================================================================================================\n");
        return;
    }
    NodeRiwayat* temp = head;
    int found = 0;
    char masuk[10], mulai[10], estimasi[10], selesai[10];
    while (temp != NULL) {
        int cocok = 1;
        if (mode == 1 && strcasecmp(temp->data.jalur, "VIP") != 0) cocok = 0;
        if (mode == 2 && strcasecmp(temp->data.jalur, "Reguler") != 0) cocok = 0;
        if (keyword && strlen(keyword) > 0) {
            if (!strstr(temp->data.nama, keyword) &&
                !strstr(temp->data.jenisMobil, keyword) &&
                !strstr(temp->data.platNomor, keyword) &&
                !strstr(temp->data.jalur, keyword)) {
                cocok = 0;
            }
        }
        if (cocok) {
            printf("| %-5d | %-20s | %-8s | %-10s | %-8s | %-8s | %-8s | %-8s |\n",
                temp->data.id,
                temp->data.nama,
                temp->data.jenisMobil,
                temp->data.platNomor,
                temp->data.waktuDatangStr,
                temp->data.waktuMulaiCuciStr,
                temp->data.estimasiSelesaiStr,
                temp->data.waktuSelesaiStr
            );
            found = 1;
        }
        temp = temp->next;
    }
    if (!found) {
        printf("| %-94s |\n", "Data tidak ditemukan.");
    }
    printf("====================================================================================================\n");
}