/*
  =============================================================
  SISTEM INFORMASI ANTRIAN CUCI MOBIL
  -------------------------------------------------------------
  Deskripsi:
  Program ini merupakan sistem simulasi antrian cuci mobil dengan
  fitur lengkap seperti pendaftaran antrian, proses pencucian,
  pembatalan, manajemen kupon, pencatatan riwayat, dan rekap waktu.

  Struktur program menggunakan pendekatan modular dengan file .c dan .h
  yang dipisah berdasarkan fitur. Visualisasi antrian menggunakan teks,
  serta pencatatan riwayat dan rekap menggunakan linked list dan binary tree.

  Dibuat oleh:
  - Muhammad Naufal Nurmayadi (Pembatalan, Riwayat, TreeWaktu)
  - Revaldi Ardhi Prasetyo (Jalur & Kupon)
  - Rina Permata Dewi (Mobil & Antrian)
  =============================================================
*/

#include "../header/antrian.h"
#include "../header/jalur.h"
#include "../header/pembatalan.h"
#include "../header/riwayat.h"
#include "../header/treewaktu.h"
#include "../header/mobil.h"
#include "../header/kupon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))

/*
  Petunjuk Penggunaan:
  ---------------------
  1. Jalankan program utama dari file main.c
  2. Pilih menu sesuai kebutuhan:
     - Tambah mobil: Input data mobil dan masukkan ke antrian.
     - Tampilkan antrian: Lihat semua mobil dalam jalur cuci.
     - Proses antrian: Jalankan simulasi cuci → bilas → kering.
     - Pembatalan: Batalkan antrian berdasarkan ID.
     - Riwayat: Tampilkan mobil yang telah selesai dicuci.
     - Kupon: Tambahkan dan kelola kupon untuk pelanggan.
     - Jalur: Pantau kondisi masing-masing jalur cuci.
     - Rekap waktu: Lihat statistik berdasarkan jam atau rentang.
     - Keluar: Menyelesaikan program.

  Struktur Folder:
  ----------------
  /main.c                → Program utama
  /header/*.h            → Header setiap modul
  /body/*.c              → Implementasi fungsi tiap modul
  /data/*.txt            → File penyimpanan riwayat & tree waktu

  Modul yang Tersedia:
  ---------------------
  - mobil.c / mobil.h
  - antrian.c / antrian.h
  - pembatalan.c / pembatalan.h
  - riwayat.c / riwayat.h
  - treewaktu.c / treewaktu.h
  - kupon.c / kupon.h
  - jalur.c / jalur.h

  Dependencies:
  --------------
  - Kompiler: GCC
  - Platform: Windows
  - Library: pthread, windows.h

  Catatan:
  ---------
  - Gunakan terminal berukuran besar agar tampilan tabel tidak terpotong.
  - File riwayat dan tree waktu akan otomatis disimpan dalam folder /data.
  - ID mobil bersifat unik, hasil auto increment.
*/
