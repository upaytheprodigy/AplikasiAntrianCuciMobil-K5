#include "../header/riwayat.h"
#include "../header/treewaktu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern int jumlahDataBaruRiwayat;
extern TreeWaktu* rootTreeWaktu;

// ===================== FUNGSI PEMBANTU =====================

// Fungsi sederhana untuk parsing "HH:MM:SS" ke struct tm
void parseTime(const char* waktuStr, struct tm* waktu) {
    int h, m, s;
    sscanf(waktuStr, "%d:%d:%d", &h, &m, &s);
    waktu->tm_hour = h;
    waktu->tm_min = m;
    waktu->tm_sec = s;
}

// ===================== FUNGSI UTAMA =====================

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
    if (jumlahDataBaruRiwayat >= 10) {
        simpanRiwayatKeFile(*head);
        jumlahDataBaruRiwayat = 0;
    }
    rootTreeWaktu = insertTreeWaktu(rootTreeWaktu, data, data.waktuSelesaiEpoch);
    simpanTreeWaktu(rootTreeWaktu, "treewaktu/treewaktu.txt");
}

// ===================== SIMPAN RIWAYAT KE FILE =====================

void simpanRiwayatKeFile(NodeRiwayat* head) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char filename[100];
    strftime(filename, sizeof(filename), "riwayat/riwayat-%Y%m%d.txt", t);

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Gagal membuka file %s\n", filename);
        return;
    }

    // Tabel VIP
    fprintf(file, "==================== RIWAYAT VIP ====================\n");
    fprintf(file, "| %-4s | %-20s | %-8s | %-10s | %-8s | %-8s | %-8s | %-8s |\n",
        "ID", "Nama", "Jenis", "Plat", "Masuk", "Mulai", "Estimasi", "Selesai");
    fprintf(file, "-------------------------------------------------------------------------------\n");
    NodeRiwayat* temp = head;
    int adaVIP = 0;
    while (temp != NULL) {
        if (strcasecmp(temp->data.jalur, "VIP") == 0) {
            // Cek tanggal sama dengan hari ini (bisa ditambah validasi jika ingin sortir per hari)
            struct tm waktuSelesai;
            parseTime(temp->data.waktuSelesaiStr, &waktuSelesai);
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
        fprintf(file, "| %-94s |\n", "Belum ada riwayat VIP hari ini.");
    }
    fprintf(file, "===============================================================================\n\n");

    // Tabel Reguler
    fprintf(file, "==================== RIWAYAT REGULER ====================\n");
    fprintf(file, "| %-4s | %-20s | %-8s | %-10s | %-8s | %-8s | %-8s | %-8s |\n",
        "ID", "Nama", "Jenis", "Plat", "Masuk", "Mulai", "Estimasi", "Selesai");
    fprintf(file, "-------------------------------------------------------------------------------\n");
    temp = head;
    int adaReguler = 0;
    while (temp != NULL) {
        if (strcasecmp(temp->data.jalur, "Reguler") == 0) {
            struct tm waktuSelesai;
            parseTime(temp->data.waktuSelesaiStr, &waktuSelesai);
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
        fprintf(file, "| %-94s |\n", "Belum ada riwayat Reguler hari ini.");
    }
    fprintf(file, "===============================================================================\n");

    fclose(file);
}

// ===================== TAMPILKAN RIWAYAT BERDASARKAN TANGGAL =====================

void tampilkanRiwayatTanggal() {
    int tgl, bln, thn;
    printf("Masukkan tanggal (dd mm yyyy): ");
    scanf("%d %d %d", &tgl, &bln, &thn);
    getchar();

    char filename[100];
    snprintf(filename, sizeof(filename), "riwayat/riwayat-%04d%02d%02d.txt", thn, bln, tgl);

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File riwayat untuk tanggal %02d-%02d-%04d tidak ditemukan.\n", tgl, bln, thn);
        return;
    }

    // Baca isi file dan tampilkan ke layar
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}