/*
Exercício 5 - Binarização com limiar fixo
Objetivo: Introduzir a ideia de limiarização e segmentação simples.
Enunciado: Converta uma imagem em tons de cinza para preto e branco usando um limiar fixo, por exemplo 128.
Entregáveis: 
  • Imagem binária produzida manualmente ou com threshold
Conceitos: limiar, imagem binária, contraste
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

  const int limiar = 128;
  cv::Mat imagem_binaria = imagem_cinza.clone();

  for (int y = 0; y < imagem_cinza.rows; ++y) {
    for (int x = 0; x < imagem_cinza.cols; ++x) {
      const uchar valor = imagem_cinza.at<uchar>(y, x);
      imagem_binaria.at<uchar>(y, x) = (valor >= limiar) ? 255 : 0;
    }
  }

  cv::imshow("Imagem em Cinza", imagem_cinza);
  cv::imshow("Imagem Binaria (Limiar 128)", imagem_binaria);
  cv::waitKey(0);

  return 0;
}