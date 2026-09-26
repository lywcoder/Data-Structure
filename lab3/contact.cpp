#include "contact.h"

//操作一是进行插入操作
void SequentialList::insert(const char* n, const char* p, const char* c, const char* d) {
    if (length >= 20005) return;
    strcpy(data[length].name, n); 
    strcpy(data[length].phone, p); 
    strcpy(data[length].classId, c); 
    strcpy(data[length].dorm, d); 
    length++;
}

//操作二是进行删除操作
void SequentialList::remove(const char* n) {
    int index = -1;
    //查找名字
    for (int i = 0; i < length; i++) {
        if (strcmp(data[i].name, n) == 0) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        //找到之后将后续的元素前移
        for (int i = index; i < length - 1; i++) {
            data[i] = data[i + 1];
        }
        length--;
    }
}

//操作三进行编辑操作
void SequentialList::edit(const char* n, int item, const char* newVal) {
    for (int i = 0; i < length; i++) {
        if (strcmp(data[i].name, n) == 0) {
            if (item == 1) strcpy(data[i].phone, newVal);
            else if (item == 2) strcpy(data[i].classId, newVal);
            else if (item == 3) strcpy(data[i].dorm, newVal);
            return;
        }
    }
}

//进行查找操作
void SequentialList::search(const char* n) {
    for (int i = 0; i < length; i++) {
        if (strcmp(data[i].name, n) == 0) {
            std::cout << 1 << std::endl;
            return;
        }
    }
    std::cout << 0 << std::endl;
}

//输出异或值
void SequentialList::printClassXor(const char* c) {
    int xorResult = 0;

    for (int i = 0; i < length; i++) {
        if (strcmp(data[i].classId, c) == 0) {
            int dormNum = 0;

            for (int k = 0; data[i].dorm[k] != '\0'; k++) {
                if (data[i].dorm[k] >= '0' && data[i].dorm[k] <= '9') {
                    dormNum = dormNum * 10 + (data[i].dorm[k] - '0');
                }
            }

            xorResult ^= dormNum;
        }
    }

    std::cout << xorResult << std::endl;
}     