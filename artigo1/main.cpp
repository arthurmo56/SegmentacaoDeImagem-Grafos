#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>

#include "imageloader.hpp"


using namespace std;
// Função para imprimir a matriz de pixels
void printImage(const vector<vector<Pixel>>& image, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& p = image[y][x];
            cout << "(" << p.r << ", " << p.g << ", " << p.b << ")";
            if (x < width - 1) cout << " "; // Espaço entre pixels
        }
        cout << "\n"; // Nova linha após cada linha de pixels
    }
}

int main() {
    string filename = "japao.ppm";
    int width, height;

    try {
        // Carregar a imagem e obter a matriz de pixels
        auto image = loadPPM(filename, width, height);

        // Imprimir a imagem de forma organizada
        cout << "Imagem carregada (" << width << "x" << height << "):\n";
        printImage(image, width, height);
    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
        return -1;
    }

    return 0;
}