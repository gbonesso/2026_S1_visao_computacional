/*
Implemente a operação de convolução que
procure e imprima a posição mais semelhante
que uma determinada janela ocorre dentro de
uma imagem grande.
– Crie e mostre uma imagem de região de 50x50
centralizada na posição encontrada. (Se ficar na
borda, preencher com pixels pretos)
*/

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>  

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Uso: " << argv[0] << " <caminho_da_imagem> <caminho_da_janela>" << std::endl;
    return 1;
  }

  const std::string caminho_imagem = argv[1];
  const std::string caminho_janela = argv[2];

  cv::Mat imagem_grande = cv::imread(caminho_imagem, cv::IMREAD_COLOR);
  cv::Mat janela = cv::imread(caminho_janela, cv::IMREAD_COLOR);

  if (imagem_grande.empty()) {
    std::cerr << "Erro: nao foi possivel abrir a imagem: " << caminho_imagem << std::endl;
    return 1;
  }
  
  if (janela.empty()) {
    std::cerr << "Erro: nao foi possivel abrir a janela: " << caminho_janela << std::endl;
    return 1;
  }

  // Verificar se a janela é maior que a imagem
  if (janela.cols > imagem_grande.cols || janela.rows > imagem_grande.rows) {
    std::cerr << "Erro: A janela deve ser menor ou igual à imagem." << std::endl;
    return 1;
  }

  // Realizar a convolução para encontrar a posição mais semelhante
  cv::Mat resultado;
  cv::matchTemplate(imagem_grande, janela, resultado, cv::TM_CCOEFF_NORMED);

  double minVal, maxVal;
  cv::Point minLoc, maxLoc;
  cv::minMaxLoc(resultado, &minVal, &maxVal, &minLoc, &maxLoc);

  // A posição mais semelhante é dada por maxLoc
  std::cout << "Posição mais semelhante encontrada em: (" << maxLoc.x << ", " << maxLoc.y << ")" << std::endl;

  // Criar uma imagem de região de 50x50 centralizada na posição encontrada
  int x_center = maxLoc.x + janela.cols / 2;
  int y_center = maxLoc.y + janela.rows / 2;

  int x_start = std::max(0, x_center - 25);
  int y_start = std::max(0, y_center - 25);
  int x_end = std::min(imagem_grande.cols, x_center + 25);
  int y_end = std::min(imagem_grande.rows, y_center + 25);  

  cv::Mat regiao = cv::Mat::zeros(50, 50, imagem_grande.type());
  imagem_grande(cv::Rect(x_start, y_start, x_end - x_start, y_end - y_start)).copyTo(regiao(cv::Rect(25 - (x_center - x_start), 25 - (y_center - y_start), x_end - x_start, y_end - y_start)));

  // Mostrar a imagem da região encontrada
  cv::imshow("Regiao Encontrada", regiao);
  cv::waitKey(0);
  return 0;
}