#include <iostream>
#include <cstring>

//定义联系人结构体
struct Contact {
    char name[50];
    char phone[20];
    char classId[50];
    char dorm[20];
};

//定义顺序表
class SequentialList {
private:
    //定义结构体数组和对应的当前线性表的长度
    Contact data[20000];
    int length;
    
public:
    //构造函数，初始化空列表
    SequentialList() {
        length = 0;
    }

    //尾部插入的操作（对应的操作码是0）
    void insert(const char* n, const char* p, const char* c, const char* d);     
        
    //删除操作（对应的操作码是1）
    void remove(const char* n);

    //编辑操作 （对应的操作码是2）
    void edit(const char* n, int item, const char* newVal);
    
    //查找操作（对应的操作码是3) 
    void search(const char* n); 

    //输出一个班所有人员信息的异或值（对应操作码4）
    void printClassXor(const char* c);
};