// g++ main.cpp -o teste_img $(pkg-config --cflags --libs opencv4)
#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    cv::Mat img = cv::imread("../imagens/imagem.jpg");
    if (img.empty()) {
        std::cerr << "Erro ao abrir a imagem." << std::endl;
        return 1;
    }
    std::cout << "Largura: " << img.cols << std::endl;
    std::cout << "Altura: " << img.rows << std::endl;
    return 0;
}