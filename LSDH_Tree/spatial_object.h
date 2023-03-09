#pragma once
#include <string>

using namespace std;

template <typename T>
struct spatial_object{
	int dimension = 0;
	string nombre; string autor;
	T* data;

	spatial_object(int _dim, T* _data) {
		dimension = _dim;
		//nombre = _nombre; autor = _autor;
		data = new T[dimension];
		
		for (int i = 0; i < dimension; ++i) {
			data[i] = _data[i];
		}
	}

	/*~spatial_object() {
		delete[] data;
	}*/

	bool operator==(T* a) {
		bool aux = 1;//1 es igual
		for (int i = 0; i < dimension; ++i) {
			if (data[i] != a[i]) {
				aux = 0;
				return aux;
			}
		}
		return aux;
	}

	bool operator==(spatial_object<T> a){
		bool aux = 1;//1 es igual
		for (int i = 0; i < dimension; ++i) {
			if (data[i] != a.data[i]) {
				aux = 0;
				return aux;
			}
		}
		return aux;
	}
};