#include <iostream>

using namespace std;

class MinHeap {
private:
    int data[60];
    int sz;

    void swapVal(int& a, int& b) {
        int t = a;
        a = b;
        b = t;
    }

    void siftUp(int idx) {
        while (idx > 1 && data[idx] < data[idx / 2]) {
            swapVal(data[idx], data[idx / 2]);
            idx /= 2;
        }
    }

    void siftDown(int idx) {
        while (idx * 2 <= sz) {
            int child = idx * 2;
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

    int size() const {
        return sz;
    }

    void push(int val) {
        data[++sz] = val;
        siftUp(sz);
    }

    // 弹出并返回堆顶最小值，合并了 top 和 pop 方便调用
    int pop() {
        int topVal = data[1];
        data[1] = data[sz--];
        siftDown(1);
        return topVal;
    }
};

char s[1000005];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> s;

    // 1. 统计 26 个小写字母频次
    int freq[26] = {0};
    int len = 0;
    for (; s[len] != '\0'; ++len) {
        freq[s[len] - 'a']++;
    }

    // 2. 将出现过的字母频次入堆
    MinHeap heap;
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            heap.push(freq[i]);
        }
    }

    // 特判：只有 1 种字符时，每个字符占 1 bit
    if (heap.size() == 1) {
        cout << len << "\n";
        return 0;
    }

    // 3. 贪心合并：累加所有非叶节点权值即为总编码长度
    long long totalLen = 0;
    while (heap.size() > 1) {
        int merged = heap.pop() + heap.pop();
        totalLen += merged;
        heap.push(merged);
    }

    cout << totalLen << "\n";
    return 0;
}
