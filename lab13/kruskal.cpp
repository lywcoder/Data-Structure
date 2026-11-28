#include <iostream>

using namespace std;

const int MAXN = 200005;
const int MAXM = 500005;

// 边结构体
struct Edge {
    int u;
    int v;
    int w;
};

Edge edges[MAXM];
Edge tempEdges[MAXM]; // 归并排序的辅助数组

// 手写归并排序：按边权 w 从小到大排序，时间复杂度稳定为 O(e log e)
void mergeSort(int left, int right) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;
    mergeSort(left, mid);
    mergeSort(mid + 1, right);

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (edges[i].w <= edges[j].w) {
            tempEdges[k++] = edges[i++];
        } else {
            tempEdges[k++] = edges[j++];
        }
    }
    while (i <= mid) {
        tempEdges[k++] = edges[i++];
    }
    while (j <= right) {
        tempEdges[k++] = edges[j++];
    }

    for (int p = left; p <= right; p++) {
        edges[p] = tempEdges[p];
    }
}

// 并查集类：用于维护顶点连通性并检测环
class DisjointSet {
private:
    int parent[MAXN];
    int rank[MAXN]; // 记录树的高度上界，用于按秩合并

public:
    // 初始化：每个顶点最初各自独立为一个集合
    void init(int n) {
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    // 查找所在集合的根节点（带路径压缩）
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    // 合并两个顶点所在的集合（按秩合并）
    // 返回 true 表示合并成功；返回 false 表示原本已连通（加边会成环）
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) {
            return false;
        }

        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        return true;
    }
};

DisjointSet dsu;

long long kruskal(int n, int e) {
    // 1. 将所有边按权值从小到大排序
    mergeSort(0, e - 1);

    // 2. 初始化并查集
    dsu.init(n);

    long long totalWeight = 0; // 记录总权值，使用 long long 防止溢出
    int selectedEdges = 0;     // 记录已选入生成树的边数

    // 3. 贪心策略：依次考察权值最小的边
    for (int i = 0; i < e; i++) {
        if (dsu.unite(edges[i].u, edges[i].v)) {
            totalWeight += edges[i].w;
            selectedEdges++;

            // n 个顶点的生成树恰好包含 n - 1 条边，选满即可提前结束
            if (selectedEdges == n - 1) {
                break;
            }
        }
    }

    return totalWeight;
}

int main() {
    // 关闭 C++ 与 C 标准流的同步，大幅提升 cin/cout 读写速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, e;
    cin >> n >> e;

    for (int i = 0; i < e; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    cout << kruskal(n, e) << "\n";

    return 0;
}