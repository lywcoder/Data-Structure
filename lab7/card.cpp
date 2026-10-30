#include <iostream>

class CircularQueue {
private:
    int* data; // 存储元素的数组
    int frontIndex; // 队头指针
    int rearIndex; // 队尾指针
    int capacity; //队列的最大容量，包括空位
    int count; // 当前队列中的元素个数

public:
    //构造函数
    CircularQueue(int cap) {
        capacity = cap + 1; //预留一个额外的位置来区分队满和队空
        data = new int[capacity];
        frontIndex = 0;
        rearIndex = 0;
        count = 0;
    }

    //析构函数，释放内存
    ~CircularQueue() {
        delete[] data;
    }

    bool empty() const {
        return count == 0;
    }

    //获取当前元素的个数
    int size() const {
        return count;
    }

    //入队的操作
    void push(int val) {
        if (count == capacity - 1) return; // 队列已满
        data[rearIndex] = val;
        rearIndex = (rearIndex + 1) % capacity;
        count++;
    }
    
    //出队的操作
    void pop() {
        if (empty()) return;
        frontIndex = (frontIndex + 1) % capacity;
        count--;
    }

    //获得队头的元素
    int front() const {
        return data[frontIndex];
    }
};

int main() {
    //优化输出输出
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    //1. 初始化循环队列并且将1~n的卡片一次入队
    CircularQueue q(n);
    for (int i = 1; i <= n; ++i) {
        q.push(i);
    }

    //2. 模拟卡片的游戏过程
    while (q.size() > 1) {
        q.pop(); //扔掉第一张牌（队头出队）
        int topCard = q.front(); //读取新的第一张牌
        q.pop(); //将其出队
        q.push(topCard); // 重新放回整叠牌的最后（队尾入队）
    }

    //3.输出最后剩下的牌
    std::cout << q.front() << '\n';

    return 0;
}