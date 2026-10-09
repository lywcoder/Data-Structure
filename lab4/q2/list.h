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
class LinkedListIterator {
private:
    listNode* ptr;

    //构造私有，只允许 LinkedList 创建迭代器
    LinkedListIterator(listNode* p) : ptr(p) {}

public:
    int& operator*() const { return ptr->data; }
    LinkedListIterator& operator++() { ptr = ptr->next; return *this; }
    LinkedListIterator operator++(int) {
        LinkedListIterator tmp = *this;
        ptr = ptr->next;
        return tmp;
    }
    bool operator==(const LinkedListIterator& o) const { return ptr == o.ptr; }
    bool operator!=(const LinkedListIterator& o) const { return ptr != o.ptr; }

    friend class LinkedList;
};

class LinkedList {
public:
    LinkedList();      //默认构造：空链表
    LinkedList(int n); //构造：读入 n 个整数（尾插）
    ~LinkedList();

    void insert(int idx, int val);   //在 0-indexed 的 idx 处插入 val
    void insertSorted(int val);      //按升序插入 val（用于把无序序列建成有序链表）
    void push_back(int val);         //尾插，O(1)
    void remove(int val);            //删除第一个值为 val 的节点；不存在则输出 -1
    bool search(int val) const;      //搜索链表中是否存在值为 val 的节点
    void reverse();                  //原地逆置链表，O(1) 额外空间
    int indexOf(int val) const;      //返回第一个值为 val 的索引（0-indexed），不存在返回 -1
    long long xorSum() const;        //基于迭代器遍历，计算 sum(i ^ chain[i])
    void output() const;             //输出 xorSum 的值
    void print() const;              //基于迭代器遍历，打印链表元素

    //基于迭代器合并两个升序链表，结果（升序）写入 result
    static void mergeSorted(const LinkedList& a, const LinkedList& b, LinkedList& result);

    LinkedListIterator begin() const { return LinkedListIterator(head->next); }
    LinkedListIterator end() const { return LinkedListIterator(nullptr); }

private:
    listNode* head; //头结点（哑节点，不存数据）
    listNode* tail; //尾节点指针（维护它使尾插为 O(1)）
    int size;       //链表节点个数
};

#endif
