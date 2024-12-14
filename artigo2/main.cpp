

#include "graph.hpp"
#include "imageloader.hpp" // mesmo codigo do primeiro artigo

int main()
{
    string filename = "images/teste.ppm";
    int width, height;

    try
    {
        // Carregar a imagem e obter a matriz de pixels
        auto image = loadPPM(filename, width, height);

        double sigma = 50.0;

        vector<double> objProb(width * height, 0.7); // Probabilidade inicial de ser objeto
        vector<double> bgProb(width * height, 0.3);  // Probabilidade inicial de ser fundo

        vector<bool> isSeedObject(width * height, false);
        vector<bool> isSeedBackground(width * height, false);
        // Exemplo: marque alguns pixels manualmente como seeds
        isSeedObject[2] = true;    
        isSeedBackground[8] = true; 

        // Criar o grafo a partir da imagem
        Graph graph(image, width, height, sigma, objProb, bgProb);

        // Adicionar restrições duras para seeds manuais
        for (int i = 0; i < width * height; ++i)
        {
            if (isSeedObject[i])
            {
                graph.tLinkSource[i] = INFINITY;
                graph.tLinkSink[i] = 0;
            }
            if (isSeedBackground[i])
            {
                graph.tLinkSource[i] = 0;
                graph.tLinkSink[i] = INFINITY;
            }
        }

        // Exibir o grafo para verificação
        graph.printGraph();

    }
    catch (const exception &e)
    {
        cerr << "Erro: " << e.what() << endl;
        return -1;
    }

    return 0;
}