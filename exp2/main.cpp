#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath> 
#include <algorithm>
#include <limits>
#include <sstream>
#include "Stack.h"  


#define N_OPTR 15
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, SIN, COS, TAN, LOG, LN, L_P, R_P, EOE, NEG} Operator;

// 运算符优先级表（修改后包含阶乘）
const char pri[N_OPTR][N_OPTR] = {
    {'>', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'},//ADD
    {'>', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'},//SUB
    {'>', '>', '>', '>', '<', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'},//MUL
    {'>', '>', '>', '>', '<', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'},//DIV
    {'>', '>', '>', '>', '>', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'}, //POW
    {'>', '>', '>', '>', '>', '>', '<', '<', '<', '<', '<','<', '<', '>', '>'}, //FAC
    {'>', '>', '>', '>', '>', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'}, //SIN
    {'>', '>', '>', '>', '>', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'}, //COS
    {'>', '>', '>', '>', '>', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'}, //TAN
    {'>', '>', '>', '>', '>', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'},//LOG
    {'>', '>', '>', '>', '>', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'}, //LN
    {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<','<', '<', '=', ' '},//L_P
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' '},//R_P
    {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<','<', '<', ' ', '='},//EOE
    {'>', '>', '>', '>', '>', '<', '<', '<', '<', '<', '<','<', '<', '>', '>'},//NEG
};

// 函数将运算符字符转换为枚举值
Operator optr2rank(char op) {
    switch (op) {
    case '+': return ADD;
    case '-': return SUB;
    case '*': return MUL;
    case '/': return DIV;
    case '^': return POW;
    case '!': return FAC;
    case '(': return L_P;
    case ')': return R_P;
    case '\0': return EOE;
    case 's': return SIN;
    case 'c': return COS;
    case 'n': return NEG; // 处理负号
    case 't': return TAN;
    case 'g': return LOG; // 使用 'g' 代表 log
    case 'l': return LN;
    default: throw std::runtime_error("Invalid operator"); // 处理无效的操作符
    }
}

char orderBetween(char op1, char op2) {
    return pri[optr2rank(op1)][optr2rank(op2)];
}

// 函数从输入字符串中读取一个数字
void readNumber(const char*& s, Stack<float>& opnd) {
    std::stringstream ss;
    while (isdigit(*s) || *s == '.') {
        ss << *s;
        s++;
    }
    float num;
    ss >> num;
    opnd.push(num);
}

// 函数将运算符或操作数追加到反波兰表示法字符串
void append(std::string& rpn, char op) {
    rpn += op;
    rpn += ' ';  // 在操作符和操作数之间加一个空格
}
void append(std::string& rpn, float num) {
    rpn += std::to_string(num);
    rpn += ' ';
}

// 函数执行计算
float calcu(float pOpnd1, char op, float pOpnd2) {
    switch (op) {
    case '+': return pOpnd1 + pOpnd2;
    case '-': return pOpnd1 - pOpnd2;
    case '*': return pOpnd1 * pOpnd2;
    case '/':
        if (pOpnd2 == 0) throw std::runtime_error("Division by zero");
        return pOpnd1 / pOpnd2;
    case '^': return pow(pOpnd1, pOpnd2);
    default: throw std::runtime_error("Invalid binary operator");
    }
}

float calcu(char op, float pOpnd) {
    switch(op){
    case '!':
    {
        if (pOpnd < 0 || pOpnd != static_cast<int>(pOpnd)) throw std::runtime_error("Factorial requires non-negative integer.");
        int n = static_cast<int>(pOpnd);
        float result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }
    case 's': return sin(pOpnd);    // 正弦函数
    case 'c': return cos(pOpnd);    // 余弦函数
    case 't': return tan(pOpnd);    // 正切函数
    case 'g': return log10(pOpnd);  // 以 10 为底的对数
    case 'l': return log(pOpnd);     // 自然对数 (ln)
    case 'n': return -pOpnd;          // 负数
    default: throw std::runtime_error("Invalid unary operator");
    }
}

float evaluate(const char* S, std::string& RPN, const std::string& expression) {
    Stack<float> opnd;
    Stack<char> optr;
    optr.push('\0');

    while (true) {
        if (*S == '\0' && optr.top() == '\0' && optr.size() == 1) {
            break; // 仅当运算符栈中只有 '\0' 时退出
        }

        if (isdigit(*S) || *S == '.') {
            readNumber(S, opnd);
            append(RPN, opnd.top());
        } else {
            try {
                char current_op = *S;
                if (current_op == ')') {
                    current_op = '\0'; // 将 ')' 视为表达式结束符进行优先级比较
                }

                switch (orderBetween(optr.top(), current_op)) {
                case '<':
                    if (*S == '-' && (S == expression.c_str() || (!isalnum(*(S - 1)) && *(S - 1) != ')'))) {
                        optr.push('n'); // 负号
                        S++;
                    } else if (isalpha(*S)) { // 函数调用
                        std::string funcName;
                        while (isalpha(*S)) {
                            funcName += *S;
                            S++;
                        }

                        char funcOp;
                        if (funcName == "sin") funcOp = 's';
                        else if (funcName == "cos") funcOp = 'c';
                        else if (funcName == "tan") funcOp = 't';
                        else if (funcName == "log") funcOp = 'g';
                        else if (funcName == "ln") funcOp = 'l';
                        // 处理其他函数
                        else throw std::runtime_error("Invalid function name");

                        optr.push(funcOp);
                        if (*S == '(') {
                            optr.push('(');
                            S++;
                        } else {
                            throw std::runtime_error("Expected '(' after function name");
                        }
                    } else {
                        optr.push(*S);
                        S++;
                    }
                    break;

                case '=':
                    if (*S == ')') {
                        S++;
                    }
                    optr.pop(); // 弹出匹配的 '(' 或 ')'
                    break;

                case '>': {
                    char op = optr.top(); // 获取栈顶运算符
                    optr.pop();           // 移除它

                    // 防止将 '\0' 添加到 RPN 中
                    if (op != '\0') {
                        append(RPN, op);
                    }

                    if (op == 'n' || op == 's' || op == 'c' || op == 't' || op == 'g' || op == 'l' || op == '!') {
                        float pOpnd = opnd.top(); // 获取操作数
                        opnd.pop();               // 移除它
                        opnd.push(calcu(op, pOpnd));
                    } else {
                        float pOpnd2 = opnd.top();
                        opnd.pop();
                        float pOpnd1 = opnd.top();
                        opnd.pop();
                        opnd.push(calcu(pOpnd1, op, pOpnd2));
                    }
                    break;
                }

                default:
                    throw std::runtime_error("Invalid expression");
                }
            } catch (const std::runtime_error& error) {
                std::cerr << "Error: " << error.what() << std::endl;
                return NAN;
            }
        }
    }

    // 最终结果应该是操作数栈中唯一的一个元素
    if (opnd.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }

    return opnd.top(); // 获取最终结果
}



// 函数：计算柱状图中最大矩形的面积
int largestRectangleArea(const Vector<int>& heights) {
    Stack<int> stack; // 使用栈存储柱子的索引
    stack.push(-1);   // 推入哨兵值，帮助计算宽度
    int maxArea = 0;

    for (Rank i = 0; i < heights.size(); ++i) {
        // 当当前柱子高度小于栈顶柱子的高度时，计算面积
        while (!stack.empty() && stack.top() != -1 && heights[i] < heights[stack.top()]) {
            int topIndex = stack.pop();
            int height = heights[topIndex];
            int width = i - stack.top() - 1;
            maxArea = std::max(maxArea, height * width);
        }
        stack.push(i);
    }

    // 处理栈中剩余的柱子
    while (!stack.empty() && stack.top() != -1) {
        int topIndex = stack.pop();
        int height = heights[topIndex];
        int width = heights.size() - stack.top() - 1;
        maxArea = std::max(maxArea, height * width);
    }

    return maxArea;
}

// 函数：生成随机的柱子高度数组
Vector<int> generateRandomHeights(int n, int maxHeight) {
    Vector<int> heights;
    for (int i = 0; i < n; ++i) {
        heights.push_back(rand() % (maxHeight + 1)); // 0 <= heights[i] <= maxHeight
    }
    return heights;
}

int main() {
	    Stack<std::string> expressions;
    expressions.push("1+2*3");
    expressions.push("(1+2)*3");
    expressions.push("sin(0.5)+cos(0.5)");
    expressions.push("log(100)");
    expressions.push("2-10");
    expressions.push("1+sin(5)");

    while (!expressions.empty()) {
        std::string expression = expressions.top();
        expressions.pop();

        // 移除空格
        expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());

        std::string rpn;
        try {
            float result = evaluate(expression.c_str(), rpn, expression);
            std::cout << "表达式: " << expression << std::endl;
            std::cout << "RPN: " << rpn << std::endl;
            std::cout << "结果: " << result << std::endl;
        } catch (const std::runtime_error& error) {
            std::cerr << "错误: " << error.what() << std::endl;
        } catch (const std::logic_error& le) { // 捕捉 logic_error
            std::cerr << "逻辑错误: " << le.what() << std::endl;
        }
        std::cout << std::endl;
    }
	printf("------------------------");
	std::cout << std::endl;
	
	
	
    // 初始化随机数生成器
    srand(static_cast<unsigned int>(time(0)));

    // 定义测试用例数量和柱子高度的范围
    const int numRandomTests = 10;
    const int maxSize = 105;   // 1 <= heights.length <= 1e5
    const int maxHeight = 104;  // 0 <= heights[i] <= 1e4

    // 添加具体的示例测试用例
    std::cout << "添加具体的示例测试用例:\n";

    // 示例 1
    {
        Vector<int> heights;
        heights.push_back(2);
        heights.push_back(1);
        heights.push_back(5);
        heights.push_back(6);
        heights.push_back(2);
        heights.push_back(3);
        int maxArea = largestRectangleArea(heights);
        std::cout << "示例 1:\n";
        std::cout << "柱子高度: 2 1 5 6 2 3\n";
        std::cout << "最大矩形面积: " << maxArea << " (期望: 10)\n\n";
    }

    // 示例 2
    {
        Vector<int> heights;
        heights.push_back(2);
        heights.push_back(4);
        int maxArea = largestRectangleArea(heights);
        std::cout << "示例 2:\n";
        std::cout << "柱子高度: 2 4\n";
        std::cout << "最大矩形面积: " << maxArea << " (期望: 4)\n\n";
    }

    // 添加随机测试用例
    std::cout << "添加随机测试用例:\n\n";

    for (int test = 1; test <= numRandomTests; ++test) {
        // 随机生成柱子的数量
        int n = rand() % maxSize + 1; // 1 <= n <= 1e5
        // 生成随机柱子高度
        Vector<int> heights = generateRandomHeights(n, maxHeight);

        // 计算最大矩形面积
        int maxArea = largestRectangleArea(heights);

        // 输出测试结果
        std::cout << "随机测试用例 " << test << ":\n";
        std::cout << "柱子数量: " << n << "\n";
        // 为避免输出过长，仅在柱子数量较少时输出高度数组
        
        std::cout << "柱子高度: ";
        for (Rank i = 0; i < heights.size(); ++i) {
            std::cout << heights[i] << " ";
            }
        std::cout << "\n";
        
  
        std::cout << "最大矩形面积: " << maxArea << "\n\n";
    }

    return 0;
}

