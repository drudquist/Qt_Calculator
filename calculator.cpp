#include "calculator.h"
#include "ui_calculator.h"

#include <QMessageBox>

#include <map>

namespace {

constexpr int numOfNumberButtons{10};

double calcVal{0.0};
double memVal{0.0};

std::map<const QPushButton*, bool> triggeredOperators;

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
    triggeredOperators.emplace(ui->Divide, false);
    triggeredOperators.emplace(ui->Multiply, false);
    triggeredOperators.emplace(ui->Add, false);
    triggeredOperators.emplace(ui->Subtract, false);
}

void Calculator::ResetOperatorTriggered()
{
    for(auto& triggeredOperator : triggeredOperators)
    {
        triggeredOperator.second = false;
    }
}

void Calculator::NumPressed()
{
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
    ResetOperatorTriggered();

   const QPushButton* const button = static_cast<const QPushButton*>(sender());

   //C++17 if staement with initializer
   if(auto iter = triggeredOperators.find(button); iter != triggeredOperators.end())
   {
       iter->second = true;
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

bool Calculator::AnyOperatorTriggered()
{
    for(const auto& triggeredOperator : triggeredOperators)
    {
        if(triggeredOperator.second)
        {
            return true;
        }
    }

    return false;
}

void Calculator::EqualButtonPressed()
{
    const QPushButton* const button = static_cast<const QPushButton*>(sender());

    if(button == ui->Equal)
    {
        const QString displayVal = ui->Display->text();
        const double dblDisplayVal = displayVal.toDouble();

        if(AnyOperatorTriggered())
        {
            if(triggeredOperators.at(ui->Divide))
            {
                if(dblDisplayVal == 0.0)
                {
                    //set display to previous calculated value
                    QMessageBox::warning(this, "Warning", "Cannot divide by zero");
                    ui->Display->setText(QString::number(calcVal));
                    return;
                }
                else
                {
                    calcVal /= dblDisplayVal;
                }
            }
            else if(triggeredOperators.at(ui->Multiply))
            {
                calcVal *= dblDisplayVal;
            }
            else if(triggeredOperators.at(ui->Add))
            {
                calcVal += dblDisplayVal;
            }
            else
            {
                calcVal -= dblDisplayVal;
            }

            ui->Display->setText(QString::number(calcVal));
            const QString prevDisplayVal = ui->DisplayPrevious->text();
            ui->DisplayPrevious->setText(prevDisplayVal + " " + displayVal);
            calcVal = 0.0;
        }
        else
        {
            QMessageBox::warning(this, "Warning", "Cannot calculate new value without an operator");
        }

    }
    else
    {
        QMessageBox::warning(this, "Warning", "Button: " + button->text() + " is not the equals button.");
        return;
    }
}

void Calculator::ChangeSignButtonPressed()
{
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
}

void Calculator::MemoryClearButtonPressed()
{
    memVal = 0.0;
}

void Calculator::MemoryGetButtonPressed()
{
    ui->Display->setText(QString::number(memVal));
}
