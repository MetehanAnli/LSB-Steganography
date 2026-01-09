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
    Color** matrix; // 2D array holding pixel data
public:
    Image(int w, int h) {
        width = w;
        height = h;
        matrix = new Color*[height];
        for (int i = height - 1; i >= 0; i--)
            matrix[i] = new Color[width];
    }

    // Load BMP image into matrix
    void loadBMP(const char* filename) {
        ifstream file(filename, ios::binary);
        unsigned char header[54];
        file.read((char*)header, 54);

        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                unsigned char pixel[3];
                file.read((char*)pixel, 3);
                matrix[i][j].b = pixel[0];
                matrix[i][j].g = pixel[1];
                matrix[i][j].r = pixel[2];
            }
        }
        file.close();
        cout << "Image loaded successfully." << endl;
    }

    void saveBMP(const char* filename) {
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
        cout << "Image saved as " << filename << endl;
    }

    void embedMessage(int baseCapacity) {
        int bitDepth;
        cout << "Enter LSB bit depth (1-8): ";
        cin >> bitDepth;
        cin.ignore();

        switch (bitDepth) {
            case 1: case 2: case 3: case 4:
            case 5: case 6: case 7: case 8: {
                int currentCapacity = baseCapacity * bitDepth;
                cout << "Embedding started (" << bitDepth << "-bit LSB mode)." << endl;

                char* message = new char[currentCapacity];
                cout << "Enter message (max " << currentCapacity - 1 << " characters): ";
                cin.getline(message, currentCapacity);

                int messageLength = strlen(message);
                message[messageLength] = '\0';
                messageLength++;

                int charIndex = 0, bitIndex = 0;

                for (int i = height - 1; i >= 0; i--) {
                    for (int j = 0; j < width; j++) {
                        unsigned char* channels[3] = {
                            &matrix[i][j].b,
                            &matrix[i][j].g,
                            &matrix[i][j].r
                        };
                        for (int k = 0; k < 3; k++) {
                            if (charIndex >= messageLength) {
                                cout << "Embedding completed." << endl;
                                delete[] message;
                                return;
                            }

                            unsigned char bits =
                                (message[charIndex] >> bitIndex) & ((1 << bitDepth) - 1);

                            *channels[k] =
                                (*channels[k] & ~((1 << bitDepth) - 1)) | bits;

                            bitIndex += bitDepth;
                            if (bitIndex >= 8) {
                                bitIndex = 0;
                                charIndex++;
                            }
                        }
                    }
                }
                delete[] message;
                break;
            }

            default:
                cout << "Invalid bit depth!" << endl;
        }
    }

    void extractMessage(int baseCapacity) {
        int bitDepth;
        cout << "Enter LSB bit depth used during embedding (1-8): ";
        cin >> bitDepth;

        cout << "Extracting hidden message..." << endl;
        cout << "----------------------------------------" << endl;
        cout << "EXTRACTED MESSAGE: ";

        int charCount = 0;
        unsigned char currentChar = 0;
        int bitCounter = 0;

        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                unsigned char colors[3] = {
                    matrix[i][j].b,
                    matrix[i][j].g,
                    matrix[i][j].r
                };
                for (int k = 0; k < 3; k++) {
                    unsigned char part = colors[k] & ((1 << bitDepth) - 1);
                    currentChar |= (part << bitCounter);
                    bitCounter += bitDepth;

                    if (bitCounter >= 8) {
                        char found = (char)currentChar;
                        if (found == '\0') {
                            cout << "\n\n[End of Message]" << endl;
                            return;
                        }
                        cout << found;
                        currentChar = 0;
                        bitCounter = 0;
                        charCount++;
                        if (charCount >= baseCapacity * bitDepth)
                            return;
                    }
                }
            }
        }
        cout << "\n----------------------------------------" << endl;
    }

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
    int choice;

    cout << "--- BMP LSB Steganography Tool ---" << endl;

    cout << "Enter image width and height (e.g. 400 300): ";
    cin >> w >> h;
    if (w <= 0 || h <= 0) {
        cout << "Invalid dimensions!" << endl;
        return 1;
    }

    Image img(w, h);

    cout << "Enter input BMP filename: ";
    cin >> inputFile;

    ifstream file(inputFile, ios::binary);
    if (!file) {
        cout << "ERROR: Cannot open BMP file!" << endl;
        return 1;
    }

    img.loadBMP(inputFile);

    cout << "\nOPERATION:" << endl;
    cout << "1. Embed message" << endl;
    cout << "2. Extract message" << endl;
    cout << "Choice: ";
    cin >> choice;
    cin.ignore();

    int maxCapacity = (w * h * 3) / 8;

    if (choice == 1) {
        img.embedMessage(maxCapacity);
        cout << "Enter output filename: ";
        cin >> outputFile;
        img.saveBMP(outputFile);
    }
    else if (choice == 2) {
        img.extractMessage(maxCapacity);
    }
    else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
