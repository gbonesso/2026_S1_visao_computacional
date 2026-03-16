/*
Exercício 8 - Comparação entre amostragem e quantização
Objetivo: Consolidar a diferença entre perda de resolução espacial e perda de resolução de intensidade.
Enunciado: Produza três imagens a partir da original: uma com resolução reduzida, uma com poucos níveis de cinza
e uma com os dois efeitos combinados. Em seguida, escreva uma breve comparação visual.
Entregáveis 
  • Três imagens processadas
  • Texto curto explicando as diferenças percebidas
Conceitos: amostragem x quantização, detalhe espacial, suavidade tonal
Dificuldade: Média
*/

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

// Reduz para metade e reamplifica (perda de resolução espacial)
cv::Mat reduzir_resolucao(const cv::Mat& imagem) {
  const cv::Size metade(imagem.cols / 2, imagem.rows / 2);
  cv::Mat reduzida, reampliada;
  cv::resize(imagem, reduzida, metade, 0, 0, cv::INTER_NEAREST);
  cv::resize(reduzida, reampliada, imagem.size(), 0, 0, cv::INTER_NEAREST);
  return reampliada;
}

// Reduz para N níveis de cinza (perda de resolução de intensidade)
cv::Mat quantizar(const cv::Mat& imagem, int niveis) {
  const int passo = 256 / niveis;
  cv::Mat resultado = imagem.clone();
  for (int y = 0; y < imagem.rows; ++y) {
    for (int x = 0; x < imagem.cols; ++x) {
      const uchar v = imagem.at<uchar>(y, x);
      resultado.at<uchar>(y, x) = static_cast<uchar>((v / passo) * passo + passo / 2);
    }
  }
  return resultado;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << " <caminho_da_imagem>" << std::endl;
    return 1;
  }

  const std::string caminho = argv[1];
  cv::Mat imagem = cv::imread(caminho, cv::IMREAD_GRAYSCALE);

  if (imagem.empty()) {
    std::cerr << "Erro: nao foi possivel abrir a imagem: " << caminho << std::endl;
    return 1;
  }

  cv::Mat img_espacial   = reduzir_resolucao(imagem);          // só amostragem
  cv::Mat img_quantizada = quantizar(imagem, 4);               // só quantização (4 níveis)
  cv::Mat img_combinada  = quantizar(reduzir_resolucao(imagem), 4); // ambos

  cv::imshow("Original", imagem);
  cv::imshow("Resolucao reduzida (amostragem)", img_espacial);
  cv::imshow("Poucos niveis (quantizacao)", img_quantizada);
  cv::imshow("Combinado (amostragem + quantizacao)", img_combinada);

  std::cout << "\n--- Comparacao visual ---\n"
            << "Resolucao reduzida (amostragem) : bordas e detalhes finos ficam borrados/pixelados, mas os tons de cinza permanecem suaves.\n"
            << "Poucos niveis (quantizacao) : detalhes espaciais sao preservados, mas as regioes de transição entre as cores ficam mais grosseiras\n"
            << "Combinado (amostragem + quantizacao) : acontece a combinacao dos dois efeitos, bordas e detalhes finos ficam borrados/pixelados e.\n"
            << "     as regioes de transicao entre as cores ficam mais grosseiras, resultando em uma imagem de qualidade visual pior.\n";

  cv::waitKey(0);
  return 0;
}