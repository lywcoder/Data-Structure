#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>

//1.基于动态数组描述的Stack类的实现
// 
template<typename T>
class ArrayStack {
private:
    T* elements;
    int capacity;
    int topIndex;

    void resize(int newCapacity) {
        T* newElements = new T[newCapacity];
        for (int i = 0; i <= topIndex; ++i) {
            newElements[i] = elements[i];
        }
        delete[] elements;
        elements = newElements;
        capacity = newCapacity;
    }

public:
    ArrayStack(int initCapacity = 10) : capacity(initCapacity), topIndex(-1) {
        elements = new T[capacity];
    }

    ~ArrayStack() {
        delete[] elements;
    }        

    bool isEmpty() const {
        return topIndex == -1;
    }

    int size() const {
        return topIndex + 1;
    }

    void push(const T& val) {
        if (topIndex + 1 == capacity) {
            resize(capacity * 2);
        }
        elements[++topIndex] = val;
    }

    void pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty");
        }
        --topIndex;
    }

    T top() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty");
        }
        return elements[topIndex];
    }
};

// 2.算术表达式求值
int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0; //‘（'的优先级在栈内判定时设为最低，防止被普通运算符弹出
}

double applyOp(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw std::runtime_error("除数不能为0");
            return a / b;
    }
    return 0;
}

double evaluateExpression(const std::string& expr) {
    ArrayStack<double> numStack; // 操作数栈
    ArrayStack<char> opStack; // 运算符栈

    for (size_t i = 0;i < expr.length(); ++i) {
        char ch = expr[i];
        if (isspace(ch)) continue;

        if (isdigit(ch)) {
            double val = 0;
            while (i < expr.length() && isdigit(expr[i])) {
                val = val * 10 + (expr[i] - '0');
                i++;
            }
            i--; // 抵消外层循环的++i
            numStack.push(val);
        } else if (ch == '(') {
            opStack.push(ch);
        } else if (ch == ')') {
            while (!opStack.isEmpty() && opStack.top() != '(') {
                char op = opStack.top();
                opStack.pop();
                double val2 = numStack.top(); numStack.pop();
                double val1 = numStack.top(); numStack.pop();
                numStack.push(applyOp(val1, val2, op));
            }
            if (!opStack.isEmpty() && opStack.top() == '(') {
                opStack.pop(); //弹出'('
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            while (!opStack.isEmpty() && getPriority(opStack.top()) >= getPriority(ch)) {
                char op = opStack.top(); opStack.pop();
                double val2 = numStack.top(); numStack.pop();
                double val1 = numStack.top(); numStack.pop();
                numStack.push(applyOp(val1, val2, op));
            }
            opStack.push(ch);
        }
    }

    while (!opStack.isEmpty()) {
        char op = opStack.top(); opStack.pop();
        double val2 = numStack.top(); numStack.pop();
        double val1 = numStack.top(); numStack.pop();
        numStack.push(applyOp(val1, val2, op));
    }

    return numStack.top();
}

//3. 迷宫求解 非递归，栈式回溯DFS
struct Point {
    int r, c;
};

//顺时针方向向量：上，右，下，左
const int dr[] = {-1, 0, 1, 0};
const int dc[] = {0, 1, 0, -1};

bool solveMaze(const std::vector<std::vector<int>>& maze, Point start, Point end, ArrayStack<Point>& path) {
    int rows = maze.size();
    int cols = maze[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    path.push(start);
    visited[start.r][start.c] = true;

    while (!path.isEmpty()) {
        Point curr = path.top();

        if (curr.r == end.r && curr.c == end.c) {
            return true;
        }

        bool foundNext = false;
        for (int i = 0; i < 4; ++i) {
            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];

            //判断邻居节点是否计为障碍（1）或者已经访问
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && maze[nr][nc] == 0 && !visited[nr][nc]) {
                visited[nr][nc] = true;
                path.push({nr, nc});
                foundNext = true;
                break; // 优先访问深入新节点
            }
        }

        if (!foundNext) {
            path.pop(); //四周无路可走，出栈回溯到上一个节点
        }
    }

    return false; //栈空仍未到达终点，说明没有通路
}

//打印栈中保留的路径（翻转之后顺序输出）
void printPath(ArrayStack<Point>& path) {
    std::vector<Point> coords;
    while (!path.isEmpty()) {
        coords.push_back(path.top());
        path.pop();
    }
    std::reverse(coords.begin(), coords.end());
    for (size_t i = 0; i < coords.size(); ++i) {
        std::cout << "(" << coords[i].r << ", " << coords[i].c << ")";
        if (i + 1 < coords.size()) std::cout << " -> ";
    }
    std::cout << "\n";
}

int main() {
    // ---- 验证 1：表达式求值 ----
    std::string expr = "2+3*(4+5)-6/4";
    std::cout << "表达式: " << expr << " 的计算结果为: " << evaluateExpression(expr) << "\n\n";

    // ---- 验证 2：迷宫求解 ----
    // 0 表示通路，1 表示障碍
    std::vector<std::vector<int>> maze = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    ArrayStack<Point> path;
    Point start = {0, 0};
    Point end = {4, 4};

    if (solveMaze(maze, start, end, path)) {
        std::cout << "找到迷宫通路坐标序列:\n";
        printPath(path);
    } else {
        std::cout << "迷宫无通路。\n";
    }

    return 0;
}