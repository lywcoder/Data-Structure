#include <iostream>
using namespace std;

struct Node {
    int val;
    int leftSize; // 左子树节点总数
    Node* left;
    Node* right;

    Node(int v) : val(v), leftSize(0), left(nullptr), right(nullptr) {}
};

class IndexedBST {
private:
    Node* root;
    int treeSize;

    // 判断元素 x 是否存在
    bool contains(int x) const {
        Node* curr = root;
        while (curr != nullptr) {
            if (x == curr->val) return true;
            curr = (x < curr->val) ? curr->left : curr->right;
        }
        return false;
        //搜索判断是否存在，避免发现不存在之后扰乱leftsize
    }

    // 删除指定节点 curr（parent 为其父节点）
    void removeNode(Node* curr, Node* parent) {
        // 情况 1：有两个孩子 -> 用右子树最小节点替换值
        if (curr->left != nullptr && curr->right != nullptr) {
            Node* succParent = curr;//找到中序后继-右子树的最小值，这个指向后继的父节点
            Node* succ = curr->right;//指向的是后继节点
            
            while (succ->left != nullptr) {
                succ->leftSize--; // 即将从 succ 的左子树中抽走一个节点
                succParent = succ;
                succ = succ->left;
            } //这段循环可以找到后继节点和后继节点的父节点
            
            curr->val = succ->val; // 替换数据域，curr 的 leftSize 无需改变
            curr = succ;           // ！！！转为删除最多只有右孩子的 succ
            parent = succParent;   //！！！同理parent节点转为只有右孩子节点的succ的parent节点
        }
        //现在已经将两个孩子的节点转化为删除一个孩子的节点

        // 情况 2：有 0 个或 1 个孩子 -> 直接用孩子接替位置
        Node* child = (curr->left != nullptr) ? curr->left : curr->right;
        
        //如果要删除的只有一个孩子 = 左孩子，直接确定就可以，如果要删除节点的没有左孩子，只有一个右孩子，child = 右
        if (parent == nullptr) {
            root = child;
        } else if (parent->left == curr) {
            parent->left = child; //将父节点的指针重新指向为孩子节点进行定位
        } else {
            parent->right = child;
        }
        
        delete curr; //删除对应的节点
        treeSize--; //总结点--
    }

public:
    IndexedBST() : root(nullptr), treeSize(0) {}

    // 操作 0：插入 x
    int insert(int x) {
        if (contains(x)) return 0;

        Node* newNode = new Node(x);
        if (root == nullptr) {
            root = newNode;
            treeSize++;
            return 0;
        }

        int xorSum = 0;
        Node* curr = root;
        Node* parent = nullptr;
        
        while (curr != nullptr) {
            xorSum ^= curr->val;
            parent = curr;
            if (x < curr->val) {
                curr->leftSize++;
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }

        if (x < parent->val) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        
        treeSize++;
        return xorSum;
    }

    // 操作 1：查找 x
    int search(int x) const {
        int xorSum = 0;
        Node* curr = root;
        
        while (curr != nullptr) {
            xorSum ^= curr->val;
            if (x == curr->val) return xorSum;
            curr = (x < curr->val) ? curr->left : curr->right;
        }
        return 0;
    }

    // 操作 2：删除 x
    int erase(int x) {
        if (!contains(x)) return 0;

        int xorSum = 0;
        Node* curr = root;
        Node* parent = nullptr;
        while (curr != nullptr) {
            xorSum ^= curr->val;
            if (x == curr->val) break;
            
            parent = curr;
            if (x < curr->val) {
                curr->leftSize--;
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        
        removeNode(curr, parent);
        return xorSum;
    }

    // 操作 3：按名次 k 查找
    int searchByRank(int k) const {
        if (k < 1 || k > treeSize) return 0;

        int xorSum = 0;
        Node* curr = root;
        
        while (curr != nullptr) {
            xorSum ^= curr->val;
            int rank = curr->leftSize + 1;
            
            if (k == rank) {
                return xorSum;
            } else if (k < rank) {
                curr = curr->left;
            } else {
                k -= rank; //如果在右子树，那么减去rank，就是查找节点在右子树的索引排名
                curr = curr->right;
            }
        }
        return 0;
    }

    // 操作 4：按名次 k 删除
    int eraseByRank(int k) {
        if (k < 1 || k > treeSize) return 0;

        int xorSum = 0;
        Node* curr = root;
        Node* parent = nullptr;
        
        while (curr != nullptr) {
            xorSum ^= curr->val;
            int rank = curr->leftSize + 1;
            if (k == rank) {
                break;
            } else if (k < rank) {
                curr->leftSize--;
                parent = curr;
                curr = curr->left;
            } else {
                k -= rank;
                parent = curr;
                curr = curr->right;
            }
        }
        
        removeNode(curr, parent);
        return xorSum;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    IndexedBST bst;
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        if (a == 0)      cout << bst.insert(b) << '\n';
        else if (a == 1) cout << bst.search(b) << '\n';
        else if (a == 2) cout << bst.erase(b) << '\n';
        else if (a == 3) cout << bst.searchByRank(b) << '\n';
        else if (a == 4) cout << bst.eraseByRank(b) << '\n';
    }
    return 0;
}