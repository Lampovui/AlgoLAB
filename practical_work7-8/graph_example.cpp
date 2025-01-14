#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Предварительное объявление класса Vertex
template <class V, class E>
class Vertex;

// Класс для представления ребра
template <class V, class E>
class Edge {
public:
    Edge(const E& properties, Vertex<V, E>* vertex1, Vertex<V, E>* vertex2)
        : properties_(properties), vertex1_(vertex1), vertex2_(vertex2) {}

    const E& getProperties() const { return properties_; }
    const Vertex<V, E>* getVertex1() const { return vertex1_; }
    const Vertex<V, E>* getVertex2() const { return vertex2_; }

private:
    E properties_;
    Vertex<V, E>* vertex1_;
    Vertex<V, E>* vertex2_;
};

// Класс для представления вершины
template <class V, class E>
class Vertex {
public:
    Vertex(const V& properties) : properties_(properties) {}

    const V& getProperties() const { return properties_; }
    const std::vector<Edge<V, E>*>& getEdges() const { return edges_; }

    void addEdge(const E& properties, Vertex<V, E>* target) {
        Edge<V, E>* edge = new Edge<V, E>(properties, this, target);
        edges_.push_back(edge);
        target->edges_.push_back(edge);
    }

private:
    V properties_;
    std::vector<Edge<V, E>*> edges_;
};

// Функция для поиска пути и подсчета его стоимости
template <class V, class E>
bool depthFirstSearch(const Vertex<V, E>* current, const V& target, 
                      std::vector<const Vertex<V, E>*>& visited, int& cost) {
    // Проверяем, была ли вершина уже посещена
    if (std::find(visited.begin(), visited.end(), current) != visited.end()) {
        return false;
    }
    visited.push_back(current);

    // Если нашли целевую вершину, завершаем поиск
    if (current->getProperties() == target) {
        return true;
    }

    // Рекурсивно обходим соседние вершины
    for (const auto& edge : current->getEdges()) {
        const Vertex<V, E>* next = (edge->getVertex1() == current) ? edge->getVertex2() : edge->getVertex1();
        if (depthFirstSearch(next, target, visited, cost)) {
            cost += edge->getProperties();
            return true;
        }
    }

    visited.pop_back();
    return false;
}

int main() {
    // Создаем вершины
    Vertex<std::string, int> a("A");
    Vertex<std::string, int> b("B");
    Vertex<std::string, int> c("C");
    Vertex<std::string, int> d("D");
    Vertex<std::string, int> e("E");
    Vertex<std::string, int> f("F");
    Vertex<std::string, int> g("G");
    Vertex<std::string, int> h("H");

    // Добавляем ребра
    a.addEdge(10, &b);
    a.addEdge(15, &c);
    b.addEdge(10, &f);
    b.addEdge(20, &d);
    c.addEdge(5, &d);
    c.addEdge(25, &e);
    d.addEdge(5, &g); // Петля
    g.addEdge(5, &d); // Петля
    e.addEdge(25, &h); // Тупик

    // Поиск пути из A в H
    std::vector<const Vertex<std::string, int>*> visited;
    int cost = 0;
    if (depthFirstSearch(&a, std::string("H"), visited, cost)) {
        std::cout << "Path found with cost: " << cost << "\n";
        std::cout << "Visited nodes: ";
        for (const auto& vertex : visited) {
            std::cout << vertex->getProperties() << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "Path not found.\n";
    }

    return 0;
}
