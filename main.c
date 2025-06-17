#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "header/mobil.h"
#include "header/antrian.h"
#include "header/riwayat.h"
#include "header/jalur.h"
#include "header/treewaktu.h"
#include "header/kupon.h"
#include "header/pembatalan.h"

#define N_CUCI   TOTAL_JALUR
#define N_BILAS  2
#define N_KERING 2

// ===================== GLOBAL VARIABEL =====================
NodeAntrian* antrianVIP = NULL;
NodeAntrian* antrianReguler = NULL;
NodeAntrian* antrianPembilasanVIP = NULL;
NodeAntrian* antrianPembilasanReguler = NULL;
NodeAntrian* antrianPengeringanVIP = NULL;
NodeAntrian* antrianPengeringanReguler = NULL;

NodeRiwayat* riwayat = NULL;
TreeWaktu* rootTreeWaktu = NULL;
time_t waktuMulaiSimulasi;
JalurCuci jalurCuci[TOTAL_JALUR];
JalurCuci jalurBilas[N_BILAS];
JalurCuci jalurKering[N_KERING];
int jumlahDataBaruRiwayat = 0;

extern pthread_mutex_t antrianVIPMutex;
extern pthread_mutex_t antrianRegulerMutex;
extern pthread_mutex_t jalurCuciMutex;
extern pthread_mutex_t jalurBilasMutex;
extern pthread_mutex_t jalurKeringMutex;
extern int modePesanProses;

// ===================== MAIN PROGRAM =====================
int main() {
    loadKuponDariFile();
    waktuMulaiSimulasi = time(NULL);
    inisialisasiJalur(jalurCuci, jalurBilas, jalurKering);
    rootTreeWaktu = loadTreeWaktu("treewaktu/treewaktu.txt");

    pthread_t threadProses;
    JalurCuci* jalurArr[3] = {jalurCuci, jalurBilas, jalurKering};
    pthread_create(&threadProses, NULL, loopProsesAntrian, jalurArr);

    int pilihan;
    do {
        printf("\n=== APLIKASI ANTRIAN CUCI MOBIL ===\n");
        printf("1. Tambah Mobil\n");
        printf("2. Tampilkan Antrian\n");
        printf("3. Pembatalan\n");
        printf("4. Riwayat Mobil\n");
        printf("5. Manajemen Kupon\n");
        printf("6. Rekap Waktu\n");
        printf("0. Keluar\n");
        printf("Pilih: ");
        if (scanf("%d", &pilihan) != 1) {
            while (getchar() != '\n'); // Bersihkan input buffer
            printf("Input tidak valid!\n");
            continue;
        }
        getchar(); // Bersihkan newline

        // ===================== MENU UTAMA =====================
        switch (pilihan) {
            case 1:
                tambahMobil();
                break;

            case 2:
                tampilkanSemuaJalur(jalurCuci, N_CUCI, jalurBilas, N_BILAS, jalurKering, N_KERING);
                break;

            case 3: { // ===================== MENU PEMBATALAN =====================
                int subPembatalan;
                do {
                    printf("\n=== MENU PEMBATALAN ===\n");
                    printf("1. Batalkan Antrian\n");
                    printf("2. Daftar Pembatalan Saat Ini\n");
                    printf("3. Riwayat Pembatalan Antrian\n");
                    printf("0. Kembali\n");
                    printf("Pilih: ");
                    if (scanf("%d", &subPembatalan) != 1) {
                        while (getchar() != '\n');
                        printf("Input tidak valid!\n");
                        continue;
                    }
                    getchar();
                    if (subPembatalan == 0) break;

                    if (subPembatalan == 1) {
                        int mode;
                        char keyword[50];
                        do {
                            printf("\n--- Batalkan Antrian ---\n");
                            printf("1. Nama\n");
                            printf("2. Jenis Mobil\n");
                            printf("3. Plat Nomor\n");
                            printf("0. Kembali\n");
                            printf("Pilih: ");
                            if (scanf("%d", &mode) != 1) {
                                while (getchar() != '\n');
                                printf("Input tidak valid!\n");
                                continue;
                            }
                            getchar();
                            if (mode == 0) break;
                            printf("Masukkan kata kunci: ");
                            fgets(keyword, sizeof(keyword), stdin);
                            keyword[strcspn(keyword, "\n")] = 0;
                            batalkanAntrian(mode, keyword);
                        } while (mode != 0);
                    } else if (subPembatalan == 2) {
                        printStackPembatalan(stackPembatalan);
                    } else if (subPembatalan == 3) {
                        openPembatalanFile();
                    } else {
                        printf("Pilihan tidak valid!\n");
                    }
                } while (subPembatalan != 0);
                break;
            }

            case 4: { // ===================== MENU RIWAYAT MOBIL =====================
                int subPilihan;
                do {
                    printf("\n=== Menu Riwayat Mobil ===\n");
                    printf("1. Tampilkan Riwayat\n");
                    printf("2. Simpan Data Sekarang\n");
                    printf("0. Kembali\n");
                    printf("Pilih: ");
                    if (scanf("%d", &subPilihan) != 1) {
                        while (getchar() != '\n');
                        printf("Input tidak valid!\n");
                        continue;
                    }
                    getchar();
                    if (subPilihan == 0) break;
                    if (subPilihan == 1) {
                        tampilkanRiwayatTanggal();
                    } else if (subPilihan == 2) {
                        simpanRiwayatKeFile(riwayat);
                        jumlahDataBaruRiwayat = 0;
                        printf("Data riwayat berhasil disimpan ke file.\n");
                    } else {
                        printf("Pilihan tidak valid!\n");
                    }
                } while (subPilihan != 0);
                break;
            }

            case 5:
                menuKupon();
                break;
            case 6: { // ===================== MENU REKAP WAKTU =====================
                int sub;
                do {
                    printf("\n=== MENU REKAP WAKTU ===\n");
                    printf("1. Rekap per Jam (Keseluruhan)\n");
                    printf("2. Rekap per Hari\n");
                    printf("3. Rekap Rentang Jam pada Hari Tertentu\n");
                    printf("0. Kembali\n");
                    printf("Pilih: ");
                    if (scanf("%d", &sub) != 1) {
                        while (getchar() != '\n');
                        printf("Input tidak valid!\n");
                        continue;
                    }
                    getchar();
                    if (sub == 0) break;
                    if (sub == 1) {
                        tampilkanRekapPerJam(rootTreeWaktu);
                    } else if (sub == 2) {
                        tampilkanRekapPerHari(rootTreeWaktu);
                    } else if (sub == 3) {
                        tampilkanRekapPerJamRentang(rootTreeWaktu);
                    } else {
                        printf("Pilihan tidak valid!\n");
                    }
                } while (sub != 0);
                break;
            }
            case 0:
                printf("Terima kasih!\n");
                break;

            default:
                printf("Pilihan tidak valid!\n");
                break;
        }
    } while (pilihan != 0);

    pthread_join(threadProses, NULL);
    simpanTreeWaktu(rootTreeWaktu, "treewaktu.txt");
    return 0;
}