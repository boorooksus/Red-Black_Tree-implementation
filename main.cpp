#include <iostream>
#define BLACK 0
#define RED 1
using namespace std;

// 진료 기록
struct record {
	string disease;
	int cost = 0;
	record* next = NULL;
};

// 환자 정보
struct pinfo {
	int pid = 0;
	string name = "";
	int tel = 0;
	int ax = 0;
	int ay = 0;
	record* records = NULL;
};

// 트리 노드
struct treeNode {
	pinfo* patient = NULL;
	int dept = 0;
	int color = BLACK;
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
			// 트리의 좌측 서브트리로 보냄
			cur = cur->left;
		}
		else {
			// 트리의 우측 서브트리로 보냄
			cur = cur->right;
		}
		curDept++;
	}

	cur->patient = p;
	// 루트 노드인 경우 black, 아닌 경우 red
	cur->color = (cur->parent) ? RED : BLACK;

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

	// double red가 아닌 경우 리턴
	if (!node->parent || node->parent->color == BLACK) {
		return;
	}

	treeNode* grand = node->parent->parent; // grand 노드
	// uncle 노드
	treeNode* uncle = (node->parent != grand->left) ?
		grand->left : grand->right;
	treeNode* parent = node->parent;

	// uncle 노드의 색에 따라 restructuring 또는 recoloring
	if (uncle->color == BLACK) {
		// Restructuring
		
		treeNode *x, *y, *z;

		if (grand->patient->pid < parent->patient->pid && parent->patient->pid > node->patient->pid) {
			x = grand;
			y = node;
			z = parent;
		}
		else if (grand->patient->pid > parent->patient->pid && parent->patient->pid < node->patient->pid) {
			x = parent;
			y = node;
			z = grand;
		}
		else if (grand->patient->pid > parent->patient->pid && parent->patient->pid > node->patient->pid) {
			x = node;
			y = parent;
			z = grand;
		}
		else {
			x = grand;
			y = parent;
			z = node;
		}


	}
	else {
		// Recoloring
		cout << "recoloring\n";
	}

	return;
}