#include "contact.h"

SequentialList list;

int main() {
    int n = 0;
    std::cin >> n;
    
    int op = 0;

    for (int i = 0; i < n; i++) {
        std::cin >> op;
        if (op == 0) {
            char name[50], phone[20], cls[50], dorm[20];
            std::cin >> name >> phone >> cls >> dorm;
            list.insert(name, phone, cls, dorm);
        } else if (op == 1) {
            char name[50];
            std::cin >> name;
            list.remove(name);
        } else if (op == 2) {
            char name[50], newVal[50];
            int item;
            std::cin >> name >> item >> newVal;
            list.edit(name, item, newVal);
        } else if (op == 3) {
            char name[50];
            std::cin >> name;
            list.search(name);
        } else if (op == 4) {
            char cls[50];
            std::cin >> cls;
            list.printClassXor(cls);
        }
    }

    return 0;
}