#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDataWidgetMapper>
#include <QSqlQueryModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, int _salesman_id = 0);
    ~MainWindow();

public slots:
    void on_tableView_pressed(const QModelIndex &index);

    void on_btn_deleteVIP_clicked();

    void on_btn_addVIP_clicked();

    void on_btn_modifyVIP_clicked();


private slots:
    void on_t1_vipId_textChanged(const QString &arg1);

    void on_btn_confirm_purchase_clicked();

    void on_t1_originPay_textChanged(const QString &arg1);

private:
    void openDataBase();
    void flushModel();
    void setTableView();

private:
    Ui::MainWindow *ui;
    const int salesman_id;
    QSqlDatabase db;
    QSqlQueryModel *model = nullptr;
    QDataWidgetMapper *mapper = nullptr;
};

#endif // MAINWINDOW_H
