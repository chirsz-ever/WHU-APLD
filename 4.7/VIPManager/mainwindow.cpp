#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgaddvip.h"
#include "ui_dlgaddvip.h"
#include "discount_calc.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QTime>

MainWindow::MainWindow(QWidget *parent, int _salesman_id) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    salesman_id(_salesman_id)
{
    ui->setupUi(this);

    openDataBase();

    setTableView();

    ui->menuBar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("vipmanage.db3");
    db.open();
    db.exec(R"(
    CREATE TABLE IF NOT EXISTS [salesman](
            [cnid] VARCHAR(18) PRIMARY KEY NOT NULL UNIQUE,
            [name] VARCHAR(10) NOT NULL UNIQUE COLLATE NOCASE,
            [password] CHAR(32) NOT NULL COLLATE NOCASE,
            [is_admin] BOOL))");
    db.exec(R"(
    CREATE TABLE IF NOT EXISTS [vip](
            [vipid] VARCHAR(10) PRIMARY KEY NOT NULL,
            [name] TEXT,
            [phone] VARCHAR(15),
            [points] INTEGER NOT NULL DEFAULT 0,
            CHECK([POINTS] >= 0));)");
    db.exec(R"(
    CREATE TABLE IF NOT EXISTS [sale_record](
            [vipid] VARCHAR(10) NOT NULL REFERENCES [vip]([vipid]) ON DELETE CASCADE,
            [salesman_id] VARCHAR(18) NOT NULL REFERENCES [salesman]([cnid]),
            [amount] CURRENCY NOT NULL,
            [pay] CURRENCY NOT NULL,
            [time] TIMESTAMP NOT NULL);)");
    db.exec(R"(CREATE TABLE IF NOT EXISTS [discount_rule]([rule] TEXT NOT NULL);)");
    db.exec(R"(
CREATE VIEW [accinfo](
[vipid],
[accamount],
[accpay],
[accbenefit])
AS
SELECT DISTINCT
              [vipid],
              SUM ([amount]) AS [accamount],
              SUM ([pay]) AS [accpay],
              SUM ([amount]) - SUM ([pay]) AS [accbenefit]
FROM   [sale_record]
            GROUP  BY [vipid];)");
}

void MainWindow::flushModel()
{
    model->setQuery("SELECT vip.vipid,vip.name,vip.phone,vip.points,IFNULL(accbenefit,0) FROM vip LEFT JOIN accinfo ON vip.vipid=accinfo.vipid",db);
}

