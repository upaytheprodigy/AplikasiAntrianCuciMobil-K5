#ifndef KUPON_H
#define KUPON_H

typedef struct {
    int idPelanggan;
    int jumlahKupon;
} Kupon;

extern int kupon[100]; // global array

void tambahKupon(int id); // Tambah kupon untuk pelanggan dengan id tertentu
int cekKupon(int id); // Cek apakah pelanggan dengan id tertentu memiliki kupon gratis
void resetKupon(int id); // Reset kupon untuk pelanggan dengan id tertentu

#endif
