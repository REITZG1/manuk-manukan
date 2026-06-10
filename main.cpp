#include <conio.h>
#include <ctime>
#include "burung.h"

void tampilMenu(RiwayatSkor riwayat[], int jumlah);
int  pilihLevel();
void mainGame(RiwayatSkor riwayat[], int &jumlah, const string &nama, int level);
void instruksi();
void halamanRiwayat(RiwayatSkor riwayat[], int jumlah);
void respawn(Burung *b, Pipa *p);

int kecepatanLevel(int level) {
    if (level == LEVEL_MUDAH)  return 150;
    if (level == LEVEL_SEDANG) return 100;
    return 40;
}

int lebar(int level) {
    if (level == LEVEL_MUDAH)  return 7;          
    if (level == LEVEL_SEDANG) return 4;
    return 1;
}

const char *namaLevel(int level) {
    if (level == LEVEL_MUDAH)  return "Mudah";
    if (level == LEVEL_SEDANG) return "Sedang";
    return "Susah";
}

void respawn(Burung *b, Pipa *p) {
    b->y        = TINGGI / 2;
    b->x        = 5;
    b->kebal    = true;
    b->timerKebal = 30;
    p->x        = LEBAR - 2;
    p->celah  = 3 + rand() % (TINGGI - 8);
}

int main() {
	system("color B0");
    srand((unsigned)time(0));

    RiwayatSkor riwayat[MAKS_RIWAYAT];
    int jumlahRiwayat = 0;
    DataSkor(riwayat, jumlahRiwayat);

    while (true) {
        tampilMenu(riwayat, jumlahRiwayat);
        char pilihan = getche();

        if (pilihan == '1') {
            int level = pilihLevel();
            if (level == 0) continue;

            system("cls");
            cout << "Masukkan Nama Pemain: ";
            string nama;
            cin >> nama;

            mainGame(riwayat, jumlahRiwayat, nama, level);

        } else if (pilihan == '2') {
            instruksi();
        } else if (pilihan == '3') {
            halamanRiwayat(riwayat, jumlahRiwayat);
        } else if (pilihan == '4') {
            system("cls");
            cout << "\nTerima kasih telah bermain Sky Hopper!\n";
            Sleep(1500);
            exit(0);
        }
    }
    return 0;
}

void tampilMenu(RiwayatSkor riwayat[], int jumlah) {
    system("cls");
    cout << "\n  ================================\n";
    cout <<   "         SKY HOPPER \n";
    cout <<   "  ================================\n\n";
    cout << "  1. Main\n";
    cout << "  2. Instruksi\n";
    cout << "  3. Riwayat Skor\n";
    cout << "  4. Keluar\n";

    if (jumlah > 0)
        cout << "\n  Rekor: " << riwayat[0].nama << " - " << riwayat[0].skor << " poin\n";

    cout << "\n  Pilih [1-4]: ";
}

int pilihLevel() {
    system("cls");
    cout << "\n  ============================\n";
    cout <<   "       PILIH TINGKAT\n";
    cout <<   "  ============================\n\n";
    cout << "  1. Mudah  - Lambat, celah lebar\n";
    cout << "  2. Sedang - Kecepatan sedang\n";
    cout << "  3. Susah  - Cepat, celah sempit\n";
    cout << "  0. Kembali\n\n";
    cout << "  Pilih [0-3]: ";

    char p = getche();
    if (p == '1') return LEVEL_MUDAH;
    if (p == '2') return LEVEL_SEDANG;
    if (p == '3') return LEVEL_SUSAH;
    return 0;
}

void mainGame(RiwayatSkor riwayat[], int &jumlahRiwayat,
              const string &nama, int level) {

    Burung burung = { TINGGI / 2, 5, 0, 3, false, 0 };
    Pipa   pipa   = { LEBAR - 2, 3 + rand() % (TINGGI - 8) };
    int celah  = lebar(level);
	int delay  = kecepatanLevel(level);
	bool aktif = true;

	system("cls");

	while (aktif) {
		
        if (_kbhit() && _getch() == ' ')
            lompat(&burung);

        burung.y += 1;  
        pipa.x   -= 1;  

        if (pipa.x <= 0) {
            pipa.x     = LEBAR - 2;
            pipa.celah = 3 + rand() % (TINGGI - 8);
            burung.skor += 10;
        }

        if (burung.kebal && --burung.timerKebal <= 0)
            burung.kebal = false;

        bool nabrak = (burung.y >= TINGGI - 1 || burung.y <= 0);
        bool kenaPipa = (burung.x == pipa.x &&
                        (burung.y < pipa.celah || burung.y > pipa.celah + celah));

        if ((nabrak || kenaPipa) && !burung.kebal) {
            burung.nyawa--;
            if (burung.nyawa <= 0)
                aktif = false;  
            else
                respawn(&burung, &pipa);
        }

        rintangan(&burung, &pipa, level);
        Sleep(delay);
    }

    Save_R(riwayat, jumlahRiwayat, nama.c_str(), burung.skor, level);

    system("cls");
    cout << "\n  ==============================\n";
    cout <<   "        G A M E  O V E R\n";
    cout <<   "  ==============================\n\n";
    cout << "  Pemain : " << nama          << "\n";
    cout << "  Level  : " << namaLevel(level) << "\n";
    cout << "  Skor   : " << burung.skor   << "\n";

    if (jumlahRiwayat > 0 && riwayat[0].skor == burung.skor &&
        string(riwayat[0].nama) == nama)
        cout << "\n  *** REKOR BARU! ***\n";

    cout << "\n  --- Riwayat Skor Terbaik ---";
    histori(riwayat, jumlahRiwayat);
    cout << "\n  Tekan apa saja untuk kembali...";
    getch();
}

void instruksi() {
    system("cls");
    cout << "\n  ============================\n";
    cout <<   "         INSTRUKSI\n";
    cout <<   "  ============================\n\n";
    cout << "  Cara Bermain:\n";
    cout << "    - Tekan SPASI untuk melompat\n";
    cout << "    - Hindari pipa (#) dan batas layar\n\n";
    cout << "  Nyawa & Respawn:\n";
    cout << "    - Pemain punya 3 nyawa\n";
    cout << "    - Saat kena, nyawa berkurang & burung respawn\n";
    cout << "    - Setelah respawn, burung kebal selama 3 detik\n\n";
    cout << "  Level:\n";
    cout << "    - Mudah  : lambat, celah lebar\n";
    cout << "    - Sedang : sedang, celah normal\n";
    cout << "    - Susah  : cepat, celah sempit\n\n";
    cout << "  Skor: +10 poin setiap berhasil lewati pipa\n\n";
    cout << "  Tekan apa saja untuk kembali...";
    getch();
}

void halamanRiwayat(RiwayatSkor riwayat[], int jumlah) {
    system("cls");
    cout << "\n  ============================\n";
    cout <<   "       RIWAYAT SKOR\n";
    cout <<   "  ============================";
    histori(riwayat, jumlah);
    cout << "\n  Tekan apa saja untuk kembali...";
    getch();
}
