/*
Exercício 7 - Redução de resolução espacial
Objetivo: Relacionar amostragem com perda de detalhes espaciais.
Enunciado: Reduza a imagem para metade da largura e da altura. Depois, amplie a imagem reduzida de volta 
ao tamanho original para comparar com a imagem inicial.
Entregáveis:
 • Imagem original
 • Imagem reduzida
 • Imagem reampliada
Conceitos: amostragem, resolução espacial, interpolação
Dificuldade: Fácil
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
  cv::Mat imagem = cv::imread(caminho, cv::IMREAD_GRAYSCALE);

  if (imagem.empty()) {
    std::cerr << "Erro: nao foi possivel abrir a imagem: " << caminho << std::endl;
    return 1;
  }

  const cv::Size tamanho_original = imagem.size();
  const cv::Size tamanho_reduzido(imagem.cols / 2, imagem.rows / 2);

  cv::Mat imagem_reduzida;
  cv::resize(imagem, imagem_reduzida, tamanho_reduzido, 0, 0, cv::INTER_NEAREST);

  cv::Mat imagem_reampliada;
  cv::resize(imagem_reduzida, imagem_reampliada, tamanho_original, 0, 0, cv::INTER_NEAREST);

  std::cout << "Original:   " << imagem.cols        << "x" << imagem.rows        << std::endl;
  std::cout << "Reduzida:   " << imagem_reduzida.cols << "x" << imagem_reduzida.rows << std::endl;
  std::cout << "Reampliada: " << imagem_reampliada.cols << "x" << imagem_reampliada.rows << std::endl;

  cv::imshow("Original", imagem);
  cv::imshow("Reduzida (metade)", imagem_reduzida);
  cv::imshow("Reampliada (original)", imagem_reampliada);
  cv::waitKey(0);

  return 0;
}