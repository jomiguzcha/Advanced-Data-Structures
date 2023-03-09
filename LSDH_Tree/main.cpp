// Copyright 2020 Roger Peralta Aranibar Advanced Data Estructures
#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <chrono>
#include"lsdh_tree.h"
#include "input.h"

template <typename>
class Timer;

template <typename R, typename... T>
class Timer<R(T...)> {
public:
    typedef R(*function_type)(T...);
    function_type function;

    explicit Timer(function_type function, std::string process_name = "")
        : function_(function), process_name_(process_name) {}

    R operator()(T... args) {
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        start = std::chrono::high_resolution_clock::now();

        R result = function_(std::forward<T>(args)...);

        end = std::chrono::high_resolution_clock::now();
        int64_t duration =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count();

        std::cout << std::setw(10) << process_name_ << std::setw(30)
            << "Duration: " + std::to_string(duration) + " ns\n";
        return result;
    }

private:
    function_type function_;
    std::string process_name_;
};

lsdh_tree<float> G_DS(14,6);//dimension--puntos maximos por bucket
float query_a[14] = { 0.5,0.6,0.7,0.1,0.2,0.5,0.6,0.4,-10,0.5,0.5,0.414,0.4,0.9 };

int build_data_structure() {
    read_record(G_DS);//archivo para leer el dataset
    return 0;
}

int build_data_structure2() {
    G_DS.insert(query_a);//insercion individual
    return 0;
}

std::vector<std::vector<float>> query_knn(int k, float* query) {
    return G_DS.knn(k,query);//numero k de vecinos ---- array de n dimensiones
}

int main() {
    Timer<int()> timed_built(build_data_structure, "Index");
    timed_built();

    Timer<std::vector<std::vector<float>>(int, float*)> timed_query(
        query_knn, "Query KNN");
    std::vector<float> query = { 1, 2, 3 };
    int k = 4;
    std::vector<std::vector<float>> result = timed_query(k, query_a);

    for (int i = 0; i < result.size(); ++i) {
        if (result[i].size() != 0) {
            cout << "Objeto: ";
            for (int j = 0; j < result[i].size(); ++j) {
                cout << result[i][j] << " ";
            }
            cout << endl;
        }
    }
}