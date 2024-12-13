#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "imageloader.hpp" // Inclua o arquivo com a funcao loadPPM

using namespace std;

// Estrutura para uma aresta do grafo
struct Edge {
    int src; // Vértice de origem
    int dest; // Vértice de destino
    double weight; // Peso da aresta
};

// Classe para representar o grafo
class Graph {
public:
    int numVertices; // Número total de vértices (pixels)
    vector<Edge> edges; // Lista de arestas

    // Construtor para criar o grafo a partir da imagem
    Graph(const vector<vector<Pixel>>& image, int width, int height) {
        numVertices = width * height;

        // Adicionar arestas baseadas em pixels vizinhos
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int current = y * width + x;

                // Adicionar aresta para o pixel da direita
                if (x + 1 < width) {
                    int right = y * width + (x + 1);
                    double weight = calculateWeight(image[y][x], image[y][x + 1]);
                    edges.push_back({current, right, weight});
                }

                // Adicionar aresta para o pixel abaixo
                if (y + 1 < height) {
                    int below = (y + 1) * width + x;
                    double weight = calculateWeight(image[y][x], image[y + 1][x]);
                    edges.push_back({current, below, weight});
                }

                // Opcional: Adicionar arestas diagonais (inferior direito)
                if (x + 1 < width && y + 1 < height) {
                    int diagRight = (y + 1) * width + (x + 1);
                    double weight = calculateWeight(image[y][x], image[y + 1][x + 1]);
                    edges.push_back({current, diagRight, weight});
                }

                // Opcional: Adicionar arestas diagonais (inferior esquerdo)
                if (x - 1 >= 0 && y + 1 < height) {
                    int diagLeft = (y + 1) * width + (x - 1);
                    double weight = calculateWeight(image[y][x], image[y + 1][x - 1]);
                    edges.push_back({current, diagLeft, weight});
                }
            }
        }
    }

    // Função para exibir o grafo
    void printGraph() const {
        for (const auto& edge : edges) {
            cout << "Origem: " << edge.src
                 << ", Destino: " << edge.dest
                 << ", Peso: " << edge.weight << endl;
        }
    }

    // Função estática para criar e retornar o grafo a partir de uma imagem
    static Graph createGraph(const vector<vector<Pixel>>& image, int width, int height) {
        return Graph(image, width, height);
    }

private:
    // Função para calcular o peso de uma aresta com base na diferença de intensidade
    double calculateWeight(const Pixel& p1, const Pixel& p2) {
        double diffR = p1.r - p2.r;
        double diffG = p1.g - p2.g;
        double diffB = p1.b - p2.b;
        return sqrt(diffR * diffR + diffG * diffG + diffB * diffB); // Distância Euclidiana no espaço RGB
    }
};

#endif
