#include <iostream>

template <class T>
class Sorter {
private:
    T* data;
    int length;

public:
    //使用动态函数动态分配内存
    Sorter(int n) {
        length = n;
        data = new T[length];
    }

    //使用析构函数释放内存
    ~Sorter() {
       delete[] data; 
    }

    //输入数据
    void input() {
        for (int i = 0; i < length; i++) {
            std::cin >> data[i];
        }
    }

    void print() {
        for (int i = 0; i < length; i++) {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    //名次排序
    void rankSort() {
        int* rank = new int[length]();

        for (int i = 1; i < length; i++) {
            for (int j = 0; j < i; j++) {
                if (data[j] <= data[i]) {
                    rank[i]++;
                } else {
                    rank[j]++;
                }
            }
        }

        //根据名次重新排列成临时数组
        T* temp = new T[length];
        for (int i = 0; i < length; i++) {
            temp[rank[i]] = data[i];
        }

        for (int i = 0; i < length; i++) {
            data[i] = temp[i];
        }

        delete[] rank;
        delete[] temp;
    }

    // 及时终止的选择排序
    void selectionSort() {
        for (int i = 0; i < length - 1; i++) {
            int minIndex = i;
            bool isSorted = true; //用来检测剩余的部分数据是否已经有序

            for (int j = i + 1; j < length; j++) {
                if (data[j] < data[minIndex]) {
                    minIndex = j;
                }
                
                //检查是否已经完全升序
                if (data[j - 1] > data[j]) {
                    isSorted = false;
                }
            }

            // 如果剩余的序列已经有序并且当前就是最小值，那么提前终止
            if (isSorted && minIndex == i) {
                break;
            }

            if (minIndex != i) {
                T temp = data[i];
                data[i] = data[minIndex];
                data[minIndex] = temp;
            }
        }
    }

    //及时终止的冒泡排序
    void bubbleSort() {
        for (int i = 0; i < length - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < length - 1 - i; j++) {
                if (data[j] > data[j + 1]) {
                    T temp = data[j];
                    data[j] = data[j + 1];
                    data[j + 1] = temp;
                    swapped = true;
                }
            }

            //如果有一趟冒泡排序没有发生交换，说明这一轮已经有序，终止这一趟
            if (!swapped) {
                break;
            }
        }
    }
    
    // 插入排序
    void insertionSort() {
        for (int i = 1; i < length; i++) {
            T key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }
};

