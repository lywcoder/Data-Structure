// C++14 single-file OJ submission. Generated from the split source files.
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

/**
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
    // 对格式错误、越界、重复或乱序的坐标抛出异常；合法 OJ 输入不会触发。
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


#include <algorithm>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace {

// 本题 GNU C++14 评测环境的 int 为 32 位。
// 用编译期检查明确这一前提，防止移植到不同 int 宽度时悄悄改变语义。
static_assert(std::numeric_limits<int>::digits == 31,
              "This problem requires a 32-bit int.");
static_assert(std::numeric_limits<unsigned int>::digits == 32,
              "This problem requires a 32-bit unsigned int.");

/**
 * 将 32 位无符号余数映射回题目要求的有符号 int 值。
 *
 * 不直接把超出 INT_MAX 的 unsigned 强转成 int，因为在 C++14 中
 * 该转换的结果由实现定义。这里的每一次转成 int 都保证没有越界。
 */
int toSignedValue(unsigned int bits) noexcept {
    const unsigned int maxInt =
        static_cast<unsigned int>(std::numeric_limits<int>::max());
    if (bits <= maxInt) {
        return static_cast<int>(bits);
    }
    return -1 - static_cast<int>(
        std::numeric_limits<unsigned int>::max() - bits);
}

// C++ 的有符号整数溢出是未定义行为，不能直接写 a+b 或 a*b 来模拟回绕。
// unsigned 运算按 2^32 取模；负 int 转 unsigned 也有标准规定的模语义。
// 矩阵中的元素和累加结果仍然全部用 int 存储。
int wrappedAdd(int a, int b) noexcept {
    return toSignedValue(static_cast<unsigned int>(a) +
                         static_cast<unsigned int>(b));
}

int wrappedMultiply(int a, int b) noexcept {
    return toSignedValue(static_cast<unsigned int>(a) *
                         static_cast<unsigned int>(b));
}

bool precedes(const MatrixTerm& a, const MatrixTerm& b) noexcept {
    return a.row < b.row ||
           (a.row == b.row && a.column < b.column);
}

}  // namespace

SparseMatrix::SparseMatrix(int rows, int columns)
    : rows_(rows), columns_(columns) {
    if (rows < 0 || columns < 0) {
        throw std::invalid_argument("Matrix dimensions must be nonnegative.");
    }
}

SparseMatrix SparseMatrix::readDense(std::istream& input) {
    int rows, columns;
    if (!(input >> rows >> columns)) {
        throw std::runtime_error("Cannot read matrix dimensions.");
    }
    SparseMatrix matrix(rows, columns);
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            int value;
            if (!(input >> value)) {
                throw std::runtime_error("Cannot read matrix element.");
            }
            if (value != 0) {
                // 输入天然按行主序排列，追加即可维持所有存储不变量。
                matrix.terms_.push_back({row, column, value});
            }
        }
    }
    return matrix;
}

SparseMatrix SparseMatrix::readTriplets(std::istream& input) {
    int rows, columns, count;
    if (!(input >> rows >> columns >> count)) {
        throw std::runtime_error("Cannot read sparse matrix header.");
    }
    SparseMatrix matrix(rows, columns);
    if (count < 0) {
        throw std::invalid_argument("Nonzero count must be nonnegative.");
    }
    matrix.terms_.reserve(static_cast<std::size_t>(count));
    MatrixTerm previous{-1, -1, 0};
    for (int i = 0; i < count; ++i) {
        int row, column, value;
        if (!(input >> row >> column >> value)) {
            throw std::runtime_error("Cannot read matrix triplet.");
        }
        if (row < 1 || row > rows || column < 1 || column > columns) {
            throw std::out_of_range("Triplet coordinate is out of range.");
        }
        MatrixTerm term{row - 1, column - 1, value};
        if (i != 0 && !precedes(previous, term)) {
            throw std::invalid_argument("Triplets must be strictly row-major.");
        }
        previous = term;
        // 题目保证输入为非零项；额外允许显式输入零，但不会存储它。
        if (value != 0) {
            matrix.terms_.push_back(term);
        }
    }
    return matrix;
}

