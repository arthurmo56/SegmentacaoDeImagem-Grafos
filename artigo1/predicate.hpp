#ifndef PREDICATE_H
#define PREDICATE_H

#include <vector>
#include <cmath>
#include <algorithm>
#include "graph.hpp"
#include "component.hpp"

using namespace std;

class Predicate {
public:
    vector<double> internalDifference; // Diferenças internas das componentes
    vector<int> componentSize; // Tamanho de cada componente
    GraphComponents& components; // Referência aos componentes conectados
    double k; // Parâmetro de controle

    // Função para calcular o tau de uma região
    double calculateTau(int regionSize) {
        return k / regionSize;
    }

    // Função auxiliar para obter a diferença mínima entre duas regiões
    double getMinEdgeWeight(const Edge& edge) {
        return edge.weightR + edge.weightG + edge.weightB; // Soma dos pesos RGB
    }


    Predicate(GraphComponents& graphComponents, int numVertices, double kParam)
        : components(graphComponents), k(kParam) {
        internalDifference.resize(numVertices, 0.0);
        componentSize.resize(numVertices, 1);
    }

    // Função para avaliar o predicado de comparação
    bool evaluate(const Edge& edge) {
        int root1 = components.find(edge.src);
        int root2 = components.find(edge.dest);

        if (root1 == root2) {
            // Os vértices já estão na mesma componente
            return false;
        }

        // Calcular tau para cada região
        double tau1 = calculateTau(componentSize[root1]);
        double tau2 = calculateTau(componentSize[root2]);

        // Calcular o limiar de comparação
        double threshold = max(internalDifference[root1] + tau1, internalDifference[root2] + tau2);

        // Obter o peso da aresta
        double edgeWeight = getMinEdgeWeight(edge);

        // Avaliar o predicado: true se as regiões permanecerem separadas
        return edgeWeight > threshold;
    }

    // Atualizar as informações após unir duas regiões
    void mergeRegions(const Edge& edge) {
        int root1 = components.find(edge.src);
        int root2 = components.find(edge.dest);

        if (root1 != root2) {
            // Atualizar o tamanho e a diferença interna da nova região
            double edgeWeight = getMinEdgeWeight(edge);
            components.unite(root1, root2);

            int newRoot = components.find(root1);
            internalDifference[newRoot] = max(internalDifference[root1], max(internalDifference[root2], edgeWeight));
            componentSize[newRoot] = componentSize[root1] + componentSize[root2];
        }
    }
};

#endif
