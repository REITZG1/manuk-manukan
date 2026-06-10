#include "burung.h"
#include <windows.h>

static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void posisi(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hConsole, pos);
}

void lompat(Burung *b) {
    b->y -= 2;
}

void rintangan(Burung *b, Pipa *p, int level) {

    posisi(0, 0);

    const char *namaLevel =
        (level == LEVEL_MUDAH) ? "Mudah" :
        (level == LEVEL_SEDANG) ? "Sedang" : "Susah";
        
    for (int i = 0; i < TINGGI; i++) {
        for (int j = 0; j < LEBAR; j++) {
            if (i == 0 || i == TINGGI - 1) {
                cout << "=";
            }
            else if (i == b->y && j == b->x) {
                bool t =
                    !b->kebal ||
                    ((b->timerKebal / 3) % 2 == 0);
                cout << (t ? ">" : " ");
            }
            else if (j == p->x &&
                    (i < p->celah ||
                     i > p->celah + 4)) {
                cout << "#";
            }
            else {
                cout << " ";
            }
        }
        cout << '\n';
    }
    cout << "  Skor: " << b->skor;
    cout << "   Nyawa: ";

    for (int n = 0; n < 3; n++)
        cout << (n < b->nyawa ? "<3 " : "-- ");
        
    cout << "   Level: " << namaLevel;
    if (b->kebal)
        cout << "   [KEBAL "
             << (b->timerKebal / 10) + 1
             << "s]";
    cout << "                    ";
    cout << "\n";
    cout.flush();
}

void DataSkor(RiwayatSkor riwayat[], int &jumlah) {
    jumlah = 0;
    ifstream file("riwayat.txt");
    while (jumlah < MAKS_RIWAYAT &&
           file >> riwayat[jumlah].nama >> riwayat[jumlah].skor >> riwayat[jumlah].level)
        jumlah++;
}

void Save_R(RiwayatSkor riwayat[], int &jumlah, const char *nama, int skor, int level) {
    if (jumlah < MAKS_RIWAYAT) {
        strncpy(riwayat[jumlah].nama, nama, 31);
        riwayat[jumlah].skor  = skor;
        riwayat[jumlah].level = level;
        jumlah++;
    } else {
     
        int minimal = 0;
        for (int i = 1; i < jumlah; i++)
            if (riwayat[i].skor < riwayat[minimal].skor) minimal = i;
        if (skor > riwayat[minimal].skor) {
            strncpy(riwayat[minimal].nama, nama, 31);
            riwayat[minimal].skor  = skor;
            riwayat[minimal].level = level;
        }
    }

    
    for (int i = 0; i < jumlah - 1; i++)
        for (int j = i + 1; j < jumlah; j++)
            if (riwayat[j].skor > riwayat[i].skor) {
                RiwayatSkor tmp = riwayat[i];
                riwayat[i] = riwayat[j];
                riwayat[j] = tmp;
            }

    ofstream file("riwayat.txt");
    for (int i = 0; i < jumlah; i++)
        file << riwayat[i].nama << " " << riwayat[i].skor << " " << riwayat[i].level << "\n";
}

void histori(RiwayatSkor riwayat[], int jumlah) {
    cout << "\n  +----+------------------+--------+----------+\n";
    cout <<   "  | No | Nama             | Skor   | Level    |\n";
    cout <<   "  +----+------------------+--------+----------+\n";

    if (jumlah == 0) {
        cout << "  |         top skor masih kosong :(        |\n";
    }

    for (int i = 0; i < jumlah; i++) {
        const char *lvl = (riwayat[i].level == LEVEL_MUDAH)  ? "Mudah  " :
                          (riwayat[i].level == LEVEL_SEDANG) ? "Sedang " : "Susah  ";

        string namaPad = riwayat[i].nama;
        while ((int)namaPad.size() < 16) namaPad += " ";

        string skorStr = to_string(riwayat[i].skor);
        while ((int)skorStr.size() < 6) skorStr += " ";

        cout << "  | " << i + 1 << "  | " << namaPad << " | " << skorStr << " | " << lvl << "  |\n";
    }

    cout << "  +----+------------------+--------+----------+\n";
}
