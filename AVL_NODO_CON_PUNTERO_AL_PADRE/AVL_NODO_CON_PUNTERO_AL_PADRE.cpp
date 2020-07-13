#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
#define COUNT 10

template <class T>
struct NODE {
	T valor;
	int peso;

	NODE<T>* padre;
	NODE<T>* nodes[2];

	NODE(T _valor, NODE<T>* _padre = 0) : valor(_valor) {
		peso = 0;
		padre = _padre;
		nodes[0] = nodes[1] = 0;
	}

	void addPeso(bool i) {
		if (i) {
			peso++;
			return;
		}
		peso--;
		return;
	}
};

template <class T>
class CTREE {
public:
	NODE<T>* m_root;

	CTREE() {
		m_root = 0;
	}

	void destruir(NODE<T>* k) {
		if (!k) return;
		destruir(k->nodes[0]);
		destruir(k->nodes[1]);
		delete k;
	}

	bool find(T x, NODE<T>**& p) {
		p = &m_root;

		while (*p && (*p)->valor != x) {
			p = &((*p)->nodes[x > (*p)->valor]);
		}
		return *p != 0;
	}

	bool find(T x, NODE<T>**& padre, NODE<T>**& hijo) {
		padre = &m_root;
		hijo = &m_root;

		while (*hijo && (*hijo)->valor != x) {
			if (*hijo != m_root)
				padre = &((*padre)->nodes[x > (*padre)->valor]);
			hijo = &((*hijo)->nodes[x > (*hijo)->valor]);
		}
		return *hijo != 0;
	}

	bool insert(T x) {
		NODE<T>** padre;
		NODE<T>** hijo;

		if (find(x, padre, hijo)) {
			return 0;
		}
		
		if (padre != hijo) {
			*hijo = new NODE<T>(x, *padre);
		}
		else {
			*hijo = new NODE<T>(x, NULL);
		}

		for (NODE<T>** i = hijo; *i != m_root; i = &(*i)->padre) {
			NODE<T>* A = (*i)->padre;
			NODE<T>* B = *i;

			A->addPeso(!(B == A->nodes[0]));
			if (A->peso == 0)
				break;
			if (A->peso == 2 || A->peso == -2) {
				if (A->peso == 2) {
					if (B->peso == 1) {
						NODE<T>* C = B->nodes[1];
						A->nodes[1] = B->nodes[0];
						if (B->nodes[0]) {
							(B->nodes[0])->padre = A;
						}
						B->nodes[0] = A;
						if (!(A->padre == NULL)) {
							if (A->padre->nodes[0] == A) {
								A->padre->nodes[0] = B;
								B->padre = A->padre;
								A->padre = B;
							}
							else {
								A->padre->nodes[1] = B;
								B->padre = A->padre;
								A->padre = B;
							}
						}
						else {
							B->padre = A->padre;
							A->padre = B;
							m_root = B;
						}
						A->peso = 0;
						B->peso = 0;
					}
					else if (B->peso == -1) {
						NODE<T>* C = B->nodes[0];
						A->nodes[1] = C->nodes[0];
						if (C->nodes[0]) {
							(C->nodes[0])->padre = A;
						}
						B->nodes[0] = C->nodes[1];
						if (C->nodes[1]) {
							(C->nodes[1])->padre = B;
						}
						C->nodes[0] = A;
						C->nodes[1] = B;
						if (!(A->padre == NULL)) {
							if (A->padre->nodes[0] == A) {
								A->padre->nodes[0] = C;
								C->padre = A->padre;
								A->padre = C;
								B->padre = C;
							}
							else {
								A->padre->nodes[1] = C;
								C->padre = A->padre;
								A->padre = C;
								B->padre = C;
							}
						}
						else {
							C->padre = A->padre;
							A->padre = C;
							B->padre = C;
							m_root = C;
						}
						if (C->peso == -1) {
							B->peso = 0;
							A->peso = 1;
						}
						else if (C->peso == 0) {
							B->peso = 0;
							A->peso = 0;
						}
						else if (C->peso == 1) {
							B->peso = -1;
							A->peso = 0;
						}
					}
				}
				else if (A->peso == -2) {
					if (B->peso == -1) {
						NODE<T>* C = B->nodes[0];
						A->nodes[0] = B->nodes[1];
						if (B->nodes[1]) {
							(B->nodes[1])->padre = A;
						}
						B->nodes[1] = A;
						if (!(A->padre == NULL)) {
							if (A->padre->nodes[0] == A) {
								A->padre->nodes[0] = B;
								B->padre = A->padre;
								A->padre = B;
							}
							else {
								A->padre->nodes[1] = B;
								B->padre = A->padre;
								A->padre = B;
							}
						}
						else {
							B->padre = A->padre;
							A->padre = B;
							m_root = B;
						}
						A->peso = 0;
						B->peso = 0;
					}
					else if (B->peso == 1) {
						NODE<T>* C = B->nodes[1];
						A->nodes[0] = C->nodes[1];
						if (C->nodes[1]) {
							(C->nodes[1])->padre = A;
						}
						B->nodes[1] = C->nodes[0];
						if (C->nodes[0]) {
							(C->nodes[0])->padre = B;
						}
						C->nodes[0] = B;
						C->nodes[1] = A;
						if (!(A->padre == NULL)) {
							if (A->padre->nodes[0] == A) {
								A->padre->nodes[0] = C;
								C->padre = A->padre;
								A->padre = C;
								B->padre = C;
							}
							else {
								A->padre->nodes[1] = C;
								C->padre = A->padre;
								A->padre = C;
								B->padre = C;
							}
						}
						else {
							C->padre = A->padre;
							A->padre = C;
							B->padre = C;
							m_root = C;
						}
						if (C->peso == -1) {
							B->peso = 0;
							A->peso = 1;
						}
						else if (C->peso == 0) {
							B->peso = 0;
							A->peso = 0;
						}
						else if (C->peso == 1) {
							B->peso = -1;
							A->peso = 0;
						}
					}
				}
				return 1;
			}
		}

		return 1;
	}