bool SparseMatrix::canAdd(const SparseMatrix& rhs) const noexcept {
    return rows_ == rhs.rows_ && columns_ == rhs.columns_;
}

bool SparseMatrix::canMultiply(const SparseMatrix& rhs) const noexcept {
    return columns_ == rhs.rows_;
}

SparseMatrix SparseMatrix::add(const SparseMatrix& rhs) const {
    if (!canAdd(rhs)) {
        throw std::invalid_argument("Matrix addition dimension mismatch.");
    }
    SparseMatrix result(rows_, columns_);
    std::size_t i = 0, j = 0;
    while (i < terms_.size() && j < rhs.terms_.size()) {
        const MatrixTerm& a = terms_[i];
        const MatrixTerm& b = rhs.terms_[j];
        if (precedes(a, b)) {
            result.terms_.push_back(a);
            ++i;
        } else if (precedes(b, a)) {
            result.terms_.push_back(b);
            ++j;
        } else {
            // 坐标相同才需要相加；相消或回绕成零时必须删除这一项。
            const int sum = wrappedAdd(a.value, b.value);
            if (sum != 0) {
                result.terms_.push_back({a.row, a.column, sum});
            }
            ++i;
            ++j;
        }
    }
    // 某一方先耗尽时，另一方的剩余项可直接按原顺序追加。
    for (; i < terms_.size(); ++i) {
        result.terms_.push_back(terms_[i]);
    }
    for (; j < rhs.terms_.size(); ++j) {
        result.terms_.push_back(rhs.terms_[j]);
    }
    return result;
}

std::vector<std::size_t> SparseMatrix::buildRowStarts() const {
    std::vector<std::size_t> start(static_cast<std::size_t>(rows_) + 1, 0);
    for (const MatrixTerm& term : terms_) {
        ++start[static_cast<std::size_t>(term.row) + 1];
    }
    for (std::size_t r = 1; r < start.size(); ++r) {
        start[r] += start[r - 1];
    }
    return start;
}

SparseMatrix SparseMatrix::multiply(const SparseMatrix& rhs) const {
    if (!canMultiply(rhs)) {
        throw std::invalid_argument("Matrix multiplication dimension mismatch.");
    }
    SparseMatrix result(rows_, rhs.columns_);
    if (terms_.empty() || rhs.terms_.empty()) {
        return result;
    }

    // A(i,k) 只需要与 B 的第 k 行配对。
    // 行边界能直接找到 B(k,*)，无需反复搜索 B 的所有非零元素。
    const std::vector<std::size_t> rightStart = rhs.buildRowStarts();

    // 以下是一行的工作区，长度是结果的列数，不是 rows*columns。
    // sums[j]：当前结果行第 j 列的部分和。
    // mark[j]：这个位置最近属于哪一行；-1 表示从未使用。
    // touched：当前行实际被访问过的列，避免逐行扫描/清零全部列。
    std::vector<int> sums(static_cast<std::size_t>(rhs.columns_), 0);
    std::vector<int> mark(static_cast<std::size_t>(rhs.columns_), -1);
    std::vector<int> touched;

    std::size_t aPos = 0;
    while (aPos < terms_.size()) {
        const int row = terms_[aPos].row;
        touched.clear();

        // 只访问 A 的非空行；同一行的所有项连续存放。
        while (aPos < terms_.size() && terms_[aPos].row == row) {
            const MatrixTerm& a = terms_[aPos];
            const std::size_t k = static_cast<std::size_t>(a.column);
            for (std::size_t bPos = rightStart[k];
                 bPos < rightStart[k + 1]; ++bPos) {
                const MatrixTerm& b = rhs.terms_[bPos];
                const int column = b.column;
                if (mark[column] != row) {
                    // 本行首次访问该列时覆盖旧值，无需清零整行数组。
                    mark[column] = row;
                    sums[column] = 0;
                    touched.push_back(column);
                }
                const int product = wrappedMultiply(a.value, b.value);
                sums[column] = wrappedAdd(sums[column], product);
            }
            ++aPos;
        }

        // 不同 k 可能以任意次序触及列，先排序才能维持行主序不变量。
        std::sort(touched.begin(), touched.end());
        for (int column : touched) {
            if (sums[column] != 0) {
                result.terms_.push_back({row, column, sums[column]});
            }
        }
    }
    return result;
}

