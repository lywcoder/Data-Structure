#include "SparseMatrix.h"

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