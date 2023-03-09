#include <stdio.h>
#include <math.h>
#include "node_directory.h"
#define num_dimension 14

template<typename T>
class point_distance{
public:
    point_distance() {};

    point_distance(T* p1, T* p2) {//punto p central------punto con el que se quiere hallar la distancia
        distance = find_distance(p1,p2);
        point = p2;
    };

    virtual ~point_distance() {};

    T find_distance(T* p1, T* p2){
        distance = 0;

        for (int i = 0; i < num_dimension; ++i){
            distance += (pow(p1[i] - p2[i], 2));
        }

        distance = sqrt(distance);

        return distance;
    }

    friend bool operator < (const point_distance& lhs, const point_distance& rhs) {
        return !(lhs.distance < rhs.distance);
    }

    friend bool operator > (const point_distance& lhs, const point_distance& rhs) {
        return !(lhs.distance > rhs.distance);
    }

    T distance; T* point;
};


template<typename T>
class node_distance {
public:
    node_distance() {};

    node_distance(T* p, node_bucket<T>* _b) {//punto p central------nodo con el que se quiere hallar la distancia
        distance = find_distance(p,_b);
        b = _b; d = NULL;
    };

    node_distance(T* p, node_directory<T>* _d) {//punto p central------nodo con el que se quiere hallar la distancia
        distance = find_distance(p,_d);
        d = _d; b = NULL;
    };

    virtual ~node_distance() {};

    T find_distance(T* p, node_bucket<T>* _b) {
        distance = 0;
        
        vector<T> m; vector<T> s;

        for (int i = 0; i < num_dimension; ++i){
            m.push_back((_b->range[i][1] - _b->range[i][0]) / 2);
            s.push_back(_b->range[i][1] - _b->range[i][0]);
        }

        for (int i = 0; i < num_dimension; ++i){
            distance += (pow((abs(p[i] - m[i]) - (s[i] / 2)), 2));
        }

        distance = sqrt(distance);

        return distance;
    }

    T find_distance(T* p, node_directory<T>* _d) {
        return 1;
    }

    friend bool operator < (const node_distance& lhs, const node_distance& rhs) {
        return !(lhs.distance < rhs.distance);
    }

    friend bool operator > (const node_distance& lhs, const node_distance& rhs) {
        return !(lhs.distance > rhs.distance);
    }

    T distance; node_bucket<T>* b; node_directory<T>* d;
};