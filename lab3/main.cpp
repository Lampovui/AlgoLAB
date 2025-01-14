#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>

// Класс для чтения матрицы из файла
template <typename T>
class MatrixReader {
public:
    static std::vector<std::vector<T>> readMatrixFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::vector<std::vector<T>> matrix;
        
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return matrix;
        }
        
        unsigned rows, cols;
        file >> rows >> cols;  // Чтение размеров матрицы

        matrix.resize(rows, std::vector<T>(cols));

        for (unsigned i = 0; i < rows; ++i) {
            for (unsigned j = 0; j < cols; ++j) {
                file >> matrix[i][j];
            }
        }

        file.close();
        return matrix;
    }
};

// Класс для записи матрицы в файл
template <typename T>
class MatrixWriter {
public:
    static void writeMatrixToFile(const std::string& filename, const std::vector<std::vector<T>>& matrix) {
        std::ofstream file(filename);
        
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        
        unsigned rows = matrix.size();
        unsigned cols = matrix[0].size();

        file << rows << " " << cols << std::endl;  // Запись размеров матрицы

        for (unsigned i = 0; i < rows; ++i) {
            for (unsigned j = 0; j < cols; ++j) {
                file << matrix[i][j] << " ";
            }
            file << std::endl;
        }

        file.close();
    }
};

// Класс для плотных матриц
template <typename T>
class MatrixDense {
private:
    std::vector<std::vector<T>> _data;
    unsigned _rows, _cols;

public:
    // Конструктор
    MatrixDense(unsigned rows, unsigned cols) : _rows(rows), _cols(cols) {
        _data.resize(rows, std::vector<T>(cols, 0));
    }

    // Конструктор от готовых данных
    MatrixDense(const std::vector<std::vector<T>>& data) : _data(data) {
        _rows = data.size();
        _cols = data[0].size();
    }

    // Перегрузка оператора +
    MatrixDense<T> operator+(const MatrixDense<T>& other) {
        if (_rows != other._rows || _cols != other._cols) {
            throw std::invalid_argument("Matrix dimensions must match for addition.");
        }

        MatrixDense<T> result(_rows, _cols);
        for (unsigned i = 0; i < _rows; ++i) {
            for (unsigned j = 0; j < _cols; ++j) {
                result._data[i][j] = _data[i][j] + other._data[i][j];
            }
        }

        return result;
    }

    // Метод для проверки, является ли матрица разреженной
    bool isSparse() const {
        unsigned nonZeroCount = 0;
        for (unsigned i = 0; i < _rows; ++i) {
            for (unsigned j = 0; j < _cols; ++j) {
                if (_data[i][j] != 0) {
                    nonZeroCount++;
                }
            }
        }
        return (nonZeroCount <= (_rows * _cols) / 2); // Если больше половины элементов — нули
    }

    void printMatrix() const {
        for (unsigned i = 0; i < _rows; ++i) {
            for (unsigned j = 0; j < _cols; ++j) {
                std::cout << _data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    const std::vector<std::vector<T>>& getData() const {
        return _data;
    }

    // Деструктор для освобождения памяти
    ~MatrixDense() {
        std::cout << "Memory released for MatrixDense\n";
    }
};

// Класс для диагональных матриц
template <typename T>
class MatrixDiagonal : public MatrixDense<T> {
public:
    MatrixDiagonal(const std::vector<std::vector<T>>& data) : MatrixDense<T>(data) {}

    // Метод для проверки, является ли матрица диагональной
    bool isDiagonal() const {
        const auto& matrix = this->getData();
        unsigned rows = matrix.size();
        unsigned cols = matrix[0].size();

        for (unsigned i = 0; i < rows; ++i) {
            for (unsigned j = 0; j < cols; ++j) {
                if (i != j && matrix[i][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }
};

// Основная программа
int main() {
    // Чтение матрицы из файла
    std::vector<std::vector<int>> matrixData = MatrixReader<int>::readMatrixFromFile("matrix.txt");

    // Создаем экземпляр MatrixDense
    MatrixDense<int> matrix1(matrixData);

    // Проверка на плотную или разреженную матрицу
    if (matrix1.isSparse()) {
        std::cout << "The matrix is sparse." << std::endl;
    } else {
        std::cout << "The matrix is dense." << std::endl;
    }

    // Создаем экземпляр MatrixDiagonal для проверки на диагональность
    MatrixDiagonal<int> diagonalMatrix(matrixData);

    if (diagonalMatrix.isDiagonal()) {
        std::cout << "The matrix is diagonal." << std::endl;
    } else {
        std::cout << "The matrix is not diagonal." << std::endl;
    }

    // Запись результата в файл
    MatrixWriter<int>::writeMatrixToFile("result_matrix.txt", matrixData);

    return 0;
}
