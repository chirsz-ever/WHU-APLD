#include "dlgaddvip.h"
#include "ui_dlgaddvip.h"

DlgAddVIP::DlgAddVIP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddVIP)
{
    ui->setupUi(this);
}

DlgAddVIP::~DlgAddVIP()
{
    delete ui;
}
