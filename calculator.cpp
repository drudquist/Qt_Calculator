#include "calculator.h"
#include "ui_calculator.h"

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

    for( int i = 0; i < numOfNumberButtons; ++i)
    {
        QString buttonName = QString("Button") + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton*>(buttonName);
        connect(numButtons[i], SIGNAL(clicked()), this, SLOT(NumPressed()));
    }
}

Calculator::~Calculator()
{
    delete ui;
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

