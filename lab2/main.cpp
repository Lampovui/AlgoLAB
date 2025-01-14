#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

// Перечисление операций
enum Operations {
    VV_SUM,
    VV_SUB,
    VV_SCALAR
};

// Структура для хранения параметров задачи
struct CalcProblemParams {
    std::vector<double> v1;
    std::vector<double> v2;
    Operations operation;
};

// Функция для чтения вектора из файла
std::vector<double> ReadVectorFromFile(const std::string& filename) {
    std::vector<double> vec;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return vec;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        double num;
        while (ss >> num) {
            vec.push_back(num);
        }
    }
    file.close();
    return vec;
}

// Функция для инициализации данных
CalcProblemParams Init(const std::string& file1, const std::string& file2, Operations operation) {
    CalcProblemParams params;
    params.v1 = ReadVectorFromFile(file1);
    params.v2 = ReadVectorFromFile(file2);
    params.operation = operation;

    // Проверка: оба вектора должны быть одной длины
    if (params.v1.size() != params.v2.size()) {
        std::cerr << "Ошибка: Векторы имеют разную длину!" << std::endl;
    }

    return params;
}

// Функция для расчета
std::vector<double> Calc(const CalcProblemParams& params) {
    std::vector<double> result;

    if (params.v1.size() != params.v2.size()) {
        return result; // возвращаем пустой результат, если векторы разной длины
    }

    switch (params.operation) {
        case VV_SUM:
            for (size_t i = 0; i < params.v1.size(); ++i) {
                result.push_back(params.v1[i] + params.v2[i]);
            }
            break;
        case VV_SUB:
            for (size_t i = 0; i < params.v1.size(); ++i) {
                result.push_back(params.v1[i] - params.v2[i]);
            }
            break;
        case VV_SCALAR:
            double scalar_result = 0;
            for (size_t i = 0; i < params.v1.size(); ++i) {
                scalar_result += params.v1[i] * params.v2[i];
            }
            result.push_back(scalar_result);
            break;
    }

    return result;
}

// Функция для записи результата в файл
void Export(const std::vector<double>& result, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Не удалось открыть файл для записи: " << filename << std::endl;
        return;
    }

    for (double val : result) {
        outFile << val << " ";
    }

    outFile.close();
    std::cout << "Результат записан в файл: " << filename << std::endl;
}

int main() {
    // Имена файлов
    std::string file1 = "vector1.txt";
    std::string file2 = "vector2.txt";
    std::string output_file = "result.txt"; // Файл для записи результата

    // Инициализация с выбором операции (например, сложение векторов)
    CalcProblemParams params = Init(file1, file2, VV_SUM);

    // Расчет
    std::vector<double> result = Calc(params);

    // Запись результата в файл
    Export(result, output_file);

    return 0;
}