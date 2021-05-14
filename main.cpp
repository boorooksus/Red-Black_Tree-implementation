#include <iostream>
#define BLACK 0
#define RED 1
using namespace std;

// 진료 기록
struct record {
	string disease;  // 병명
	int cost = 0;  // 진료비
	record* next = nullptr;  // 이전 진료 기록 포인터
};

// 환자 정보
struct pinfo {
	int pid = 0;  // 환자 번호
	string name;  // 환자 이름
	int tel = 0;  // 연락처
	int ax = 0;  // 주소 x좌표
	int ay = 0;  // 주소 y좌표
	record* records = nullptr;  // 진료 기록 연결 리스트 주소
};

// 트리 노드
struct treeNode {
	pinfo* patient = nullptr;  // 환자 정보 포인터
	int dept = 0;  // 노드 깊이
	int color = BLACK;  // 노드 컬러(블랙/레드)
	treeNode* parent= nullptr;  // 부모 노드 주소
	treeNode* left = nullptr;  // left child 주소
	treeNode* right = nullptr;  // right child 주소
};

int T;  // 질의 수
char cmd;  // 질의 구분 문자
// 루트 노드
treeNode rootNode = { nullptr, 0, BLACK, nullptr, nullptr, nullptr };
treeNode *root = &rootNode;

void insertPatient(pinfo *p);
void doubleRed(treeNode* node);
void updateDept(treeNode* node, int curDept);
treeNode *findPatient(int pid);
void addRecord(int pid, record * treat);
int findDisease(treeNode* node, const string& disease);
void deleteTree(treeNode* node);
int check(treeNode* node);

int main() {
	cin >> T;
	while (T--) {
	    // 질의 입력
		cin >> cmd;

		// 질의 구분 문자에 따라 처리
		if (cmd == 'I') {
		    // 신규 가입

			auto *p = new pinfo;  // 환자 정보
			auto* r = new record;  // 진료 기록

			cin >> p->pid >> p->name >> p->tel >> p->ax >> p->ay
				>> r->disease >> r->cost;

			p->records = r;

			// 트리에 환자 정보 추가
			insertPatient(p);

			// RBTree 체크(디버깅용)
            // check(root);
		}
		else if (cmd == 'F'){
		    // 환자 검색

		    int k;  // 환자 번호
		    cin >> k;

		    // 트리에서 환자 검색
		    treeNode *cur = findPatient(k);

		    if(cur){
		        // 환자가 존재할 경우
                cout << cur->dept << " " << cur->patient->name << " "
                     << cur->patient->tel << " " << cur->patient->ax << " "
                     << cur->patient->ay << "\n";
		    }
		    else{
		        // 환자가 존재하지 않는 경우
                cout << "Not found\n";
		    }

		}
		else if (cmd == 'A'){
		    // 추가 진료

		    string disease;  // 병명
		    int k, cost;  // 환자 번호, 진료비
            cin >> k >> disease >> cost;

            // 진료 기록 리스트 노드
            auto *treat = new record;
            *treat = {disease, cost, nullptr};

            // 진료 기록 추가
            addRecord(k, treat);
		}
		else if (cmd == 'E'){
		    // 유행병 조사

		    string disease;  // 병명
		    cin >> disease;

		    // 유행병 가지고 있는 환자 수 조사
		    cout << findDisease(root, disease) << "\n";
		}
		else{
		    // 질의가 잘못된 경우

		    cout << "Wrong Command\n";
		}
	}

	// 할당된 메모리 제거
    // deleteTree(root);
}

// 신규 가입 함수
void insertPatient(pinfo *p) {

	treeNode *cur = root;  // 현재 탐색 노드
	int curDept = 0;  // 현재 깊이

	// 빈 노드가 나올때까지 탐색
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

	// 현재 노드에 환자 정보 추가
	cur->patient = p;
	// 루트 노드인 경우 black, 아닌 경우 red
	cur->color = (cur->parent) ? RED : BLACK;

	// 새로운 리프 노드 생성 후 연결
	auto* left = new treeNode;
	*left = { nullptr, curDept + 1, BLACK, cur, nullptr, nullptr };
	auto* right = new treeNode;
	*right = { nullptr, curDept + 1, BLACK, cur, nullptr, nullptr };

	cur->left = left;
	cur->right = right;

	// 더블 레드 확인
	doubleRed(cur);

	// 결과 출력
	cout << cur->dept << " " << 1 << "\n";
}

