#include "SparseMatrix.h"

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
