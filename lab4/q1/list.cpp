#include "list.h"

//构造：带头结点建空表，再从输入读入 n 个初始元素（尾插）
LinkedList::LinkedList(int n) {
    head = new listNode(0, nullptr); //head指向头结点
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
    if (idx < 0) idx = 0; //头部，防止插入的越界
    if (idx > size) idx = size; //尾部越界

    listNode* p = head;
    for (int i = 0; i < idx; i++) {
        p = p->next;
    }
    p->next = new listNode(val, p->next); //new新的节点并且在后面插入
    size++;
}

//删除第一个值为 val 的节点；不存在则输出 -1
void LinkedList::remove(int val) {
    listNode* p = head;
    while (p->next != nullptr) { //先进行判断
        if (p->next->data == val) {
            listNode* q = p->next; //标记这个需要删除的节点
            p->next = q->next; //将p（标记的q的上一个节点）下一个节点跳过q连接
            delete q; //删除这个节点
            size--;
            return; //删除成功，不输出
        }
        p = p->next; //往下继续遍历
    }
    std::cout << -1 << std::endl; //不存在
}

//搜索链表中是否存在值为 val 的节点
bool LinkedList::search(int val) const {
    for (listNode* p = head->next; p != nullptr; p = p->next) { //从链表的第一个有效节点开始进行遍历
        if (p->data == val) {
            return true; //条件判断找到是否存在符合条件值的节点
        }
    }
    return false;
}

//原地逆置链表，仅使用3个辅助指针
//逐段修改节点的next，让他从指向后一个节点变成指向前一个节点，最后让头结点指向新的第一个节点
//指针prev 代表已经逆置好的那一段的第一个节点，初始为空
//cur代表当前准备处理的节点
// 临时保存当前节点后继，防止改箭头之后找不到后面的节点
void LinkedList::reverse() {
    listNode* prev = nullptr; //空指针，prev
    listNode* cur = head->next; //第一个有效节点
    while (cur != nullptr) {
        listNode* nxt = cur->next; //暂时用不到，暂存，防止丢掉地址当前节点的下一个节点
        cur->next = prev; //这里cur节点的next指向prev
        //prev 和 cur指针向后移动
        prev = cur; //先前指针定位到现在的指针（现在的指针和当前的指针进行置换）
        cur = nxt; // 当前指针向后移动
        //依次循环往复
    }
    head->next = prev; //头结点的下一个成为prev
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
