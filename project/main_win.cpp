#include "main_win.h"
#include "ui_main_win.h"

#include <QMessageBox>
#include <QDateTime>
#include <QSqlError>

main_win::main_win(QWidget *parent, int user_id, QString username) :
    QWidget(parent),
    ui(new Ui::main_win),
    current_user_id(user_id),
    current_username(username),
    current_edit_note_id(-1)
{
    ui->setupUi(this);

    // 初始化欢迎标签
    if (!current_username.isEmpty()) {
        ui->welcome_lb->setText(current_username + "的私密空间");
    }

    // 初始化左侧便签列表 UI
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->hide(); // 隐藏表头
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止直接在表格里双击编辑

    // 获取并检测数据库
    db = QSqlDatabase::database(); // 获取已在 login 连好的默认数据库
    if (!db.isOpen())
    {
        qDebug() << "数据库未连接！";
        QMessageBox::critical(this, "错误", "数据库未连接！");
        return;
    }

    // 设置双击单元格事件连接
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &main_win::on_tableWidget_cellDoubleClicked);

    // 启动时读取当前用户的所有便签显示在左侧列表
    loadUserNotes();
}

main_win::~main_win()
{
    delete ui;
}

// 刷新并加载用户的所有便签
void main_win::loadUserNotes()
{
    if (!db.isOpen()) return;

    // 清空现有列表
    ui->tableWidget->setRowCount(0);

    QSqlQuery query(db);
    // 按时间倒序查找当前用户的所有日记
    query.prepare("SELECT note_id, title, update_time FROM notes WHERE user_id = :user_id ORDER BY update_time DESC");
    query.bindValue(":user_id", current_user_id);
    
    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            QString title = query.value(1).toString();
            QString time = query.value(2).toString();
            
            ui->tableWidget->insertRow(row);
            // 将标题和更新时间拼接作为列表项显示
            QTableWidgetItem *item = new QTableWidgetItem(title + "\n" + time);
            item->setData(Qt::UserRole, query.value(0).toInt()); // 把真实的 note_id 藏在 item 里
            ui->tableWidget->setItem(row, 0, item);
            row++;
        }
    } else {
        qDebug() << "加载便签失败：" << query.lastError().text();
    }
}

//新建标签 (发一条新日记并保存到数据库)
void main_win::on_add_bt_clicked()
{
    // 1. 获取你输入的文本信息
    QString content = ui->textEdit->toPlainText().trimmed();
    if(content.isEmpty()) {
        QMessageBox::warning(this, "提示", "总得写点什么吧~");
        return;
    }

    // 2. 截取正文第一行当做当前日记的“标题” (如果太长就截断加省略号)
    QString title = content.split('\n').first();
    if(title.length() > 15) {
        title = title.left(15) + "..."; 
    }

    // 3. 构建 SQL Insert 或 Update 语句
    QSqlQuery query(db);
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    if (current_edit_note_id == -1) 
    {
        // 属于新建写的新便签，执行 INSERT
        query.prepare("INSERT INTO notes (user_id, title, content, create_time, update_time) "
                      "VALUES (:user_id, :title, :content, :create_time, :update_time)");
        query.bindValue(":user_id", current_user_id);
        query.bindValue(":title", title);
        query.bindValue(":content", content);
        query.bindValue(":create_time", currentTime);
        query.bindValue(":update_time", currentTime);
    }
    else 
    {
        // 点击现有便签正在进行覆盖编写，执行 UPDATE 更新
        query.prepare("UPDATE notes SET title = :title, content = :content, update_time = :update_time "
                      "WHERE note_id = :note_id AND user_id = :user_id");
        query.bindValue(":title", title);
        query.bindValue(":content", content);
        query.bindValue(":update_time", currentTime);
        query.bindValue(":note_id", current_edit_note_id);
        query.bindValue(":user_id", current_user_id);
    }

    // 4. 执行SQL语句写入数据库
    if(!query.exec())
    {
        qDebug() << "保存便签失败: " << query.lastError().text();
        QMessageBox::critical(this, "错误", "保存失败！");
    }
    else 
    {
        qDebug() << "保存成功！";
        QMessageBox::information(this, "成功", "私密便签已安全保存！");
        
        // 5. 保存成功后清空文本框，重新加载左侧的列表，并且退回“新建”模式
        current_edit_note_id = -1; // 重置标识为 -1，代表接下来再写又是新建
        ui->textEdit->clear(); 
        loadUserNotes();       
    }
}
//锁定便签
void main_win::on_lock_bt_clicked()
{

}
//搜索便签
void main_win::on_find_bt_clicked()
{

}

// 双击左侧表格加载对应的日记内容
void main_win::on_tableWidget_cellDoubleClicked(int row, int column)
{
    // 获取点中的那行，取出当初存在 Qt::UserRole 里的真实 note_id
    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    if (!item) return;

    int note_id = item->data(Qt::UserRole).toInt();
    current_edit_note_id = note_id; // 把全局 ID 标记为此刻打开的日记

    // 到数据库把完整 content 提取出来
    QSqlQuery query(db);
    query.prepare("SELECT content FROM notes WHERE note_id = :note_id AND user_id = :user_id");
    query.bindValue(":note_id", note_id);
    query.bindValue(":user_id", current_user_id);
    
    if (query.exec() && query.next()) {
        QString content = query.value(0).toString();
        // 渲染到右侧的大文本框中让你随意读写和编辑
        ui->textEdit->setPlainText(content);
    } else {
        QMessageBox::warning(this, "打开失败", "无法读取该便签内容！");
    }
}

//删除便签
void main_win::on_del_bt_clicked()
{

}
