#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->tableView);

    // 先创建对象
    model = new QSqlQueryModel(this);
    selection = new QItemSelectionModel(model);

    ui->tableView->setModel(model);
    ui->tableView->setSelectionModel(selection);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadPerson()
{
    model->setQuery("select * from info;");
    if (model->lastError().isValid())
    {
        QMessageBox::warning(this, "提示", "查询失败:\n" + model->lastError().text());
        return;
    }

    // 这个必须放在查出数据之后
    QStringList strList = {"ID", "姓名", "年龄", "性别", "部门ID"};
    for (int i =0;i < strList.count(); i++) {
        model->setHeaderData(i, Qt::Horizontal, strList[i]);
    }
}

void MainWindow::loadDepart()
{
    QSqlQuery query;
    query.exec("select id, departName from department;");
    while(query.next()){
        departMap.insert(query.value("id").toInt(), query.value("departName").toString());
    }
}

void MainWindow::getEmptyRecord()
{
    QSqlQuery query;
    query.exec("select * from info where 1=0;");
    m_emptyRecord = query.record();
}

void MainWindow::getUpdateData(int curNo)
{
    QSqlRecord curRecord = model->record(curNo);

    QString idNo = curRecord.value("id").toString();
    qDebug() << curNo;
    // 查询数据库判断是否存在这一行
    QSqlQuery query;
    query.prepare("select * from info where id=?");
    query.addBindValue(idNo);
    query.exec();
    query.first(); // 指向第一行
    if (!query.isValid()){
        return;
    }
    curRecord = query.record();

    // 创建弹窗
    DataDialog* dialog = new DataDialog(this);
    dialog->setDepartItem(departMap);
    dialog->initUpdateRecord(curRecord);

    // 展示
    int ret = dialog->exec();
    if (ret == QDialog::Accepted){
        QSqlRecord newRecord = dialog->getInsetRecord();
        query.prepare("update info set name=?, gender=?, age=?, departId=? where id=?");
        query.addBindValue(newRecord.value("name"));
        query.addBindValue(newRecord.value("gender"));
        query.addBindValue(newRecord.value("age"));
        query.addBindValue(newRecord.value("departId"));
        query.addBindValue(newRecord.value("id"));
        if (query.exec()){
            model->setQuery("select * from info order by id;");
        }
        else{
            QMessageBox::warning(this, "提示", "更新失败\n"+ query.lastError().text());
        }
    }
}

void MainWindow::changeButtonState(bool flag)
{
    ui->actionOpen->setEnabled(flag);
    ui->actionClose->setEnabled(!flag);
    ui->actionInsert->setEnabled(!flag);
    ui->actionEdit->setEnabled(!flag);
    ui->actionRemove->setEnabled(!flag);
}


void MainWindow::on_actionOpen_triggered()
{
    // 创建数据库连接对象
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("person");
    if (!db.open())
    {
        QMessageBox::warning(this, "提示", "数据库打开失败:\n" + db.lastError().text());
        return;
    }
    else{
        qDebug() << "Sql connect";
    }
    loadPerson();
    loadDepart();

    // 更改按钮状态
    changeButtonState(false);
}


void MainWindow::on_actionInsert_triggered()
{
    // 获取空记录
    getEmptyRecord();

    QSqlQuery query;

    dialog = new DataDialog(this);
    dialog->initInsetRecord(m_emptyRecord);
    dialog->setDepartItem(departMap);

    int ret = dialog->exec();
    if (ret == QDialog::Accepted){

        QSqlRecord record = dialog->getInsetRecord();
        query.prepare("insert into info (id, name, age, gender, departId) values(:ID, :name, :age, :gender, :departId);");

        query.bindValue(":ID", record.value("id").toString());
        query.bindValue(":name", record.value("name").toString());
        query.bindValue(":age", record.value("age").toInt());
        query.bindValue(":gender", record.value("gender").toString());
        query.bindValue(":departId", record.value("departId").toInt());
        bool result = query.exec();
        if (result){
            model->setQuery("select * from info order by id;");
        }
        else{
            QMessageBox::warning(this, "警告", "插入失败\n" + query.lastError().text());
        }

    }

    delete dialog;
    dialog = nullptr;
}

// 点击更新按钮，获取当前更新的项目
void MainWindow::on_actionEdit_triggered()
{
    QModelIndex index = selection->currentIndex();
    if (!index.isValid()){
        QMessageBox::warning(this, "提示", "请先选择数据" );
    }
    int curNo = index.row();
    getUpdateData(curNo);
}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    if (index.row() == -1)
    {
        QMessageBox::warning(this, "提示", "请先选择数据" );
    }
    getUpdateData(index.row());
}


void MainWindow::on_actionClose_triggered()
{
    db.close();
    db= QSqlDatabase(); // 将db设为空连接对象

    model->clear();
    changeButtonState(true);
}


void MainWindow::on_actionRemove_triggered()
{
    int curNo = selection->currentIndex().row();
    qDebug() << curNo;
    if (curNo == -1){
        QMessageBox::information(this, "提示", "请先选择数据");
        return;
    }
    QSqlQuery query;
    query.prepare("delete from info where id=?");
    query.addBindValue(model->record(curNo).value("id"));
    if (query.exec())
    {
        model->setQuery("select * from info order by id;");
    }
    else {
        QMessageBox::warning(this, "提示", "删除失败: \n" + query.lastError().text());
    }
}

