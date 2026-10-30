#include <iostream>
#include <iomanip>
#include <string>

//使用数组来实现栈
//使用数组来模拟栈
//首先括号内需要首先计算，并且乘除的优先级大于加减，同级运算从左到右
template <typename T>
class ArrayStack {
private:
    T data[2005];
    int count = 0; //表示栈中有多少元素，同时也是下一个入栈元素的下标

public: 
    //判断栈为空
    bool empty() const {
        return count == 0;
    }

    //入栈
    void push(T value) {
        data[count] = value;
        count++;
    }

    //弹出栈
    T pop() {
        count--;
        return data[count];
    }

    //读取栈顶
    T top() const {
        return data[count - 1];
    }
};

//运算符优先级
int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

//完成一次栈顶运算
//使用两个栈来保存暂时还不能计算的内容, 分别是操作数栈（保留数字和中间计算的结果），运算符栈（保留运算符和中间内容）
void calculate(ArrayStack<double>& numbers, ArrayStack<char>& operators) {
    //后弹出的数字是左操作数，先弹出的是右操作数
    double b = numbers.pop();
    double a = numbers.pop();
    char op = operators.pop();

    switch (op) {
        case '+': numbers.push(a + b); break;
        case '-': numbers.push(a - b); break;
        case '*': numbers.push(a * b); break;
        case '/': numbers.push(a / b); break;
    }
}

//核心思想是：遇到新的运算符的时候，检查前面的运算符是否应该先完成，如果应该先完成，就弹栈计算
//核心函数：接受一个表达式字符串并且计算出它的计算结果
double evaluate(const std::string& expression) {
    ArrayStack<double> numbers; //数字栈，建立两个空栈并且逐字符扫描
    ArrayStack<char> operators; //表达式栈

    for (char c : expression) {
        if (c >= '0' && c <= '9') {
            //题目保证每个数字只有一位
            numbers.push(c - '0'); //转换成数值并且压栈,题目说了每个表达式中的数字是单个整数
        } else if (c == '(') {
            operators.push(c); //左括号压栈作为边界，不能提前计算完成,括号里面的运算应该先完成，不能提早启用括号外边的元素
        } else if (c == ')') {
            while (operators.top() != '(') {  //查看运算符的栈顶，如果不是左括号，就计算一次,不断重复，直到栈顶为左括号
                calculate(numbers, operators);
            }
            operators.pop(); //弹出左括号并且丢弃,右括号本身不会入栈
        } else {
            while (!operators.empty() &&   //运算符栈不为空
                    operators.top() != '(' && //不能越过左括号进行计算
                    priority(operators.top()) >= priority(c)) { //先前等待的运算符优先级不低于当前运算符，就处理先前的运算,同级从左到右计算
                calculate(numbers, operators);
            }
            operators.push(c); //将结果进行压栈
        }
    }

    //处理剩余的运算符
    while (!operators.empty()) {
        calculate(numbers, operators); //扫描从左到右结束之后，就将剩下的运算符计算完
    }

    return numbers.top(); //返回最终答案
}

int main() {
    //加速io速度,减少输入输出的部分开销
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::cout << std::fixed << std::setprecision(2); //使用固定小数形式输出,并保留小数点后面两位

    while (n--) {
        //读取表达式字符串
        std::string expression;
        std::cin >> expression;
        std::cout << evaluate(expression) << '\n';
    }

    return 0;
}

//从左到右扫描表达式：
// 1.遇到数字就压入数字栈
// 2.遇到左括号就压入运算符栈
// 3.遇到右括号就不断计算栈顶运算，知道遇到(，然后弹出左括号，目的就是消除括号
// 4.遇到运算符：如果栈顶的运算符的优先级大雨或者等于当前运算符，就先计算栈顶运算，然后再将当前运算符入栈
// 5.扫描结束之后，计算剩余运算，数字栈最后留下的值就是答案
// 优先级相等的时候要先计算，从左到右

//第一个重要设计：先把读到的数字和运算符保存下来，知道确定计算顺序之后，再执行运算
//要先处理最内层，再返回外层。进入一层括号，就暂时挂起外层计算；完成内层后，再继续外层。
// 
// | 栈 | 作用 | 例子 |
// |---|---|---|
// 数字栈 | 保存已经读到的数，以及已经算好的中间结果 | `2、3、9` |
// 运算符栈 | 保存仍然等待执行的运算符，以及括号边界 | `+、*、(` |
// 栈算法很漂亮的地方：无论括号里多复杂，处理完后，对外层来说都只是一个数