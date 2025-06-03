#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/mobil.h"
#include "../header/antrian.h"

// Deklarasi antrian global (harus didefinisikan di main program)
extern NodeAntrian* antrianVIP;
extern NodeAntrian* antrianReguler;

// ID otomatis global
int idCounter = 1;

// Fungsi menghitung durasi total berdasarkan jenis mobil
int hitungDurasiTotal(const char* jenisMobil) {
    if (strcmp(jenisMobil, "Kecil") == 0 || strcmp(jenisMobil, "kecil") == 0)
        return 45;
    else if (strcmp(jenisMobil, "Sedang") == 0 || strcmp(jenisMobil, "sedang") == 0)
        return 60;
    else if (strcmp(jenisMobil, "Besar") == 0 || strcmp(jenisMobil, "besar") == 0)
        return 90;
    return 0;
}

// Fungsi garis pembatas
void garis() {
    printf("======================================================\n");
}

// Header form UI
void headerForm() {
    garis();
    printf("                FORM PENDAFTARAN MOBIL               \n");
    garis();
}

// Fungsi utama untuk menambah mobil ke antrian
void tambahMobil() {
    Mobil m;
    m.id = idCounter++;
    m.statusProses = 0;

    headerForm();

    printf("Nama Pemilik Mobil       : ");
    fflush(stdin);
    fgets(m.nama, sizeof(m.nama), stdin);
    m.nama[strcspn(m.nama, "\n")] = '\0';

    printf("Jenis Mobil [Kecil/Sedang/Besar] : ");
    fgets(m.jenisMobil, sizeof(m.jenisMobil), stdin);
    m.jenisMobil[strcspn(m.jenisMobil, "\n")] = '\0';

    int durasiTotal = hitungDurasiTotal(m.jenisMobil);
    if (durasiTotal == 0) {
        printf("\nJenis mobil tidak dikenali. Silakan coba lagi.\n");
        return;
    }

    printf("Plat Nomor Mobil         : ");
    fgets(m.platNomor, sizeof(m.platNomor), stdin);
    m.platNomor[strcspn(m.platNomor, "\n")] = '\0';

    printf("Jalur [VIP/Reguler]      : ");
    fgets(m.jalur, sizeof(m.jalur), stdin);
    m.jalur[strcspn(m.jalur, "\n")] = '\0';

    int isVIP = (strcmp(m.jalur, "VIP") == 0 || strcmp(m.jalur, "vip") == 0);
    int isReguler = (strcmp(m.jalur, "Reguler") == 0 || strcmp(m.jalur, "reguler") == 0);

    if (!isVIP && !isReguler) {
        printf("\nJalur tidak valid. Silakan coba lagi.\n");
        return;
    }

    printf("Waktu Kedatangan [0 - 420] menit sejak jam 09.00 : ");
    scanf("%d", &m.waktuDatang);
    getchar(); // menangkap newline

    if (m.waktuDatang < 0 || m.waktuDatang > 420) {
        printf("\nWaktu kedatangan di luar jam operasional (09.00 - 16.00).\n");
        return;
    }

    // Perhitungan durasi berdasarkan persentase
    m.durasiCuci = durasiTotal * 0.5;
    m.durasiBilas = durasiTotal * 0.3;
    m.durasiKering = durasiTotal * 0.2;
    m.estimasiSelesai = m.waktuDatang + m.durasiCuci + m.durasiBilas + m.durasiKering;

    garis();
    printf("Data Mobil yang Didaftarkan:\n");
    printf("ID Mobil         : %d\n", m.id);
    printf("Nama Pemilik     : %s\n", m.nama);
    printf("Jenis Mobil      : %s\n", m.jenisMobil);
    printf("Plat Nomor       : %s\n", m.platNomor);
    printf("Jalur            : %s\n", m.jalur);
    printf("Waktu Datang     : %d menit sejak 09.00\n", m.waktuDatang);
    printf("Estimasi Selesai : %d menit sejak 09.00\n", m.estimasiSelesai);
    garis();

    // Masukkan ke antrian sesuai jalur
    if (isVIP) {
        enqueue(&antrianVIP, m);
        printf("Mobil dimasukkan ke antrian VIP.\n");
    } else {
        enqueue(&antrianReguler, m);
        printf("Mobil dimasukkan ke antrian Reguler.\n");
    }

    printf("\nPendaftaran berhasil. Silakan menunggu giliran.\n");
    garis();
}
