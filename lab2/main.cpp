#include "sort.hpp"

int main() {
    int n;
    std::cin >> n;

    // 进行模版的实例化
    Sorter<int> sorter(n);

    //读取数据
    sorter.input();
    
    //计数排序
    sorter.rankSort();

    //及时终止的选择排序
    sorter.selectionSort();

    //及时终止的冒泡排序
    sorter.bubbleSort();
    
    //插入排序
    sorter.insertionSort();

    //打印结果
    sorter.print();

    return 0;
}