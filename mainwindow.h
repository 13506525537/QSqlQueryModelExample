#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlRecord>
#include <datadialog.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadPerson();

    // 获取部门信息
    void loadDepart();

    // 获取空表头
    void getEmptyRecord();

    // 获取需要更新的数据
    void getUpdateData(int CurNo);

    void changeButtonState(bool flag);

private slots:
    void on_actionOpen_triggered();

    void on_actionInsert_triggered();

    void on_actionEdit_triggered();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_actionClose_triggered();

    void on_actionRemove_triggered();

private:
    Ui::MainWindow *ui;

    // 查询模型
    QSqlQueryModel * model;
    QItemSelectionModel* selection;
    QSqlDatabase db;

    QMap<int, QString> departMap;

    // 弹出弹窗
    DataDialog * dialog = NULL;

    // 记录空的记录作为表头
    QSqlRecord m_emptyRecord;

};
#endif // MAINWINDOW_H
