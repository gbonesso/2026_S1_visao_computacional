/*
Exercício 2 - Conversão para escala de cinza
Objetivo: Relacionar imagem colorida com intensidade luminosa em uma imagem monocromática.
Enunciado: Leia uma imagem colorida e gere uma versão em tons de cinza. Exiba a imagem original e a imagem convertida.
Entregáveis 
  • Imagem original
  • Imagem em tons de cinza
Conceitos: intensidade, tons de cinza, espaço de cor
Dificuldade: Muito fácil
*/

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Uso: " << argv[0] << " <caminho_da_imagem>" << std::endl;
    return 1;
  }

  const std::string caminho = argv[1];
  cv::Mat imagem_colorida = cv::imread(caminho, cv::IMREAD_COLOR);

  if (imagem_colorida.empty()) {
    std::cerr << "Erro: nao foi possivel abrir a imagem: " << caminho << std::endl;
    return 1;
  }

  cv::Mat imagem_cinza;
  cv::cvtColor(imagem_colorida, imagem_cinza, cv::COLOR_BGR2GRAY);

  std::cout << "Largura: " << imagem_cinza.cols
			<< ", Altura: " << imagem_cinza.rows
			<< ", Canais: " << imagem_cinza.channels() << std::endl;

  cv::imshow("Imagem Original", imagem_colorida);
  cv::imshow("Imagem em Tons de Cinza", imagem_cinza);
  cv::waitKey(0);

  return 0;
}