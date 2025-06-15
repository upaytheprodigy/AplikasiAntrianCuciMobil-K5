#include "../header/kupon.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

DataKupon dataKupon[MAX_KUPON];
int jumlahDataKupon = 0;

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

    // Konversi waktuSelesaiStr (format "HH:MM:SS") ke waktu lengkap (hari, tanggal, tahun, jam)
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
}

// Cari index kupon berdasarkan plat nomor
int cariKuponPlat(const char* platNomor) {
    for (int i = 0; i < jumlahDataKupon; i++) {
        if (strcmp(dataKupon[i].platNomor, platNomor) == 0) {
            return i;
        }
    }
    return -1;
}

int cekDanGunakanKupon(const char* platNomor) {
    int idx = cariKuponPlat(platNomor);
    if (idx != -1 && dataKupon[idx].jumlahKupon > 0) {
        printf("Pelanggan dengan plat %s memiliki %d kupon. Gunakan kupon? (y/n): ", dataKupon[idx].platNomor, dataKupon[idx].jumlahKupon);
        char jawab;
        scanf(" %c", &jawab); getchar();
        if (jawab == 'y' || jawab == 'Y') {
            dataKupon[idx].jumlahKupon--;
            printf("Kupon digunakan. Sisa kupon: %d\n", dataKupon[idx].jumlahKupon);
            return 1;
        }
    }
    return 0;
}

// Tampilkan menu data kupon
void menuKupon() {
    printf("\n==================== DATA KUPON PELANGGAN ====================\n");
    if (jumlahDataKupon == 0) {
        printf("Belum ada pelanggan yang memiliki kupon.\n");
        return;
    }
    for (int i = 0; i < jumlahDataKupon; i++) {
        printf("Plat: %s | Jenis: %s | Kupon: %d\n", dataKupon[i].platNomor, dataKupon[i].jenisMobil, dataKupon[i].jumlahKupon);
        printf("Riwayat waktu cuci: ");
        for (int j = 0; j < dataKupon[i].riwayatCount; j++) {
            printf("%s ", dataKupon[i].waktuCuci[j]);
        }
        printf("\n-------------------------------------------------------------\n");
    }
}