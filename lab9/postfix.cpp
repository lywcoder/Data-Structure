#include <iostream>

const int MAXN = 100000;
int pre[MAXN], in[MAXN], pos[MAXN];
// pre存放的是前序遍历的数组
// in存放的是中序遍历的数组
// pos存放的是每个节点中序数组的位置，pos[x] = i意思就是x值到中序数组下标的映射数组
void solve(int preL, int preR, int inL, int inR) {
    //preL, preR当前处理的子树的左边界下标和右边界下标
    //inL, inR中序序列的左边界下标和右边界下标
    if (preL > preR) return;

    int root = pre[preL]; //首先前序遍历就知道根节点的位置和数字
    int k = pos[root]; //定位当前根节点在中序序列中的下标位置

    int leftSize = k - inL; //这个是左子树的节点个数
    solve(preL + 1, preL + leftSize, inL, k - 1);   // 递归左子树
    solve(preL + leftSize + 1, preR, k + 1, inR);   // 递归右子树

    std::cout << root;
    std::cout << " ";
}

int main() {
    int n;
    std::cin >> n;

    for (int i = 0; i < MAXN; ++i) pos[i] = -1;
    for (int i = 0; i < n; ++i) std::cin >> pre[i];
    for (int i = 0; i < n; ++i) {
        std::cin >> in[i];
        pos[in[i]] = i;
    }

    solve(0, n - 1, 0, n - 1);
    return 0;
}
