#ifndef LIST_H
#define LIST_H

#include <iostream>

//链表节点
struct listNode {
    int data;
    listNode* next;

    listNode(int d = 0, listNode* n = nullptr) : data(d), next(n) {}
};

//链表迭代器
class Iterator {
private:
    listNode* ptr;

    //构造私有，只允许 LinkedList 创建迭代器
    Iterator(listNode* p) : ptr(p) {}

public:
    int& operator*() const { return ptr->data; }
    Iterator& operator++() { ptr = ptr->next; return *this; }
    Iterator operator++(int) {
        Iterator tmp = *this;
        ptr = ptr->next;
        return tmp;
    }
    bool operator==(const Iterator& o) const { return ptr == o.ptr; }
    bool operator!=(const Iterator& o) const { return ptr != o.ptr; }

    friend class LinkedList;
};

class LinkedList {
public:
    LinkedList(int n); //构造：读入 n 个整数作为初始元素

    ~LinkedList();

    void insert(int idx, int val); //在 0-indexed 的 idx 处插入 val

    void remove(int val);          //删除第一个值为 val 的节点；不存在则输出 -1

    bool search(int val) const;    //搜索链表中是否存在值为 val 的节点

    void reverse();                //原地逆置链表，O(1) 额外空间

    int indexOf(int val) const;    //返回第一个值为 val 的索引（0-indexed），不存在返回 -1

    void output() const;           //基于迭代器遍历，输出 sum(i ^ chain[i])

    void print() const;            //基于迭代器遍历，打印链表元素

    Iterator begin() const { return Iterator(head->next); }
    
    Iterator end() const { return Iterator(nullptr); }

private:
    listNode* head; //头结点（哑节点，不存数据）
    int size;       //链表节点个数
};

#endif
