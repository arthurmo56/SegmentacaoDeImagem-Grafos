#include "graph.hpp"
#include "imageloader.hpp"
#include "maxflow.hpp"
#include "color_segments.hpp"

int main()
{
    string filename = "images/teste2.ppm";
    int width, height;

    try
    {
        // Carregar a imagem e obter a matriz de pixels
        auto image = loadPPM(filename, width, height);

        double sigma = 10.0;

        vector<double> objProb(width * height, 0.5); // Probabilidade inicial de ser objeto
        vector<double> bgProb(width * height, 0.5);  // Probabilidade inicial de ser fundo

        vector<bool> isSeedObject(width * height, false);
        vector<bool> isSeedBackground(width * height, false);
        // Exemplo: marque alguns pixels manualmente como seeds
        // Função para verificar proximidade de uma cor

        isSeedObject[1] = true;
        objProb[1] = 0.99; // Alta probabilidade de objeto
        bgProb[1] = 0.01;

        isSeedBackground[15] = true;
        objProb[15] = 0.01; // Alta probabilidade de fundo
        bgProb[15] = 0.99;

        // Criar o grafo a partir da imagem
        Graph graph(image, width, height, sigma, objProb, bgProb);

        // Configurar os t-links
        for (int i = 0; i < width * height; ++i)
        {
            graph.tLinkSource[i] = -log(objProb[i]);
            graph.tLinkSink[i] = -log(bgProb[i]);

            if (isSeedObject[i])
            {
                graph.tLinkSource[i] = INFINITY; // Conecta ao source
                graph.tLinkSink[i] = 0;          // Não conecta ao sink
            }
            if (isSeedBackground[i])
            {
                graph.tLinkSource[i] = 0;      // Não conecta ao source
                graph.tLinkSink[i] = INFINITY; // Conecta ao sink
            }
        }

        // Calcular o fluxo máximo e obter o corte mínimo
        double maxFlowValue = graph.maxFlow();
        cout << "Fluxo máximo: " << maxFlowValue << endl;

        vector<bool> segmentation = graph.segment();
        cout << "Segmentação concluída. Tamanho: " << segmentation.size() << endl;

        vector<vector<Pixel>> coloredImage = ImageSegmentColorizer::colorizeSegmentation(image, segmentation, width, height);
        ImageSegmentColorizer::savePPM("images/saida.ppm", coloredImage, width, height);
    }
    catch (const exception &e)
    {
        cerr << "Erro: " << e.what() << endl;
        return -1;
    }

    return 0;
}
