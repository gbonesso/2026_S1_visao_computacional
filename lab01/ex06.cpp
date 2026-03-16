/*
Exercício 6 - Redução de níveis de cinza (quantização)
Objetivo: Observar visualmente o efeito da quantização sobre a qualidade da imagem.
Enunciado: A partir de uma imagem em tons de cinza de 256 níveis, gere versões com 128, 64, 16 e 4 níveis de cinza.
Entregáveis 
  • Quatro imagens quantizadas
  • Breve comparação visual
Conceitos: quantização, níveis de intensidade, bandas falsas
Dificuldade: Média
*/

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Mat quantizar(const cv::Mat& imagem, int niveis) {
  const int passo = 256 / niveis;
  cv::Mat resultado = imagem.clone();
  for (int y = 0; y < imagem.rows; ++y) {
    for (int x = 0; x < imagem.cols; ++x) {
      const uchar v = imagem.at<uchar>(y, x);
      // Usa divisão inteira para mapear o valor para o nível mais próximo 
      // e depois centraliza no meio do intervalo do nível (+ passo/2)
      // Exemplo: para 4 níveis, os intervalos são [0-63], [64-127], [128-191], [192-255]
      // O valor 70, por exemplo, cairia no intervalo [64-127], que é o segundo nível (1), e seria mapeado para 96 (64 + 32)
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
  cv::Mat imagem_cinza = cv::imread(caminho, cv::IMREAD_GRAYSCALE);

  if (imagem_cinza.empty()) {
    std::cerr << "Erro: nao foi possivel abrir a imagem: " << caminho << std::endl;
    return 1;
  }

  const int niveis[] = {128, 64, 16, 4};

  cv::imshow("Original (256 niveis)", imagem_cinza);
  for (const int n : niveis) {
    cv::Mat quantizada = quantizar(imagem_cinza, n);
    cv::imshow("Quantizada - " + std::to_string(n) + " niveis", quantizada);
  }

  cv::waitKey(0);
  return 0;
}