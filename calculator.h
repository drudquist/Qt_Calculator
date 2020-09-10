#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;

    bool DividingByZero(double val);
    bool AnyOperatorTriggered();

    double CalculateValue(double lhs, double rhs);

    void ConnectOperatorButtons();
    void ConnectNumberButtons();
    void ConnectButtons();
    void InitializeTriggeredOperators();
    void ResetTriggeredOperators();
    void ClearStatusBar();

private slots:
    void NumPressed();
    void OperatorButtonPressed();
    void EqualButtonPressed();
    void ChangeSignButtonPressed();
    void ClearButtonPressed();
    void MemoryAddButtonPressed();
    void MemoryClearButtonPressed();
    void MemoryGetButtonPressed();
};
#endif // CALCULATOR_H
