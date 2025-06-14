#ifndef KUPON_H
#define KUPON_H

typedef struct {
    int idPelanggan;
    int jumlahKupon;
} Kupon;

extern int kupon[100]; // global array

void tampilkanKupon(const char* JenisMobil, int id); 
void tambahKupon(int id); // Tambah kupon untuk pelanggan dengan id tertentu
int cekKupon(int id); // Cek apakah pelanggan dengan id tertentu memiliki kupon gratis
void menuKupon(); // Menu untuk manajemen kupon

#endif
