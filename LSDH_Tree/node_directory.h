#pragma once
#include "node_bucket.h"

template<typename T>
struct node_directory : public node{
	T pos;
	node_directory<T>* sons_d[2];
	node_bucket<T>* sons_b[2];

	node_directory(int _dim, T _pos, node_bucket<T>* left, node_bucket<T>* right) {
		type = 1;
		dimension = _dim;
		pos = _pos;

		sons_b[0] = left; sons_b[1] = right;
		sons_d[0] = NULL; sons_d[1] = NULL;
	}

	node_directory(int _dim, T _pos, node_directory<T>* left, node_bucket<T>* right) {
		type = 1;
		dimension = _dim;
		pos = _pos;

		sons_b[0] = NULL; sons_b[1] = right;
		sons_d[0] = left; sons_d[1] = NULL;
	}

	node_directory(int _dim, T _pos, node_bucket<T>* left, node_directory<T>* right) {
		type = 1;
		dimension = _dim;
		pos = _pos;

		sons_b[0] = left; sons_b[1] = NULL;
		sons_d[0] = NULL; sons_d[1] = right;
	}

	node_directory(int _dim, T _pos, node_directory<T>* left, node_directory<T>* right) {
		type = 1;
		dimension = _dim;
		pos = _pos;

		sons_b[0] = NULL; sons_b[1] = NULL;
		sons_d[0] = left; sons_d[1] = right;
	}

};
