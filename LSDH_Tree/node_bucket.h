#pragma once
#include "node.h"
#include "spatial_object.h"
#include <vector>

template <typename T>
struct node_bucket : public node{

	std::vector<spatial_object<T>> objects;
	T** range;// 0 min // 1 max

	node_bucket(int _dim, T* _min, T* _max){
		type = 0;
		dimension = _dim;//cantidad de dimensiones
		range = new T*[dimension];

		for (int i = 0; i < dimension; ++i) {
			range[i] = new T[2];
			range[i][0] = _min[i];
			range[i][1] = _max[i];
		}
	}

	~node_bucket() {
		for (int i = 0; i < dimension; ++i) {
			delete[] range[i];
		}
		delete[] range;
	}

	void update_range(T* _min, T* _max) {
		for (int i = 0; i < dimension; ++i) {
			range[i][0] = _min[i];
			range[i][1] = _max[i];
		}
	}

	int found(T* a) {
		for (int i = 0; i < objects.size(); ++i) {
			if (objects[i] == a) {
				return i;
			}
		}
		return 0;
	}

	void insert(T* a){
		objects.push_back(spatial_object<T>(dimension, a));
	}

	void insert(spatial_object<T> a) {
		objects.push_back(spatial_object<T>(dimension, a.data));
	}

	void erase(T* a) {
		for (int i = 0; i < objects.size(); ++i) {
			if (objects[i] == a) {
				objects.erase(objects.begin() + i);
				break;
			}
		}
	}

	void clear() {
		objects.clear();
	}
};

