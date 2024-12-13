#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include "graph.hpp"

using namespace std;

class GraphComponents {
private:
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

public:
    GraphComponents(const Graph& graph) {
        // Inicializar o DSU para todos os vértices
        parent.resize(graph.numVertices);
        rank.resize(graph.numVertices, 0);
        for (int i = 0; i < graph.numVertices; ++i) {
            parent[i] = i; // Cada vértice é seu próprio pai inicialmente
        }

        // Processar todas as arestas para unir componentes
        for (const auto& edge : graph.edges) {
            unite(edge.src, edge.dest);
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