SparseMatrix SparseMatrix::transpose() const {
    SparseMatrix result(columns_, rows_);
    if (terms_.empty()) {
        return result;
    }
    result.terms_.resize(terms_.size());

    // 原矩阵第 c 列 -> 转置矩阵第 c 行。
    // 第一步：统计每一列包含多少个非零元素。
    std::vector<std::size_t> next(static_cast<std::size_t>(columns_), 0);
    for (const MatrixTerm& term : terms_) {
        ++next[term.column];
    }

    // 第二步：把计数原地改为各目标行的起始下标（排他的前缀和）。
    std::size_t offset = 0;
    for (std::size_t c = 0; c < next.size(); ++c) {
        const std::size_t count = next[c];
        next[c] = offset;
        offset += count;
    }

    // 第三步：稳定散布。原数组按行有序，同一原列遇到的原行号递增，
    // 因此目标同一行内的列号也递增，不需要再调用 sort。
    for (const MatrixTerm& term : terms_) {
        result.terms_[next[term.column]++] =
            MatrixTerm{term.column, term.row, term.value};
    }
    return result;
}

void SparseMatrix::writeDense(std::ostream& output) const {
    output << rows_ << ' ' << columns_ << '\n';
    std::size_t pos = 0;
    for (int row = 0; row < rows_; ++row) {
        for (int column = 0; column < columns_; ++column) {
            if (column != 0) {
                output << ' ';
            }
            if (pos < terms_.size() && terms_[pos].row == row &&
                terms_[pos].column == column) {
                output << terms_[pos].value;
                ++pos;
            } else {
                output << 0;
            }
        }
        output << '\n';
    }
}


#include <exception>
#include <iostream>
#include <stdexcept>
#include <utility>

int main() {
    // 关闭 C/C++ 流同步并解除 cin/cout 绑定，避免每次输入都触发刷新。
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int operationCount;
    if (!(std::cin >> operationCount)) {
        return 0;
    }

    try {
        if (operationCount < 0) {
            throw std::invalid_argument("Operation count must be nonnegative.");
        }
        SparseMatrix current;
        for (int i = 0; i < operationCount; ++i) {
            int operation;
            if (!(std::cin >> operation)) {
                throw std::runtime_error("Cannot read operation code.");
            }

            switch (operation) {
            case 1:
                current = SparseMatrix::readDense(std::cin);
                break;
            case 2:
            case 3: {
                // 即使维度不合法，也必须完整读入 Q，随后用 Q 替换 P。
                SparseMatrix right = SparseMatrix::readTriplets(std::cin);
                const bool valid = operation == 2
                    ? current.canMultiply(right)
                    : current.canAdd(right);

                if (!valid) {
                    current = std::move(right);
                    std::cout << -1 << '\n';
                } else if (operation == 2) {
                    current = current.multiply(right);
                } else {
                    current = current.add(right);
                }
                break;
            }
            case 4:
                current.writeDense(std::cout);
                break;
            case 5:
                current = current.transpose();
                break;
            default:
                throw std::invalid_argument("Unknown operation code.");
            }
        }
    } catch (const std::exception& error) {
        // 异常表示输入/环境错误。题目规定的维度不合法已在上面正常处理。
        // 错误诊断写入 stderr，不向评测要求的 stdout 混入提示信息。
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
