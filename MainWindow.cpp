#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Calculator");
    setFixedSize(260, 420);

    expressionDisplay = new QLineEdit(this);
    expressionDisplay->setReadOnly(true);
    expressionDisplay->setAlignment(Qt::AlignRight);
    expressionDisplay->setStyleSheet("font-size: 16px; padding: 4px; background-color: #1e1e1e; color: white; border: none;");

    display = new QLineEdit("0", this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setStyleSheet("font-size: 24px; padding: 6px; background-color: #1e1e1e; color: white; border: none;");

    QWidget* central = new QWidget(this);
    QGridLayout* grid = new QGridLayout(central);

    const QString labels[5][4] =
    {
        {"(", ")", "C", "/"},
        {"7", "8", "9", "*"},
        {"4", "5", "6", "-"},
        {"1", "2", "3", "+"},
        {"0", ".", "=", "%"}
    };

    for (int r = 0; r < 5; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            const QString& text = labels[r][c];
            QPushButton* btn = new QPushButton(text, central);
            btn->setFixedSize(50, 50);
            btn->setStyleSheet("font-size: 18px; background-color: #2d2d2d; color: white;");
            grid->addWidget(btn, r, c);
            if (text == "C")
                connect(btn, &QPushButton::clicked, this, &MainWindow::onClearClicked);
            else
                connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
        }
    }

    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(expressionDisplay);
    vbox->addWidget(display);
    vbox->addWidget(central);

    QWidget* container = new QWidget(this);
    container->setLayout(vbox);
    setCentralWidget(container);
}

void MainWindow::onButtonClicked()
{
    auto* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QString key = btn->text();

    if (key == "=")
    {
        QString expression = expressionDisplay->text();
        bool ok = true;
        double result = evaluateExpression(expression, &ok);
        if (ok) {
            display->setText(QString::number(result));
            expressionDisplay->setText(expression + " = " + QString::number(result));
        }
        else {
            display->setText("Impossible");
            expressionDisplay->setText(expression + " = Impossible");
        }
        waitingForOperand = true;
    }
    else
    {
        if (waitingForOperand && (key[0].isDigit() || key == "."))
        {
            expressionDisplay->clear();
            waitingForOperand = false;
        }
        expressionDisplay->setText(expressionDisplay->text() + key);
        display->setText(key);
    }
}

void MainWindow::onClearClicked()
{
    display->setText("0");
    expressionDisplay->clear();
    waitingForOperand = false;
}

int MainWindow::precedence(QChar op)
{
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    return 0;
}

double MainWindow::applyOp(double a, double b, QChar op, bool* ok)
{
    switch (op.toLatin1())
    {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0.0) {
            *ok = false;
            return 0;
        }
        return a / b;
    case '%':
        if (b == 0.0) {
            *ok = false;
            return 0;
        }
        return std::fmod(a, b);
    default:
        *ok = false;
        return 0;
    }
}

double MainWindow::evaluateExpression(const QString& expr, bool* ok)
{
    QStack<double> values;
    QStack<QChar> ops;
    QString num;
    *ok = true;

    for (int i = 0; i < expr.length(); ++i)
    {
        QChar ch = expr[i];

        if (ch.isDigit() || ch == '.')
        {
            num += ch;
        }
        else if (ch.isSpace())
        {
            continue;
        }
        else
        {
            if (!num.isEmpty())
            {
                values.push(num.toDouble());
                num.clear();
            }

            if (ch == '(')
            {
                ops.push(ch);
            }
            else if (ch == ')')
            {
                while (!ops.isEmpty() && ops.top() != '(')
                {
                    if (values.size() < 2) { *ok = false; return 0; }
                    double b = values.pop();
                    double a = values.pop();
                    QChar op = ops.pop();
                    values.push(applyOp(a, b, op, ok));
                    if (!*ok) return 0;
                }
                if (!ops.isEmpty()) ops.pop();
                else { *ok = false; return 0; }
            }
            else if (precedence(ch) > 0)
            {
                while (!ops.isEmpty() && precedence(ops.top()) >= precedence(ch))
                {
                    if (values.size() < 2) { *ok = false; return 0; }
                    double b = values.pop();
                    double a = values.pop();
                    QChar op = ops.pop();
                    values.push(applyOp(a, b, op, ok));
                    if (!*ok) return 0;
                }
                ops.push(ch);
            }
            else {
                *ok = false;
                return 0;
            }
        }
    }

    if (!num.isEmpty())
    {
        values.push(num.toDouble());
    }

    while (!ops.isEmpty())
    {
        if (values.size() < 2) { *ok = false; return 0; }
        double b = values.pop();
        double a = values.pop();
        QChar op = ops.pop();
        values.push(applyOp(a, b, op, ok));
        if (!*ok) return 0;
    }

    return values.isEmpty() ? 0 : values.top();
}
