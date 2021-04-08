// POBR_LAB1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

unsigned cutValue(int original) {
    if (original > 255) return 255;
    else if (original < 0) return 0;
    else return original;
}

void contrast(cv::Mat& image) {
    CV_Assert(image.channels() == 3);
    int cols = image.cols;
    int rows = image.rows;
    double X = 1.5;
    cv::Mat_<cv::Vec3b> _image = image;
    for (int j = 1; j < cols; ++j) {
        for (int i = 0; i < std::min(j, rows - j); ++i) {
            unsigned newVal[3];
            newVal[0] = X * _image(i, j)[0];
            _image(i, j)[0] = cutValue(newVal[0]);
            newVal[1] = X * _image(i, j)[1];
            _image(i, j)[1] = cutValue(newVal[1]);
            newVal[2] = X * _image(i, j)[2];
            _image(i, j)[2] = cutValue(newVal[2]);
        }
    }
}

void grey(cv::Mat& image) {
    CV_Assert(image.channels() == 3);
    int cols = image.cols;
    int rows = image.rows;
    cv::Mat_<cv::Vec3b> _image = image;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j <= std::min(i, cols - i); ++j) {
            unsigned grey = (_image(i, j)[0] + _image(i, j)[1] + _image(i, j)[2]) / 3;
            int newVal = cutValue(_image(i, j)[2] - grey);
            _image(i, j)[0] = newVal;
            _image(i, j)[1] = newVal;
            _image(i, j)[2] = newVal;
        }
    }
}

void luminosity(cv::Mat& image) {
    CV_Assert(image.channels() == 3);
    int cols = image.cols;
    int rows = image.rows;
    int Y = 50;
    cv::Mat_<cv::Vec3b> _image = image;
    for (int j = 0; j < cols; ++j) {
        for (int i = rows - 1; i > std::max(j, rows - j); --i) {
            _image(i, j)[0] = cutValue(_image(i, j)[0] + Y);
            _image(i, j)[1] = cutValue(_image(i, j)[1] + Y);
            _image(i, j)[2] = cutValue(_image(i, j)[2] + Y);
        }
    }
}

std::vector<unsigned> histogram(const cv::Mat& image, const std::vector<unsigned>& divisions) {
    CV_Assert(image.channels() == 3);
    std::vector<unsigned> buckets(divisions.size() - 1, 0);
    cv::Mat_<cv::Vec3b> _image = image;
    for (auto i = _image.begin(); i != _image.end(); ++i) {
        unsigned luminosity = ((*i)[0] + (*i)[1] + (*i)[2]) / 3;
        for (unsigned j = 0; j < divisions.size(); ++j) {
            if (luminosity <= divisions.at(j+1)) {
                ++buckets.at(j);
                break;
            }
        }
    }
    return buckets;
}

std::vector<unsigned> createDivisions(const unsigned nOfBuckets) {
    std::vector<unsigned> divisions;
    divisions.push_back(0);
    for (unsigned i = 1; i < nOfBuckets; ++i) {
        divisions.push_back(255 * i / nOfBuckets);
    }
    divisions.push_back(255);
    return divisions;
}

void printHistogram(const std::vector<unsigned>& divisions, const std::vector<unsigned>& buckets) {
    CV_Assert(divisions.size() == buckets.size() + 1);
    for (unsigned i = 0; i < buckets.size(); ++i) {
        std::cout << "Pikseli o jasnosci " << divisions.at(i) << "-" << divisions.at(i + 1) << " jest " << buckets.at(i) << std::endl;
    }
    unsigned nOfPixels = 0;
    for (const auto& number : buckets) {
        nOfPixels += number;
    }
    std::cout << "Suma pikseli " << nOfPixels << std::endl;
}


int main()
{
    std::cout << "Starting\n";
    cv::Mat image = cv::imread("Lena.png");
    std::cout << image.cols << "x" << image.rows << std::endl;
    contrast(image);
    grey(image);
    luminosity(image);
    cv::imshow("result", image);
    cv::imwrite("Result.png", image);

    std::vector<unsigned> divisions = createDivisions(8);
    std::vector<unsigned> buckets = histogram(image, divisions);
    printHistogram(divisions, buckets);

    cv::waitKey(-1);
    return 0;
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