// double red handling function
void doubleRed(treeNode * node) {

	// double red가 아닌 경우 리턴
	if (node->color == BLACK || !node->parent || node->parent->color == BLACK) {
		return;
	}

	treeNode* grand = node->parent->parent; // grand 노드
	treeNode* parent = node->parent;  // parent 노드

	// uncle 노드의 색에 따라 restructuring 또는 recoloring
	if (grand->left->color != grand->right->color) {
		// Restructuring

		treeNode* greatGrand = grand->parent;  // parent node of grand node

		// gand 노드의 자식과 손자 노드 위치에 따라 case 4가지로 나누어 처리
		if (grand->patient->pid < parent->patient->pid && parent->patient->pid > node->patient->pid) {
			// case1: right-left case

            // grand node를 현재 서브트리 루트의 왼쪽 자식으로 설정
			grand->right = node->left;
			grand->right->parent = grand;
			grand->color = RED;

			// parent node를 현재 서브트리 루트의 오른쪽 자식으로 설정
			parent->left = node->right;
			parent->left->parent = parent;

			// 추가된 노드를 현재 서브트리의 루트로 설정
			node->left = grand;
			node->left->parent = node;
			node->right = parent;
			node->right->parent = node;
			node->parent = greatGrand;
			if (greatGrand) {
                // great grand node의 자식 노드 주소 재설정
                if(greatGrand->left == grand)
                    // great grand node의 좌측 자식인 경우
                    greatGrand->left = node;
                else
                    // great grand node의 우측 자식인 경우
                    greatGrand->right = node;
            }
            node->color = BLACK;

			// 전체 트리의 루트 노드가 바뀐 경우 전역 변수 재설정
			if (grand->dept == 0)
			    root = node;

            // 각 노드의 dept 재설정
            updateDept(node, grand->dept);

		}
		else if (grand->patient->pid > parent->patient->pid && parent->patient->pid < node->patient->pid) {
			// case2: left-right case

            // grand node를 현재 서브트리 루트의 우측 자식으로 설정
			grand->left = node->right;
			grand->left->parent = grand;
			grand->color = RED;

            // parent node를 현재 서브트리 루트의 왼쪽 자식으로 설정
			parent->right = node->left;
			parent->right->parent = parent;

            // 추가된 node를 현재 서브트리 루트로 설정
            node->parent = greatGrand;
            if (greatGrand) {
                // great grand node의 자식 노드 주소 재설정
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

			// 전체 트리의 루트가 바뀐 경우 전역 변수 재설정
            if (grand->dept == 0)
                root = node;

            // 각 노드의 dept 재설정
            updateDept(node, grand->dept);
		}
		else if (grand->patient->pid > parent->patient->pid && parent->patient->pid > node->patient->pid) {
			// case3: left-left case

            // grand node를 현재 서브트리 루트의 우측 자식으로 설정
			grand->left = parent->right;
			grand->left->parent = grand;
			grand->color = RED;

            // parent node를 현재 서브트리 루트로 설정
            parent->parent = greatGrand;
            if (greatGrand) {
                // great grand node의 자식 노드 주소 재설정
                if(greatGrand->left == grand)
                    greatGrand->left = parent;
                else
                    greatGrand->right = parent;
            }
			parent->right = grand;
			parent->right->parent = parent;
			parent->color = BLACK;

            if (grand->dept == 0)
                root = parent;

            // 각 노드의 dept 재설정
            updateDept(parent, grand->dept);

		}
		else {
            // case4: right-right case

            // grand node를 현재 서브트리 루트의 좌측 자식으로 설정
            grand->right = parent->left;
            grand->right->parent = grand;
            grand->color = RED;

            // parent node를 현재 서브트리 루트로 설정
            parent->parent = greatGrand;
            if (greatGrand) {
                // great grand 노드의 자식 주소로 설정
                if(greatGrand->left == grand)
                    greatGrand->left = parent;
                else
                    greatGrand->right = parent;
            }
            parent->left = grand;
            parent->left->parent = parent;
            parent->color = BLACK;

            if (grand->dept == 0)
                // 전체 트리의 루트가 바뀐경우 전역 변수 재설정
                root = parent;

            // 각 노드의 dept 재설정
            updateDept(parent, grand->dept);
        }

	}
	else {
		// Recoloring

		grand->color = RED;
		grand->left->color = BLACK;
        grand->right->color = BLACK;

        // grand node가 root인 경우 색상을 다시 BLACK으로
        // 아닌 경우 double red인지 확인
		if (!grand->parent)
		    grand->color = BLACK;
		else
		    doubleRed(grand);
	}

}

// 트리에서 노드들의 깊이를 업데이트하는 함수
void updateDept(treeNode* node, int curDept){
    if(!node)
        return;

    node->dept = curDept;  // 현재 노드의 깊이 업데이트
    // 서브 트리의 깊이 업데이트
    updateDept(node->left, curDept + 1);
    updateDept(node->right, curDept + 1);
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

    // 환자 정보가 없을 때
    return nullptr;
}

// 추가 진료 함수
void addRecord(int pid, record * treat){

    // 환자 정보가 들어있는 노드 탐색
    treeNode *node = findPatient(pid);

    if(node){
        // 트리에 환자 정보가 있는 경우
        treat->next = node->patient->records;
        node->patient->records = treat;

        cout << node->dept << "\n";
    }
    else{
        // 환자 정보가 존재하지 않는 경우
        cout << "Not found\n";
    }
}

// 유행병 조사 함수
int findDisease(treeNode* node, const string& disease){

    if(!node->patient)
        // leaf node
        return 0;

    // current node count + left subTree count + right subTree count
    int cnt = (node->patient->records->disease == disease) ? 1 : 0;
    // get count of subTree by recursion
    int left = findDisease(node->left, disease);
    int right = findDisease(node->right, disease);

    return cnt + left + right;
}

// 트리 제거 함수
void deleteTree(treeNode* node){
    // 각 노드와 환자 정보, 진료 기록에 할당된 동적 메모리 삭제
    if(!node)
        return;

    if(node->patient){
        record* next = node->patient->records;
        while(next){
            record* prev = next;
            next = next->next;
            delete prev;
        }

        delete node->patient;
    }

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}

// RBTree check function(for debugging)
int check(treeNode* node){
    if(!node->patient)
        return 0;

    if(node->color == RED){
        if(node->left->color == RED)
            cout << node->left->dept << " Double RED!!======================\n";
        if(node->right->color == RED)
            cout << node->right->dept << "Double RED!!======================\n";
    }

    int left = check(node->left);
    int right = check(node->right);

    if (left - right > 1 || left - right < -1){
        cout << "Not Balanced!!!!!!================\n";
    }

    if (left < right)
        left = right;

    return left + 1;
}