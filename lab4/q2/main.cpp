#include "list.h"

int main() {
    int N, M;
    std::cin >> N >> M;

    //第一组无序序列 -> 有序链表（升序）
    LinkedList listA;
    for (int i = 0; i < N; i++) {
        int v;
        std::cin >> v;
        listA.insertSorted(v);
    }

    //第二组无序序列 -> 有序链表（升序）
    LinkedList listB;
    for (int i = 0; i < M; i++) {
        int v;
        std::cin >> v;
        listB.insertSorted(v);
    }

    //用迭代器合并两个有序链表
    LinkedList listC;
    LinkedList::mergeSorted(listA, listB, listC);

    //分别输出三个有序链表的异或累加和
    std::cout << listA.xorSum() << std::endl;
    std::cout << listB.xorSum() << std::endl;
    std::cout << listC.xorSum() << std::endl;

    return 0;
}
