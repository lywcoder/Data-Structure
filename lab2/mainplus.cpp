#include "sortplus.hpp"

int main() {
    int n;
    std::cout << "请输入要排序的数字个数 n: ";
    if (std::cin >> n && n > 0) {
        Sorter<int> sorter(n);

        //生成随机数据
        sorter.generateRandom();

        std::cout << "\n初始随机序列: " ;
        sorter.print();
        std::cout << std::endl;

        //依次执行四种排序算法并且输出过程和统计
        sorter.rankSort();
        std::cout << std::endl;

        sorter.selectionSort();
        std::cout << std::endl;

        sorter.bubbleSort();
        std::cout << std::endl;

        sorter.insertionSort();
    }

    return 0;
}
