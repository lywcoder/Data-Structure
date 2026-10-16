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
    friend class LinkedList; // 设计的是友元类，LinkedList类可以访问其中的迭代器
    
    int& operator*() const { return ptr->data; } //取出其中迭代器所指的数据
    Iterator& operator++() { ptr = ptr->next; return *this; } //前置加加，直接返回引用
    Iterator operator++(int) { //后置加加，先存储temp，然后加加返回temp
        Iterator tmp = *this;
        ptr = ptr->next;
        return tmp;
    }
    bool operator==(const Iterator& o) const { return ptr == o.ptr; } //迭代器相等
    bool operator!=(const Iterator& o) const { return ptr != o.ptr; } //迭代器不等
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

    void print() const;            //用于调试的函数，打印链表的步骤，基于迭代器遍历，打印链表元素

    Iterator begin() const { return Iterator(head->next); }
    
    Iterator end() const { return Iterator(nullptr); }

private:
    listNode* head; //指向头结点（哑节点，不存数据）
    int size;       //链表结点个数
};

#endif
