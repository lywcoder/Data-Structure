#include <iostream>
#include <vector>
#include <algorithm>

// 稀疏矩阵的三元组节点定义
struct MatrixTerm {
    int row;   // 行序号 (从1开始)
    int col;   // 列序号 (从1开始)
    int value; // 元素值
};

// 稀疏矩阵类定义
class SparseMatrix {
private:
    int rows;                      // 矩阵行数
    int cols;                      // 矩阵列数
    std::vector<MatrixTerm> terms; // 行主顺序存储非零元素的三元组数组

public:
    SparseMatrix(int r = 0, int c = 0);

    // 重置矩阵尺寸和内容（读入 n*m 完整矩阵）
    void reset(int r, int c);

    // 读入三元组输入的稀疏矩阵
    void readSparse(int r, int c, int t);

    // 矩阵加法：合法返回 true，非法返回 false
    bool add(const SparseMatrix& Q);

    // 矩阵乘法：合法返回 true，非法返回 false
    bool multiply(const SparseMatrix& Q);

    // 矩阵转置：原地转置
    void transpose();

    // 按阵列形式输出矩阵
    void print() const;
};

SparseMatrix::SparseMatrix(int r, int c) : rows(r), cols(c) {}

// 操作1：重置矩阵 (读入完整密集矩阵)
void SparseMatrix::reset(int r, int c) {
    rows = r;
    cols = c;
    terms.clear();

    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            int val;
            std::cin >> val;
            if (val != 0) {
                terms.push_back({i, j, val});
            }
        }
    }
}

// 读入三元组输入的矩阵 Q
void SparseMatrix::readSparse(int r, int c, int t) {
    rows = r;
    cols = c;
    terms.clear();
    terms.reserve(t);

    for (int i = 0; i < t; ++i) {
        int x, y, v;
        std::cin >> x >> y >> v;
        if (v != 0) {
            terms.push_back({x, y, v});
        }
    }
}

// 操作2：稀疏矩阵乘法
bool SparseMatrix::multiply(const SparseMatrix& Q) {
    // 判断乘法是否合法 (P 的列数必须等于 Q 的行数)
    if (cols != Q.rows) {
        *this = Q; // 不合法，将 Q 赋给 P
        return false;
    }

    // 1. 统计 Q 矩阵每列非零元素个数
    std::vector<int> qColCount(Q.cols + 1, 0);
    for (const auto& term : Q.terms) {
        qColCount[term.col]++;
    }

    // 2. 计算 Q 矩阵每列的起始位置索引
    std::vector<int> qColStart(Q.cols + 2, 0);
    for (int c = 1; c <= Q.cols; ++c) {
        qColStart[c + 1] = qColStart[c] + qColCount[c];
    }

    // 3. 构建 Q 按列排序的三元组数组 qColTerms
    std::vector<MatrixTerm> qColTerms(Q.terms.size());
    std::vector<int> currentPos = qColStart;
    for (const auto& term : Q.terms) {
        int dest = currentPos[term.col]++;
        qColTerms[dest] = term;
    }

    // 4. 统计 P 矩阵每行的起始索引，方便按行扫描 P
    std::vector<int> pRowCount(rows + 1, 0);
    for (const auto& term : terms) {
        pRowCount[term.row]++;
    }
    std::vector<int> pRowStart(rows + 2, 0);
    for (int r = 1; r <= rows; ++r) {
        pRowStart[r + 1] = pRowStart[r] + pRowCount[r];
    }

    std::vector<MatrixTerm> result;

    // 5. 按行-列计算 P * Q
    for (int r = 1; r <= rows; ++r) {
        int pStart = pRowStart[r];
        int pEnd = pRowStart[r + 1];
        if (pStart == pEnd) continue; // P 的第 r 行为空

        for (int c = 1; c <= Q.cols; ++c) {
            int qStart = qColStart[c];
            int qEnd = qColStart[c + 1];
            if (qStart == qEnd) continue; // Q 的第 c 列为空

            // 双指针计算 P 的第 r 行与 Q 的第 c 列的点积
            int sum = 0;
            int pIdx = pStart;
            int qIdx = qStart;

            while (pIdx < pEnd && qIdx < qEnd) {
                if (terms[pIdx].col < qColTerms[qIdx].row) {
                    pIdx++;
                } else if (terms[pIdx].col > qColTerms[qIdx].row) {
                    qIdx++;
                } else {
                    sum += terms[pIdx].value * qColTerms[qIdx].value; // int 自然溢出
                    pIdx++;
                    qIdx++;
                }
            }

            if (sum != 0) {
                result.push_back({r, c, sum});
            }
        }
    }

    cols = Q.cols; // 更新结果列数（行数不变）
    terms = std::move(result);
    return true;
}