void MainWindow::setTableView()
{
    model = new QSqlQueryModel(ui->tableView);
    flushModel();
    model->setHeaderData(0,Qt::Horizontal,QObject::tr(u8"会员号"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr(u8"姓名"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr(u8"电话号码"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr(u8"会员积分"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr(u8"累计折扣金额"));

    qDebug()<< model->query().lastError();
    ui->tableView->setModel(model);

    mapper = new QDataWidgetMapper(this);
    //mapper->setSubmitPolicy(QDataWidgetMapper::SubmitPolicy::AutoSubmit);
    mapper->setModel(model);
    mapper->addMapping(ui->t2_vipName,1,"text");
    mapper->addMapping(ui->t2_vipPhone,2,"text");
    mapper->addMapping(ui->t2_vipPoint,3,"text");
    mapper->addMapping(ui->t2_accbenefit,4,"text");
    connect(ui->tableView,&QTableView::clicked,mapper,&QDataWidgetMapper::setCurrentModelIndex);
}

void MainWindow::on_btn_deleteVIP_clicked()
{
    int row = mapper->currentIndex();
    auto vipid = model->record(row).value("vipid").toString();
    auto query = db.exec("DELETE FROM vip WHERE vipid='"+vipid+"'");
    qDebug()<<query.lastQuery()<<query.lastError();
    flushModel();
}

void MainWindow::on_btn_addVIP_clicked()
{
    DlgAddVIP dlg_AddVIP;
    if(QDialog::Accepted != dlg_AddVIP.exec())
        return;

    QSqlQuery query(db);
    query.exec("SELECT max(vipid)+1 FROM vip;");
    query.first();
    QString new_vipid = QString("'%1'").arg(query.value(0).toInt(),10,10,QLatin1Char('0'));

    QString o_vipName = dlg_AddVIP.ui->vipName->text();
    QString vipName = (o_vipName=="")?"NULL":QString("'%1'").arg(o_vipName);

    QString o_vipPhone = dlg_AddVIP.ui->vipPhone->text();
    QString vipPhone = (o_vipPhone=="")?"NULL":QString("'%1'").arg(o_vipPhone);

    QString values = QString("VALUES(%1,%2,%3)").arg(new_vipid).arg(vipName).arg(vipPhone);
    query.exec("INSERT INTO vip(vipid, name, phone) "+values);

    qDebug()<<query.lastQuery()<<query.lastError();

    flushModel();
}

void MainWindow::on_btn_modifyVIP_clicked()
{
    int row = mapper->currentIndex();
    DlgAddVIP dlg_AddVIP;
    dlg_AddVIP.ui->vipName->setText(ui->t2_vipName->text());
    dlg_AddVIP.ui->vipPhone->setText(ui->t2_vipPhone->text());
    if(QDialog::Accepted != dlg_AddVIP.exec())
        return;

    QString vipid = model->record(mapper->currentIndex()).value("vipid").toString();
    QSqlQuery query(db);

    QString o_vipName = dlg_AddVIP.ui->vipName->text();
    QString vipName = (o_vipName=="")?"NULL":QString("'%1'").arg(o_vipName);

    QString o_vipPhone = dlg_AddVIP.ui->vipPhone->text();
    QString vipPhone = (o_vipPhone=="")?"NULL":QString("'%1'").arg(o_vipPhone);

    query.exec(QString("UPDATE vip SET name=%1,phone=%2 WHERE vipid='%3'").arg(vipName,vipPhone,vipid));

    qDebug()<<query.lastQuery()<<query.lastError();

    flushModel();
    mapper->setCurrentIndex(row);
}

void MainWindow::on_tableView_pressed(const QModelIndex &index)
{
    ui->tableView->selectRow(index.row());
}

void MainWindow::on_t1_vipId_textChanged(const QString &arg1)
{
    QString vipid = QString("'%1'").arg(arg1,10,QLatin1Char('0'));
    auto query = db.exec("SELECT name,points FROM vip WHERE vipid="+vipid);
    if(!query.first())
    {
        qDebug()<<query.size()<<query.lastError();
        ui->t1_vipName->clear();
        ui->t1_vipPoint->clear();
        ui->t1_vipName->setProperty("UserExists",false);
        return;
    }
    ui->t1_vipName->setText(query.value("name").toString());
    ui->t1_vipPoint->setText(query.value("points").toString());
    ui->t1_vipName->setProperty("UserExists",true);

}


void MainWindow::on_t1_originPay_textChanged(const QString &arg1)
{
    double amount = arg1.toDouble();
    ui->t1_realPay->setText(QString::asprintf("%.2f", discount_calc(amount,ui->t1_vipPoint->text().toInt())));
}


void MainWindow::on_btn_confirm_purchase_clicked()
{
    QString vipid = QString("%1").arg(ui->t1_vipId->text(),10,QLatin1Char('0'));
    double amount = ui->t1_originPay->text().toDouble();
    if(!ui->t1_vipName->property("UserExists").toBool())
    {
        QMessageBox::warning(this,u8"错误",QString(u8"会员号%1不存在").arg(vipid));
        return;
    }
    if(ui->t1_originPay->text().isEmpty())
    {
        QMessageBox::warning(this,u8"错误",QString(u8"请输入消费额！"));
        return;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO sale_record VALUES(?,?,?,?,?)");
    query.addBindValue(vipid);
    query.addBindValue("000000000000000000");
    query.addBindValue(amount);
    query.addBindValue(discount_calc(amount,ui->t1_vipPoint->text().toInt()));
    query.addBindValue(QTime::currentTime());
    if(!query.exec())
    {
        qDebug()<<query.lastError();
    }

    if(!query.exec(QString("UPDATE vip SET points=points+%1 WHERE vipid='%2'").arg(int(amount)/10).arg(vipid)))
    {
        qDebug()<<query.lastError();
    }
    on_t1_vipId_textChanged(ui->t1_vipId->text());
    flushModel();
    ui->t1_originPay->clear();
}
