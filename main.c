#include <stdio.h>
#include <stdlib.h>
#include "mobil.h"
#include "antrian.h"

// Inisialisasi antrian global (VIP & Reguler)
NodeAntrian* antrianVIP = NULL;
NodeAntrian* antrianReguler = NULL;

// Fungsi untuk menampilkan menu utama
void tampilkanMenu() {
    printf("\n");
    printf("======================================================\n");
    printf("             SISTEM ANTRIAN CUCI MOBIL               \n");
    printf("======================================================\n");
    printf("1. Tambah Mobil ke Antrian\n");
    printf("2. Tampilkan Antrian VIP\n");
    printf("3. Tampilkan Antrian Reguler\n");
    printf("4. Keluar\n");
    printf("------------------------------------------------------\n");
    printf("Pilih menu: ");
}

int main() {
    int pilihan;

    do {
        tampilkanMenu();
        scanf("%d", &pilihan);
        getchar(); // menangkap newline agar tidak mengganggu input berikutnya

        switch (pilihan) {
            case 1:
                tambahMobil();
                break;
            case 2:
                printf("\n--- ANTRIAN VIP ---\n");
                printQueue(antrianVIP, "VIP");
                break;
            case 3:
                printf("\n--- ANTRIAN REGULER ---\n");
                printQueue(antrianReguler, "Reguler");
                break;
            case 4:
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (pilihan != 4);

    return 0;
}
