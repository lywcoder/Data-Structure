#include "list.h"

//构造：带头结点建空表，再从输入读入 n 个初始元素（尾插）
LinkedList::LinkedList(int n) {
    head = new listNode(0, nullptr);
    size = 0;

    listNode* tail = head;
    for (int i = 0; i < n; i++) {
        int v;
        std::cin >> v;
        tail->next = new listNode(v, nullptr);
        tail = tail->next;
        size++;
    }
}

LinkedList::~LinkedList() {
    while (head != nullptr) {
        listNode* p = head;
        head = head->next;
        delete p;
    }
}

//在 0-indexed 的 idx 处插入 val（idx 越界时夹取到 [0, size]）
void LinkedList::insert(int idx, int val) {
    if (idx < 0) idx = 0;
    if (idx > size) idx = size;

    listNode* p = head;
    for (int i = 0; i < idx; i++) {
        p = p->next;
    }
    p->next = new listNode(val, p->next);
    size++;
}

//删除第一个值为 val 的节点；不存在则输出 -1
void LinkedList::remove(int val) {
    listNode* p = head;
    while (p->next != nullptr) {
        if (p->next->data == val) {
            listNode* q = p->next;
            p->next = q->next;
            delete q;
            size--;
            return; //删除成功，不输出
        }
        p = p->next;
    }
    std::cout << -1 << std::endl; //不存在
}

//搜索链表中是否存在值为 val 的节点
bool LinkedList::search(int val) const {
    for (listNode* p = head->next; p != nullptr; p = p->next) {
        if (p->data == val) {
            return true;
        }
    }
    return false;
}

//原地逆置链表，仅使用常量个辅助指针
void LinkedList::reverse() {
    listNode* prev = nullptr;
    listNode* cur = head->next;
    while (cur != nullptr) {
        listNode* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    head->next = prev;
}

//返回第一个值为 val 的索引（0-indexed），不存在返回 -1
int LinkedList::indexOf(int val) const {
    int i = 0;
    for (listNode* p = head->next; p != nullptr; p = p->next, i++) {
        if (p->data == val) {
            return i;
        }
    }
    return -1;
}

//基于迭代器遍历链表，输出 sum(i ^ chain[i])
void LinkedList::output() const {
    long long sum = 0;
    int i = 0;
    for (Iterator it = begin(); it != end(); ++it, i++) {
        sum += i ^ (*it);
    }
    std::cout << sum << std::endl;
}

//基于迭代器遍历链表，打印所有元素
void LinkedList::print() const {
    std::cout << "链表: ";
    bool first = true;
    for (Iterator it = begin(); it != end(); ++it) {
        if (!first) {
            std::cout << " ";
        }
        std::cout << *it;
        first = false;
    }
    std::cout << std::endl;
}
