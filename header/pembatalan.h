#ifndef PEMBATALAN_H
#define PEMBATALAN_H
#include "mobil.h"

typedef struct NodePembatalan {
    Mobil data;
    struct NodePembatalan* next;
} NodePembatalan;

void pushPembatalan(NodePembatalan** top, Mobil data);
Mobil popPembatalan(NodePembatalan** top);
void printStackPembatalan(NodePembatalan* top);

#endif
