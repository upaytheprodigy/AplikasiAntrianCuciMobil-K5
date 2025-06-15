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
#include "../header/kupon.h"
#include <time.h>

// Deklarasi antrian global (didefinisikan di main.c)
extern NodeAntrian* antrianVIP;
extern NodeAntrian* antrianReguler;
extern time_t waktuMulaiSimulasi;

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

// Fungsi bantu: mengubah menit sejak 09.00 menjadi format hh.mm (misal 10.30)
// void formatWaktu(int menitSejak0900, char* output) {
//     int totalMenit = 9 * 60 + menitSejak0900;
//     int jam = totalMenit / 60;
//     int menit = totalMenit % 60;
//     sprintf(output, "%02d.%02d", jam, menit);
// }

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
    if (hitungDurasiTotal(m.jenisMobil) == 0) {
        printf("\nJenis mobil tidak dikenali. Silakan coba lagi.\n");
        return;
    }

    printf("Plat Nomor Mobil         : ");
    fgets(m.platNomor, sizeof(m.platNomor), stdin);
    m.platNomor[strcspn(m.platNomor, "\n")] = '\0';

    // Cek dan tawarkan penggunaan kupon
    int pakaiKupon = cekDanGunakanKupon(m.platNomor);
    if (pakaiKupon) {
        printf("Cuci gratis menggunakan kupon!\n");
    }

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

    // Hitung waktu datang otomatis
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    strftime(m.waktuDatangStr, sizeof(m.waktuDatangStr), "%H:%M:%S", local);
    // int jam = local->tm_hour;
    // int menit = local->tm_min;
    // m.waktuDatang = (jam - 9) * 60 + menit;

    // if (m.waktuDatang < 0 || m.waktuDatang > 1200) {
    //     printf("\nWaktu sekarang di luar jam operasional (09.00 - 16.00).\n");
    //     return;
    // }

    // Hitung durasi berdasarkan persentase dari total durasi
    // m.durasiCuci = durasiTotal * 0.5;
    // m.durasiBilas = durasiTotal * 0.3;
    // m.durasiKering = durasiTotal * 0.2;
    m.durasiCuci = 5;    // 5 detik untuk cuci
    m.durasiBilas = 3;   // 3 detik untuk bilas
    m.durasiKering = 2;  // 2 detik untuk kering

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

    // Konversi estimasi selesai ke waktu lokal (real time)
    time_t estimasiSelesaiTime = waktuMulaiSimulasi + m.estimasiSelesai;
    struct tm *localEstimasi = localtime(&estimasiSelesaiTime);
    strftime(m.estimasiSelesaiStr, sizeof(m.estimasiSelesaiStr), "%H:%M:%S", localEstimasi);

    // Format waktu tampil dalam hh.mm
    char waktuDatangStr[6], waktuSelesaiStr[6];

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