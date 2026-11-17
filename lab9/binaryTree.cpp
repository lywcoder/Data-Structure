#include <iostream>
using namespace std;

template <typename T>
class Stack {
private:
    T* data;
    int topIdx;
    int capacity;

public:
    Stack(int cap = 100000) : topIdx(-1), capacity(cap) {
        data = new T[capacity];
    }
    ~Stack() {
        delete[] data;
    }
    bool empty() const {
        return topIdx == -1;
    }
    void push(const T& val) {
        data[++topIdx] = val;
    }
    void pop() {
        if (topIdx >= 0) --topIdx;
    }
    T top() const {
        return data[topIdx];
    }
};

// 手写泛型顺序队列（替代 std::queue）
template <typename T>
class Queue {
private:
    T* data;
    int head;
    int tail;
    int capacity;

public:
    Queue(int cap = 100000) : head(0), tail(0), capacity(cap) {
        data = new T[capacity];
    }
    ~Queue() {
        delete[] data;
    }
    bool empty() const {
        return head == tail;
    }
    void push(const T& val) {
        data[tail++] = val;
    }
    void pop() {
        if (head < tail) ++head;
    }
    T front() const {
        return data[head];
    }
};

// 二叉树链表节点
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    int subSize;    // 子树节点数目
    int subHeight;  // 子树高度

    TreeNode() : val(0), left(nullptr), right(nullptr), subSize(0), subHeight(0) {}
};

// 二叉树类
class BinaryTree {
private:
    int n;

public:
    TreeNode* root;
    TreeNode* nodes;

    BinaryTree(int nodeCount) : n(nodeCount), root(nullptr) {
        nodes = new TreeNode[n + 1];
        for (int i = 1; i <= n; ++i) {
            nodes[i].val = i;
        }
    }

    ~BinaryTree() {
        delete[] nodes;
    }

    // 读取输入构建链式二叉树
    void build() {
        for (int i = 1; i <= n; ++i) {
            int l, r;
            cin >> l >> r;
            if (l != -1) nodes[i].left = &nodes[l];
            if (r != -1) nodes[i].right = &nodes[r];
        }
        if (n >= 1) {
            root = &nodes[1];
        }
    }

    // 1. 递归前序遍历（直接单函数递归）
    void preOrder(TreeNode* node) {
        if (node == nullptr) return;
        cout << node->val << " ";
        preOrder(node->left);
        preOrder(node->right);
    }

    // 2. 非递归中序遍历
    void inOrder() {
        Stack<TreeNode*> st(n);
        TreeNode* curr = root;

        while (curr != nullptr || !st.empty()) {
            while (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top();
            st.pop();
            cout << curr->val << " ";
            curr = curr->right;
        }
        cout << "\n";
    }

    // 3. 非递归后序遍历
    void postOrder() {
        Stack<TreeNode*> st(n);
        TreeNode* curr = root;
        TreeNode* lastVisited = nullptr;

        while (curr != nullptr || !st.empty()) {
            if (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            } else {
                TreeNode* peekNode = st.top();
                if (peekNode->right != nullptr && lastVisited != peekNode->right) {
                    curr = peekNode->right;
                } else {
                    cout << peekNode->val << " ";
                    lastVisited = peekNode;
                    st.pop();
                }
            }
        }
        cout << "\n";
    }

    // 4. 层次遍历
    void levelOrder() {
        Queue<TreeNode*> q(n);
        q.push(root);

        while (!q.empty()) {
            TreeNode* curr = q.front();
            q.pop();
            cout << curr->val << " ";

            if (curr->left != nullptr) q.push(curr->left);
            if (curr->right != nullptr) q.push(curr->right);
        }
        cout << "\n";
    }

    // 5. 计算子树节点数目（直接单函数递归）
    int calcSize(TreeNode* node) {
        if (node == nullptr) return 0;
        node->subSize = 1 + calcSize(node->left) + calcSize(node->right);
        return node->subSize;
    }

    // 6. 计算子树高度（直接单函数递归）
    int calcHeight(TreeNode* node) {
        if (node == nullptr) return 0;
        int leftH = calcHeight(node->left);
        int rightH = calcHeight(node->right);
        node->subHeight = 1 + (leftH > rightH ? leftH : rightH);
        return node->subHeight;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    std::cin >> n;
    BinaryTree tree(n);
    tree.build();

    tree.preOrder(tree.root);
    cout << "\n";

    tree.inOrder();
    tree.postOrder();
    tree.levelOrder();

    tree.calcSize(tree.root);
    for (int i = 1; i <= n; ++i) {
        cout << tree.nodes[i].subSize << " ";
    }
    cout << "\n";

    tree.calcHeight(tree.root);
    for (int i = 1; i <= n; ++i) {
        cout << tree.nodes[i].subHeight << " ";
    }
    cout << "\n";
    return 0;
}
