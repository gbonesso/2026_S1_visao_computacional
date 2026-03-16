/*
Exercício 1 - Leitura e exibição de imagem
Objetivo: Familiarizar o aluno com cv::Mat, imread, imshow e as dimensões da imagem.
Enunciado: Leia uma imagem colorida do disco, exiba-a em uma janela e mostre no terminal sua largura, altura
e número de canais.
Entregáveis:
  • Janela com a imagem original
  • Mensagem no terminal com resolução e canais 
Conceitos: resolução espacial, imagem colorida, estrutura matricial | Dificuldade: Muito fácil
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
    // https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html
	cv::Mat imagem = cv::imread(caminho, cv::IMREAD_COLOR); // Força leitura em modo colorido

	if (imagem.empty()) {
		std::cerr << "Erro: nao foi possivel abrir a imagem: " << caminho << std::endl;
		return 1;
	}

	std::cout << "Largura: " << imagem.cols
			  << ", Altura: " << imagem.rows
			  << ", Canais: " << imagem.channels() << std::endl;

	cv::imshow("Imagem Original", imagem);
	cv::waitKey(0);
	return 0;
}