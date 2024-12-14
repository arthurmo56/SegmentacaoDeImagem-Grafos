#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>

#include "imageloader.hpp"
#include "graph.hpp"
#include "component.hpp"
#include "predicate.hpp"

using namespace std;
// Função para imprimir a matriz de pixels
void printImage(const vector<vector<Pixel>> &image, int width, int height)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const Pixel &p = image[y][x];
            cout << "(" << p.r << ", " << p.g << ", " << p.b << ")";
            if (x < width - 1)
                cout << " "; // Espaço entre pixels
        }
        cout << "\n"; // Nova linha após cada linha de pixels
    }
}

// Função para mostrar todos os pixels
void printPixels(const vector<vector<Pixel>> &image, int width, int height)
{
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                cout << "Pixel [" << y << "][" << x << "]: "
                     << "R=" << image[y][x].r << ", "
                     << "G=" << image[y][x].g << ", "
                     << "B=" << image[y][x].b << endl;
            }
        }
}

int main()
{
    string filename = "teste.ppm";
    int width, height;

    try
    {
        // Carregar a imagem e obter a matriz de pixels
        auto image = loadPPM(filename, width, height);

        // Imprimir a imagem de forma organizada
        // cout << "Imagem carregada (" << width << "x" << height << "):\n";
        // printImage(image, width, height);

        // Criar o grafo a partir da imagem
        Graph graph(image, width, height);

        // Número de vértices do grafo criado
        //printf("\n%d\n", graph.numVertices);

        // Imprimir informações do grafo para verificar
        //graph.printGraph();

        // Parâmetro k de controle 
        double K = 1.0;

        GraphComponents graphComponents(graph, K);


        // Inicializar o predicado
        Predicate predicate(graphComponents, graph.numVertices, K);

        // Processar as arestas
        for (const auto& edge : graph.edges) {
            if (!predicate.evaluate(edge)) {
                predicate.mergeRegions(edge);
            }
        }

        // Exibir os componentes resultantes
        graphComponents.printComponents();

        // Número total de componentes
        int totalComponents = graphComponents.getNumberOfComponents();
        cout << "Numero total de componentes: " << totalComponents << endl;

        // Imprimir pixels e suas posições
        // printPixels(image, width, height);
    }
    catch (const exception &e)
    {
        cerr << "Erro: " << e.what() << endl;
        return -1;
    }

    return 0;
}