	NODE<T>** REP(NODE<T>** p) {
		srand(time(NULL));
		bool random = rand() % 2;
		if ((*p)->nodes[random]) {
			p = &((*p)->nodes[random]);
			while ((*p)->nodes[!random]) {
				p = &((*p)->nodes[!random]);
			}
			return p;
		}
		p = &((*p)->nodes[!random]);
		while ((*p)->nodes[random]) {
			p = &((*p)->nodes[random]);
		}
		return p;
	}

	bool remove(T x) {
		NODE<T>** p;

		if (!find(x, p)) {
			return 0;
		}

		if ((*p)->nodes[0] && (*p)->nodes[1]) {
			NODE<T>** q = REP(p);
			(*p)->valor = (*q)->valor;
			p = q;
		}
		NODE<T>* temp = *p;
		*p = (*p)->nodes[!(*p)->nodes[0]];
		delete temp;
		return 1;
	}

	void inorder(NODE<T>* k) {
		if (!k) return;
		inorder(k->nodes[0]);
		cout << k->valor << "->";
		inorder(k->nodes[1]);
	}

	void pre_order(NODE<T>* k) {
		if (!k) return;
		cout << k->valor << "->";
		pre_order(k->nodes[0]);
		pre_order(k->nodes[1]);
	}

	void post_order(NODE<T>* k) {
		if (!k) return;
		post_order(k->nodes[0]);
		post_order(k->nodes[1]);
		cout << k->valor << "->";
	}

	void reverse(NODE<T>* k) {
		if (!k) return;
		reverse(k->nodes[1]);
		cout << k->valor << "->";
		reverse(k->nodes[0]);
	}

	void print2d(NODE<T>* root, int space) {
		if (root == NULL)
			return;
		space += COUNT;

		print2d(root->nodes[1], space);

		cout << endl;
		for (int i = COUNT; i < space; i++)
			cout << " ";
		cout << root->valor << "\n";

		print2d(root->nodes[0], space);

	}
	void print(NODE<T>* root) {
		print2d(root, 0);
	}

};

template<class T>
void print(int& x, CTREE<T> arbol) {

	switch (x) {
	case 1:
		cout << "Inorder: \n";
		arbol.inorder(arbol.m_root);
		cout << endl;
		break;
	case 2:
		cout << "Preorder: \n";
		arbol.pre_order(arbol.m_root);
		cout << endl;
		break;
	case 3:
		cout << "Postorder: \n";
		arbol.post_order(arbol.m_root);
		cout << endl;
		break;
	case 4:
		cout << "Reverse: \n";
		arbol.reverse(arbol.m_root);
		cout << endl;
		break;

	case 5:
		cout << "Por Niveles: \n";
		arbol.print(arbol.m_root);
		cout << endl;
		break;
	}
}


template<class T>
void menu(CTREE<T>& arbol) {
	int x;
	int orden;

	cout << "Escoge que el tipo de recorrido: " << endl;
	cout << "1. Inorder \n";
	cout << "2. Preorder \n";
	cout << "3. Postorder \n";
	cout << "4. Reverse \n";
	cout << "5. Por Niveles \n";

	cin >> orden;

	while (1) {
		system("cls");
		print<T>(orden, arbol);
		cout << "Que quieres haccer?" << endl;
		cout << "1. Add \n";
		cout << "2. Del \n";
		cout << "3. Cambiar tipo de recorrido\n";
		cout << "4. Salir del Programa\n";
		cin >> x;

		switch (x) {
		case 1:
			T intTemp;
			cout << "Ingresa elemento para insertar: ";
			cin >> intTemp;
			arbol.insert(intTemp);
			continue;
		case 2:
			T intTemp2;
			cout << "Ingresa elemento para remover: ";
			cin >> intTemp2;
			arbol.remove(intTemp2);
			continue;
		case 3:
			cout << "Escoge que el tipo de recorrido: " << endl;
			cout << "1. Inorder \n";
			cout << "2. Preorder \n";
			cout << "3. Postorder \n";
			cout << "4. Reverse \n";
			cout << "5. Por Niveles \n";
			cin >> orden;
			continue;
		case 4:
			arbol.destruir(arbol.m_root);
			cout << "Arbol Destruido.";
			exit(0);
		}
	}
}

int main()
{
	CTREE<int> arbol;

	menu<int>(arbol);

	return 0;

}