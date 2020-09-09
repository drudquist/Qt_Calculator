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

    bool OperatorTriggered();

    void ConnectOperatorButtons();
    void ConnectNumberButtons();
    void ConnectButtons();

private slots:
    void NumPressed();
    void OperatorButtonPressed();
    void EqualButtonPressed();
    void ChangeSignButtonPressed();
    void ClearButtonPressed();
};
#endif // CALCULATOR_H
