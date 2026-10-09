#include "list.h"

int main() {
    int N, Q;
    std::cin >> N >> Q;
    LinkedList list(N); //构造链表并读入初始 N 个元素

    while (Q-- > 0) {
        int opt;
        std::cin >> opt;
        switch (opt) {
            case 1: { //在 idx 处插入 val
                int idx, val;
                std::cin >> idx >> val;
                list.insert(idx, val);
                break;
            }
            case 2: { //删除第一个值为 val 的节点，不存在输出 -1
                int val;
                std::cin >> val;
                list.remove(val);
                break;
            }
            case 3:   //原地逆置
                list.reverse();
                break;
            case 4: { //查询 val 的索引，不存在输出 -1
                int val;
                std::cin >> val;
                std::cout << list.indexOf(val) << std::endl;
                break;
            }
            case 5:   //基于迭代器输出异或累加和
                list.output();
                break;
        }

        //每次操作后基于迭代器打印一遍链表状态
        // list.print();
    }

    return 0;
}
