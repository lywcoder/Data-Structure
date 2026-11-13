#include <cstdio>

// 手写模板栈（替代 std::stack）
template <typename T>
class Stack {
private:
    T* data;
    int topIdx;
    int capacity;

public:
    Stack(int cap = 100005) : topIdx(-1), capacity(cap) {
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

// 手写模板队列（替代 std::queue）
template <typename T>
class Queue {
private:
    T* data;
    int head;
    int tail;
    int capacity;

public:
    Queue(int cap = 100005) : head(0), tail(0), capacity(cap) {
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

// 二叉树链表节点定义
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    int subSize;    // 以当前节点为根的子树节点数目
    int subHeight;  // 以当前节点为根的子树高度

    TreeNode() : val(0), left(nullptr), right(nullptr), subSize(0), subHeight(0) {}
};

// 二叉树类
class BinaryTree {
private:
    int n;
    TreeNode* root;
    TreeNode* nodes;  // 节点内存池，内部完全通过 left / right 指针构成链式结构

    // 递归前序遍历辅助函数
    void preOrderRecursive(TreeNode* node, bool& first) const {
        if (node == nullptr) return;
        if (!first) printf(" ");
        printf("%d", node->val);
        first = false;
        preOrderRecursive(node->left, first);
        preOrderRecursive(node->right, first);
    }

    // 一次后序 DFS 计算所有子树的节点数目
    int calcSizeDFS(TreeNode* node) {
        if (node == nullptr) return 0;
        node->subSize = 1 + calcSizeDFS(node->left) + calcSizeDFS(node->right);
        return node->subSize;
    }

    // 一次后序 DFS 计算所有子树的高度
    int calcHeightDFS(TreeNode* node) {
        if (node == nullptr) return 0;
        int leftH = calcHeightDFS(node->left);
        int rightH = calcHeightDFS(node->right);
        node->subHeight = 1 + (leftH > rightH ? leftH : rightH);
        return node->subHeight;
    }

public:
    BinaryTree(int nodeCount) : n(nodeCount), root(nullptr) {
        nodes = new TreeNode[n + 1];
        for (int i = 1; i <= n; ++i) {
            nodes[i].val = i;
        }
    }

    ~BinaryTree() {
        delete[] nodes;
    }

    // 读取输入并构建链式二叉树
    void build() {
        for (int i = 1; i <= n; ++i) {
            int l, r;
            scanf("%d %d", &l, &r);
            if (l != -1) nodes[i].left = &nodes[l];
            if (r != -1) nodes[i].right = &nodes[r];
        }
        if (n >= 1) {
            root = &nodes[1];
        }
    }

    // 1. 递归前序遍历
    void preOrder() const {
        bool first = true;
        preOrderRecursive(root, first);
        printf("\n");
    }

    // 2. 非递归中序遍历（左 -> 根 -> 右）
    void inOrder() const {
        Stack<TreeNode*> st(n + 5);
        TreeNode* curr = root;
        bool first = true;

        while (curr != nullptr || !st.empty()) {
            while (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            }
            curr = st.top();
            st.pop();

            if (!first) printf(" ");
            printf("%d", curr->val);
            first = false;

            curr = curr->right;
        }
        printf("\n");
    }

    // 3. 非递归后序遍历（左 -> 右 -> 根，单栈 + lastVisited 指针法）
    void postOrder() const {
        Stack<TreeNode*> st(n + 5);
        TreeNode* curr = root;
        TreeNode* lastVisited = nullptr;
        bool first = true;

        while (curr != nullptr || !st.empty()) {
            if (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            } else {
                TreeNode* peekNode = st.top();
                // 若右子树存在且尚未被访问，则转向右子树
                if (peekNode->right != nullptr && lastVisited != peekNode->right) {
                    curr = peekNode->right;
                } else {
                    // 左右子树均处理完毕，访问当前根节点
                    if (!first) printf(" ");
                    printf("%d", peekNode->val);
                    first = false;
                    lastVisited = peekNode;
                    st.pop();
                }
            }
        }
        printf("\n");
    }

    // 4. 层次遍历（队列 BFS）
    void levelOrder() const {
        if (root == nullptr) {
            printf("\n");
            return;
        }
        Queue<TreeNode*> q(n + 5);
        q.push(root);
        bool first = true;

        while (!q.empty()) {
            TreeNode* curr = q.front();
            q.pop();

            if (!first) printf(" ");
            printf("%d", curr->val);
            first = false;

            if (curr->left != nullptr) q.push(curr->left);
            if (curr->right != nullptr) q.push(curr->right);
        }
        printf("\n");
    }

    // 5. 计算并输出以每个节点为根的子树节点数目
    void printSubtreeSizes() {
        calcSizeDFS(root);
        for (int i = 1; i <= n; ++i) {
            if (i > 1) printf(" ");
            printf("%d", nodes[i].subSize);
        }
        printf("\n");
    }

    // 6. 计算并输出以每个节点为根的子树高度
    void printSubtreeHeights() {
        calcHeightDFS(root);
        for (int i = 1; i <= n; ++i) {
            if (i > 1) printf(" ");
            printf("%d", nodes[i].subHeight);
        }
        printf("\n");
    }
};

int main() {
    int n;
    if (scanf("%d", &n) == 1) {
        BinaryTree tree(n);
        tree.build();
        tree.preOrder();
        tree.inOrder();
        tree.postOrder();
        tree.levelOrder();
        tree.printSubtreeSizes();
        tree.printSubtreeHeights();
    }
    return 0;
}