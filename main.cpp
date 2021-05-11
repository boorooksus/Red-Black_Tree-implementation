#include <iostream>
#define BLACK 0
#define RED 1
using namespace std;

struct record {
	string disease;
	int cost;
	record* next = NULL;
};

struct pinfo {
	int pid = 0;
	string name = "";
	int tel = 0;
	int ax = 0;
	int ay = 0;
	record* records = NULL;
};

struct node {
	pinfo* patient = NULL;
	int dept;
	int color;
	node* parent;
	node* left = NULL;
	node* right = NULL;
};

int T;
char cmd;

void insertPatient();
node findPatient(int pid);

int main() {
	cin >> T;
	while (T--) {
		cin >> cmd;
		if (cmd == 'I') {
			insertPatient();
		}
	}

}

void insertPatient() {
	pinfo p;
	record r;
	cin >> p.pid >> p.name >> p.tel >> p.ax >> p.ay
		>> r.disease >> r.cost;

	node x = findPatient(p.pid);
	if (x.parent) {
		cout << x.dept << " " << 0;
		return;
	}

	p.records = &r;
	

}