#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <QDialog>
#include <QMap>
#include <QDebug>
#include <QSqlRecord>

namespace Ui {
class DataDialog;
}

class DataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataDialog(QWidget *parent = nullptr);
    ~DataDialog();

    void setDepartItem(QMap<int, QString>& map);

    // 插入数据时初始化
    void initInsetRecord(QSqlRecord& record);

    // 更新数据初始化
    void initUpdateRecord(QSqlRecord& record);

    QSqlRecord getInsetRecord();

private:
    Ui::DataDialog *ui;

    QSqlRecord m_record;

    QMap<int, QString> deptMap;
};

#endif // DATADIALOG_H
