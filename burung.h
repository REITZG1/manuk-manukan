#ifndef BURUNG_H
#define BURUNG_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
using namespace std;

#define TINGGI 22
#define LEBAR  50

#define LEVEL_MUDAH  1
#define LEVEL_SEDANG 2
#define LEVEL_SUSAH  3

#define MAKS_RIWAYAT 5

struct Burung {
    int y, x;
    int skor;
    int nyawa;
    bool kebal;
    int timerKebal; 
};

struct Pipa {
    int x;      
    int celah; 
};

struct RiwayatSkor {
    char nama[32];
    int  skor;
    int  level;
};

void lompat(Burung *b);
void rintangan(Burung *b, Pipa *p, int level);
void DataSkor(RiwayatSkor riwayat[], int &jumlah);
void Save_R(RiwayatSkor riwayat[], int &jumlah, const char *nama, int skor, int level);
void histori(RiwayatSkor riwayat[], int jumlah);

#endif
