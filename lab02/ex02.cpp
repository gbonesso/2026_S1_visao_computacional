/*
Exercício

● Pesquise e implemente o filtro de suavização gaussiano
● Baixe 100 imagens de células do link abaixo. Depois tente fazer
limiarizações para separar as células do fundo
– http://www.cellimagelibrary.org/pages/project_20269
● Tente usar filtros de suavização para melhorar os resultados e deixar as
células com o mínimo de componentes possíveis.
– Faça o teste com:
● Os filtros média, mediana e moda diferentes
– Caso seja necessário um filtro muito grande para ver alterações, reduza a resolução da imagem para
256x256 pixels.

● 3 tamanhos de janela para cada filtro
– Comparar resultados dos filtros sobre todas imagens
– Qual filtro você escolheria para a base? Justifique.
*/

#include <iostream>
#include <array>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

bool tamanhoJanelaValido(int tamanho_janela) {
	return tamanho_janela >= 1 && (tamanho_janela % 2 != 0);
}

cv::Mat aplicarFiltroGaussiano(const cv::Mat& imagem, int tamanho_janela) {
	if (imagem.empty()) {
		return cv::Mat();
	}

	if (!tamanhoJanelaValido(tamanho_janela)) {
		return cv::Mat();
	}

	cv::Mat imagem_filtrada;
	cv::GaussianBlur(imagem, imagem_filtrada, cv::Size(tamanho_janela, tamanho_janela), 0.0);
	return imagem_filtrada;
}

cv::Mat aplicarFiltroMedia(const cv::Mat& imagem, int tamanho_janela) {
	if (imagem.empty()) {
		return cv::Mat();
	}

	if (!tamanhoJanelaValido(tamanho_janela)) {
		return cv::Mat();
	}

	cv::Mat imagem_filtrada;
	cv::blur(imagem, imagem_filtrada, cv::Size(tamanho_janela, tamanho_janela));
	return imagem_filtrada;
}

cv::Mat aplicarFiltroMediana(const cv::Mat& imagem, int tamanho_janela) {
	if (imagem.empty()) {
		return cv::Mat();
	}

	if (!tamanhoJanelaValido(tamanho_janela)) {
		return cv::Mat();
	}

	cv::Mat imagem_filtrada;
	cv::medianBlur(imagem, imagem_filtrada, tamanho_janela);
	return imagem_filtrada;
}

cv::Mat aplicarFiltroModa(const cv::Mat& imagem, int tamanho_janela) {
	if (imagem.empty()) {
		return cv::Mat();
	}

	if (!tamanhoJanelaValido(tamanho_janela)) {
		return cv::Mat();
	}

	if (imagem.depth() != CV_8U) {
		return cv::Mat();
	}

	const int raio = tamanho_janela / 2;
	cv::Mat imagem_borda;
	cv::copyMakeBorder(
		imagem,
		imagem_borda,
		raio,
		raio,
		raio,
		raio,
		cv::BORDER_REFLECT101
	);

	cv::Mat imagem_filtrada(imagem.size(), imagem.type());

	if (imagem.channels() == 1) {
		for (int y = 0; y < imagem.rows; ++y) {
			for (int x = 0; x < imagem.cols; ++x) {
				std::array<int, 256> frequencias = {};
				int melhor_valor = 0;
				int melhor_frequencia = -1;

				for (int wy = 0; wy < tamanho_janela; ++wy) {
					for (int wx = 0; wx < tamanho_janela; ++wx) {
						const int valor = imagem_borda.at<uchar>(y + wy, x + wx);
						const int frequencia_atual = ++frequencias[valor];
						if (frequencia_atual > melhor_frequencia ||
							(frequencia_atual == melhor_frequencia && valor < melhor_valor)) {
							melhor_frequencia = frequencia_atual;
							melhor_valor = valor;
						}
					}
				}

				imagem_filtrada.at<uchar>(y, x) = static_cast<uchar>(melhor_valor);
			}
		}
	} else if (imagem.channels() == 3) {
		for (int y = 0; y < imagem.rows; ++y) {
			for (int x = 0; x < imagem.cols; ++x) {
				cv::Vec3b pixel_filtrado;

				for (int c = 0; c < 3; ++c) {
					std::array<int, 256> frequencias = {};
					int melhor_valor = 0;
					int melhor_frequencia = -1;

					for (int wy = 0; wy < tamanho_janela; ++wy) {
						for (int wx = 0; wx < tamanho_janela; ++wx) {
							const int valor = imagem_borda.at<cv::Vec3b>(y + wy, x + wx)[c];
							const int frequencia_atual = ++frequencias[valor];
							if (frequencia_atual > melhor_frequencia ||
								(frequencia_atual == melhor_frequencia && valor < melhor_valor)) {
								melhor_frequencia = frequencia_atual;
								melhor_valor = valor;
							}
						}
					}

					pixel_filtrado[c] = static_cast<uchar>(melhor_valor);
				}

				imagem_filtrada.at<cv::Vec3b>(y, x) = pixel_filtrado;
			}
		}
	} else {
		return cv::Mat();
	}

	return imagem_filtrada;
}

