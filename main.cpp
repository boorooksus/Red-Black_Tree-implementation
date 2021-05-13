#include <iostream>
#define BLACK 0
#define RED 1
using namespace std;

// 진료 기록
struct record {
	string disease;
	int cost = 0;
	record* next = nullptr;
};

// 환자 정보
struct pinfo {
	int pid = 0;
	string name;
	int tel = 0;
	int ax = 0;
	int ay = 0;
	record* records = nullptr;
};

// 트리 노드
struct treeNode {
	pinfo* patient = nullptr;
	int dept = 0;
	int color = BLACK;
	treeNode* parent= nullptr;
	treeNode* left = nullptr;
	treeNode* right = nullptr;
};

int T;
char cmd;
treeNode root = { nullptr, 0, BLACK, nullptr, nullptr, nullptr };

void insertPatient(pinfo *p);
void doubleRed(treeNode* node);
//treeNode findPatient(int pid);

int main() {
	cin >> T;
	while (T--) {
		cin >> cmd;
		if (cmd == 'I') {
			auto *p = new pinfo;
			auto* r = new record;
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

	auto* left = new treeNode;
	*left = { nullptr, curDept + 1, BLACK, cur, nullptr, nullptr };
	auto* right = new treeNode;
	*right = { nullptr, curDept + 1, BLACK, cur, nullptr, nullptr };

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

//		treeNode *x, *y, *z;
//		treeNode* greatGrand = grand->parent;
//
//		if (grand->patient->pid < parent->patient->pid && parent->patient->pid > node->patient->pid) {
//			// right-left case
//
//			grand->parent = node;
//			grand->right = node->left;
//			parent->parent = node;
//			parent->left = node->right;
//			node->left = grand;
//			node->right = parent;
//			node->parent = greatGrand;
//		}
//		else if (grand->patient->pid > parent->patient->pid && parent->patient->pid < node->patient->pid) {
//			// left-right case
//
//			grand->parent = node;
//			grand->left = node->right;
//			parent->parent = node;
//			parent->right = node->left;
//			node->parent = greatGrand;
//			node->left = parent;
//			node->right = grand;
//		}
//		else if (grand->patient->pid > parent->patient->pid && parent->patient->pid > node->patient->pid) {
//			// left-left case
//
//			grand->parent = parent;
//			grand->left = parent->right;
//			parent->parent = greatGrand;
//			parent->left = node->right;
//
//		}
//		else {
//			x = grand;
//			y = parent;
//			z = node;
//		}


	}
	else {
		// Recoloring
		cout << "recoloring\n";
	}

	}
