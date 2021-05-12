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

struct treeNode {
	pinfo* patient = NULL;
	int dept;
	int color;
	treeNode* parent;
	treeNode* left = NULL;
	treeNode* right = NULL;
};

int T;
char cmd;
treeNode root = { NULL, 0, BLACK, NULL, NULL, NULL };

void insertPatient(pinfo *p);
void doubleRed(treeNode* node);
//treeNode findPatient(int pid);

int main() {
	cin >> T;
	while (T--) {
		cin >> cmd;
		if (cmd == 'I') {
			pinfo *p = new pinfo;
			record* r = new record;
			cin >> p->pid >> p->name >> p->tel >> p->ax >> p->ay
				>> r->disease >> r->cost;

			p->records = r;
			insertPatient(p);
		}
	}

}

void insertPatient(pinfo *p) {

	treeNode *cur = &root;
	int curDept = 0;

	while (cur->patient) {
		if (p->pid == cur->patient->pid) {
			// when the patient is already exist
			cout << cur->dept << " " << 0 << "\n";
			return;
		}
		else if (p->pid < cur->patient->pid) {
			// Ʈ���� ���� ����Ʈ���� ����
			cur = cur->left;
		}
		else {
			// Ʈ���� ���� ����Ʈ���� ����
			cur = cur->right;
		}
		curDept++;
	}

	cur->patient = p;
	if (cur->parent) {
		// ��Ʈ ��尡 �ƴ� ���
		cur->color = RED;
	}
	else {
		// ��Ʈ ����� ���
		cur->color = BLACK;
	}

	treeNode* left = new treeNode;
	*left = { NULL, curDept + 1, BLACK, cur, NULL, NULL };
	treeNode* right = new treeNode;
	*right = { NULL, curDept + 1, BLACK, cur, NULL, NULL };

	cur->left = left;
	cur->right = right;

	doubleRed(cur);

	cout << curDept << " " << 1 << "\n";
}

// double red handling function
void doubleRed(treeNode * node) {

	// double red�� �ƴ� ��� ����
	if (!node->parent || node->parent->color == BLACK) {
		return;
	}

	treeNode* grand = node->parent->parent; // grand ���
	// uncle ���
	treeNode* uncle = (node->parent != grand->left) ?
		grand->left : grand->right;

	// uncle ����� ���� ���� restructuring �Ǵ� recoloring
	if (uncle->color == BLACK) {
		// Restructuring
		cout << "restruct\n";
	}
	else {
		// Recoloring
		cout << "recoloring\n";
	}

	return;
}