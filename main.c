#include <stdio.h>
#include <stdlib.h>

#include "header/antrian.h"
#include "header/mobil.h"
#include "header/kupon.h"
#include "header/pembatalan.h"
#include "header/riwayat.h"
#include "header/treewaktu.h"

// Deklarasi antrian global
NodeAntrian* antrianVIP = NULL;
NodeAntrian* antrianReguler = NULL;
NodeAntrian* antrianPembilasan = NULL;
NodeAntrian* antrianPengeringan = NULL;


void tampilkanMenu() {
    printf("=== PROGRAM ANTRIAN CUCI MOBIL ===\n");
    printf("1. Tambah Mobil\n");
    printf("2. Daftar Antrian\n");
    printf("3. Proses Antrian (Cuci → Bilas → Kering)\n");
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
        system("cls"); // Jika tidak jalan di DEV C, ganti dengan `system("clear");`
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
            case 3:
                selesaikanAntrian(); // Proses bertahap: cuci → bilas → kering → riwayat
                break; // dari antrian.h
            case 4:
                batalkanAntrian(); // Batalkan mobil dari antrian, simpan ke stack pembatalan
                break;  // dari pembatalan.h
            case 5:
                printRiwayat(); // Tampilkan riwayat pencucian
                break; // dari riwayat.h
            case 6:
                kelolaKupon(); // Manajemen kupon pelanggan
                break; // dari kupon.h
            case 7:
                tampilkanStatusJalur(); // Tampilkan status slot cuci
                break; // dari jalur.h
            case 8:
                rekapWaktu(); // Rekap/pencarian waktu (tree traversal)
                break; // dari treewaktu.h
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