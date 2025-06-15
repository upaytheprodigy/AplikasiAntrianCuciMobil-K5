#include "../header/kupon.h"
#include <stdio.h>

int kupon[100] = {0};

void tampilkanKupon(const char* jenisMobil, int idKupon) {
    printf("\n==================== KUPON CUCI MOBIL ====================\n");
    printf("Jenis Mobil: %s\n", jenisMobil);
    printf("ID Kupon: %d\n", idKupon);
    printf("===========================================================\n");
}

void tambahKupon(int id) {
    if (id < 0 || id >= 100) {
        printf("ID pelanggan tidak valid.\n");
        return;
    }
    kupon[id]++;
    printf("Kupon untuk pelanggan ID %d berhasil ditambahkan. Total kupon: %d\n", id, kupon[id]);
}

int cekKupon(int id) {
    if (id < 0 || id >= 100) {
        printf("ID pelanggan tidak valid.\n");
        return 0;
    }
    return kupon[id];
}

void menuKupon() {
    int id;
    printf("Masukkan ID Pelanggan: ");
    scanf("%d", &id);
    int jumlahKupon = cekKupon(id);
    printf("Jumlah Kupon Pelanggan ID %d: %d\n", id, jumlahKupon);
}
    