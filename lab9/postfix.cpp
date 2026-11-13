#include <cstdio>

const int MAXN = 100005;
const int HASH_SIZE = 1 << 18;      // 262144，负载因子 < 0.4，保证 O(1) 冲突极少
const int HASH_MASK = HASH_SIZE - 1;

// 快速读入（应对 10 ms 极限时限）
inline int readInt() {
    int x = 0, sgn = 1;
    int c = getchar();
    while (c < '0' || c > '9') {
        if (c == '-') sgn = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = getchar();
    }
    return x * sgn;
}

// 快速输出
inline void writeInt(int x) {
    unsigned int ux = static_cast<unsigned int>(x);
    if (x < 0) {
        putchar('-');
        ux = 0u - ux;
    }
    if (ux == 0) {
        putchar('0');
        return;
    }
    char buf[12];
    int top = 0;
    while (ux > 0) {
        buf[top++] = static_cast<char>('0' + (ux % 10));
        ux /= 10;
    }
    while (top > 0) {
        putchar(buf[--top]);
    }
}

// 手写开放寻址哈希表：记录值 val 在中序序列中的下标（替代 std::unordered_map）
class IndexHashMap {
private:
    int keys[HASH_SIZE];
    int vals[HASH_SIZE];
    bool used[HASH_SIZE];

public:
    IndexHashMap() {
        for (int i = 0; i < HASH_SIZE; ++i) {
            used[i] = false;
        }
    }

    void insert(int key, int val) {
        unsigned int h = static_cast<unsigned int>(key) * 2654435761u;
        int idx = h & HASH_MASK;
        while (used[idx] && keys[idx] != key) {
            idx = (idx + 1) & HASH_MASK;
        }
        used[idx] = true;
        keys[idx] = key;
        vals[idx] = val;
    }

    int find(int key) const {
        unsigned int h = static_cast<unsigned int>(key) * 2654435761u;
        int idx = h & HASH_MASK;
        while (used[idx]) {
            if (keys[idx] == key) return vals[idx];
            idx = (idx + 1) & HASH_MASK;
        }
        return -1;
    }
};

// 二叉树链表节点
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// 二叉树类
class BinaryTree {
private:
    TreeNode* root;
    TreeNode pool[MAXN];   // 静态节点内存池，避免 10^5 次动态 new 开销
    int nodeCnt;

    int pre[MAXN];
    int in[MAXN];
    IndexHashMap inPosMap;

    // 分治递归建树：根据前序起点 preL 与中序区间 [inL, inR] 构建子树
    TreeNode* buildRecursive(int preL, int inL, int inR) {
        if (inL > inR) return nullptr;

        int rootVal = pre[preL];
        TreeNode* u = &pool[nodeCnt++];
        u->val = rootVal;

        // O(1) 定位根节点在中序遍历中的位置
        int k = inPosMap.find(rootVal);
        int leftSize = k - inL;  // 左子树的节点个数

        // 递归构建左子树与右子树
        u->left = buildRecursive(preL + 1, inL, k - 1);
        u->right = buildRecursive(preL + leftSize + 1, k + 1, inR);

        return u;
    }

    // 递归后序遍历输出（左 -> 右 -> 根）
    void postOrderRecursive(TreeNode* u, bool& first) const {
        if (u == nullptr) return;
        postOrderRecursive(u->left, first);
        postOrderRecursive(u->right, first);
        if (!first) putchar(' ');
        writeInt(u->val);
        first = false;
    }

public:
    BinaryTree() : root(nullptr), nodeCnt(0) {}

    void buildFromPreAndIn(int n) {
        for (int i = 0; i < n; ++i) {
            pre[i] = readInt();
        }
        for (int i = 0; i < n; ++i) {
            in[i] = readInt();
            inPosMap.insert(in[i], i);
        }
        root = buildRecursive(0, 0, n - 1);
    }

    void printPostOrder() const {
        bool first = true;
        postOrderRecursive(root, first);
        putchar('\n');
    }
};

// 放在全局区，避免大数组占用主函数栈空间
BinaryTree tree;

int main() {
    int n = readInt();
    tree.buildFromPreAndIn(n);
    tree.printPostOrder();
    return 0;
}