// 操作3：稀疏矩阵加法 (归并双指针)
bool SparseMatrix::add(const SparseMatrix& Q) {
    // 判断加法是否合法 (行列数必须相同)
    if (rows != Q.rows || cols != Q.cols) {
        *this = Q; // 不合法，将 Q 赋给 P
        return false;
    }

    std::vector<MatrixTerm> result;
    size_t i = 0, j = 0;

    // 按行主序归并两个三元组数组
    while (i < terms.size() && j < Q.terms.size()) {
        int posP = (terms[i].row - 1) * cols + terms[i].col;
        int posQ = (Q.terms[j].row - 1) * cols + Q.terms[j].col;

        if (posP < posQ) {
            result.push_back(terms[i++]);
        } else if (posP > posQ) {
            result.push_back(Q.terms[j++]);
        } else {
            int sumVal = terms[i].value + Q.terms[j].value; // int 自然溢出
            if (sumVal != 0) {
                result.push_back({terms[i].row, terms[i].col, sumVal});
            }
            i++;
            j++;
        }
    }

    // 处理剩余元素
    while (i < terms.size()) result.push_back(terms[i++]);
    // 【修正死循环 Bug】正确递增 j 和压入 Q.terms[j]
    while (j < Q.terms.size()) result.push_back(Q.terms[j++]);

    terms = std::move(result);
    return true;
}

// 操作5：稀疏矩阵快速转置
void SparseMatrix::transpose() {
    if (terms.empty()) {
        std::swap(rows, cols);
        return;
    }

    std::vector<int> colCount(cols + 1, 0);
    for (const auto& term : terms) {
        colCount[term.col]++;
    }

    std::vector<int> rowStart(cols + 2, 0);
    for (int c = 1; c <= cols; ++c) {
        rowStart[c + 1] = rowStart[c] + colCount[c];
    }

    std::vector<MatrixTerm> transposed(terms.size());
    std::vector<int> currentPos = rowStart;

    for (const auto& term : terms) {
        int dest = currentPos[term.col]++;
        transposed[dest] = {term.col, term.row, term.value};
    }

    std::swap(rows, cols);
    terms = std::move(transposed);
}

// 操作4：按照阵列输出矩阵
void SparseMatrix::print() const {
    std::cout << rows << " " << cols << '\n';
    size_t idx = 0;
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            if (idx < terms.size() && terms[idx].row == i && terms[idx].col == j) {
                std::cout << terms[idx].value;
                idx++;
            } else {
                std::cout << 0;
            }
            if (j < cols) std::cout << " ";
        }
        std::cout << '\n';
    }
}

int main() {
    // 优化标准输入输出性能
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int w;
    if (!(std::cin >> w)) return 0;

    SparseMatrix P;

    while (w--) {
        int op;
        std::cin >> op;

        if (op == 1) { // 重置矩阵
            int n, m;
            std::cin >> n >> m;
            P.reset(n, m);
        } else if (op == 2) { // 矩阵乘法
            int n, m, t;
            std::cin >> n >> m >> t;
            SparseMatrix Q;
            Q.readSparse(n, m, t);
            if (!P.multiply(Q)) {
                std::cout << -1 << "\n";
            }
        } else if (op == 3) { // 矩阵加法
            int n, m, t;
            std::cin >> n >> m >> t;
            SparseMatrix Q;
            Q.readSparse(n, m, t);
            if (!P.add(Q)) {
                std::cout << -1 << "\n";
            }
        } else if (op == 4) { // 输出操作
            P.print();
        } else if (op == 5) { // 转置操作
            P.transpose();
        }
    }

    return 0;
}
