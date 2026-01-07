#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class Color {
public:
    unsigned char r, g, b;
    Color(unsigned char rr = 0, unsigned char gg = 0, unsigned char bb = 0) {
        r = rr; g = gg; b = bb;
    }
};

class Image {
private:
    int width, height;
    Color** matrix; // Pikselleri tutan 2 boyutlu dizi
public:
    Image(int w, int h) {
        width = w;
        height = h;
        matrix = new Color*[height];
        for (int i = height - 1; i >= 0; i--)
            matrix[i] = new Color[width];
    }

    // Dosyadan resmi okuyup matrix'e doldurur
    void yukleBMP(const char* filename) {
        ifstream file(filename, ios::binary);
        unsigned char header[54];
        file.read((char*)header, 54);

        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                unsigned char pixel[3];
                file.read((char*)pixel, 3);
                matrix[i][j].b = pixel[0]; // Mavi
                matrix[i][j].g = pixel[1]; // Yeşil
                matrix[i][j].r = pixel[2]; // Kırmızı
            }
        }
        file.close();
        cout << "Resim basariyla yuklendi." << endl;
    }

    void kaydetBMP(const char* filename) {
        // BMP dosya boyutu hesaplama (Header + Data)
        // Padding (satır sonu boşlukları) ihmal edilmiştir, 
        // standart 4'ün katı genişlik varsayıyoruz.
        int fileSize = 54 + 3 * width * height;

        unsigned char fileHeader[14] = {
            'B','M',
            (unsigned char)(fileSize),
            (unsigned char)(fileSize >> 8),
            (unsigned char)(fileSize >> 16),
            (unsigned char)(fileSize >> 24),
            0,0,0,0,
            54,0,0,0 
        };

        unsigned char infoHeader[40] = {
            40,0,0,0,
            (unsigned char)(width),
            (unsigned char)(width >> 8),
            (unsigned char)(width >> 16),
            (unsigned char)(width >> 24),
            (unsigned char)(height),
            (unsigned char)(height >> 8),
            (unsigned char)(height >> 16),
            (unsigned char)(height >> 24),
            1,0,
            24,0,0,0,0,0,0,0,0,0,0,0,0,0 
        };

        ofstream file(filename, ios::binary);
        file.write((char*)fileHeader, 14);
        file.write((char*)infoHeader, 40);

        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                unsigned char pixel[3] = {
                    matrix[i][j].b,
                    matrix[i][j].g,
                    matrix[i][j].r
                };
                file.write((char*)pixel, 3);
            }
        }
        file.close();
        cout << "Resim (" << filename << ") olarak kaydedildi." << endl;
    }

    void Sifreyazma(int a) { // 'a' burada 1 bitlik temel kapasitedir.
        
        int Bitsayisi;
        cout << "Sondan kac bite sifreyi gommek istersiniz (1 ile 8 arasinda olmali): ";
        cin >> Bitsayisi;
        cin.ignore(); // Tampon temizleme

        // Switch en dışta, böylece her case için kapasiteyi özel ayarlayabiliriz.
        switch (Bitsayisi) {
            case 1: {
                int guncelKapasite = a * 1; 
                cout << "Sifre yazma islemi basladi (1 Bit Modu)." << endl;
                char* Mesaj = new char[guncelKapasite]; 
                
                cout << "Lutfen sifrelenecek mesaji giriniz (max " << guncelKapasite - 1 << " karakter): ";
                cin.getline(Mesaj, guncelKapasite);

                int mesajUzunlugu = strlen(Mesaj);
                Mesaj[mesajUzunlugu] = '\0'; mesajUzunlugu++; 
                int charIndex = 0; int bitIndex = 0;

                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char* renkKanallari[3] = { &matrix[i][j].b, &matrix[i][j].g, &matrix[i][j].r };
                        for (int k = 0; k < 3; k++) {
                            if (charIndex >= mesajUzunlugu) { cout << "Sifreleme tamamlandi!" << endl; delete[] Mesaj; return; }
                            
                            unsigned char bit = (Mesaj[charIndex] >> bitIndex) & 1;
                            *renkKanallari[k] = (*renkKanallari[k] & 0xFE) | bit; // Maske: 11111110
                            
                            bitIndex++;
                            if (bitIndex >= 8) { bitIndex = 0; charIndex++; }
                        }
                    }
                }
                delete[] Mesaj; break;
            }

            case 2: {
                int guncelKapasite = a * 2;
                cout << "Sifre yazma islemi basladi (2 Bit Modu)." << endl;
                char* Mesaj = new char[guncelKapasite];

                cout << "Lutfen sifrelenecek mesaji giriniz (max " << guncelKapasite - 1 << " karakter): ";
                cin.getline(Mesaj, guncelKapasite);
                
                int mesajUzunlugu = strlen(Mesaj);
                Mesaj[mesajUzunlugu] = '\0'; mesajUzunlugu++;
                int charIndex = 0; int bitIndex = 0;

                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char* renkKanallari[3] = { &matrix[i][j].b, &matrix[i][j].g, &matrix[i][j].r };
                        for (int k = 0; k < 3; k++) {
                            if (charIndex >= mesajUzunlugu) { cout << "Sifreleme tamamlandi!" << endl; delete[] Mesaj; return; }

                            unsigned char bit = (Mesaj[charIndex] >> bitIndex) & 3;
                            *renkKanallari[k] = (*renkKanallari[k] & 0xFC) | bit; // Maske: 11111100

                            bitIndex += 2;
                            if (bitIndex >= 8) { bitIndex = 0; charIndex++; }
                        }
                    }
                }
                delete[] Mesaj; break;
            }

            case 3: {
                int guncelKapasite = a * 3;
                cout << "Sifre yazma islemi basladi (3 Bit Modu)." << endl;
                char* Mesaj = new char[guncelKapasite];

                cout << "Lutfen sifrelenecek mesaji giriniz (max " << guncelKapasite - 1 << " karakter): ";
                cin.getline(Mesaj, guncelKapasite);
                
                int mesajUzunlugu = strlen(Mesaj);
                Mesaj[mesajUzunlugu] = '\0'; mesajUzunlugu++;
                int charIndex = 0; int bitIndex = 0;

                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char* renkKanallari[3] = { &matrix[i][j].b, &matrix[i][j].g, &matrix[i][j].r };
                        for (int k = 0; k < 3; k++) {
                            if (charIndex >= mesajUzunlugu) { cout << "Sifreleme tamamlandi!" << endl; delete[] Mesaj; return; }
                            
                            unsigned char bit = (Mesaj[charIndex] >> bitIndex) & 7;
                            *renkKanallari[k] = (*renkKanallari[k] & 0xF8) | bit; // Maske: 11111000

                            bitIndex += 3;
                            if (bitIndex >= 8) { bitIndex = 0; charIndex++; }
                        }
                    }
                }
                delete[] Mesaj; break;
            }

            case 4: {
                int guncelKapasite = a * 4;
                cout << "Sifre yazma islemi basladi (4 Bit Modu)." << endl;
                char* Mesaj = new char[guncelKapasite];

                cout << "Lutfen sifrelenecek mesaji giriniz (max " << guncelKapasite - 1 << " karakter): ";
                cin.getline(Mesaj, guncelKapasite);
                
                int mesajUzunlugu = strlen(Mesaj);
                Mesaj[mesajUzunlugu] = '\0'; mesajUzunlugu++;
                int charIndex = 0; int bitIndex = 0;

                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char* renkKanallari[3] = { &matrix[i][j].b, &matrix[i][j].g, &matrix[i][j].r };
                        for (int k = 0; k < 3; k++) {
                            if (charIndex >= mesajUzunlugu) { cout << "Sifreleme tamamlandi!" << endl; delete[] Mesaj; return; }

                            unsigned char bit = (Mesaj[charIndex] >> bitIndex) & 15;
                            *renkKanallari[k] = (*renkKanallari[k] & 0xF0) | bit; // Maske: 11110000

                            bitIndex += 4;
                            if (bitIndex >= 8) { bitIndex = 0; charIndex++; }
                        }
                    }
                }
                delete[] Mesaj; break;
            }

            case 5: {
                int guncelKapasite = a * 5;
                cout << "Sifre yazma islemi basladi (5 Bit Modu)." << endl;
                char* Mesaj = new char[guncelKapasite];

                cout << "Lutfen sifrelenecek mesaji giriniz (max " << guncelKapasite - 1 << " karakter): ";
                cin.getline(Mesaj, guncelKapasite);
                
                int mesajUzunlugu = strlen(Mesaj);
                Mesaj[mesajUzunlugu] = '\0'; mesajUzunlugu++;
                int charIndex = 0; int bitIndex = 0;

                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char* renkKanallari[3] = { &matrix[i][j].b, &matrix[i][j].g, &matrix[i][j].r };
                        for (int k = 0; k < 3; k++) {
                            if (charIndex >= mesajUzunlugu) { cout << "Sifreleme tamamlandi!" << endl; delete[] Mesaj; return; }
                            
                            unsigned char bit = (Mesaj[charIndex] >> bitIndex) & 31;
                            *renkKanallari[k] = (*renkKanallari[k] & 0xE0) | bit; // Maske: 11100000
                            
                            bitIndex += 5;
                            if (bitIndex >= 8) { bitIndex = 0; charIndex++; }
                        }
                    }
                }
                delete[] Mesaj; break;
            }

            case 6: {
                int guncelKapasite = a * 6;
                cout << "Sifre yazma islemi basladi (6 Bit Modu)." << endl;
                char* Mesaj = new char[guncelKapasite];

                cout << "Lutfen sifrelenecek mesaji giriniz (max " << guncelKapasite - 1 << " karakter): ";
                cin.getline(Mesaj, guncelKapasite);
                
                int mesajUzunlugu = strlen(Mesaj);
                Mesaj[mesajUzunlugu] = '\0'; mesajUzunlugu++;
                int charIndex = 0; int bitIndex = 0;

                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char* renkKanallari[3] = { &matrix[i][j].b, &matrix[i][j].g, &matrix[i][j].r };
                        for (int k = 0; k < 3; k++) {
                            if (charIndex >= mesajUzunlugu) { cout << "Sifreleme tamamlandi!" << endl; delete[] Mesaj; return; }

                            unsigned char bit = (Mesaj[charIndex] >> bitIndex) & 63;
                            *renkKanallari[k] = (*renkKanallari[k] & 0xC0) | bit; // Maske: 11000000

                            bitIndex += 6;
                            if (bitIndex >= 8) { bitIndex = 0; charIndex++; }
                        }
                    }
                }
                delete[] Mesaj; break;
            }

            case 7: {
                int guncelKapasite = a * 7;
                cout << "Sifre yazma islemi basladi (7 Bit Modu)." << endl;
                char* Mesaj = new char[guncelKapasite];

                cout << "Lutfen sifrelenecek mesaji giriniz (max " << guncelKapasite - 1 << " karakter): ";
                cin.getline(Mesaj, guncelKapasite);
                
                int mesajUzunlugu = strlen(Mesaj);
                Mesaj[mesajUzunlugu] = '\0'; mesajUzunlugu++;
                int charIndex = 0; int bitIndex = 0;

                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char* renkKanallari[3] = { &matrix[i][j].b, &matrix[i][j].g, &matrix[i][j].r };
                        for (int k = 0; k < 3; k++) {
                            if (charIndex >= mesajUzunlugu) { cout << "Sifreleme tamamlandi!" << endl; delete[] Mesaj; return; }

                            unsigned char bit = (Mesaj[charIndex] >> bitIndex) & 127;
                            *renkKanallari[k] = (*renkKanallari[k] & 0x80) | bit; // Maske: 10000000

                            bitIndex += 7;
                            if (bitIndex >= 8) { bitIndex = 0; charIndex++; }
                        }
                    }
                }
                delete[] Mesaj; break;
            }

            case 8: {
                int guncelKapasite = a * 8;
                cout << "Sifre yazma islemi basladi (8 Bit Modu - Tam Degisim)." << endl;
                char* Mesaj = new char[guncelKapasite];

                cout << "Lutfen sifrelenecek mesaji giriniz (max " << guncelKapasite - 1 << " karakter): ";
                cin.getline(Mesaj, guncelKapasite);
                
                int mesajUzunlugu = strlen(Mesaj);
                Mesaj[mesajUzunlugu] = '\0'; mesajUzunlugu++;
                int charIndex = 0; int bitIndex = 0;

                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char* renkKanallari[3] = { &matrix[i][j].b, &matrix[i][j].g, &matrix[i][j].r };
                        for (int k = 0; k < 3; k++) {
                            if (charIndex >= mesajUzunlugu) { cout << "Sifreleme tamamlandi!" << endl; delete[] Mesaj; return; }

                            // 8 bit olduğu için direkt harfi gömüyoruz
                            unsigned char bit = (Mesaj[charIndex] >> bitIndex) & 255;
                            *renkKanallari[k] = (*renkKanallari[k] & 0x00) | bit; // Maske: 00000000 (Rengi tamamen siler)

                            bitIndex += 8;
                            if (bitIndex >= 8) { bitIndex = 0; charIndex++; }
                        }
                    }
                }
                delete[] Mesaj; break;
            }

            default:
                cout << "Gecersiz deger! 1-8 arasinda secim yapiniz." << endl;
                break;
        }
    }


    void Sifrecozme(int a) {
        int Bitsayisi;
        cout << "Bu resim kac bit derinligi ile sifrelendi? (1 ile 8 arasinda): ";
        cin >> Bitsayisi;

        cout << "Sifre cozme islemi basladi..." << endl;
        cout << "------------------------------------------------" << endl;
        cout << "BULUNAN MESAJ: ";

        // Okunan karakter sayısını ve bitleri tutacak değişkenler
        int okunanKarakterSayisi = 0;
        unsigned char mesajBitleri = 0;
        int bitSayaci = 0;

        switch (Bitsayisi) {
            case 1: {
                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char Renkler[3] = {matrix[i][j].b, matrix[i][j].g, matrix[i][j].r};
                        for (int k = 0; k < 3; k++) {
                            // 1. ADIM: Maskeleme (Son 1 biti al)
                            unsigned char parca = Renkler[k] & 1; 

                            // 2. ADIM: Bitleri biriktir
                            mesajBitleri |= (parca << bitSayaci);
                            bitSayaci += 1; // 1 bit ilerle

                            // 3. ADIM: 8 bit doldu mu?
                            if (bitSayaci >= 8) {
                                char bulunan = (char)mesajBitleri;
                                
                                // Karakteri Yazdır
                                if (bulunan == '\0') { cout << "\n\n[Bitti] Gizli Mesaj Sonu." << endl; return; }
                                cout << bulunan;

                                // Sıfırla
                                mesajBitleri = 0;
                                bitSayaci = 0;
                                okunanKarakterSayisi++;
                                if (okunanKarakterSayisi >= a) return;
                            }
                        }
                    }
                }
                break;
            }

            case 2: {
                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char Renkler[3] = {matrix[i][j].b, matrix[i][j].g, matrix[i][j].r};
                        for (int k = 0; k < 3; k++) {
                            // Maskeleme: Son 2 bit (binary 11 -> 3)
                            unsigned char parca = Renkler[k] & 3; 

                            mesajBitleri |= (parca << bitSayaci);
                            bitSayaci += 2; // 2 bit ilerle

                            if (bitSayaci >= 8) {
                                char bulunan = (char)mesajBitleri;
                                if (bulunan == '\0') { cout << "\n\n[Bitti] Gizli Mesaj Sonu." << endl; return; }
                                cout << bulunan;
                                mesajBitleri = 0; bitSayaci = 0; okunanKarakterSayisi++;
                                if (okunanKarakterSayisi >= a * 2) return;
                            }
                        }
                    }
                }
                break;
            }

            case 3: {
                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char Renkler[3] = {matrix[i][j].b, matrix[i][j].g, matrix[i][j].r};
                        for (int k = 0; k < 3; k++) {
                            // Maskeleme: Son 3 bit (binary 111 -> 7)
                            unsigned char parca = Renkler[k] & 7; 

                            mesajBitleri |= (parca << bitSayaci);
                            bitSayaci += 3; // 3 bit ilerle

                            if (bitSayaci >= 8) {
                                char bulunan = (char)mesajBitleri; // Fazlalık bitler otomatik atılır (cast)
                                if (bulunan == '\0') { cout << "\n\n[Bitti] Gizli Mesaj Sonu." << endl; return; }
                                cout << bulunan;
                                mesajBitleri = 0; bitSayaci = 0; okunanKarakterSayisi++;
                                if (okunanKarakterSayisi >= a * 3) return;
                            }
                        }
                    }
                }
                break;
            }

            case 4: {
                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char Renkler[3] = {matrix[i][j].b, matrix[i][j].g, matrix[i][j].r};
                        for (int k = 0; k < 3; k++) {
                            // Maskeleme: Son 4 bit (binary 1111 -> 15)
                            unsigned char parca = Renkler[k] & 15; 

                            mesajBitleri |= (parca << bitSayaci);
                            bitSayaci += 4; // 4 bit ilerle

                            if (bitSayaci >= 8) {
                                char bulunan = (char)mesajBitleri;
                                if (bulunan == '\0') { cout << "\n\n[Bitti] Gizli Mesaj Sonu." << endl; return; }
                                cout << bulunan;
                                mesajBitleri = 0; bitSayaci = 0; okunanKarakterSayisi++;
                                if (okunanKarakterSayisi >= a * 4) return;
                            }
                        }
                    }
                }
                break;
            }

            case 5: {
                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char Renkler[3] = {matrix[i][j].b, matrix[i][j].g, matrix[i][j].r};
                        for (int k = 0; k < 3; k++) {
                            unsigned char parca = Renkler[k] & 31; // Maske: 11111 (31)
                            mesajBitleri |= (parca << bitSayaci);
                            bitSayaci += 5;

                            if (bitSayaci >= 8) {
                                char bulunan = (char)mesajBitleri;
                                if (bulunan == '\0') { cout << "\n\n[Bitti] Gizli Mesaj Sonu." << endl; return; }
                                cout << bulunan;
                                mesajBitleri = 0; bitSayaci = 0; okunanKarakterSayisi++;
                                if (okunanKarakterSayisi >= a * 5) return;
                            }
                        }
                    }
                }
                break;
            }

            case 6: {
                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char Renkler[3] = {matrix[i][j].b, matrix[i][j].g, matrix[i][j].r};
                        for (int k = 0; k < 3; k++) {
                            unsigned char parca = Renkler[k] & 63; // Maske: 111111 (63)
                            mesajBitleri |= (parca << bitSayaci);
                            bitSayaci += 6;

                            if (bitSayaci >= 8) {
                                char bulunan = (char)mesajBitleri;
                                if (bulunan == '\0') { cout << "\n\n[Bitti] Gizli Mesaj Sonu." << endl; return; }
                                cout << bulunan;
                                mesajBitleri = 0; bitSayaci = 0; okunanKarakterSayisi++;
                                if (okunanKarakterSayisi >= a * 6) return;
                            }
                        }
                    }
                }
                break;
            }

            case 7: {
                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char Renkler[3] = {matrix[i][j].b, matrix[i][j].g, matrix[i][j].r};
                        for (int k = 0; k < 3; k++) {
                            unsigned char parca = Renkler[k] & 127; // Maske: 1111111 (127)
                            mesajBitleri |= (parca << bitSayaci);
                            bitSayaci += 7;

                            if (bitSayaci >= 8) {
                                char bulunan = (char)mesajBitleri;
                                if (bulunan == '\0') { cout << "\n\n[Bitti] Gizli Mesaj Sonu." << endl; return; }
                                cout << bulunan;
                                mesajBitleri = 0; bitSayaci = 0; okunanKarakterSayisi++;
                                if (okunanKarakterSayisi >= a * 7) return;
                            }
                        }
                    }
                }
                break;
            }

            case 8: {
                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char Renkler[3] = {matrix[i][j].b, matrix[i][j].g, matrix[i][j].r};
                        for (int k = 0; k < 3; k++) {
                            // 8 bit direkt okuma (Maske 255 -> 11111111)
                            // Burada biriktirmeye gerek yok, her renk 1 harf taşır.
                            char bulunan = (char)(Renkler[k] & 255);
                            
                            if (bulunan == '\0') { cout << "\n\n[Bitti] Gizli Mesaj Sonu." << endl; return; }
                            cout << bulunan;
                            
                            okunanKarakterSayisi++;
                            if (okunanKarakterSayisi >= a * 8) return;
                        }
                    }
                }
                break;
            }

            default:
                cout << "Gecersiz bit derinligi!" << endl;
                break;
        }
        cout << "\n------------------------------------------------" << endl;
    }

    // Yıkıcı fonksiyon (Bellek temizliği)
    ~Image() {
        for (int i = 0; i < height; i++)
            delete[] matrix[i];
        delete[] matrix;
    }
};

