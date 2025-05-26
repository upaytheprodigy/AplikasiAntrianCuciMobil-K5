#ifndef KUPON_H
#define KUPON_H

typedef struct {
    int idPelanggan;
    int jumlahKupon;
} Kupon;

extern int kupon[100]; // global array

void tambahKupon(int id);
int cekKuponGratis(int id);
void resetKupon(int id);

#endif
