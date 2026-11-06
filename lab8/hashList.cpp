#include <iostream>

using namespace std;

// 自定义单链表节点
struct Node {
    int val;
    Node* next;
    Node(int v, Node* n = nullptr) : val(v), next(n) {}
};

int main() {
    // 优化 I/O 读写性能
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int D, m;
    std::cin >> D >> m;
    
    // 创建散列表槽位指针数组
    Node** head = new Node*[D];
    for (int i = 0; i < D; ++i) {
        head[i] = nullptr;
    }

    while (m--) {
        int opt, x;
        cin >> opt >> x;

        // 计算防负数的哈希下标
        int idx = (x % D + D) % D;

        if (opt == 0) { // 插入 x
            bool existed = false;
            Node* curr = head[idx];
            while (curr != nullptr) {
                if (curr->val == x) {
                    existed = true;
                    break;
                }
                curr = curr->next;
            }

            if (existed) {
                cout << "Existed\n";
            } else {
                // 使用头插法插入新节点
                head[idx] = new Node(x, head[idx]);
            }
        } 
        else if (opt == 1) { // 查询 x
            bool found = false;
            int length = 0;
            Node* curr = head[idx];
            
            while (curr != nullptr) {
                if (curr->val == x) {
                    found = true;
                }
                length++;
                curr = curr->next;
            }

            if (!found) {
                cout << "Not Found\n";
            } else {
                cout << length << "\n";
            }
        } 
        else if (opt == 2) { // 删除 x
            Node* curr = head[idx];
            Node* prev = nullptr;
            bool found = false;

            // 查找并删除节点
            while (curr != nullptr) {
                if (curr->val == x) {
                    found = true;
                    if (prev == nullptr) {
                        head[idx] = curr->next;
                    } else {
                        prev->next = curr->next;
                    }
                    delete curr;
                    break;
                }
                prev = curr;
                curr = curr->next;
            }

            if (!found) {
                cout << "Delete Failed\n";
            } else {
                // 统计删除后的链表长度
                int length = 0;
                Node* p = head[idx];
                while (p != nullptr) {
                    length++;
                    p = p->next;
                }
                cout << length << "\n";
            }
        }
    }

    // 释放动态分配的内存
    for (int i = 0; i < D; ++i) {
        Node* curr = head[i];
        while (curr != nullptr) {
            Node* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
    }
    delete[] head;

    return 0;
}