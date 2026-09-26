#include <iostream>
#include <cstring> // 用于 strlen, strcpy, strcmp

using namespace std;

// 定义联系人结构体
struct Contact {
    char name[50];
    char phone[20];
    char classId[50];
    char dorm[20];
};

// 定义顺序表类
class SequentialList {
private:
    Contact data[20005]; // 静态数组，题目限制 n<=20000
    int length;          // 当前线性表长度

public:
    // 构造函数，初始化空表
    SequentialList() {
        length = 0;
    }

    // 插入操作 (对应操作码 0)
    // 题目保证输入合法，直接尾部插入
    void insert(const char* n, const char* p, const char* c, const char* d) {
        if (length >= 20005) return; // 简单的溢出保护
        strcpy(data[length].name, n);
        strcpy(data[length].phone, p);
        strcpy(data[length].classId, c);
        strcpy(data[length].dorm, d);
        length++;
    }

    // 删除操作 (对应操作码 1)
    void remove(const char* n) {
        int index = -1;
        // 查找名字
        for (int i = 0; i < length; i++) {
            if (strcmp(data[i].name, n) == 0) {
                index = i;
                break;
            }
        }
        
        if (index != -1) {
            // 找到后，将后续元素前移
            for (int i = index; i < length - 1; i++) {
                data[i] = data[i + 1];
            }
            length--;
        }
    }

    // 编辑操作 (对应操作码 2)
    // item: 1-电话, 2-班级, 3-宿舍
    void edit(const char* n, int item, const char* newVal) {
        for (int i = 0; i < length; i++) {
            if (strcmp(data[i].name, n) == 0) {
                if (item == 1) strcpy(data[i].phone, newVal);
                else if (item == 2) strcpy(data[i].classId, newVal);
                else if (item == 3) strcpy(data[i].dorm, newVal);
                return; // 假设名字唯一，找到即修改并返回
            }
        }
    }

    // 查找操作 (对应操作码 3)
    // 找到输出1，未找到输出0
    void search(const char* n) {
        for (int i = 0; i < length; i++) {
            if (strcmp(data[i].name, n) == 0) {
                cout << 1 << endl;
                return;
            }
        }
        cout << 0 << endl;
    }

    // 输出一个班所有人员信息的异或值 (对应操作码 4)
    void printClassXor(const char* c) {
        int xorResult = 0;
        bool foundAny = false; // 标记是否有该班级成员

        for (int i = 0; i < length; i++) {
            if (strcmp(data[i].classId, c) == 0) {
                // 将宿舍号字符串转换为整数进行异或
                // 假设宿舍号是纯数字，虽然题目描述是"宿舍"，这里按常规数字处理
                int dormNum = 0;
                // 简单的字符串转数字
                for (int k = 0; data[i].dorm[k] != '\0'; k++) {
                    if (data[i].dorm[k] >= '0' && data[i].dorm[k] <= '9') {
                        dormNum = dormNum * 10 + (data[i].dorm[k] - '0');
                    }
                }
                
                xorResult ^= dormNum;
                foundAny = true;
            }
        }
        
        // 题目要求输出异或值，如果没有该班级成员，异或值默认为0
        cout << xorResult << endl;
    }
};

SequentialList list;

int main() {
    int n;
    cin >> n;

    int op;

    for (int i = 0; i < n; i++) {
        cin >> op;
        if (op == 0) {
            char name[50], phone[20], cls[50], dorm[20];
            cin >> name >> phone >> cls >> dorm;
            list.insert(name, phone, cls, dorm);
        } 
        else if (op == 1) {
            char name[50];
            cin >> name;
            list.remove(name);
        } 
        else if (op == 2) {
            char name[50], newVal[50];
            int item;
            cin >> name >> item >> newVal;
            list.edit(name, item, newVal);
        } 
        else if (op == 3) {
            char name[50];
            cin >> name;
            list.search(name);
        } 
        else if (op == 4) {
            char cls[50];
            cin >> cls;
            list.printClassXor(cls);
        }
    }

    return 0;
}