cv::Mat aplicarLimiarizacaoOtsu(const cv::Mat& imagem) {
	if (imagem.empty()) {
		return cv::Mat();
	}

	cv::Mat imagem_cinza;
	if (imagem.channels() == 3) {
		cv::cvtColor(imagem, imagem_cinza, cv::COLOR_BGR2GRAY);
	} else if (imagem.channels() == 1) {
		imagem_cinza = imagem;
	} else {
		return cv::Mat();
	}

	cv::Mat imagem_binaria;
	cv::threshold(imagem_cinza, imagem_binaria, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// Converte para 3 canais para manter compatibilidade na composicao do mosaico.
	cv::Mat imagem_binaria_bgr;
	cv::cvtColor(imagem_binaria, imagem_binaria_bgr, cv::COLOR_GRAY2BGR);
	return imagem_binaria_bgr;
}

cv::Mat adicionarRotulo(const cv::Mat& imagem, const std::string& rotulo) {
	if (imagem.empty()) {
		return cv::Mat();
	}

	cv::Mat imagem_rotulada = imagem.clone();
	const cv::Scalar cor_fundo(0, 0, 0);
	const cv::Scalar cor_texto(255, 255, 255);
	const int x_inicial = 8;
	const int y_inicial = 22;
	const cv::Point origem_texto(x_inicial, y_inicial);

	int baseline = 0;
	const double escala_fonte = 0.55;
	const int espessura_fonte = 1;
	const cv::Size tamanho_texto = cv::getTextSize(
		rotulo,
		cv::FONT_HERSHEY_SIMPLEX,
		escala_fonte,
		espessura_fonte,
		&baseline
	);

	const cv::Point ret_topo_esq(0, 0);
	const cv::Point ret_base_dir(
		std::min(imagem_rotulada.cols - 1, x_inicial + tamanho_texto.width + 10),
		std::min(imagem_rotulada.rows - 1, y_inicial + baseline + 6)
	);

	cv::rectangle(imagem_rotulada, ret_topo_esq, ret_base_dir, cor_fundo, cv::FILLED);
	cv::putText(
		imagem_rotulada,
		rotulo,
		origem_texto,
		cv::FONT_HERSHEY_SIMPLEX,
		escala_fonte,
		cor_texto,
		espessura_fonte,
		cv::LINE_AA
	);

	return imagem_rotulada;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Uso: " << argv[0] << " <caminho_da_imagem> [arquivo_saida]" << std::endl;
		return 1;
	}

	const std::string caminho_imagem = argv[1];
	const std::string caminho_saida = (argc >= 3) ? argv[2] : "ex02_resultado_analise.png";
	cv::Mat imagem = cv::imread(caminho_imagem, cv::IMREAD_COLOR);

	if (imagem.empty()) {
		std::cerr << "Erro: nao foi possivel abrir a imagem: " << caminho_imagem << std::endl;
		return 1;
	}

	// Reduz a resolucao no inicio para padronizar os testes dos filtros.
	cv::resize(imagem, imagem, cv::Size(256, 256));

	const std::vector<int> tamanhos_janela = {3, 5, 9};
	std::vector<cv::Mat> linha_gaussiano;
	std::vector<cv::Mat> linha_gaussiano_limiar;
	std::vector<cv::Mat> linha_media;
	std::vector<cv::Mat> linha_media_limiar;
	std::vector<cv::Mat> linha_mediana;
	std::vector<cv::Mat> linha_mediana_limiar;
	std::vector<cv::Mat> linha_moda;
	std::vector<cv::Mat> linha_moda_limiar;

	const cv::Mat limiar_original = aplicarLimiarizacaoOtsu(imagem);
	if (limiar_original.empty()) {
		std::cerr << "Erro: nao foi possivel limiarizar a imagem original." << std::endl;
		return 1;
	}

	linha_gaussiano.push_back(adicionarRotulo(imagem, "Original"));
	linha_gaussiano_limiar.push_back(adicionarRotulo(limiar_original, "Limiar original"));

	linha_media.push_back(adicionarRotulo(imagem, "Original"));
	linha_media_limiar.push_back(adicionarRotulo(limiar_original, "Limiar original"));
	linha_mediana.push_back(adicionarRotulo(imagem, "Original"));
	linha_mediana_limiar.push_back(adicionarRotulo(limiar_original, "Limiar original"));
	linha_moda.push_back(adicionarRotulo(imagem, "Original"));
	linha_moda_limiar.push_back(adicionarRotulo(limiar_original, "Limiar original"));

	for (const int tamanho_janela : tamanhos_janela) {
		cv::Mat imagem_gaussiana = aplicarFiltroGaussiano(imagem, tamanho_janela);
		if (imagem_gaussiana.empty()) {
			std::cerr << "Erro: nao foi possivel aplicar filtro gaussiano com janela "
					  << tamanho_janela << "x" << tamanho_janela << "." << std::endl;
			return 1;
		}

		cv::Mat limiar_gaussiana = aplicarLimiarizacaoOtsu(imagem_gaussiana);
		if (limiar_gaussiana.empty()) {
			std::cerr << "Erro: nao foi possivel limiarizar o filtro gaussiano com janela "
					  << tamanho_janela << "x" << tamanho_janela << "." << std::endl;
			return 1;
		}

		cv::Mat imagem_media = aplicarFiltroMedia(imagem, tamanho_janela);
		if (imagem_media.empty()) {
			std::cerr << "Erro: nao foi possivel aplicar filtro de media com janela "
					  << tamanho_janela << "x" << tamanho_janela << "." << std::endl;
			return 1;
		}

		cv::Mat limiar_media = aplicarLimiarizacaoOtsu(imagem_media);
		if (limiar_media.empty()) {
			std::cerr << "Erro: nao foi possivel limiarizar o filtro de media com janela "
					  << tamanho_janela << "x" << tamanho_janela << "." << std::endl;
			return 1;
		}

		cv::Mat imagem_mediana = aplicarFiltroMediana(imagem, tamanho_janela);
		if (imagem_mediana.empty()) {
			std::cerr << "Erro: nao foi possivel aplicar filtro de mediana com janela "
					  << tamanho_janela << "x" << tamanho_janela << "." << std::endl;
			return 1;
		}

		cv::Mat limiar_mediana = aplicarLimiarizacaoOtsu(imagem_mediana);
		if (limiar_mediana.empty()) {
			std::cerr << "Erro: nao foi possivel limiarizar o filtro de mediana com janela "
					  << tamanho_janela << "x" << tamanho_janela << "." << std::endl;
			return 1;
		}

		cv::Mat imagem_moda = aplicarFiltroModa(imagem, tamanho_janela);
		if (imagem_moda.empty()) {
			std::cerr << "Erro: nao foi possivel aplicar filtro de moda com janela "
					  << tamanho_janela << "x" << tamanho_janela << "." << std::endl;
			return 1;
		}

		cv::Mat limiar_moda = aplicarLimiarizacaoOtsu(imagem_moda);
		if (limiar_moda.empty()) {
			std::cerr << "Erro: nao foi possivel limiarizar o filtro de moda com janela "
					  << tamanho_janela << "x" << tamanho_janela << "." << std::endl;
			return 1;
		}

		linha_gaussiano.push_back(adicionarRotulo(imagem_gaussiana, "Gauss " + std::to_string(tamanho_janela) + "x" + std::to_string(tamanho_janela)));
		linha_gaussiano_limiar.push_back(adicionarRotulo(limiar_gaussiana, "Limiar gauss " + std::to_string(tamanho_janela) + "x" + std::to_string(tamanho_janela)));

		linha_media.push_back(adicionarRotulo(imagem_media, "Media " + std::to_string(tamanho_janela) + "x" + std::to_string(tamanho_janela)));
		linha_media_limiar.push_back(adicionarRotulo(limiar_media, "Limiar media " + std::to_string(tamanho_janela) + "x" + std::to_string(tamanho_janela)));
		linha_mediana.push_back(adicionarRotulo(imagem_mediana, "Mediana " + std::to_string(tamanho_janela) + "x" + std::to_string(tamanho_janela)));
		linha_mediana_limiar.push_back(adicionarRotulo(limiar_mediana, "Limiar mediana " + std::to_string(tamanho_janela) + "x" + std::to_string(tamanho_janela)));
		linha_moda.push_back(adicionarRotulo(imagem_moda, "Moda " + std::to_string(tamanho_janela) + "x" + std::to_string(tamanho_janela)));
		linha_moda_limiar.push_back(adicionarRotulo(limiar_moda, "Limiar moda " + std::to_string(tamanho_janela) + "x" + std::to_string(tamanho_janela)));
	}

	cv::Mat mosaico_gaussiano;
	cv::Mat mosaico_gaussiano_limiar;

	cv::Mat mosaico_media;
	cv::Mat mosaico_media_limiar;
	cv::Mat mosaico_mediana;
	cv::Mat mosaico_mediana_limiar;
	cv::Mat mosaico_moda;
	cv::Mat mosaico_moda_limiar;
	cv::hconcat(linha_gaussiano, mosaico_gaussiano);
	cv::hconcat(linha_gaussiano_limiar, mosaico_gaussiano_limiar);
	cv::hconcat(linha_media, mosaico_media);
	cv::hconcat(linha_media_limiar, mosaico_media_limiar);
	cv::hconcat(linha_mediana, mosaico_mediana);
	cv::hconcat(linha_mediana_limiar, mosaico_mediana_limiar);
	cv::hconcat(linha_moda, mosaico_moda);
	cv::hconcat(linha_moda_limiar, mosaico_moda_limiar);

	std::vector<cv::Mat> linhas_final = {
		mosaico_gaussiano,
		mosaico_gaussiano_limiar,
		mosaico_media,
		mosaico_media_limiar,
		mosaico_mediana,
		mosaico_mediana_limiar,
		mosaico_moda,
		mosaico_moda_limiar
	};
	cv::Mat comparacao;
	cv::vconcat(linhas_final, comparacao);

	if (!cv::imwrite(caminho_saida, comparacao)) {
		std::cerr << "Erro: nao foi possivel salvar o arquivo de saida: " << caminho_saida << std::endl;
		return 1;
	}

	std::cout << "Arquivo de saida salvo em: " << caminho_saida << std::endl;

	cv::imshow("Linhas: Filtros e Limiarizacoes | Colunas: Original, 3x3, 5x5, 9x9", comparacao);
	cv::waitKey(0);

	return 0;
}

