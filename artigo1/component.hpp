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
    vector<int> parent;
    vector<int> rank;

    GraphComponents(int numVertices) {
        parent.resize(numVertices);
        rank.resize(numVertices, 0);
        for (int i = 0; i < numVertices; ++i) {
            parent[i] = i;
        }
    }

    int find(int vertex) {
        if (parent[vertex] != vertex) {
            parent[vertex] = find(parent[vertex]);
        }
        return parent[vertex];
    }

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

    vector<Edge> findMST(Graph& graph, int componentRoot) {
        vector<Edge> mstEdges;
        vector<Edge> componentEdges;

        for (const auto& edge : graph.edges) {
            if (find(edge.a) == componentRoot && find(edge.b) == componentRoot) {
                componentEdges.push_back(edge);
            }
        }

        sort(componentEdges.begin(), componentEdges.end(), [](const Edge& a, const Edge& b) {
            return a.weightR + a.weightG + a.weightB < b.weightR + b.weightG + b.weightB;
        });

        GraphComponents mstUF(graph.numVertices);

        for (const auto& edge : componentEdges) {
            if (mstUF.find(edge.a) != mstUF.find(edge.b)) {
                mstUF.unite(edge.a, edge.b);
                mstEdges.push_back(edge);
            }
        }

        return mstEdges;
    }

    double calculateInternalDifferenceFromMST(Graph& graph, int componentRoot) {
        vector<Edge> mstEdges = findMST(graph, componentRoot);
        double maxWeight = 0.0;
        for (const auto& edge : mstEdges) {
            maxWeight = max(maxWeight, edge.weightR + edge.weightG + edge.weightB);
        }
        return maxWeight;
    }

    void segmentGraph(Graph& graph, double k) {
        vector<double> internalDifference(graph.numVertices, 0.0);
        vector<int> componentSize(graph.numVertices, 1);

        sort(graph.edges.begin(), graph.edges.end(), [](const Edge& a, const Edge& b) {
            return a.weightR + a.weightG + a.weightB < b.weightR + b.weightG + b.weightB;
        });

        for (const auto& edge : graph.edges) {
            int root1 = find(edge.a);
            int root2 = find(edge.b);

            if (root1 != root2) {
                internalDifference[root1] = calculateInternalDifferenceFromMST(graph, root1);
                internalDifference[root2] = calculateInternalDifferenceFromMST(graph, root2);

                double diff = edge.weightR + edge.weightG + edge.weightB;
                double tau1 = k / componentSize[root1];
                double tau2 = k / componentSize[root2];

                double minInternalDifference = min(internalDifference[root1] + tau1, internalDifference[root2] + tau2);

                if (diff <= minInternalDifference) {
                    unite(root1, root2);
                    int newRoot = find(root1);

                    internalDifference[newRoot] = calculateInternalDifferenceFromMST(graph, newRoot);
                    componentSize[newRoot] = componentSize[root1] + componentSize[root2];
                }
            }
        }
    }

    int getNumberOfComponents() {
        int components = 0;
        for (int i = 0; i < parent.size(); ++i) {
            if (parent[i] == i) {
                components++;
            }
        }
        return components;
    }

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
