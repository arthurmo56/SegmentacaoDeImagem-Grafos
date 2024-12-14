#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "graph.hpp"

using namespace std;

class GraphComponents {
public:
    vector<int> parent; // Vetor para armazenar o pai de cada vértice
    vector<int> rank;   // Vetor para armazenar a "altura" da árvore de cada conjunto

    // Encontrar o representante (raiz) do conjunto ao qual o vértice pertence
    int find(int vertex) {
        if (parent[vertex] != vertex) {
            parent[vertex] = find(parent[vertex]); // Compressão de caminho
        }
        return parent[vertex];
    }

    // Unir dois conjuntos com base na classificação (rank)
    void unite(int vertex1, int vertex2) {
        int root1 = find(vertex1);
        int root2 = find(vertex2);

        if (root1 != root2) {
            if (rank[root1] > rank[root2]) {
                parent[root2] = root1;
            } else if (rank[root1] < rank[root2]) {
                parent[root1] = root2;
            } else {
                parent[root2] = root1;
                rank[root1]++;
            }
        }
    }

    // Calcular a norma euclidiana dos pesos RGB
    double calculateEdgeWeight(const Edge& edge) {
        return sqrt(edge.weightR * edge.weightR +
                    edge.weightG * edge.weightG +
                    edge.weightB * edge.weightB);
    }

    // Predicado para decidir se dois componentes devem ser unidos
    bool predicate(double internalDiff1, double internalDiff2, double edgeWeight, double K) {
        double minInternalDiff = min(internalDiff1, internalDiff2);
        return edgeWeight > minInternalDiff + K; // Se true, não unir; se false, unir
    }


    GraphComponents(Graph& graph, double K) {
        // Inicializar o DSU para todos os vértices
        parent.resize(graph.numVertices);
        rank.resize(graph.numVertices, 0);
        for (int i = 0; i < graph.numVertices; ++i) {
            parent[i] = i; // Cada vértice é seu próprio pai inicialmente
        }

        // Calcular pesos totais para todas as arestas
        for (auto& edge : graph.edges) {
            edge.weightR = calculateEdgeWeight(edge);
        }

        // Ordenar arestas pelo peso total
        sort(graph.edges.begin(), graph.edges.end(), [](const Edge& a, const Edge& b) {
            return a.weightR < b.weightR; // Usando o peso calculado
        });

        // Inicializar o vetor de diferenças internas
        vector<double> internalDiff(graph.numVertices, 0.0);

        // Processar as arestas ordenadas
        for (const auto& edge : graph.edges) {
            int root1 = find(edge.src);
            int root2 = find(edge.dest);

            if (root1 != root2) {
                double edgeWeight = calculateEdgeWeight(edge);

                // Calcular o predicado
                if (!predicate(internalDiff[root1], internalDiff[root2], edgeWeight, K)) {
                    unite(root1, root2);
                    // Atualizar a diferença interna do novo componente
                    internalDiff[find(root1)] = edgeWeight;
                }
            }
        }
    }

    // Obter o número de componentes conexas
    int getNumberOfComponents() {
        int components = 0;
        for (int i = 0; i < parent.size(); ++i) {
            if (parent[i] == i) {
                components++;
            }
        }
        return components;
    }

    // Exibir as componentes conexas
    void printComponents() {
        vector<vector<int>> components(parent.size());
        for (int i = 0; i < parent.size(); ++i) {
            components[find(i)].push_back(i);
        }

        for (const auto& component : components) {
            if (!component.empty()) {
                cout << "Componente: ";
                for (int vertex : component) {
                    cout << vertex << " ";
                }
                cout << endl;
            }
        }
    }
};

#endif
