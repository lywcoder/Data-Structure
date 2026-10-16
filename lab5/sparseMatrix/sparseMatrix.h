#pragma once

#include <iostream>
#include <vector>

//稀疏矩阵的三元组节点定义
struct MatrixTerm {
    int row; // 行序号
    int col; // 列序号
    int value; // 元素值
};

//稀疏矩阵的定义
class SparseMatrix {
private:
    int rows; //矩阵的行数
    int cols; //矩阵的列数
    std::vector<MatrixTerm> terms; //行主顺序存储非零元素的三元组数组
public:
    //构造函数
    SparseMatrix(int r = 0, int c = 0);

    //重置矩阵尺寸和内容
    void reset(int r, int c);

    //读入稀疏矩阵三元组输入的矩阵
    void readSparse(int r, int c, int t);

    //矩阵加法合法就是true, 非法就是false
    bool add(const SparseMatrix& Q);

    //矩阵乘法，如果合法true，非法就是false
    bool multiply(const SparseMatrix& Q);

    //矩阵的转置：将当前的矩阵原地或者生成转置矩阵
    void transpose();

    //矩阵按照阵列的形势输出
    void print() const;
};
