#include <iostream>

using namespace std;

const int MAXN = 100005;

// 邻接链表节点
struct Node {
    int v;
    Node* next;
    Node(int v, Node* next = nullptr) : v(v), next(next) {}
};

class Graph {
private:
    int n;
    Node* head[MAXN];   // 每个顶点的邻接表头指针
    bool vis[MAXN];     // 访问标记数组
    int seq[MAXN];      // 存放 DFS / BFS 序列
    int len;            // 当前序列长度
    int dist[MAXN];     // 记录最短路径长度
    int q[MAXN];        // 手写数组队列

    // 递归执行 DFS
    void dfs(int u) {
        vis[u] = true;
        seq[len] = u;
        len++;
        for (Node* p = head[u]; p != nullptr; p = p->next) {
            if (!vis[p->v]) {
                dfs(p->v);
            }
        }
    }

    // 辅助函数：将 v 按升序插入 u 的邻接链表（保持有序且去重）
    void insertNode(int u, int v) {
        Node* prev = nullptr;
        Node* curr = head[u];
        while (curr != nullptr && curr->v < v) {
            prev = curr;
            curr = curr->next;
        }
        // 若边已存在，直接返回
        if (curr != nullptr && curr->v == v) return;

        Node* newNode = new Node(v, curr);
        if (prev == nullptr) {
            head[u] = newNode;
        } else {
            prev->next = newNode;
        }
    }

    // 辅助函数：从 u 的邻接链表中删除 v
    void deleteNode(int u, int v) {
        Node* prev = nullptr;
        Node* curr = head[u];
        while (curr != nullptr && curr->v != v) {
            prev = curr;
            curr = curr->next;
        }
        if (curr == nullptr) return;

        if (prev == nullptr) {
            head[u] = curr->next;
        } else {
            prev->next = curr->next;
        }
        delete curr;
    }

public:
    Graph(int vertices) : n(vertices) {
        for (int i = 1; i <= n; ++i) {
            head[i] = nullptr;
        }
    }

    // 插入无向边
    void addEdge(int u, int v) {
        insertNode(u, v);
        insertNode(v, u);
    }

    // 删除无向边
    void removeEdge(int u, int v) {
        deleteNode(u, v);
        deleteNode(v, u);
    }

    // 1 & 2. 求连通分量个数及每个分量的最小顶点编号
    void printComponents() {
        for (int i = 1; i <= n; ++i) vis[i] = false;
        int count = 0;
        int minVertex[MAXN];

        for (int i = 1; i <= n; ++i) {
            if (!vis[i]) {
                minVertex[count] = i; // 从小到大遍历，i 必为该连通分量的最小点
                count++;

                // 用 BFS 标记整个连通分量
                int front = 0, rear = 0;
                q[rear] = i;
                rear++;
                
                vis[i] = true;
                while (front < rear) {
                    int u = q[front];
                    front++;
                    
                    for (Node* p = head[u]; p != nullptr; p = p->next) {
                        if (!vis[p->v]) {
                            vis[p->v] = true;
                            q[rear] = p->v;
                            rear++;
                        }
                    }
                }
            }
        }

        cout << count << '\n';
        
        for (int i = 0; i < count; ++i) {
            cout << minVertex[i] << ' ';
        }
        cout << '\n';
    }

    // 3 & 4. 输出从 s 开始的字典序最小 DFS 序列
    void printDFS(int s) {
        for (int i = 1; i <= n; ++i) vis[i] = false;
        len = 0;
        
        dfs(s);

        cout << len << '\n';
        for (int i = 0; i < len; ++i) {
            cout << seq[i] << ' ';
        }
        cout << '\n';
    }

    // 5, 6 & 7. 输出从 t 开始的字典序最小 BFS 序列，以及 s 到 t 的最短路径
    void printBFSAndShortestPath(int t, int s) {
        for (int i = 1; i <= n; ++i) {
            vis[i] = false;
            dist[i] = -1;
        }
        len = 0;

        int front = 0, rear = 0;
        q[rear] = t;
        rear++;
        
        vis[t] = true;
        dist[t] = 0;

        while (front < rear) {
            int u = q[front];
            front++;
            
            seq[len] = u;
            len++;

            for (Node* p = head[u]; p != nullptr; p = p->next) {
                int v = p->v;
                if (!vis[v]) {
                    vis[v] = true;
                    dist[v] = dist[u] + 1;
                    q[rear] = v;
                    rear++;
                }
            }
        }

        cout << len << '\n';
        
        for (int i = 0; i < len; ++i) {
            cout << seq[i] << ' ';
        }
        cout << '\n';
        
        cout << dist[s] << '\n';
    }
};

int main() {
    // 关闭 C/C++ 标准流同步，加快 cin/cout 读写速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    static Graph g(n);

    for (int i = 0; i < m; ++i) {
        int op, u, v;
        cin >> op >> u >> v;
        if (op == 0) {
            g.addEdge(u, v);
        } else {
            g.removeEdge(u, v);
        }
    }

    g.printComponents();
    g.printDFS(s);
    g.printBFSAndShortestPath(t, s);

    return 0;
}