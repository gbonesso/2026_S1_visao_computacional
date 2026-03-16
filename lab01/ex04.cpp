/*
Exercício 4 - Negativo da imagem
Objetivo:  Praticar transformação pontual simples sobre cada pixel.
Enunciado: Gere o negativo de uma imagem em tons de cinza usando a fórmula novo = 255 - antigo.
Entregáveis 
  • Imagem original em cinza
  • Imagem negativa
Dificuldade: Fácil
*/

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << " <caminho_da_imagem>" << std::endl;
    return 1;
  }

  const std::string caminho = argv[1];
  cv::Mat imagem_cinza = cv::imread(caminho, cv::IMREAD_GRAYSCALE);

  if (imagem_cinza.empty()) {
    std::cerr << "Erro: nao foi possivel abrir a imagem: " << caminho << std::endl;
    return 1;
  }

  cv::Mat imagem_negativa = imagem_cinza.clone();

  for (int y = 0; y < imagem_cinza.rows; ++y) {
    for (int x = 0; x < imagem_cinza.cols; ++x) {
      const uchar antigo = imagem_cinza.at<uchar>(y, x);
      imagem_negativa.at<uchar>(y, x) = static_cast<uchar>(255 - antigo);
    }
  }

  cv::imshow("Imagem Original em Cinza", imagem_cinza);
  cv::imshow("Imagem Negativa", imagem_negativa);
  cv::waitKey(0);

  return 0;
}