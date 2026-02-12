#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    Mat img;
    // Carga la imagen desde la carpeta img que acabas de ubicar
    img = imread("../img/logoOpenCV.jpg");

    if (img.empty()) {
        cout << "No se pudo cargar la imagen. Revisa el nombre y la carpeta." << endl;
        return -1;
    }

    imshow("Imagen Original", img);
    waitKey(0);
    return 0;
}