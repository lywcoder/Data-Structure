#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <cstddef>
#include <iosfwd>
#include <vector>

/**
 * 一个非零元素的三元组。
 * 对外输入使用从 1 开始的下标；类内部统一使用从 0 开始的下标。
 * 元素值必须使用 int，以符合本题的数据类型要求。
 */
struct MatrixTerm {
    int row;
    int column;
    int value;
};

/*
 * 行主序三元组稀疏矩阵。
 *
 * 始终保持以下不变量：
 * 1. rows_、columns_ 非负，所有三元组的坐标均在矩阵范围内。
 * 2. terms_ 只存储非零值，不存储“占位的零”。
 * 3. 三元组按 (row, column) 严格递增，同一坐标最多出现一次。
 *
 * vector<MatrixTerm> 是动态、连续的一维数组，符合题目的存储要求。
 * 类负责矩阵算法；OJ 的操作编号和“不合法时替换 P”规则放在 main.cpp。
 * 类中的加法、乘法均返回新对象，不修改两个输入矩阵。
 */

class SparseMatrix {
public:
    // 默认表示 0×0 的空矩阵；构造指定尺寸时，所有元素默认为 0。
    explicit SparseMatrix(int rows = 0, int columns = 0);

    int rows() const noexcept { return rows_; }
    int columns() const noexcept { return columns_; }
    std::size_t nonZeroCount() const noexcept { return terms_.size(); }

    // 读取 n、m，随后读取 n*m 个值；边读边压缩，不构造稠密矩阵。
    static SparseMatrix readDense(std::istream& input);

    // 读取 n、m、t，随后读取 t 个按行主序排列的 1-based 三元组。
    // 对格式错误、越界、重复或乱序的坐标抛出异常；
    static SparseMatrix readTriplets(std::istream& input);

    bool canAdd(const SparseMatrix& rhs) const noexcept;
    bool canMultiply(const SparseMatrix& rhs) const noexcept;

    // 双指针归并，时间 O(nnz(A)+nnz(B))。
    // 尺寸不匹配时抛出 std::invalid_argument。
    SparseMatrix add(const SparseMatrix& rhs) const;

    // 按行展开非零乘积，只使用一个结果行的累加工作区。
    // 尺寸不匹配时抛出 std::invalid_argument。
    SparseMatrix multiply(const SparseMatrix& rhs) const;

    // 快速转置：计数 + 前缀和 + 稳定散布，时间 O(columns()+nnz)。
    SparseMatrix transpose() const;

    // 输出尺寸和通常的阵列形式；直接扫描三元组，不建立二维数组。
    void writeDense(std::ostream& output) const;

private:
    int rows_;
    int columns_;
    std::vector<MatrixTerm> terms_;

    // 返回行边界：第 r 行对应 [start[r], start[r+1])。
    // 注意：返回的是非零数组的下标，不是矩阵的列下标。
    std::vector<std::size_t> buildRowStarts() const;
};

#endif  // SPARSE_MATRIX_H
