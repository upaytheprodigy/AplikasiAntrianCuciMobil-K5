#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h> // Tambahkan ini
#include "header/mobil.h"
#include "header/antrian.h"
#include "header/riwayat.h"
#include "header/jalur.h"

// Inisialisasi antrian global (VIP & Reguler)
NodeAntrian* antrianVIP = NULL;
NodeAntrian* antrianReguler = NULL;
NodeAntrian* antrianPembilasanVIP = NULL;
NodeAntrian* antrianPembilasanReguler = NULL;
NodeAntrian* antrianPengeringanVIP = NULL;
NodeAntrian* antrianPengeringanReguler = NULL;

// Inisialisasi riwayat
NodeRiwayat* riwayat = NULL;

// waktu mulai simulasi
time_t waktuMulaiSimulasi;

// Inisialisasi jalur
JalurCuci jalurCuci[TOTAL_JALUR];
JalurCuci jalurBilas[2];
JalurCuci jalurKering[2];

// Deklarasi mutex (ada di jalur.h)
extern pthread_mutex_t antrianVIPMutex;
extern pthread_mutex_t antrianRegulerMutex;
extern pthread_mutex_t jalurCuciMutex;
extern pthread_mutex_t jalurBilasMutex;
extern pthread_mutex_t jalurKeringMutex;

extern int modePesanProses;

// Fungsi utama
int main() {
    int pilihan;
    waktuMulaiSimulasi = time(NULL);
    inisialisasiJalur(jalurCuci, jalurBilas, jalurKering);
    pthread_t threadProses;
    // Buat array pointer untuk passing ke thread
    JalurCuci* jalurArr[3] = {jalurCuci, jalurBilas, jalurKering};
    pthread_create(&threadProses, NULL, loopProsesAntrian, jalurArr);
    do {
        printf("\n=== APLIKASI ANTRIAN CUCI MOBIL ===\n");
        printf("1. Tambah Mobil\n");
        printf("2. Tampilkan Antrian\n");
        printf("3. Proses Antrian\n");
        printf("4. Pembatalan Antrian\n");
        printf("5. Riwayat Mobil\n");
        printf("6. Manajemen Kupon\n");
        printf("0. Keluar\n");
        printf("Pilih: ");
        scanf("%d", &pilihan);
        getchar(); // Membersihkan newline

        switch (pilihan) {
            case 1:
                tambahMobil(); // Pendaftaran mobil
                break;
            case 2:
                tampilAntrian(); // Tampilkan semua antrian
                break;
            case 3:
                modePesanProses = 1;
                prosesAntrianOtomatis(jalurCuci, jalurBilas, jalurKering, 1);
                break;
            case 4: {
                int mode;
                char keyword[50];

                printf("\n=== Pembatalan Antrian ===\n");
                printf("Cari berdasarkan:\n");
                printf("1. Nama\n");
                printf("2. Jenis Mobil\n");
                printf("3. Plat Nomor\n");
                printf("Pilih: ");
                scanf("%d", &mode);
                getchar(); // Consume newline

                printf("Masukkan kata kunci: ");
                fgets(keyword, sizeof(keyword), stdin);
                keyword[strcspn(keyword, "\n")] = 0; // Hapus newline

                batalkanAntrian(mode, keyword);
                break;
            }
            case 5: {
                int subPilihan;
                printf("\n=== Menu Riwayat Mobil ===\n");
                printf("1. Semua Riwayat\n");
                printf("2. Riwayat VIP\n");
                printf("3. Riwayat Reguler\n");
                printf("Pilih: ");
                scanf("%d", &subPilihan);
                getchar();
                int cari = 0;
                char keyword[50];
                printf("Cari data? (1=Ya, 0=Tidak): ");
                scanf("%d", &cari);
                getchar();
                if (cari) {
                    printf("Masukkan kata kunci: ");
                    fgets(keyword, sizeof(keyword), stdin);
                    keyword[strcspn(keyword, "\n")] = 0;
                }
                if (subPilihan == 1) {
                    printRiwayatFilter(riwayat, 0, cari ? keyword : NULL);
                } else if (subPilihan == 2) {
                    printRiwayatFilter(riwayat, 1, cari ? keyword : NULL);
                } else if (subPilihan == 3) {
                    printRiwayatFilter(riwayat, 2, cari ? keyword : NULL);
                } else {
                    printf("Pilihan tidak valid!\n");
                }
                break;
            }
            // case 6:
            //     kelolaKupon(); // Manajemen kupon pelanggan
            //     break; // dari kupon.h
            // case 7:
            //     tampilkanStatusJalur(); // Tampilkan status slot cuci
            //     break; // dari jalur.h
            // case 8:
            //     rekapWaktu(); // Rekap/pencarian waktu (tree traversal)
            //     break; // dari treewaktu.h   
            case 0:
                printf("Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
                break;
        }
    } while (pilihan != 0);

    return 0;
}