//1.-acousticness, 2.-danceability, 3.-duration_ms, 4.-energy, 5.-explicit, 6.-instrumentalness, 7.-key,
//8.-liveness, 9.-loudness, 10.-mode, 11.-popularity, 12.-speechiness, 13.-tempo, 14.-valence

#pragma once
#include "node_directory.h"
#include "distance_cal.h"
#include <queue> 
 
template<typename T>
struct lsdh_tree{
	int dimension = 0, maxPoints = 0;
	node_directory<T>* head;
	node_bucket<T>* aux_head;
	
	priority_queue<point_distance<T>> OPQ; //Object priority queue
	priority_queue<node_distance<T>> NPQ; //Node priority queue

	lsdh_tree(int _dim,int _max) {
		T def_min[] = {0,0,0,0,0,0,0,0,-30,0,0,0,0,0}; 
		T def_max[] = {1,1,5403500,1,1,1,11,1,2,1,100,1,250,1};

		dimension = _dim;
		maxPoints = _max;

		aux_head = new node_bucket<T>(dimension, def_min, def_max);
		head = NULL;
	}

	void in_range(node_bucket<T>*& new_b, node_bucket<T>**& old_b){
		bool flag;

		for (int i = 0; i < (*old_b)->objects.size(); ++i) {
			flag = 1;

			for (int j = 0; j < dimension; ++j) {//si esta en el rango
				if ( new_b->range[j][0] > (*old_b)->objects[i].data[j] 
					|| new_b->range[j][1] < (*old_b)->objects[i].data[j]) {
					flag = 0;
					break;
				}
			}

			if (!flag) { continue; }

			new_b->insert((*old_b)->objects[i].data);//inserta el elemento al nuevo bucket

			(*old_b)->erase((*old_b)->objects[i].data);//elimina el elemento del viejo bucket

		}

	}

	void split(T*& a, node_directory<T>**& aux_d, node_bucket<T>**& aux_b, int& d_old) {
		int i = 1;
		bool flag = 0;
		T min[14]; T max[14];

		for (int j = 0; j < dimension; ++j) {
			min[j] = (*aux_b)->range[j][0]; max[j] = (*aux_b)->range[j][1];// minimos y maximos de ese bucket
		}

		while (!flag) {//halla la dimension a partir
			T aux = (*aux_b)->objects[0].data[(d_old + i) % dimension];//primer objeto

			for (int j = 1; j < (*aux_b)->objects.size(); ++j) {
				if (aux != ((*aux_b)->objects[j].data[(d_old + i) % dimension])) {//demás objetos
					i = (d_old + i) % dimension;
					flag = 1;
					break;
				}
			}

			if (!flag) { i++; }
		}

		T pos = (((*aux_b)->range[i][1]) - ((*aux_b)->range[i][0])) / 2;//(max-min)/2 ------- posicion de split

		min[i] += pos;
		node_bucket<T>* new_bucket = new node_bucket<T>(dimension, min, max);// nuevo bucket derecha

		in_range(new_bucket, aux_b);//redistribuye los puntos

		min[i] -= pos; max[i] -= pos;
		(*aux_b)->update_range(min, max); //nuevo bucket izquierda

		node_directory<T>* new_directory = new node_directory<T>(i, pos, (*aux_b), new_bucket);//nuevo directorio

		if (head) {
			(*aux_d)->sons_d[!(a[d_old] < (*aux_d)->pos)] = new_directory;
			(*aux_d)->sons_b[!(a[d_old] < (*aux_d)->pos)] = NULL;

			aux_b = &((new_directory)->sons_b[!(a[i] < new_directory->pos)]);
		}

		else {
			head = new_directory; 
			aux_b = &((new_directory)->sons_b[!(a[i] < new_directory->pos)]);
			aux_head = NULL;
		}
	}

	bool search(T*& a, node_directory<T>**& aux_d, node_bucket<T>**& aux_b,int &d){
		if (head) {
			aux_d = &head;
			d = (*aux_d)->dimension;

			while (((*aux_d)->sons_d[!(a[d] < (*aux_d)->pos)]) != NULL) {//mientras sea directorio

				aux_d = &((*aux_d)->sons_d[!(a[d] < (*aux_d)->pos)]);//direccion a tomar
				d = (*aux_d)->dimension;
			}

			aux_b = &((*aux_d)->sons_b[!(a[d] < (*aux_d)->pos)]);

			if ((*aux_b)->found(a)) {//si se encuentra ya
				return 1;
			}
			return 0;
		}
		else {
			aux_b = &aux_head;

			if ((*aux_b)->found(a)) {//si se encuentra ya
				return 1;
			}
			return 0;
		}
	}

