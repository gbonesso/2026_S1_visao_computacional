/*
Laboratório 02 — Detecção de Pratos com Transformada de Hough – Prof. Dr. Fábio Cappabianco
UC de Visão Computacional | Implementação em C++ e OpenCV

Trabalho individual ou em dupla. A atividade consiste em detectar pratos em 100 imagens de
alimentos usando a Transformada de Hough e analisar visualmente os resultados.

1. Objetivo
Implementar a Transformada de Hough para detecção de círculos em imagens de pratos de alimentos.
O programa deve localizar automaticamente os pratos presentes nas imagens.

2. Base de dados
 A base de dados contém 100 imagens e será fornecida.
 Todas as imagens devem ser utilizadas no experimento.
 A avaliação será visual, sem necessidade de separar treino e teste.
*/

/* 
3. Implementação
A implementação deve ser feita em C++ com OpenCV. O programa deve ler as imagens, aplicar o
método de detecção e gerar imagens de saída com os círculos detectados.
 Implementar a detecção de círculos com a Transformada de Hough.
 Testar diferentes conjuntos de parâmetros.
 Salvar ou exibir as imagens resultantes.
*/

/*
4. Pré-processamento
Pode-se e deve-se usar pré-processamentos, caso necessário.
 Exemplos: filtro gaussiano, filtro da mediana, suavização, equalização, detecção de bordas.
 No relatório, indicar quais pré-processamentos foram eficazes e quais não foram.
*/

/*
5. Avaliação dos resultados
Cada imagem deve ser classificada visualmente em uma das categorias abaixo:
 Detectado corretamente: o prato principal foi localizado de forma adequada.
 Detectado parcialmente: houve detecção, mas com posição, raio ou quantidade de círculos apenas
parcialmente satisfatórios.
 Não detectado ou detectado incorretamente: o prato não foi localizado, ou a detecção foi claramente
errada.
Ao final, deve ser contado quantas imagens ficaram em cada categoria.
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

int main() {
    // Caminho para a pasta de imagens
    std::filesystem::path folderPath = "sel_data";
    
    // Vetor para armazenar os resultados
    std::vector<std::string> results;

    // Crie duas variáveis para contar as imagens com deteccao e sem deteccao
    int detectedCount = 0;
    int notDetectedCount = 0;

    std::vector<std::filesystem::path> imageFiles;
    for (const auto &entry : std::filesystem::directory_iterator(folderPath)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".jpg") {
            continue;
        }

        const std::string stem = entry.path().stem().string();
        if (!std::all_of(stem.begin(), stem.end(), [](unsigned char ch) {
                return std::isdigit(ch);
            })) {
            continue;
        }

        imageFiles.push_back(entry.path());
    }

    std::sort(imageFiles.begin(), imageFiles.end(), [](const auto &a, const auto &b) {
        return std::stoi(a.stem().string()) < std::stoi(b.stem().string());
    });

    // Loop para processar cada imagem
    for (const auto &filePath : imageFiles) {
        std::string fileName = filePath.string();
        
        // Ler a imagem
        cv::Mat image = cv::imread(fileName);
        if (image.empty()) {
            std::cerr << "Erro ao ler a imagem: " << fileName << std::endl;
            continue;
        }

        // Pré-processamento melhorado para pratos transparentes
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        
        // Aplicar filtro bilateral para suavizar mantendo bordas
        cv::Mat bilateral;
        cv::bilateralFilter(
            gray, 
            bilateral, // Imagem de saída
            9, // d (diâmetro do pixel vizinho)
            75, // sigmaColor
            75  // sigmaSpace
        );
        
        // Aplicar CLAHE (Contrast Limited Adaptive Histogram Equalization)
        // Muito eficaz para realçar bordas fracas em pratos transparentes
        cv::Mat clahe_result;
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(
            2.4, // clipLimit. Iniciei com 2.0
            cv::Size(8, 8) // tileGridSize
        );
        clahe->apply(bilateral, clahe_result);
        
        // Aplicar morphological closing para conectar bordas fragmentadas
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        cv::Mat closed;
        cv::morphologyEx(clahe_result, closed, cv::MORPH_CLOSE, kernel);
        
        // Filtro Gaussiano suave apenas para suavização final
        cv::Mat blurred;
        cv::GaussianBlur(closed, blurred, cv::Size(5, 5), 1.0);

        // Detecção de círculos usando a Transformada de Hough
        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(
            blurred, 
            circles, 
            cv::HOUGH_GRADIENT, // Método de detecção de círculos
            1.2, // dp (resolução inversa do acumulador)
            blurred.rows / 3, // minDist (distância mínima entre os centros dos círculos detectados)
            120, // param1 (limiar superior para o detector de bordas Canny)
            45, // param2 (limiar para o centro do círculo na fase de acumulação)
            blurred.rows / 6, // minRadius (raio mínimo do círculo a ser detectado)
            blurred.rows / 2 // maxRadius (raio máximo do círculo a ser detectado)
        );

        cv::Vec3f bestCircle;
        bool hasBestCircle = false;
        for (const auto &circle : circles) {
            if (!hasBestCircle || circle[2] > bestCircle[2]) {
                bestCircle = circle;
                hasBestCircle = true;
            }
        }

        // Contar as imagens com detecção e sem detecção
        if (hasBestCircle) {
            detectedCount++;
        } else {
            notDetectedCount++;
        }

        // Desenhar apenas o círculo mais provável na imagem original
        if (hasBestCircle) {
            cv::Vec3i c = bestCircle;
            cv::circle(image, cv::Point(c[0], c[1]), c[2], cv::Scalar(0, 255, 0), 2);
            cv::circle(image, cv::Point(c[0], c[1]), 2, cv::Scalar(0, 0, 255), 3);
        }

        // Exibir o path da imagem e o número de círculos detectados
        std::cout << "Imagem: " << fileName << " - Círculos detectados: " << circles.size() << std::endl;

        // Exibir ou salvar a imagem resultante
        std::string outputFileName = (folderPath / ("output_" + filePath.filename().string())).string();
        cv::imwrite(outputFileName, image);
        
        // Classificar o resultado visualmente (a ser feito manualmente)
        results.push_back("Classificação da imagem " + filePath.filename().string());
    }

    // Exibir os resultados
    std::cout << "Imagens com detecção: " << detectedCount << std::endl;
    std::cout << "Imagens sem detecção: " << notDetectedCount << std::endl;
    //for (const auto& result : results) {
    //    std::cout << result << std::endl;
    //}
}