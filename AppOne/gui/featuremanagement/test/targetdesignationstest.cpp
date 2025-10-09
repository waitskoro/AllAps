#include "targetdesignationstest.h"
#include "ui_targetdesignationstest.h"

TargetDesignationsTest::TargetDesignationsTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TargetDesignationsTest)
{
    ui->setupUi(this);
}

TargetDesignationsTest::~TargetDesignationsTest()
{
    delete ui;
}