	void insert(T* a){
		node_directory<T>** aux_d = NULL; node_bucket<T>** aux_b = NULL;
		int d = 0;

		if (search(a, aux_d, aux_b, d)) {// si se encuentra ya
			return;
		}

		if (((*aux_b)->objects.size()+1)>maxPoints){// si lo rebasa
			split(a,aux_d,aux_b,d);
		}

		(*aux_b)->insert(a);
	}

	vector<vector<T>> knn(int k,T* a){
		vector<vector<T>> result(k);
		node_directory<T>** aux_d = NULL; node_bucket<T>** aux_b = NULL;
		int d = 0;
		
		if (head){
			aux_d = &head;
			d = (*aux_d)->dimension;

			while (((*aux_d)->sons_d[!(a[d] < (*aux_d)->pos)]) != NULL) {//mientras sea directorio
				if (((*aux_d)->sons_d[(a[d] < (*aux_d)->pos)]) != NULL) {//si el otro nodo es directorio
					//NPQ.push(node_distance<T>(a, (*aux_d)->sons_d[(a[d] < (*aux_d)->pos)]));
				}
				else {//si el otro nodo es bucket
					//NPQ.push(node_distance<T>(a, (*aux_d)->sons_b[(a[d] < (*aux_d)->pos)]));
				}

				aux_d = &((*aux_d)->sons_d[!(a[d] < (*aux_d)->pos)]);//direccion a tomar
				d = (*aux_d)->dimension;
			}

			aux_b = &((*aux_d)->sons_b[!(a[d] < (*aux_d)->pos)]);

			for (int i = 0; i < (*aux_b)->objects.size(); ++i) {
				OPQ.push(point_distance<T>(a, (*aux_b)->objects[i].data));//llena el opq con los objetos del bucket 
			}

			int n = 0;//numero de objetos sacados

			while (!NPQ.empty()){
				T min_distance_npq = NPQ.top().distance;//distancia del primer elemento npq
				
				for (; !OPQ.empty() && n < k && (OPQ.top().distance <= min_distance_npq); ++n) {
					result.push_back(vector<T>(OPQ.top().point, OPQ.top().point + dimension));//tomo los elementos opq
					OPQ.pop();
				}

				if (OPQ.empty() || OPQ.top().distance > min_distance_npq) {//si hay que obtener de npq
					if (NPQ.top().d != NULL) {//si es directorio
						node_directory<T>* x = NPQ.top().d;
						aux_d = &x;

						d = (*aux_d)->dimension;
						NPQ.pop();

						while (((*aux_d)->sons_d[!(a[d] < (*aux_d)->pos)]) != NULL) {//mientras sea directorio
							if (((*aux_d)->sons_d[(a[d] < (*aux_d)->pos)]) != NULL) {//si el otro nodo es directorio
								NPQ.push(node_distance<T>(a, (*aux_d)->sons_d[(a[d] < (*aux_d)->pos)]));
							}
							else {//si el otro nodo es bucket
								NPQ.push(node_distance<T>(a, (*aux_d)->sons_b[(a[d] < (*aux_d)->pos)]));
							}

							aux_d = &((*aux_d)->sons_d[!(a[d] < (*aux_d)->pos)]);//direccion a tomar
							d = (*aux_d)->dimension;
						}

						aux_b = &((*aux_d)->sons_b[!(a[d] < (*aux_d)->pos)]);
						for (int i = 0; i < (*aux_b)->objects.size(); ++i) {
							OPQ.push(point_distance<T>(a, (*aux_b)->objects[i].data));//llena el opq con los objetos del bucket 
						}
					}

					else {//si es bucket
						node_bucket<T>* x = NPQ.top().b;
						aux_b = &x;
						for (int i = 0; i < (*aux_b)->objects.size(); ++i) {
							OPQ.push(point_distance<T>(a, (*aux_b)->objects[i].data));//llena el opq con los objetos del bucket 
						}
						NPQ.pop();
					}

				}

				else {
					return result;
				}
			}

			for (; !OPQ.empty() && n < k; ++n) {
				result.push_back(vector<T>(OPQ.top().point, OPQ.top().point + dimension));//tomo los elementos opq
				OPQ.pop();
			}

			if (n < k) { cout << "No hay mas puntos que mostrar" << endl; }

			return result;
		}

		else {//solo un bucket sin directorios
			for (int i = 0; i < (*aux_head).objects.size(); ++i) {
				OPQ.push(point_distance<T>(a,(*aux_head).objects[i].data));//llena el opq con los objetos del bucket 
			}

			for (int i = 0; !OPQ.empty() && i<k; ++i) {
				result.push_back(vector<T>(OPQ.top().point, OPQ.top().point + dimension));//tomo los elementos opq
				OPQ.pop();
			}

			return result;
		}
	}
};
