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
node root;

void insertPatient();
node findPatient(int pid);

int main() {
	cin >> T;
	while (T--) {
		cin >> cmd;
		if (cmd == 'I') {
			pinfo p;
			record r;
			cin >> p.pid >> p.name >> p.tel >> p.ax >> p.ay
				>> r.disease >> r.cost;

			p.records = &r;
			insertPatient(p);
		}
	}

}

void insertPatient(node cur, int dept, pinfo p) {
	if (!cur.patient) {
		// node child = { &p, dept, RED, cur.parent, NULL, NULL };
		cur.patient = &p;
		cout << dept << " " << 1 << "\n";
		return;
	}

	// ������ ��: ���� �ڽ� ��忡 �߰��ϴ� ����� �ƴ�
	// ���� ����� pinfo�� ���̸� ���⿡ �����ϴ� ������ �����Ұ�!!!!!

	if (p.pid == cur.patient->pid) {
		// the patient is already exist
		cout << cur.parent->dept << " " << 0 << "\n";
		return;
	}
	else if (p.pid < cur.patient->pid) {
		// Ʈ���� ���� ����Ʈ���� ����
		if (cur.left) {
			insertPatient(*cur.left, dept + 1, p);
		}
		else {
			node child = {&p, dept + 1, RED, &cur, NULL, NULL };
			cur.left = &child;
			cout << dept + 1 << " " << 1 << "\n";
		}
	}
	else {
		// Ʈ���� ���� ����Ʈ���� ����
		cur = *cur.right;
	}

}