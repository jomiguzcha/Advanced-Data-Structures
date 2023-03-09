#pragma once
#include <vector>
#include "octant.h"

using namespace std;

template<long long n,int percent>
struct Octree {
	Octant* root;

    Octree(Point _a,Point _b) {//Parametros:Min,Max
        root = new Octant(_a.x,_b.x,_a.y, _b.y, _a.z, _b.z);
	}

	bool in_range(Point a, Octant* ptr) {
		bool aux = a.x > ptr->range[0][0] && a.x < ptr->range[0][1];
		bool aux2 = a.y > ptr->range[1][0] && a.y < ptr->range[1][1];
		bool aux3 = a.z > ptr->range[2][0] && a.z < ptr->range[2][1];
		return aux && aux2 && aux3;
	}

	bool not_full(Octant* ptr) {// si con uno mas no rompe la regla
		long long p = (n * percent) / 100;

		return (ptr->points.size() + 1 < p);
	}

	bool not_full_r(Octant* ptr) {// si con uno menos aun rompe la regla
		long long p = (n * percent) / 100;

		int cont = 0;
		for (int i = 0; i < 8; i++) {
			cont += (ptr->childs[i])->points.size();
		}

		return (cont - 1 >= p);
	}

    void _split(Octant* ptr) {
		Octant* ptr2 = NULL;

		//1
		ptr->childs[0] = new Octant(ptr->range[0][0], ptr->mid[0],
									ptr->range[1][0], ptr->mid[1],
									ptr->range[2][0], ptr->mid[2]);

		//2
		ptr->childs[1] = new Octant(ptr->mid[0], ptr->range[0][1],
									ptr->range[1][0], ptr->mid[1],
									ptr->range[2][0], ptr->mid[2]);

		//3
		ptr->childs[2] = new Octant(ptr->range[0][0], ptr->mid[0],
									ptr->mid[1], ptr->range[1][1],
									ptr->range[2][0], ptr->mid[2]);

		//4
		ptr->childs[3] = new Octant(ptr->mid[0], ptr->range[0][1],
									ptr->mid[1], ptr->range[1][1],
									ptr->range[2][0], ptr->mid[2]);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//5
		ptr->childs[4] = new Octant(ptr->range[0][0], ptr->mid[0],
									ptr->range[1][0], ptr->mid[1],
									ptr->mid[2], ptr->range[2][1]);

		//6
		ptr->childs[5] = new Octant(ptr->mid[0], ptr->range[0][1],
									ptr->range[1][0], ptr->mid[1],
									ptr->mid[2], ptr->range[2][1]);

		//7
		ptr->childs[6] = new Octant(ptr->range[0][0], ptr->mid[0],
									ptr->mid[1], ptr->range[1][1],
									ptr->mid[2], ptr->range[2][1]);

		//8
		ptr->childs[7] = new Octant(ptr->mid[0], ptr->range[0][1],
									ptr->mid[1], ptr->range[1][1],
									ptr->mid[2], ptr->range[2][1]);


		for (int i = 0; i < ptr->points.size(); i++) {//distribuye los puntos segun corresponda
			ptr2 = ptr;
			found(ptr->points[i], ptr2);
			ptr2->insert(ptr->points[i]);
		}

		ptr->points.clear();

	}

    void un_split(Octant* ptr){
		for (int i = 0; i < 8; i++) {//recupera los puntos de los hijos
			for (int j = 0; i < (ptr->childs[i])->points.size(); j++) {
                ptr->points.push_back((ptr->childs[i])->points[j]);
			}
		}

		for (int i = 0; i < 8; ++i) {//borra los hijos 
			delete[] ptr->childs[i];
			ptr->childs[i] = NULL; 
		}
	}
	
	bool found(Point a,Octant*& ptr,bool d = false){
		Octant* ptr2 = NULL;

		if (!d) {
			while (ptr->childs[0] != NULL) {//root no es un octante hoja
				for (int i = 0; i < 8; i++) {
					ptr2 = ptr->childs[i];
					if (in_range(a, ptr2)) {
						break;
					}
				}
				ptr = ptr2; ptr2 = NULL;
			}

			return ptr->found(a);
		}
		
		else {
			while (ptr->childs[0] != NULL) {//root no es un octante hoja
				for (int i = 0; i < 8; i++) {
					ptr2 = ptr->childs[i];
					if (in_range(a, ptr2)) {
						if (ptr2->childs[0] == NULL) {//al que va a saltar ptr es nodo hoja
							return ptr2->found(a);
						}
						break;
					}
				}
				ptr = ptr2; ptr2 = NULL;
			}

			return ptr->found(a);
		}
	}

	bool insert(Point a) {
		Octant* ptr = root;//puntero al octante donde se encuentre
		if (in_range(a,ptr) && !found(a,ptr)){
			if (not_full(ptr)){
				return ptr->insert(a);
			}
			else{
                _split(ptr);
				found(a, ptr);
				return ptr->insert(a);
			}
		}
	}

	bool del(Point a) {
		Octant* ptr = NULL;//puntero al octante donde se encuentre
		if (in_range(a, root) && found(a, ptr, 1)) {
			if (not_full_r(ptr)) {
				Octant* ptr2 = NULL;
				for (int i = 0; i < 8; i++) {
					ptr2 = ptr->childs[i];
					if (in_range(a, ptr2)) {
						return ptr->del(a);
					}
				}
			}
			else {
                un_split(ptr);
				return ptr->del(a);
			}
		}
	}

	void see() {
		Octant* ptr = root;
		Octant* ptr2 = NULL;

		for (int i = 0; i < ptr->points.size(); i++) {
			cout << "(" << ptr->points[i].x << " " << ptr->points[i].y << " " << ptr->points[i].z << ")" << "  ";
		}

		if (ptr->points.size() == 0) cout << "-" << endl;

		if (ptr->childs[0] != NULL) {//root no es un octante hoja
			for (int i = 0; i < 8; i++) {
				cout << "Hijo " << i << ":";
				for (int i = 0; i < ptr->childs[i]->points.size(); i++) {
					cout << "(" << ptr->childs[i]->points[i].x << " " << ptr->childs[i]->points[i].y << " " << ptr->childs[i]->points[i].z << ")" << "  ";
				}
				cout <<endl << "/////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
			}
		}

		for (int j = 0; j < 8; j++) {
			ptr2 = ptr->childs[j];
			if (ptr2->childs[0] != NULL) {//root no es un octante hoja
				for (int i = 0; i < 8; i++) {
					cout << "Hijos second " << i << ":";
					for (int i = 0; i < ptr2->childs[i]->points.size(); i++) {
						cout << "(" << ptr2->childs[i]->points[i].x << " " << ptr2->childs[i]->points[i].y << " " << ptr2->childs[i]->points[i].z << ")" << "  ";
					}
					cout << endl << "/////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
				}
			}
			else {
				cout << "no" << endl;
			}
		}

	}

};
