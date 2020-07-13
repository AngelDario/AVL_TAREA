#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
#define COUNT 10

template <class T>
struct NODE {
	T valor;
	int peso;

	NODE<T>* nodes[2];

	NODE(T _valor) : valor(_valor) {
		peso = 0;
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

	bool find(T x, NODE<T>**& p, NODE<T>**& list, int& cont) {
		p = &m_root;
		list[cont] = *p;

		while (*p && (*p)->valor != x) {
			p = &((*p)->nodes[x > (*p)->valor]);
			cont++;
			list[cont] = *p;
		}
		return *p != 0;
	}

	bool find(T x, NODE<T>**& p) {
		p = &m_root;

		while (*p && (*p)->valor != x) {
			p = &((*p)->nodes[x > (*p)->valor]);
		}
		return *p != 0;
	}

	bool insert(T x) {
		NODE<T>** p;
		NODE<T>** list = new NODE<T> * [100];
		int cont = 0;

		if (find(x, p, list, cont)) {
			return 0;
		}

		*p = new NODE<T>(x);
		list[cont] = *p;

		for (int i = cont; i > 0; i--) {
			list[i - 1]->addPeso(!(list[i] == list[i - 1]->nodes[0]));
			if (list[i - 1]->peso == 0)
				break;
			if (list[i - 1]->peso == 2 || list[i - 1]->peso == -2) {
				if (list[i - 1]->peso == 2) {
					if (list[i - 1]->nodes[1]->peso == 1) {
						list[i-1]->nodes[1] = list[i]->nodes[0];
						list[i]->nodes[0] = list[i - 1];
						if (!(i - 1 == 0)) {
							if (list[i - 2]->nodes[0] == list[i - 1]) {
								list[i - 2]->nodes[0] = list[i];
							}
							else {
								list[i - 2]->nodes[1] = list[i];
							}
						}
						else {
							m_root = list[i];
						}
						list[i - 1]->peso = 0;
						list[i]->peso = 0;
					}
					else if (list[i - 1]->nodes[1]->peso == -1) {
						list[i - 1]->nodes[1] = list[i + 1]->nodes[0];
						list[i]->nodes[0] = list[i + 1]->nodes[1];
						list[i + 1]->nodes[0] = list[i - 1];
						list[i + 1]->nodes[1] = list[i];
						if (!(i - 1 == 0)) {
							if (list[i - 2]->nodes[0] == list[i - 1]) {
								list[i - 2]->nodes[0] = list[i + 1];
							}
							else {
								list[i - 2]->nodes[1] = list[i + 1];
							}
						}
						else {
							m_root = list[i + 1];
						}
						if (list[i + 1]->peso == -1) {
							list[i]->peso = 0;
							list[i - 1]->peso = 1;
						}
						else if (list[i + 1]->peso == 0) {
							list[i]->peso = 0;
							list[i - 1]->peso = 0;
						}
						else if (list[i+1]->peso == 1) {
							list[i]->peso = -1;
							list[i - 1]->peso = 0;
						}
					}
				}
				else if (list[i - 1]->peso == -2) {
					if (list[i - 1]->nodes[0]->peso == -1) {
						list[i - 1]->nodes[0] = list[i]->nodes[1];
						list[i]->nodes[1] = list[i - 1];
						if (!(i - 1 == 0)) {
							if (list[i - 2]->nodes[0] == list[i - 1]) {
								list[i - 2]->nodes[0] = list[i];
							}
							else {
								list[i - 2]->nodes[1] = list[i];
							}
						}
						else {
							m_root = list[i];
						}
						list[i - 1]->peso = 0;
						list[i]->peso = 0;
					}
					else if (list[i - 1]->nodes[0]->peso == 1) {
						list[i]->nodes[1] = list[i + 1]->nodes[0];
						list[i - 1]->nodes[0] = list[i + 1]->nodes[1];
						list[i + 1]->nodes[0] = list[i];
						list[i + 1]->nodes[1] = list[i - 1];
						if (!(i - 1 == 0)) {
							if (list[i - 2]->nodes[0] == list[i - 1]) {
								list[i - 2]->nodes[0] = list[i + 1];
							}
							else {
								list[i - 2]->nodes[1] = list[i + 1];
							}
						}
						else {
							m_root = list[i + 1];
						}
						if (list[i + 1]->peso == -1) {
							list[i]->peso = 0;
							list[i - 1]->peso = 1;
						}
						else if (list[i + 1]->peso == 0) {
							list[i]->peso = 0;
							list[i - 1]->peso = 0;
						}
						else if (list[i + 1]->peso == 1) {
							list[i]->peso = -1;
							list[i - 1]->peso = 0;
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