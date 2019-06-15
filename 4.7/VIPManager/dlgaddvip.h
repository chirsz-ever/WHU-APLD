#ifndef DLGADDVIP_H
#define DLGADDVIP_H

#include <QDialog>

namespace Ui {
class DlgAddVIP;
}

class DlgAddVIP : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAddVIP(QWidget *parent = nullptr);
    ~DlgAddVIP();

private:
    Ui::DlgAddVIP *ui;

    friend class MainWindow;
};

#endif // DLGADDVIP_H
