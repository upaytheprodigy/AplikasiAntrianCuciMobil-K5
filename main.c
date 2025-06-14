#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/mobil.h"
#include "header/antrian.h"
#include "header/riwayat.h"

// Inisialisasi antrian global (VIP & Reguler)
NodeAntrian* antrianVIP = NULL;
NodeAntrian* antrianReguler = NULL;
NodeAntrian* antrianPembilasanVIP = NULL;
NodeAntrian* antrianPembilasanReguler = NULL;
NodeAntrian* antrianPengeringanVIP = NULL;
NodeAntrian* antrianPengeringanReguler = NULL;
NodeRiwayat* riwayat = NULL;

// Fungsi untuk menampilkan menu utama
void tampilkanMenu() {
    printf("\n");
    printf("======================================================\n");
    printf("             SISTEM ANTRIAN CUCI MOBIL               \n");
    printf("======================================================\n");
    printf("1. Tambah Mobil\n");
    printf("2. Daftar Antrian\n");
    printf("3. Proses Antrian (Cuci -> Bilas -> Kering)\n");
    printf("4. Pembatalan Antrian\n");
    printf("5. Riwayat Mobil\n");
    printf("6. Manajemen Kupon\n");
    printf("7. Status Jalur Cuci\n");
    printf("8. Rekap/Pencarian Waktu\n");
    printf("9. Keluar Program\n");
    printf("Pilih menu: ");
}

int main() {
    int pilihan;
    do {
        system("cls"); // Jika tidak jalan di DEV C, ganti dengan system("clear");
        tampilkanMenu();
        scanf("%d", &pilihan);
        getchar(); // Untuk menghindari input enter nyangkut

        switch (pilihan) {
            case 1:
                tambahMobil(); // Input data, tentukan VIP/Reguler, enqueue ke antrian yang sesuai
                break; // dari mobil.h
            case 2:
                tampilAntrian(); // Tampilkan semua antrian: VIP, Reguler, Pembilasan, Pengeringan
                break; // dari antrian.h
            case 3: {
                Mobil m = selesaikanAntrian(); // Proses bertahap: cuci -> bilas -> kering -> riwayat
                if (m.id != -1) {
                    insertRiwayat(&riwayat, m);
                    simpanRiwayatKeFile(riwayat);
                }
                break;
            }            case 4: {
                int idMobil;
                printf("\nMasukkan ID mobil yang ingin dibatalkan: ");
                scanf("%d", &idMobil);
                getchar(); // Consume newline
                
                Mobil* mobilVIP = findMobil(antrianVIP, idMobil);
                if (mobilVIP != NULL) {
                    batalkanAntrian(&antrianVIP, idMobil);
                } else {
                    Mobil* mobilReguler = findMobil(antrianReguler, idMobil);
                    if (mobilReguler != NULL) {
                        batalkanAntrian(&antrianReguler, idMobil);
                    } else {
                        printf("\nMobil dengan ID %d tidak ditemukan dalam antrian VIP maupun Reguler\n", idMobil);
                    }
                }
                break;
            }
            case 5: {
                int subPilihan;
                printf("\n=== Menu Riwayat Mobil ===\n");
                printf("1. Tampilkan Riwayat\n");
                printf("2. Tampilkan Riwayat Terbaru\n");
                printf("3. Cari Riwayat Mobil\n");
                printf("Pilih: ");
                scanf("%d", &subPilihan);
                getchar();
                if (subPilihan == 1) {
                    printRiwayat(riwayat);
                } else if (subPilihan == 2) {
                    printRiwayatTerbalik(riwayat);
                } else if (subPilihan == 3) {
                    int mode;
                    char keyword[50];
                    printf("Cari berdasarkan:\n");
                    printf("1. Nama\n");
                    printf("2. Jenis Mobil\n");
                    printf("3. Plat Nomor\n");
                    printf("4. Jalur\n");
                    printf("Pilih: ");
                    scanf("%d", &mode);
                    getchar();
                    printf("Masukkan kata kunci: ");
                    fgets(keyword, sizeof(keyword), stdin);
                    keyword[strcspn(keyword, "\n")] = 0; // hapus newline
                    cariRiwayatMobil(riwayat, mode, keyword);
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
            case 9:
                printf("Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
                break;
        }
        system("pause");
    } while (pilihan != 9);

    return 0;
}