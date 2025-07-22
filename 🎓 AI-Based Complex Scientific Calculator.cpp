#include <iostream>
#include <stack>
#include <cmath>
#include <map>
#include <sstream>
#include <iomanip>
using namespace std;

// Color Codes
#define RESET   "\033[0m"
#define PURPLE  "\033[35m"
#define BROWN   "\033[38;5;130m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"

// Function to calculate factorial
double factorial(int n) {
    if (n < 0) throw invalid_argument("Negative factorial not defined");
    double result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

// Function to evaluate simple operations
double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': if (b == 0) throw runtime_error("Division by zero!"); return a / b;
        case '^': return pow(a, b);
    }
    return 0;
}

// Precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Evaluate full expression (BODMAS)
double evaluate(string expr) {
    map<string, double> constants = {{"pi", 3.1415926535}, {"e", 2.7182818284}};
    map<string, int> funcUnary = {{"sin", 1}, {"cos", 1}, {"tan", 1}, {"log", 1}, {"ln", 1}, {"sqrt", 1}, {"fact", 1}};

    stack<double> values;
    stack<char> ops;

    for (size_t i = 0; i < expr.length(); ++i) {
        if (isspace(expr[i])) continue;

        if (isdigit(expr[i]) || expr[i] == '.') {
            string val;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) val += expr[i++];
            --i;
            values.push(stod(val));
        }

        else if (isalpha(expr[i])) {
            string word;
            while (i < expr.length() && isalpha(expr[i])) word += expr[i++];
            --i;

            if (constants.count(word)) {
                values.push(constants[word]);
            }
            else if (funcUnary.count(word)) {
                // Expecting format: func(expr)
                while (i < expr.length() && expr[i] != '(') ++i;
                int open = 1;
                size_t start = ++i;
                while (i < expr.length() && open) {
                    if (expr[i] == '(') ++open;
                    else if (expr[i] == ')') --open;
                    ++i;
                }
                string sub = expr.substr(start, i - start - 1);
                double val = evaluate(sub);

                if (word == "sin") values.push(sin(val));
                else if (word == "cos") values.push(cos(val));
                else if (word == "tan") values.push(tan(val));
                else if (word == "sqrt") values.push(sqrt(val));
                else if (word == "log") values.push(log10(val));
                else if (word == "ln") values.push(log(val));
                else if (word == "fact") values.push(factorial((int)val));
            }
        }

        else if (expr[i] == '(') {
            ops.push(expr[i]);
        }

        else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            ops.pop(); // remove '('
        }

        else if (string("+-*/^").find(expr[i]) != string::npos) {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            ops.push(expr[i]);
        }
    }

    while (!ops.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(a, b, op));
    }

    return values.top();
}

int main() {
    cout << PURPLE << "💡 AI-Based Complex Scientific Calculator" << RESET << endl;
    cout << BROWN << "🛠 Developed by Muhammad Umair" << RESET << endl;

    cout << GREEN << "\n📋 Features:\n"
         << "  ✔️ Follows BODMAS Rule\n"
         << "  ✔️ Supports: +, -, *, /, ^\n"
         << "  ✔️ Functions: sin(), cos(), tan(), sqrt(), log(), ln(), fact()\n"
         << "  ✔️ Constants: pi, e\n" << RESET;

    string expression;
    cout << CYAN << "\n📥 Enter expression: " << RESET;
    getline(cin, expression);

    try {
        double result = evaluate(expression);
        cout << YELLOW << "\n📊 Result: " << fixed << setprecision(5) << result << RESET << endl;
    } catch (exception& e) {
        cout << RED << "\n❌ Error: " << e.what() << RESET << endl;
    }

    return 0;
}
