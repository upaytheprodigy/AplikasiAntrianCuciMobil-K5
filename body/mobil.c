//===============================================================================================================
// Program dibuat oleh: Rina Permata Dewi
// NIM                : 241511061
// Deskripsi File     : Modul pendaftaran mobil, menentukan durasi cuci otomatis berdasarkan jenis mobil,
//                      menghitung waktu kedatangan dari jam sistem, serta menampilkan estimasi selesai
//                      dalam format jam:menit (hh.mm). Data mobil dimasukkan ke antrian sesuai jalur.
//===============================================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/mobil.h"
#include "../header/antrian.h"
#include <time.h>

// Deklarasi antrian global (didefinisikan di main.c)
extern NodeAntrian* antrianVIP;
extern NodeAntrian* antrianReguler;

// ID otomatis
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

// Fungsi bantu: mengubah menit sejak 09.00 menjadi format hh.mm (misal 10.30)
void formatWaktu(int menitSejak0900, char* output) {
    int totalMenit = 9 * 60 + menitSejak0900;
    int jam = totalMenit / 60;
    int menit = totalMenit % 60;
    sprintf(output, "%02d.%02d", jam, menit);
}

// Fungsi pembatas visual
void garis() {
    printf("======================================================\n");
}

// Header tampilan form
void headerForm() {
    garis();
    printf("                FORM PENDAFTARAN MOBIL               \n");
    garis();
}

// Fungsi utama: tambah mobil ke antrian
void tambahMobil() {
    Mobil m;
    m.id = idCounter++;
    m.statusProses = 0;

    headerForm();

    // Input data pengguna
    printf("Nama Pemilik Mobil       : ");
    fflush(stdin);
    fgets(m.nama, sizeof(m.nama), stdin);
    m.nama[strcspn(m.nama, "\n")] = '\0';

    printf("Jenis Mobil [Kecil/Sedang/Besar] : ");
    fgets(m.jenisMobil, sizeof(m.jenisMobil), stdin);
    m.jenisMobil[strcspn(m.jenisMobil, "\n")] = '\0';

    // Validasi jenis mobil dan hitung durasi
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

    // Validasi jalur
    int isVIP = (strcmp(m.jalur, "VIP") == 0 || strcmp(m.jalur, "vip") == 0);
    int isReguler = (strcmp(m.jalur, "Reguler") == 0 || strcmp(m.jalur, "reguler") == 0);

    if (!isVIP && !isReguler) {
        printf("\nJalur tidak valid. Silakan coba lagi.\n");
        return;
    }

    // Hitung waktu datang otomatis (menit sejak jam 09.00)
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    int jam = local->tm_hour;
    int menit = local->tm_min;
    m.waktuDatang = (jam - 9) * 60 + menit;

    if (m.waktuDatang < 0 || m.waktuDatang > 420) {
        printf("\nWaktu sekarang di luar jam operasional (09.00 - 16.00).\n");
        return;
    }

    // Hitung durasi berdasarkan persentase dari total durasi
    m.durasiCuci = durasiTotal * 0.5;
    m.durasiBilas = durasiTotal * 0.3;
    m.durasiKering = durasiTotal * 0.2;

    // Hitung estimasi waktu selesai
    m.estimasiSelesai = m.waktuDatang + m.durasiCuci + m.durasiBilas + m.durasiKering;

    // Format waktu tampil dalam hh.mm
    char waktuDatangStr[6], waktuSelesaiStr[6];
    formatWaktu(m.waktuDatang, waktuDatangStr);
    formatWaktu(m.estimasiSelesai, waktuSelesaiStr);

    // Tampilkan hasil input
    garis();
    printf("Data Mobil yang Didaftarkan:\n");
    printf("ID Mobil         : %d\n", m.id);
    printf("Nama Pemilik     : %s\n", m.nama);
    printf("Jenis Mobil      : %s\n", m.jenisMobil);
    printf("Plat Nomor       : %s\n", m.platNomor);
    printf("Jalur            : %s\n", m.jalur);
    printf("Waktu Datang     : %s\n", waktuDatangStr);
    printf("Estimasi Selesai : %s\n", waktuSelesaiStr);
    garis();

    // Tambahkan ke antrian
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
