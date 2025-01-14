import matplotlib.pyplot as plt
import numpy as np

# Функция для чтения данных из файла
def read_data(filename):
    x = []
    T = []
    with open(filename, 'r') as file:
        next(file)  # Пропускаем заголовок
        for line in file:
            values = line.strip().split()
            if len(values) == 2:
                x.append(float(values[0]))
                T.append(float(values[1]))
    return np.array(x), np.array(T)

# Имя файла с данными
filename = 'temperature_results.txt'

# Чтение данных
x, T = read_data(filename)

# Построение графика
plt.figure(figsize=(10, 6))
plt.plot(x, T, 'b-', label='Температура')
plt.xlabel('Позиция x (м)')
plt.ylabel('Температура T (°C)')
plt.title('График зависимости температуры от позиции')
plt.legend()
plt.grid(True)

# Отображение графика
plt.show()