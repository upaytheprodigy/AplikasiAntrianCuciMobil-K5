/*
Program        : Manajemen Data Mobil
Deskripsi     : Program ini mengelola data mobil yang akan dicuci termasuk
                input data mobil baru dan pengecekan status mobil dalam sistem
Pembuat        : Rina Permata Dewi
NIM           : 241511061
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../header/mobil.h"
#include "../header/antrian.h"
#include "../header/kupon.h"

// Deklarasi antrian global (didefinisikan di main.c)
extern NodeAntrian* antrianVIP;
extern NodeAntrian* antrianReguler;
extern time_t waktuMulaiSimulasi;

// ID otomatis
int idCounter = 1;

// ===================== FUNGSI PEMBANTU =====================

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

// Fungsi menghitung durasi total berdasarkan jenis mobil (simulasi)
int hitungDurasiTotal(const char* jenisMobil) {
    if (strcmp(jenisMobil, "Kecil") == 0 || strcmp(jenisMobil, "kecil") == 0)
        return 45;
    else if (strcmp(jenisMobil, "Sedang") == 0 || strcmp(jenisMobil, "sedang") == 0)
        return 60;
    else if (strcmp(jenisMobil, "Besar") == 0 || strcmp(jenisMobil, "besar") == 0)
        return 90;
    return 0;
}

// Fungsi untuk menghitung durasi total (spesifikasi NORMAL)
// int hitungDurasiTotal(const char* jenisMobil, int isVIP) {
//     int menit = 0;
//     if (strcasecmp(jenisMobil, "Kecil") == 0)
//         menit = 45;
//     else if (strcasecmp(jenisMobil, "Sedang") == 0)
//         menit = 60;
//     else if (strcasecmp(jenisMobil, "Besar") == 0)
//         menit = 90;
//     int detik = menit * 60;
//     if (isVIP) {
//         detik = detik * 80 / 100; // 20% lebih cepat
//     }
//     return detik;
// }

// Fungsi mencari estimasi selesai terakhir di antrian
int estimasiSelesaiTerakhir(NodeAntrian* front, int waktuDatang, int durasiTotal) {
    int waktuSelesai = waktuDatang;
    NodeAntrian* temp = front;
    while (temp != NULL) {
        int selesaiMobil = temp->data.estimasiSelesai;
        if (selesaiMobil > waktuSelesai)
            waktuSelesai = selesaiMobil;
        temp = temp->next;
    }
    return waktuSelesai;
}

// Fungsi format waktu (detik ke HH:MM:SS)
void formatWaktu(int detikSejakMulai, char* output) {
    int jam = detikSejakMulai / 3600;
    int menit = (detikSejakMulai % 3600) / 60;
    int detik = detikSejakMulai % 60;
    sprintf(output, "%02d:%02d:%02d", jam, menit, detik);
}

// ===================== FUNGSI UTAMA PENDAFTARAN MOBIL =====================

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

    printf("Plat Nomor Mobil         : ");
    fgets(m.platNomor, sizeof(m.platNomor), stdin);
    m.platNomor[strcspn(m.platNomor, "\n")] = 0;

    // Cek dan tawarkan penggunaan kupon
    int pakaiKupon = cekDanGunakanKupon(m.platNomor);
    if (pakaiKupon) {
        printf("Cuci gratis menggunakan kupon!\n");
    }

    printf("Jalur [VIP/Reguler]      : ");
    fgets(m.jalur, sizeof(m.jalur), stdin);
    m.jalur[strcspn(m.jalur, "\n")] = '\0';

    // Validasi jalur
    int isVIP = (strcasecmp(m.jalur, "VIP") == 0);
    int isReguler = (strcasecmp(m.jalur, "Reguler") == 0);

    if (!isVIP && !isReguler) {
        printf("\nJalur tidak valid. Silakan coba lagi.\n");
        return;
    }

    // Validasi jenis mobil (aktifkan jika ingin mode normal)
    // int durasiTotal = hitungDurasiTotal(m.jenisMobil, isVIP);
    // if (durasiTotal == 0) {
    //     printf("\nJenis mobil tidak dikenali. Silakan coba lagi.\n");
    //     return;
    // }

    // Hitung waktu datang otomatis (detik sejak simulasi mulai)
    time_t now = time(NULL);
    m.waktuDatang = (int)(now - waktuMulaiSimulasi);
    struct tm *local = localtime(&now);
    strftime(m.waktuDatangStr, sizeof(m.waktuDatangStr), "%H:%M:%S", local);

    // Pembagian durasi proses (aktifkan jika ingin mode normal)
    // m.durasiCuci   = durasiTotal * 50 / 100;
    // m.durasiBilas  = durasiTotal * 30 / 100;
    // m.durasiKering = durasiTotal - m.durasiCuci - m.durasiBilas;

    // Atur durasi tetap untuk simulasi
    m.durasiCuci = 30;    // 30 detik
    m.durasiBilas = 20;   // 20 detik
    m.durasiKering = 20;  // 20 detik

    // --- PENYESUAIAN ESTIMASI SELESAI BERDASARKAN ANTRIAN ---
    int durasiTotal = m.durasiCuci + m.durasiBilas + m.durasiKering;
    int estimasiMulaiCuci;
    if (isVIP) {
        estimasiMulaiCuci = estimasiSelesaiTerakhir(antrianVIP, m.waktuDatang, durasiTotal);
    } else {
        estimasiMulaiCuci = estimasiSelesaiTerakhir(antrianReguler, m.waktuDatang, durasiTotal);
    }
    m.estimasiSelesai = estimasiMulaiCuci + durasiTotal;
    // --------------------------------------------------------

    // Konversi waktu datang ke waktu lokal (real time)
    time_t waktuDatangTime = waktuMulaiSimulasi + m.waktuDatang;
    struct tm *localDatang = localtime(&waktuDatangTime);
    strftime(m.waktuDatangStr, sizeof(m.waktuDatangStr), "%H:%M:%S", localDatang);

    // Konversi estimasi selesai ke waktu lokal (real time)
    time_t estimasiSelesaiTime = waktuMulaiSimulasi + m.estimasiSelesai;
    struct tm *localEstimasi = localtime(&estimasiSelesaiTime);
    strftime(m.estimasiSelesaiStr, sizeof(m.estimasiSelesaiStr), "%H:%M:%S", localEstimasi);

    // Tampilkan hasil input
    garis();
    printf("Data Mobil yang Didaftarkan:\n");
    printf("ID Mobil         : %d\n", m.id);
    printf("Nama Pemilik     : %s\n", m.nama);
    printf("Jenis Mobil      : %s\n", m.jenisMobil);
    printf("Plat Nomor       : %s\n", m.platNomor);
    printf("Jalur            : %s\n", m.jalur);
    printf("Waktu Datang     : %s\n", m.waktuDatangStr);
    printf("Estimasi Selesai : %s\n", m.estimasiSelesaiStr);
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