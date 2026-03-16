/*
Exercício 3 - Leitura do valor de um pixel
Objetivo: Entender que a imagem é uma grade de amostras com valores numéricos.
Enunciado: Depois de carregar uma imagem em tons de cinza, peça ao usuário as coordenadas (x, y) e mostre o valor da intensidade naquele pixel.
Entregáveis 
  • Entrada do usuário para x e y
  • Saída no terminal com a intensidade
Conceitos: pixel, coordenadas, faixa de intensidades 0-255
Dificuldade: Fácil
*/

#include <iostream>
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

  int x = 0;
  int y = 0;

  std::cout << "Digite x (coluna): ";
  std::cin >> x;
  std::cout << "Digite y (linha): ";
  std::cin >> y;

  if (!std::cin) {
    std::cerr << "Erro: entrada invalida para coordenadas." << std::endl;
    return 1;
  }

  if (x < 0 || x >= imagem_cinza.cols || y < 0 || y >= imagem_cinza.rows) {
    std::cerr << "Erro: coordenadas fora dos limites da imagem." << std::endl;
    std::cerr << "Faixa valida: x em [0, " << (imagem_cinza.cols - 1)
              << "], y em [0, " << (imagem_cinza.rows - 1) << "]" << std::endl;
    return 1;
  }

  const int intensidade = static_cast<int>(imagem_cinza.at<uchar>(y, x));
  std::cout << "Intensidade no pixel (" << x << ", " << y << "): "
            << intensidade << std::endl;

  return 0;
}