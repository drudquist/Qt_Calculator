#include "calculator.h"
#include "ui_calculator.h"

#include <QMessageBox>

namespace {

bool divTriggered{false};
bool multiTriggered{false};
bool addTrigggered{false};
bool subTriggered{false};

constexpr int numOfNumberButtons{10};

double calcVal{0.0};

QPushButton* numButtons[numOfNumberButtons];
}

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcVal));

    ConnectButtons();
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
    const QString displayVal(ui->Display->text());
    calcVal = displayVal.toDouble();

    const QPushButton* const button = static_cast<const QPushButton*>(sender());

   if(button == ui->Divide)
   {
       divTriggered = true;
   }
   else if(button == ui->Multiply)
   {
       multiTriggered = true;
   }
   else if(button == ui->Add)
   {
       addTrigggered = true;
   }
   else if(button == ui->Subtract)
   {
       subTriggered = true;
   }
   else
   {
       QMessageBox::warning(this, "Warning", "Button: " + button->text() + " is not a valid operator.");
       return;
   }

   ui->Display->setText("");
}
