#ifndef PEMBATALAN_H
#define PEMBATALAN_H
#include "mobil.h"

typedef struct NodePembatalan {
    Mobil data;
    struct NodePembatalan* next;
} NodePembatalan;

void pushPembatalan(NodePembatalan** top, Mobil data); // Push data ke stack pembatalan
Mobil popPembatalan(NodePembatalan** top); // Pop data dari stack pembatalan
void printStackPembatalan(NodePembatalan* top); // Print stack pembatalan

#endif
