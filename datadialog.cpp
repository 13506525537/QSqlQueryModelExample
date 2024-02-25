#include "datadialog.h"
#include "ui_datadialog.h"

DataDialog::DataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataDialog)
{
    ui->setupUi(this);
}

DataDialog::~DataDialog()
{
    delete ui;
}

void DataDialog::setDepartItem(QMap<int, QString> &map)
{
    deptMap = map;
    ui->cbDepart->clear();
    foreach(int key, map.keys()){
        ui->cbDepart->addItem(map.value(key), key);
    }
}

void DataDialog::initInsetRecord(QSqlRecord &record)
{
    m_record = record;
    setWindowTitle("添加记录");
}

void DataDialog::initUpdateRecord(QSqlRecord &record)
{
    setWindowTitle("更新记录");
    m_record = record;
    ui->leID->setReadOnly(true);

    // 设置各个格子
    ui->leID->setText(record.value("id").toString());
    ui->leName->setText(record.value("name").toString());
    ui->spinBoxAge->setValue(record.value("age").toInt());
    ui->cbGender->setCurrentText(record.value("gender").toString());
    ui->cbDepart->setCurrentText(deptMap.value(record.value("departId").toInt()));
}

QSqlRecord DataDialog::getInsetRecord()
{
    m_record.setValue("id", ui->leID->text());
    m_record.setValue("name", ui->leName->text());
    m_record.setValue("gender", ui->cbGender->currentText());
    m_record.setValue("departId", ui->cbDepart->currentData());
    qDebug() << "departId" << ui->cbDepart->currentData();
    m_record.setValue("age", ui->spinBoxAge->value());
    return m_record;
}
