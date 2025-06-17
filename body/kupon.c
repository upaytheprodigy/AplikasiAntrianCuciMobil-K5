#include "../header/kupon.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

DataKupon dataKupon[MAX_KUPON];
int jumlahDataKupon = 0;

// ===================== NORMALISASI =====================
// Normalisasi plat nomor (hilangkan spasi, huruf kecil semua)
void normalisasiPlat(const char* src, char* dst) {
    int j = 0;
    for (int i = 0; src[i]; i++) {
        if (src[i] != ' ') {
            dst[j++] = tolower((unsigned char)src[i]);
        }
    }
    dst[j] = '\0';
}

// ===================== LOAD DATA =====================
// Load data kupon dari file ke array (untuk cek dan penggunaan kupon)
void loadKuponDariFile() {
    FILE* file = fopen("kupon/kupon.txt", "r");
    if (!file) return;
    jumlahDataKupon = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Plat:", 5) == 0) {
            DataKupon dk;
            sscanf(line, "Plat: %19[^|]| Jenis: %9[^|]| Kupon: %d", dk.platNomor, dk.jenisMobil, &dk.jumlahKupon);
            dk.riwayatCount = 0;
            fgets(line, sizeof(line), file);
            while (fgets(line, sizeof(line), file)) {
                if (strncmp(line, "-------------------------------------------------------------", 10) == 0)
                    break;
                if (line[0] == '-') {
                    char* waktu = strchr(line, '-');
                    if (waktu) {
                        waktu++;
                        while (*waktu == ' ') waktu++;
                        waktu[strcspn(waktu, "\n")] = 0;
                        strcpy(dk.waktuCuci[dk.riwayatCount++], waktu);
                    }
                }
            }
            if (dk.jumlahKupon > 0)
                dataKupon[jumlahDataKupon++] = dk;
        }
    }
    fclose(file);
}

// ===================== CARI DATA =====================
// Cari index kupon berdasarkan plat nomor (case-insensitive, tanpa spasi)
int cariKuponPlat(const char* platNomor) {
    char normInput[30], normData[30];
    normalisasiPlat(platNomor, normInput);
    for (int i = 0; i < jumlahDataKupon; i++) {
        normalisasiPlat(dataKupon[i].platNomor, normData);
        if (strcmp(normInput, normData) == 0) {
            return i;
        }
    }
    return -1;
}

// ===================== TAMBAH DATA =====================
// Tambah kupon berdasarkan plat nomor
void tambahKuponPlat(const char* platNomor, const char* jenisMobil, const char* waktuSelesaiStr) {
    int idx = cariKuponPlat(platNomor);
    if (idx == -1) {
        idx = jumlahDataKupon++;
        strcpy(dataKupon[idx].platNomor, platNomor);
        strcpy(dataKupon[idx].jenisMobil, jenisMobil);
        dataKupon[idx].jumlahKupon = 0;
        dataKupon[idx].riwayatCount = 0;
    }

    // Format waktu lengkap (hari, tanggal, tahun, jam)
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char waktuLengkap[40];
    strftime(waktuLengkap, sizeof(waktuLengkap), "%A, %d-%m-%Y ", tm_info);
    strcat(waktuLengkap, waktuSelesaiStr);

    if (dataKupon[idx].riwayatCount < MAX_RIWAYAT_KUPON) {
        strcpy(dataKupon[idx].waktuCuci[dataKupon[idx].riwayatCount++], waktuLengkap);
    }

    // Tambahkan kupon setiap 5 kali cuci
    if (dataKupon[idx].riwayatCount % 5 == 0) {
        dataKupon[idx].jumlahKupon++;
        printf("Selamat! Plat %s telah mendapat 1 kupon gratis cuci reguler.\n", platNomor);
    }
    simpanKuponKeFile();
}

// ===================== CEK & GUNAKAN KUPON =====================
// Cek dan tawarkan penggunaan kupon saat registrasi
int cekDanGunakanKupon(const char* platNomor) {
    int idx = cariKuponPlat(platNomor);
    if (idx != -1 && dataKupon[idx].jumlahKupon > 0) {
        printf("Pelanggan dengan plat %s memiliki %d kupon. Gunakan kupon? (y/n): ", dataKupon[idx].platNomor, dataKupon[idx].jumlahKupon);
        char jawab;
        scanf(" %c", &jawab); getchar();
        if (jawab == 'y' || jawab == 'Y') {
            dataKupon[idx].jumlahKupon--;
            printf("Kupon digunakan. Sisa kupon: %d\n", dataKupon[idx].jumlahKupon);
            simpanKuponKeFile();
            return 1;
        }
    }
    return 0;
}

// ===================== SIMPAN DATA =====================
// Simpan seluruh data kupon ke file (hanya yang jumlahKupon > 0)
void simpanKuponKeFile() {
    FILE* file = fopen("kupon/kupon.txt", "w");
    if (!file) {
        printf("Gagal membuka file kupon/kupon.txt\n");
        return;
    }
    for (int i = 0; i < jumlahDataKupon; i++) {
        if (dataKupon[i].jumlahKupon <= 0) continue;
        fprintf(file, "Plat: %s | Jenis: %s | Kupon: %d\n", dataKupon[i].platNomor, dataKupon[i].jenisMobil, dataKupon[i].jumlahKupon);
        fprintf(file, "Riwayat waktu cuci:\n");
        for (int j = 0; j < dataKupon[i].riwayatCount; j++) {
            fprintf(file, "- %s\n", dataKupon[i].waktuCuci[j]);
        }
        fprintf(file, "-------------------------------------------------------------\n");
    }
    fclose(file);
}

// ===================== TAMPILKAN DATA =====================
// Tampilkan isi file kupon apa adanya
void menuKupon() {
    FILE* file = fopen("kupon/kupon.txt", "r");
    if (!file) {
        printf("Belum ada data kupon.\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}