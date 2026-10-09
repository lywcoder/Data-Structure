#include "list.h"

//默认构造：空链表
LinkedList::LinkedList() {
    head = new listNode(0, nullptr);
    tail = head;
    size = 0;
}

//构造：带头结点建空表，再从输入读入 n 个元素（尾插）
LinkedList::LinkedList(int n) {
    head = new listNode(0, nullptr);
    tail = head;
    size = 0;

    for (int i = 0; i < n; i++) {
        int v;
        std::cin >> v;
        push_back(v);
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
    if (p == tail) { //插在了末尾，更新尾指针
        tail = p->next;
    }
    size++;
}

//按升序插入 val：找到第一个 >= val 的节点，插到它前面
void LinkedList::insertSorted(int val) {
    listNode* p = head;
    while (p->next != nullptr && p->next->data < val) {
        p = p->next;
    }
    p->next = new listNode(val, p->next);
    if (p == tail) {
        tail = p->next;
    }
    size++;
}

//尾插，O(1)
void LinkedList::push_back(int val) {
    tail->next = new listNode(val, nullptr);
    tail = tail->next;
    size++;
}

//删除第一个值为 val 的节点；不存在则输出 -1
void LinkedList::remove(int val) {
    listNode* p = head;
    while (p->next != nullptr) {
        if (p->next->data == val) {
            listNode* q = p->next;
            p->next = q->next;
            if (q == tail) { //删除的是尾节点，更新尾指针
                tail = p;
            }
            delete q;
            size--;
            return;
        }
        p = p->next;
    }
    std::cout << -1 << std::endl;
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

//原地逆置链表，O(1) 额外空间
void LinkedList::reverse() {
    listNode* oldFirst = head->next;
    listNode* prev = nullptr;
    listNode* cur = head->next;
    while (cur != nullptr) {
        listNode* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    head->next = prev;
    //原头节点成为新的尾节点
    tail = (oldFirst == nullptr) ? head : oldFirst;
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

//基于迭代器遍历，计算 sum(i ^ chain[i])
long long LinkedList::xorSum() const {
    long long sum = 0;
    int i = 0;
    for (LinkedListIterator it = begin(); it != end(); ++it, i++) {
        sum += i ^ (*it);
    }
    return sum;
}

//输出 xorSum 的值
void LinkedList::output() const {
    std::cout << xorSum() << std::endl;
}

//基于迭代器遍历，打印链表元素
void LinkedList::print() const {
    std::cout << "链表: ";
    bool first = true;
    for (LinkedListIterator it = begin(); it != end(); ++it) {
        if (!first) {
            std::cout << " ";
        }
        std::cout << *it;
        first = false;
    }
    std::cout << std::endl;
}

//基于迭代器合并两个升序链表，结果（升序）写入 result
void LinkedList::mergeSorted(const LinkedList& a, const LinkedList& b, LinkedList& result) {
    LinkedListIterator it1 = a.begin();
    LinkedListIterator it2 = b.begin();

    //两路归并：每次取两个链表中较小的元素尾插到结果链表
    while (it1 != a.end() && it2 != b.end()) {
        if (*it1 <= *it2) {
            result.push_back(*it1);
            ++it1;
        } else {
            result.push_back(*it2);
            ++it2;
        }
    }

    //把剩余元素依次接入
    while (it1 != a.end()) {
        result.push_back(*it1);
        ++it1;
    }
    while (it2 != b.end()) {
        result.push_back(*it2);
        ++it2;
    }
}
