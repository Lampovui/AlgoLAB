#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

// Ввод параметров, аналогичных исходной программе на Pascal
void inputParameters(double& lambda, double& rho, double& c, double& T_l, double& T_r, double& T_0, double& L, double& t_end, int& N) {
    std::cout << "Введите количество узлов (N): ";
    std::cin >> N;
    std::cout << "Введите окончательное время (t_end): ";
    std::cin >> t_end;
    std::cout << "Введите длину пластины (L): ";
    std::cin >> L;
    std::cout << "Введите коэффициент теплопроводности (lambda): ";
    std::cin >> lambda;
    std::cout << "Введите плотность материала (rho): ";
    std::cin >> rho;
    std::cout << "Введите теплоемкость материала (c): ";
    std::cin >> c;
    std::cout << "Введите начальную температуру (T_0): ";
    std::cin >> T_0;
    std::cout << "Введите температуру на границе слева (T_l): ";
    std::cin >> T_l;
    std::cout << "Введите температуру на границе справа (T_r): ";
    std::cin >> T_r;
}

// Инициализация начальной температуры
std::vector<double> initializeTemperature(int N, double T_0, double T_l, double T_r) {
    std::vector<double> T(N, T_0);
    T[0] = T_l;
    T[N - 1] = T_r;
    return T;
}

// Метод прогонки для решения уравнения теплопроводности
void solveHeatConduction(std::vector<double>& T, double tau, double h, int N, double lambda, double rho, double c, double T_l, double T_r, double t_end) {
    double a = lambda / (rho * c);  // коэффициент температуропроводности

    double time = 0.0;
    while (time < t_end) {
        time += tau;

        std::vector<double> T_prev = T;  // запоминаем предыдущий временной слой

        // Прямая прогонка для обновления температуры
        for (int i = 1; i < N - 1; ++i) {
            T[i] = T_prev[i] + a * tau / (h * h) * (T_prev[i + 1] - 2 * T_prev[i] + T_prev[i - 1]);
        }

        // Обновление граничных условий
        T[0] = T_l;
        T[N - 1] = T_r;
    }
}

// Запись результатов в файл
void writeResults(const std::vector<double>& T, double h) {
    std::ofstream outfile("temperature_results.txt");
    outfile << "Позиция x (м) Температура T (°C)" << std::endl;

    for (int i = 0; i < T.size(); ++i) {
        double x = i * h;
        outfile << x << " " << T[i] << std::endl;
    }

    outfile.close();
}

int main() {
    double lambda, rho, c, T_l, T_r, T_0, L, t_end;
    int N;

    inputParameters(lambda, rho, c, T_l, T_r, T_0, L, t_end, N);

    double h = L / (N - 1);
    double tau = 0.25 * (h * h) / (lambda / (rho * c));  // шаг по времени, оптимизированный для устойчивости

    std::vector<double> T = initializeTemperature(N, T_0, T_l, T_r);

    solveHeatConduction(T, tau, h, N, lambda, rho, c, T_l, T_r, t_end);

    writeResults(T, h);

    std::cout << "Результаты сохранены в 'temperature_results.txt'." << std::endl;

    return 0;
}
