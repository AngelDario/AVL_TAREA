#include <iostream>
#include <stdlib.h>
#include <time.h>


using namespace std;

struct NODE {
	int valor;
	NODE* nodes[2];

	NODE(int _valor) : valor(_valor) {
		nodes[0] = nodes[1] = 0;
	}
};

class CTREE {
public:
	NODE* m_root;

	CTREE() {
		m_root = 0;
	}

	bool find(int x, NODE**& p) {
		p = &m_root;
		while (*p && (*p)->valor != x) {
			p = &((*p)->nodes[x > (*p)->valor]);
		}
		return *p != 0;
	}

	bool insert(int x) {
		NODE** p;
		if (find(x, p)) {
			return 0;
		}
		*p = new NODE(x);
		return 1;
	}

	NODE** REP(NODE** p) {
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

	bool remove(int x) {
		NODE** p;
		if (!find(x, p)) {
			return 0;
		}
		if ((*p)->nodes[0] && (*p)->nodes[1]) {
			NODE** q = REP(p);
			(*p)->valor = (*q)->valor;
			p = q;
		}
		NODE* temp = *p;
		*p = (*p)->nodes[!(*p)->nodes[0]];
		delete temp;
		return 1;
	}

	void inorder(NODE* k) {
		if (!k) return;
		inorder(k->nodes[0]);
		cout << k->valor << "->";
		inorder(k->nodes[1]);
	}

	void pre_order(NODE* k) {
		if (!k) return;
		cout << k->valor << "->";
		pre_order(k->nodes[0]);
		pre_order(k->nodes[1]);
	}

	void post_order(NODE* k) {
		if (!k) return;
		post_order(k->nodes[0]);
		post_order(k->nodes[1]);
		cout << k->valor << "->";
	}

};

//n=(*p)->NODE

int main()
{
	CTREE arbol;
	int x;
	arbol.insert(9);
	arbol.insert(6);
	arbol.insert(8);
	arbol.insert(11);
	arbol.insert(1);
	arbol.insert(5);
	arbol.insert(13);
	while (1) {
		cout << "Inorder: ";
		arbol.inorder(arbol.m_root);
		cout << endl;
		cout << "Pre-order: ";
		arbol.pre_order(arbol.m_root);
		cout << endl;
		cout << "Post-order: ";
		arbol.post_order(arbol.m_root);
		cout << endl;
		cin >> x;
		arbol.remove(x);
		cout << endl;
	}

	return 0;
}