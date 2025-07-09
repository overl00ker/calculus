#include <iostream>
#include <string>

using namespace std;

constexpr int MAX = 100;

int precedence(char op) 
{
	return (op == '+' || op == '-') ? 1 : (op == '*' || op == '/') ? 2 : 0; //пріорітет операції: множення/ділення вище, ніж додавання/віднімання, дужки найвищий приорітет
}

int apply(int a, int b, char op) 
{
    switch (op) // застосування операціц в прикладі
    {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return b ? a / b : 0;
    default: return 0;
    }
}

int evaluate(const string& expr) 
{
    int val[MAX] = { 0 }; //тут лежать значення
    char op[MAX] = { 0 }; //тут лежать операції
	int vTop = -1, oTop = -1; // vTop - верх значення, oTop - верх операції

    for (size_t i = 0; i < expr.size(); ++i)  //цикл для аналізу всього виразу
    
    {
        if (isspace(expr[i])) continue; //ігоноруємо пробіли
        if (isdigit(expr[i])) //збираємо числа
        {
            int num = 0;
            while (i < expr.size() && isdigit(expr[i])) //щбираємо числа зі значень
                num = num * 10 + (expr[i++] - '0');
			--i; //скорегувати індекс, бо цикл буде його наращувати
            if (vTop + 1 >= MAX) //запобіжник перегрузу стеку з значеннями
            {
                cerr << "Error: value stack overflow\n";
                return 0;
            }
            val[++vTop] = num; //переносимо число в стек
        }
        else if (expr[i] == '(') //перенос відкриваючию дужки в стек
        {
            if (oTop + 1 >= MAX) 
            {
                cerr << "Error: operator stack overflow\n";
                return 0;
            }
            op[++oTop] = '(';
        }
		else if (expr[i] == ')') //для закриваючої переносимо все до найближчої відкриваючої
        {
            while (oTop >= 0 && op[oTop] != '(') 
            {
                if (vTop < 1) return 0;
                int b = val[vTop--];
                int a = val[vTop--];
                val[++vTop] = apply(a, b, op[oTop--]);
            }
            if (oTop >= 0 && op[oTop] == '(') --oTop; //видаляємо відкриваючу дужку
        }
        else //операції з сортуванням по пріорітету
        {
            while (oTop >= 0 && precedence(op[oTop]) >= precedence(expr[i])) 
            {
                if (vTop < 1) return 0;
                int b = val[vTop--];
                int a = val[vTop--];
                val[++vTop] = apply(a, b, op[oTop--]);
            }
            if (oTop + 1 >= MAX) return 0;
            op[++oTop] = expr[i];
        }
    }

    while (oTop >= 0)  //цикл-сортування в стеку для правильного алгоритму розрахунку прикладу
    {
        if (vTop < 1) return 0; //перевірка, що  операції буде 2 значення
        int b = val[vTop--];
        int a = val[vTop--];
		val[++vTop] = apply(a, b, op[oTop--]); //оце якраз застосування операції op[oTop] це якраз та операція зі свіча вище
    }

    return vTop == 0 ? val[0] : 0; //якщо введений лише 1 елемент, то його і виведемо в консоль
}

int main() 
{
    string s;
    while (true) //нескінченний цикл для прикладів
    {
        cout << "Enter example: ";
        getline(cin, s);
        if (s == "exit") break;
        cout << "Result: " << evaluate(s) << "\n";
    }
    return 0;
}