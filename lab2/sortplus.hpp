#include <iostream>
#include <cstdlib> //用于rand() srand()
#include <ctime> // 用于time()

template<class T>
class Sorter {
private:
    T* data;
    T* originalData; // 用于保存原始随机生成的数据，来一遍四种算法公平测试
    int length;

    //统计数据
    long long compareCount;
    long long moveCount;

public:
    // 构造函数,动态分配内存
    Sorter(int n) {
        length = n;
        data = new T[length];
        originalData = new T[length];
    }

    //析构函数
    ~Sorter() {
        delete[] data;
        delete[] originalData;
    }

    //随机生成0-1000之间的整数
    void generateRandom() {
        srand((unsigned int)time(0));
        for (int i = 0; i < length; i++) {
            data[i] = rand() % 1001;
            originalData[i] = data[i];
        }
    }

    //恢复数据为原始随机的序列并且重置统计计数
    void restoreAndResetStats() {
        for (int i = 0; i < length; i++) {
            data[i] = originalData[i];
        }
        compareCount = 0;
        moveCount = 0;
    }

    //打印当前数据
    void print() {
        for (int i = 0; i < length; i++) {
            std::cout << data[i];
            if (i < length - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }

    //打印统计结果
    void printStats(const char* sortName) {
        std::cout << "[" << sortName << " 统计] 比较次数" << compareCount 
            << ", 移动/赋值次数: " <<  moveCount << std::endl;
    }

    //名次排序
    void rankSort() {
        restoreAndResetStats();
        std::cout << "-----Rank Sort-----" << std::endl;

        int* rank = new int[length]();
        
        for (int i = 1; i < length; i++) {
            for (int j = 0; j < i; j++) {
                compareCount++;
                if (data[j] <= data[i]) {
                    rank[i]++;
                } else {
                    rank[j]++;
                }
            }
        }

        std::cout << "中间数组：" << std::endl;
        for (int i = 0; i < length; i++) {
            std::cout << rank[i] << " ";
        }
        std::cout << std::endl;

        //根据名次重新排列成临时数组
        T* temp = new T[length];
        for (int i = 0; i < length; i++) {
            temp[rank[i]] = data[i];
            moveCount++;
        }

        //拷贝会原数组
        for (int i = 0; i < length; i++) {
            data[i] = temp[i];
            moveCount++; //记录一次移动
        }

        std::cout << "最终结果：";
        print();
        printStats("名次排序");

        delete[] rank;
        delete[] temp;
    }
    
    //2.及时终止的选择排序
    // 
    void selectionSort() {
        restoreAndResetStats();
        std::cout << "-----及时终止的选择排序-----" <<  std::endl;

        for (int i = 0; i < length - 1; i++) {
            int minIndex =  i;
            bool isSorted = true;
            
            for (int j = i + 1; j < length; j++) {
                compareCount++;
                if (data[j] < data[minIndex]) {
                    minIndex = j;
                }

                compareCount++;
                if (data[j - 1] > data[j]) {
                    isSorted = false;
                }
            }

            if (isSorted && minIndex == i) {
                std::cout << "第 " << i + 1 << " 趟扫描发现剩余序列已经有序，提前终止。"; 
                break;
            }

            if (minIndex != i) {
                T temp = data[i];
                data[i] = data[minIndex];
                data[minIndex] = temp;
                moveCount += 3; //交互产生3次移动
            }

            std::cout << "第 " << i + 1 << " 趟结果: ";
            print();
        }
        printStats("选择排序");
    }

    //及时终止的冒泡排序
    void bubbleSort() {
        restoreAndResetStats();
        std::cout << "-----及时终止的冒泡排序-----" << std::endl; 

        for (int i = 0; i < length - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < length - 1 - i; j++) {
                compareCount++;
                if (data[j] > data[j + 1]) {
                    T temp = data[j];
                    data[j] = data[j + 1];
                    data[j + 1] = temp;
                    moveCount += 3; //交换产生3次移动
                    swapped = true;
                }
            }

            std::cout << "第 " << i + 1 << "趟结果: ";
            print();

            if (!swapped) {
                std::cout << "本趟未发生交换，提前终止" << std::endl;
            }
        }

        printStats("冒泡排序");
    }

    void insertionSort() {
        restoreAndResetStats();
        std::cout << "-----插入排序-----" << std::endl;

        for (int i = 1; i < length; i++) {
            T key = data[i];
            moveCount++;

            int j = i - 1;
            while (j >= 0) {
                compareCount++;
                if (data[j] > key) {
                    data[j + 1] = data[j];
                    moveCount++;
                    j = j - 1;
                } else {
                    break;
                }
            }

            data[j + 1] = key;
            moveCount++;

            std::cout << "第 " << i << " 趟结果： ";
            print();
        }

        printStats("插入排序");
    }
};