int main() {
    int w, h;
    char inputFile[100]; 
    char outputFile[100]; 
    int secim;

    cout << "--- BMP Steganografi Programi ---" << endl;

    // 1. Resim bilgilerini alıyorum
    cout << "Lutfen resim genislik ve yuksekligini giriniz (ornek: 400 300): ";
    cin >> w >> h;
    if (w <= 0 || h <= 0 || w == char(0) || h == char(0)) {
        cout << "Gecersiz boyutlar! Program sonlandiriliyor." << endl;
        return 1;
    }

    // Nesneyi oluşturuyorum
    Image img(w, h); 

    cout << "Okunacak dosya ismini giriniz (ornek: input.bmp): ";
    cin >> inputFile;

        ifstream file(inputFile, ios::binary);
        if (!file) {
            cout << "HATA: BMP dosyasi acilamadi! Program sonlandiriliyor." << endl;
            return 1;
        } 

    // 2. Resmi Yükleme yeri
    img.yukleBMP(inputFile);

    // 3. Menü Seçimi
    cout << "\nISLEM SECIMI:" << endl;
    cout << "1. Sifre Yazma (Resmin icine mesaj gizle)" << endl;
    cout << "2. Sifre Cozme (Resimdeki gizli mesaji oku)" << endl;
    cout << "Seciminiz: ";
    cin >> secim;
    
    // Tampon belleği temizle (Enter karakterini yutmak için şart)
    cin.ignore(); 

    // Maksimum karakter kapasitesi: (Genişlik * Yükseklik * 3 kanal) / 8 bit
    int maxKapasite = (w * h * 3) / 8;

    if (secim == 1) {
        //ŞİFRELEME
        img.Sifreyazma(maxKapasite); 

        cout << "Kaydedilecek dosya ismini giriniz (ornek: output.bmp): ";
        cin >> outputFile;

        img.kaydetBMP(outputFile);

    } else if (secim == 2) {
        //ŞİFRE ÇÖZME
        // Okunacak maksimum boyutu gönderiyoruz.
        img.Sifrecozme(maxKapasite); 
        
    } else {
        cout << "Gecersiz secim! Program sonlandiriliyor." << endl;
    }

    return 0;
}
    

