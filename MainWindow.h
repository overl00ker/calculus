#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QString>
#include <QStack>
#include <QChar>
#include <QApplication>

class MainWindow : public QMainWindow 
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onButtonClicked();
    void onClearClicked();

private:
    QLineEdit* display;
    QLineEdit* expressionDisplay;
    bool waitingForOperand = false;
    double operand1 = 0;
    QString pendingOp;

    double evaluateExpression(const QString& expr, bool* ok);
    double applyOp(double a, double b, QChar op, bool* ok);
    int precedence(QChar op);
};
