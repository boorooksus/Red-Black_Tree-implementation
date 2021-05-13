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
treeNode rootNode = { nullptr, 0, BLACK, nullptr, nullptr, nullptr };
treeNode *root = &rootNode;

void insertPatient(pinfo *p);
void doubleRed(treeNode* node);
void updateDepts(treeNode* node, int curDept);
treeNode *findPatient(int pid);
void addRecord(int pid, record * treat);

int main() {
	cin >> T;
	while (T--) {
		cin >> cmd;
		if (cmd == 'I') {
		    // 신규 가입

			auto *p = new pinfo;
			auto* r = new record;
			cin >> p->pid >> p->name >> p->tel >> p->ax >> p->ay
				>> r->disease >> r->cost;

			p->records = r;
			insertPatient(p);
		}
		else if (cmd == 'F'){
		    // 환자 검색

		    int k;
		    cin >> k;

		    treeNode *cur = findPatient(k);

		    if(cur){
                cout << cur->dept << " " << cur->patient->name << " "
                     << cur->patient->tel << " " << cur->patient->ax << " "
                     << cur->patient->ay << "\n";
		    }
		    else{
                cout << "Not found\n";
		    }

		}
		else if (cmd == 'A'){
		    // 추가 진료
		    string disease;
		    int k, cost;
            cin >> k >> disease >> cost;
            auto *treat = new record;
            *treat = {disease, cost, nullptr};

            addRecord(k, treat);
		}
		else{
		    cout << "Wrong Command\n";
		}
	}

}

// 신규 가입 함수
void insertPatient(pinfo *p) {

	treeNode *cur = root;
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

	cout << cur->dept << " " << 1 << "\n";
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

		treeNode* greatGrand = grand->parent;

		if (grand->patient->pid < parent->patient->pid && parent->patient->pid > node->patient->pid) {
			// case1: right-left

			grand->parent = node;
			grand->right = node->left;
			grand->right->parent = grand;
			grand->color = RED;

			parent->parent = node;
			parent->left = node->right;
			parent->left->parent = parent;

			node->left = grand;
			node->left->parent = node;
			node->right = parent;
			node->right->parent = node;
			node->parent = greatGrand;
			if (greatGrand) {
                if(greatGrand->left == grand)
                    greatGrand->left = node;
                else
                    greatGrand->right = node;
            }
            node->color = BLACK;

			if (grand->dept == 0)
			    root = node;

		}
		else if (grand->patient->pid > parent->patient->pid && parent->patient->pid < node->patient->pid) {
			// case1: left-right

			grand->parent = node;
			grand->left = node->right;
			grand->left->parent = grand;
			grand->color = RED;

			parent->parent = node;
			parent->right = node->left;
			parent->right->parent = parent;

			node->parent = greatGrand;
            if (greatGrand) {
                if(greatGrand->left == grand)
                    greatGrand->left = node;
                else
                    greatGrand->right = node;
            }
			node->left = parent;
			node->left->parent = node;
			node->right = grand;
			node->right->parent = node;
			node->color = BLACK;

            if (grand->dept == 0)
                root = node;
		}
		else if (grand->patient->pid > parent->patient->pid && parent->patient->pid > node->patient->pid) {
			// case3: left-left

			grand->parent = parent;
			grand->left = parent->right;
			grand->left->parent = grand;
			grand->color = RED;

			parent->parent = greatGrand;
            if (greatGrand) {
                if(greatGrand->left == grand)
                    greatGrand->left = parent;
                else
                    greatGrand->right = parent;
            }
			parent->left = node->right;
			parent->left->parent = parent;
			parent->color = BLACK;

            if (grand->dept == 0)
                root = parent;

		}
		else {
            // case4: right-right

            grand->parent = parent;
            grand->right = parent->left;
            grand->right->parent = grand;
            grand->color = RED;

            parent->parent = greatGrand;
            if (greatGrand) {
                if(greatGrand->left == grand)
                    greatGrand->left = parent;
                else
                    greatGrand->right = parent;
            }
            parent->left = grand;
            parent->left->parent = parent;
            parent->color = RED;

            if (grand->dept == 0)
                root = parent;
        }

        updateDepts(root, 0);
	}
	else {
		// Recoloring

		grand->color = RED;
		uncle->color = BLACK;
		parent->color = BLACK;

        // grand node가 root인 경우 색상을 다시 BLACK으로
        // 아닌 경우 double red인지 확인
		if (!grand->parent)
		    grand->color = BLACK;
		else
		    doubleRed(grand);
	}

}

// 트리에서 노드들의 깊이를 업데이트하는 함수
void updateDepts(treeNode* node, int curDept){
    if(!node)
        return;

    node->dept = curDept;
    updateDepts(node->left, curDept + 1);
    updateDepts(node->right, curDept + 1);
}

// 환자 검색 함수
treeNode *findPatient(int pid){
    treeNode *cur = root;

    // 환자 정보를 찾거나 리프 노드에 도달할 때까지 탐색
    while(cur->patient){
        if (pid < cur->patient->pid){
            // 환자 id가 현재 노드의 id 보다 작을 때
            cur = cur->left;
        }
        else if (pid > cur->patient->pid){
            // 환자 id가 현재 노드의 id 보다 클 때
            cur = cur->right;
        }
        else{
            // 환자 정보를 찾았을 때
            return cur;
        }
    }

    return nullptr;
}

// 추가 진료 함수
void addRecord(int pid, record * treat){

    treeNode *node = findPatient(pid);

    if(node){
        treat->next = node->patient->records;
        node->patient->records = treat;

        cout << node->dept << "\n";
    }
    else{
        cout << "Not found\n";
    }
}