#include <iostream>

using namespace std;

const int MAXN = 200005;
const int MAXM = 1000005; // 无向图每条边存两次，空间开 2 * e
const int INF = 2147483647;

// 1. 链式前向星存图（替代 vector 邻接表）
struct Edge {
    int to;   // 边的终点
    int w;    // 边权
    int next; // 同起点的上一条边在 edge 数组中的下标
} edge[MAXM];

int head[MAXN]; // head[u] 记录顶点 u 的第一条边在 edge 数组中的下标
int edgeCount = 0;

void addEdge(int u, int v, int w) {
    edgeCount++;
    edge[edgeCount].to = v;
    edge[edgeCount].w = w;
    edge[edgeCount].next = head[u];
    head[u] = edgeCount;
}

// 2. 手写小根堆（替代 priority_queue）
struct HeapNode {
    int v; // 顶点编号
    int w; // 连向该顶点的边权
} heap[MAXM];

int heapSize = 0;

// 向小根堆中插入元素（上浮操作）
void push(int v, int w) {
    heapSize++;
    heap[heapSize].v = v;
    heap[heapSize].w = w;

    int cur = heapSize;
    while (cur > 1) {
        int parent = cur / 2;
        if (heap[cur].w < heap[parent].w) {
            HeapNode temp = heap[cur];
            heap[cur] = heap[parent];
            heap[parent] = temp;
            cur = parent;
        } else {
            break;
        }
    }
}

// 弹出堆顶最小元素（下沉操作）
HeapNode pop() {
    HeapNode topNode = heap[1];
    heap[1] = heap[heapSize];
    heapSize--;

    int cur = 1;
    while (cur * 2 <= heapSize) {
        int child = cur * 2; // 左孩子
        // 若右孩子存在且权值更小，则指向右孩子
        if (child + 1 <= heapSize && heap[child + 1].w < heap[child].w) {
            child++;
        }
        if (heap[child].w < heap[cur].w) {
            HeapNode temp = heap[cur];
            heap[cur] = heap[child];
            heap[child] = temp;
            cur = child;
        } else {
            break;
        }
    }
    return topNode;
}

// 3. 堆优化 Prim 算法
bool vis[MAXN];     // 标记顶点是否已加入最小生成树
int minDist[MAXN];  // 记录当前生成树连向各顶点的最小边权

long long prim(int n) {
    for (int i = 1; i <= n; i++) {
        vis[i] = false;
        minDist[i] = INF;
    }

    long long totalWeight = 0; // 边权总和可能超过 int 范围，使用 long long
    int selectedCount = 0;     // 已加入生成树的顶点数

    // 从 1 号顶点开始扩展
    minDist[1] = 0;
    push(1, 0);

    while (heapSize > 0 && selectedCount < n) {
        HeapNode cur = pop();
        int u = cur.v;
        int w = cur.w;

        // 懒惰删除：如果 u 已经在生成树中，说明这是之前入堆的旧数据，直接跳过
        if (vis[u]) {
            continue;
        }

        // 将 u 正式纳入最小生成树
        vis[u] = true;
        totalWeight += w;
        selectedCount++;

        // 遍历 u 的所有邻接边，更新未访问邻居的最短连边
        for (int i = head[u]; i != 0; i = edge[i].next) {
            int v = edge[i].to;
            int weight = edge[i].w;
            if (!vis[v] && weight < minDist[v]) {
                minDist[v] = weight;
                push(v, weight);
            }
        }
    }

    return totalWeight;
}

int main() {
    // 关闭同步流，加速大规模数据的 cin 读取
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, e;
    if (cin >> n >> e) {
        for (int k = 0; k < e; k++) {
            int u, v, w;
            cin >> u >> v >> w;
            // 无向图需要双向建边
            addEdge(u, v, w);
            addEdge(v, u, w);
        }
        cout << prim(n) << "\n";
    }

    return 0;
}