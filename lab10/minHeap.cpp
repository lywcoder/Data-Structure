#include <iostream>

using namespace std;

class MinHeap {
private:
    int data[10000];
    int sz;

    void swapVal(int& a, int& b) {
        int t = a;
        a = b;
        b = t;
    }

    // 向上调整（用于插入）
    void siftUp(int idx) {
        while (idx > 1 && data[idx] < data[idx / 2]) {
            swapVal(data[idx], data[idx / 2]);
            idx /= 2;
        }
    }

    // 向下调整（用于建堆和删除）
    void siftDown(int idx) {
        while (idx * 2 <= sz) {
            int child = idx * 2;
            // 选出左右孩子中较小的那个
            if (child + 1 <= sz && data[child + 1] < data[child]) {
                child++;
            }
            if (data[idx] <= data[child]) break;
            swapVal(data[idx], data[child]);
            idx = child;
        }
    }

public:
    MinHeap() : sz(0) {}

    // O(n) 初始化建堆
    void build(int arr[], int n) {
        sz = n;
        for (int i = 1; i <= n; ++i) {
            data[i] = arr[i - 1];
        }
        for (int i = sz / 2; i >= 1; --i) {
            siftDown(i);
        }
    }

    // 插入元素
    void push(int val) {
        data[++sz] = val;
        siftUp(sz);
    }

    // 删除堆顶
    void pop() {
        data[1] = data[sz--];
        siftDown(1);
    }

    // 获取堆顶
    int top() const {
        return data[1];
    }
};

int main() {
    // 加速输入输出，应对 10ms 时间限制
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    int initArr[5005];
    for (int i = 0; i < n; ++i) {
        cin >> initArr[i];
    }

    MinHeap heap;
    heap.build(initArr, n);
    cout << heap.top() << "\n";

    int m;
    cin >> m;
    while (m--) {
        int op;
        cin >> op;
        if (op == 1) {
            int num;
            cin >> num;
            heap.push(num);
            cout << heap.top() << "\n";
        } else if (op == 2) {
            heap.pop();
            cout << heap.top() << "\n";
        } else if (op == 3) {
            int k;
            cin >> k;
            int sortArr[10005];
            for (int i = 0; i < k; ++i) {
                cin >> sortArr[i];
            }
            // 直接重建最小堆并依次弹出堆顶，即为升序结果
            heap.build(sortArr, k);
            for (int i = 0; i < k; ++i) {
                cout << heap.top() << (i == k - 1 ? "" : " ");
                heap.pop();
            }
            cout << "\n";
        }
    }

    return 0;
}