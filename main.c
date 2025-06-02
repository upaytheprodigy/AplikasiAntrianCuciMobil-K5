#include <stdio.h>
#include <stdlib.h>

// Sertakan header sesuai kebutuhan
#include "header/antrian.h"
#include "header/mobil.h"
#include "header/kupon.h"
#include "header/pembatalan.h"
#include "header/riwayat.h"
#include "header/treetime.h"

void tampilkanMenu() {
    printf("=== PROGRAM ANTRIAN CUCI MOBIL ===\n");
    printf("1. Tambah Mobil\n");
    printf("2. Daftar Antrian\n");
    printf("3. Daftar Selesai\n");
    printf("4. Pembatalan Antrian\n");
    printf("5. Riwayat Mobil\n");
    printf("6. Manajemen Kupon\n");
    printf("7. Keluar Program\n");
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
                tambahMobil(); // dari mobil.c
                break;
            case 2:
                tampilAntrian(); // dari antrian.c
                break;
            case 3:
                selesaikanAntrian(); // bisa dari treetime.c atau antrian.c
                break;
            case 4:
                batalkanAntrian(); // dari pembatalan.c
                break;
            case 5:
                tampilRiwayat(); // dari riwayat.c
                break;
            case 6:
                kelolaKupon(); // dari kupon.c
                break;
            case 7:
                printf("Terima kasih!\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
                break;
        }
        system("pause");
    } while (pilihan != 7);

    return 0;
}