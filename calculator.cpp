#include "calculator.h"
#include "ui_calculator.h"

#include <QMessageBox>

#include <map>

namespace {

struct OperatorInfo {
    bool triggered{false};
    std::function<double(double lhs, double rhs)> operation;
};

constexpr int numOfNumberButtons{10};

double calcVal{0.0};
double memVal{0.0};

std::map<const QPushButton*, OperatorInfo> operatorInfos;

QPushButton* numButtons[numOfNumberButtons];
}

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcVal));
    ui->DisplayPrevious->setText("");

    ConnectButtons();
    InitializeTriggeredOperators();
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::ConnectOperatorButtons()
{
    const auto signal = SIGNAL(clicked());
    const auto slot = SLOT(OperatorButtonPressed());

    connect(ui->Divide, signal, this, slot);
    connect(ui->Multiply, signal, this, slot);
    connect(ui->Add, signal, this, slot);
    connect(ui->Subtract, signal, this, slot);
}

void Calculator::ConnectNumberButtons()
{
    for( int i = 0; i < numOfNumberButtons; ++i)
    {
        QString buttonName = QString("Button") + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton*>(buttonName);
        connect(numButtons[i], SIGNAL(clicked()), this, SLOT(NumPressed()));
    }
}

void Calculator::ConnectButtons()
{
    ConnectNumberButtons();
    ConnectOperatorButtons();

    connect(ui->Equal, SIGNAL(clicked()), this, SLOT(EqualButtonPressed()));
    connect(ui->ChangeSign, SIGNAL(clicked()), this, SLOT(ChangeSignButtonPressed()));
    connect(ui->Clear, SIGNAL(clicked()), this, SLOT(ClearButtonPressed()));

    connect(ui->MemAdd, SIGNAL(clicked()), this, SLOT(MemoryAddButtonPressed()));
    connect(ui->MemClear, SIGNAL(clicked()), this, SLOT(MemoryClearButtonPressed()));
    connect(ui->MemGet, SIGNAL(clicked()), this, SLOT(MemoryGetButtonPressed()));
}

void Calculator::InitializeTriggeredOperators()
{
    operatorInfos.emplace(ui->Divide, OperatorInfo{false, [](const double lhs, const double rhs) { return lhs / rhs;} } );
    operatorInfos.emplace(ui->Multiply, OperatorInfo{false, [](const double lhs, const double rhs) { return lhs * rhs; } } );
    operatorInfos.emplace(ui->Add, OperatorInfo{false, [](const double lhs, const double rhs) { return lhs + rhs; } } );
    operatorInfos.emplace(ui->Subtract, OperatorInfo{false, [](const double lhs, const double rhs) { return lhs - rhs; } } );
}

void Calculator::ResetTriggeredOperators()
{
    for(auto& triggeredOperator : operatorInfos)
    {
        triggeredOperator.second.triggered = false;
    }
}

void Calculator::ClearStatusBar()
{
    ui->statusbar->showMessage("");
}

void Calculator::NumPressed()
{
    ClearStatusBar();

    const QPushButton* const button = static_cast<const QPushButton*>(sender());
    const QString buttonVal = button->text();
    const QString displayVal = ui->Display->text();

    if(displayVal.toDouble() == 0 ||  displayVal.toDouble() ==  0.0)
    {
        ui->Display->setText(buttonVal);
    }
    else
    {
        const QString newVal(displayVal + buttonVal);
        const double dblNewVal{newVal.toDouble()};
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void Calculator::OperatorButtonPressed()
{
    ResetTriggeredOperators();
    ClearStatusBar();

   const QPushButton* const button = static_cast<const QPushButton*>(sender());

   //C++17 if staement with initializer
   if(auto iter = operatorInfos.find(button); iter != operatorInfos.end())
   {
       iter->second.triggered = true;
   }
   else
   {
       QMessageBox::warning(this, "Warning", "Button: " + button->text() + " is not a valid operator.");
       return;
   }

   const QString displayVal(ui->Display->text());
   calcVal = displayVal.toDouble();

   ui->Display->setText("");
   ui->DisplayPrevious->setText(displayVal + " " + button->text());
}

bool Calculator::DividingByZero(const double val)
{
    if(operatorInfos.at(ui->Divide).triggered)
    {
        return val == 0.0;
    }

    return false;
}

bool Calculator::AnyOperatorTriggered()
{
    for(const auto& triggeredOperator : operatorInfos)
    {
        if(triggeredOperator.second.triggered)
        {
            return true;
        }
    }

    return false;
}

double Calculator::CalculateValue(const double lhs, const double rhs)
{
    if(DividingByZero(rhs))
    {
        QMessageBox::warning(this, "Warning", "Cannot divide by zero");
        return lhs;
    }

    for(const auto& operatorInfo : operatorInfos)
    {
        if(operatorInfo.second.triggered)
        {
            return operatorInfo.second.operation(lhs, rhs);
        }
    }

    QMessageBox::warning(this, "Warning", "No operator set. returning value: " + QString::number(lhs));
    return lhs;
}

void Calculator::EqualButtonPressed()
{
    ClearStatusBar();

    const QPushButton* const button = static_cast<const QPushButton*>(sender());

    if(button == ui->Equal)
    {
        const QString displayVal = ui->Display->text();
        const double dblDisplayVal = displayVal.toDouble();

        if(AnyOperatorTriggered())
        {
            calcVal = CalculateValue(calcVal, dblDisplayVal);

            ui->Display->setText(QString::number(calcVal));

            if(!DividingByZero(dblDisplayVal))
            {
                const QString prevDisplayVal = ui->DisplayPrevious->text();
                ui->DisplayPrevious->setText(prevDisplayVal + " " + displayVal);
                calcVal = 0.0;
            }
            else
            {
                ui->DisplayPrevious->setText("");
            }

            ResetTriggeredOperators();
        }
        else
        {
            QMessageBox::warning(this, "Warning", "Cannot calculate new value without an operator");
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Button: " + button->text() + " is not the equals button.");
    }
}

void Calculator::ChangeSignButtonPressed()
{
    ClearStatusBar();

    const QString displayVal = ui->Display->text();
    QRegExp reg("[-]?[0-9.]*");

    if(reg.exactMatch(displayVal))
    {
        double dblDisplayVal = displayVal.toDouble();
        dblDisplayVal *= -1;
        ui->Display->setText(QString::number(dblDisplayVal));
    }
}

void Calculator::ClearButtonPressed()
{
    ClearStatusBar();

    const QPushButton* const button = static_cast<const QPushButton*>(sender());

    if(button == ui->Clear)
    {
        calcVal = 0.0;
        ui->Display->setText(QString::number(calcVal));
        ui->DisplayPrevious->setText("");
    }
}

void Calculator::MemoryAddButtonPressed()
{
    const QString displayVal = ui->Display->text();
    memVal = displayVal.toDouble();

    ui->statusbar->showMessage("Value: '" + displayVal + "' saved to memory." );
}

void Calculator::MemoryClearButtonPressed()
{
    memVal = 0.0;
    ui->statusbar->showMessage("Memory cleared.");
}

void Calculator::MemoryGetButtonPressed()
{
    ui->Display->setText(QString::number(memVal));
    ui->statusbar->showMessage("Memory returned value: '" + QString::number(memVal) + "'.");
}
