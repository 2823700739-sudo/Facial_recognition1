#include "main_win.h"
#include "ui_main_win.h"
#include "face_in.h"
#include "login.h"

#include <QMessageBox>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

main_win::main_win(QWidget *parent, int user_id, QString username) : QWidget(parent),
    ui(new Ui::main_win),
    current_user_id(user_id),
    current_username(username),
    current_edit_note_id(-1)
{
    ui->setupUi(this);
    this->setWindowTitle("瞳记");

    // 初始化欢迎标签
    if (!current_username.isEmpty())
    {
        ui->welcome_lb->setText(current_username + "的私密空间");
    }

    // 初始化左侧便签列表 UI
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->hide();                         // 隐藏表头
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止直接在表格里双击编辑

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
    // 清空现有列表
    ui->tableWidget->setRowCount(0);

    QJsonObject req;
    req.insert("action", "load_notes");
    req.insert("user_id", current_user_id);

    QJsonObject res = login::sendSyncRequest(req);
    if (res.value("status").toString() == "success")
    {
        QJsonArray notesList = res.value("notes").toArray();
        int row = 0;
        for (int i = 0; i < notesList.size(); ++i)
        {
            QJsonObject itemObj = notesList[i].toObject();
            int note_id = itemObj.value("note_id").toInt();
            QString title = itemObj.value("title").toString();
            QString time = itemObj.value("update_time").toString();

            ui->tableWidget->insertRow(row);
            // 将标题和更新时间拼接作为列表项显示
            QTableWidgetItem *item = new QTableWidgetItem(title + "\n" + time);
            item->setData(Qt::UserRole, note_id); // 把真实的 note_id 藏在 item 里
            ui->tableWidget->setItem(row, 0, item);
            row++;
        }
    }
    else
    {
        qDebug() << "加载便签失败：" << res.value("msg").toString();
    }
}

// 新建标签 (发一条新日记并保存到数据库)
void main_win::on_add_bt_clicked()
{
    // 1. 获取你输入的文本信息
    QString content = ui->textEdit->toPlainText().trimmed();
    if (content.isEmpty())
    {
        QMessageBox::warning(this, "提示", "总得写点什么吧~");
        return;
    }

    // 2. 截取正文第一行当做当前日记的“标题” (如果太长就截断加省略号)
    QString title = content.split('\n').first();
    if (title.length() > 15)
    {
        title = title.left(15) + "...";
    }

    // 3. 构建 JSON 数据包
    QJsonObject req;
    req.insert("action", "save_note");
    req.insert("user_id", current_user_id);
    req.insert("title", title);
    req.insert("content", content);
    req.insert("time", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    if (current_edit_note_id == -1)
    {
        req.insert("is_new", 1);
    }
    else
    {
        req.insert("is_new", 0);
        req.insert("note_id", current_edit_note_id);
    }

    // 4. 发送至服务器
    QJsonObject res = login::sendSyncRequest(req);
    if (res.value("status").toString() != "success")
    {
        qDebug() << "保存便签失败: " << res.value("msg").toString();
        QMessageBox::critical(this, "错误", "保存失败！" + res.value("msg").toString());
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
// 锁定便签
void main_win::on_lock_bt_clicked()
{
    // 如果蒙版还未创建，则创建它并将其大小设置为覆盖整个窗口
    if (!maskWidget)
    {
        maskWidget = new QWidget(this);
        maskWidget->setGeometry(0, 0, this->width(), this->height());

        // 设置完全不透明的黑灰色背景，彻底遮挡下方私密内容
        maskWidget->setStyleSheet("background-color: rgba(30, 30, 30, 252);");

        // 在蒙版中间创建一个巨大的解锁按钮
        QPushButton *unlockBtn = new QPushButton("点击解锁我的便签", maskWidget);
        unlockBtn->setFixedSize(300, 80);
        unlockBtn->setStyleSheet(
                    "QPushButton {"
                    "  background-color: #3b82f6;"
                    "  color: white;"
                    "  font-size: 20px;"
                    "  font-weight: bold;"
                    "  border-radius: 10px;"
                    "}"
                    "QPushButton:hover { background-color: #2563eb; }");

        // 计算居中位置
        int btnX = (maskWidget->width() - unlockBtn->width()) / 2;
        int btnY = (maskWidget->height() - unlockBtn->height()) / 2;
        unlockBtn->move(btnX, btnY);

        // 连接解锁按钮的信号槽
        connect(unlockBtn, &QPushButton::clicked, this, &main_win::on_unlock_clicked);
        maskWidget->show();
    }
    else
    {
        maskWidget->show();
    }
}

// 解开蒙版并执行人脸识别
void main_win::on_unlock_clicked()
{
    // 调用人脸识别页面进行二次身份核验（解锁当前便签）
    // 传入参数 true 表示这是“解锁模式”，并且传入 current_user_id 校验解锁者身份
    face_in *fi = new face_in(this, true, current_user_id);

    // 如果 face_in 验证成功，在它结束或发信号后，您需要去除蒙版
    // 例如监听它发出的 loginSuccessful 信号
    connect(fi, &face_in::loginSuccessful, this, [=]()
    {
        if (maskWidget) {
            maskWidget->hide();
        }
        fi->close();
        fi->deleteLater(); });

    // 以原生模式显示
    fi->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    fi->show();
}
// 搜索便签
void main_win::on_find_bt_clicked()
{
    QString keyword = ui->lineEdit->text().trimmed(); // 如果搜索框为空，直接加载所有便签
    if (keyword.isEmpty())
    {
        loadUserNotes(); // 为空时加载所有便签
        return;
    }

    // 清空现有列表
    ui->tableWidget->setRowCount(0);

    QJsonObject req;
    req.insert("action", "search_notes");
    req.insert("user_id", current_user_id);
    req.insert("keyword", keyword);

    QJsonObject res = login::sendSyncRequest(req);
    if (res.value("status").toString() == "success")
    {
        QJsonArray notesList = res.value("notes").toArray();
        int row = 0;
        for (int i = 0; i < notesList.size(); ++i)
        {
            QJsonObject itemObj = notesList[i].toObject();
            int note_id = itemObj.value("note_id").toInt();
            QString title = itemObj.value("title").toString();
            QString time = itemObj.value("update_time").toString();

            ui->tableWidget->insertRow(row);
            QTableWidgetItem *item = new QTableWidgetItem(title + "\n" + time);
            item->setData(Qt::UserRole, note_id);
            ui->tableWidget->setItem(row, 0, item);
            row++;
        }
    }
}

// 双击左侧表格加载对应的日记内容
void main_win::on_tableWidget_cellDoubleClicked(int row, int column)
{
    // 获取点中的那行，取出当初存在 Qt::UserRole 里的真实 note_id
    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    if (!item)
        return;

    int note_id = item->data(Qt::UserRole).toInt();
    current_edit_note_id = note_id; // 把全局 ID 标记为此刻打开的日记

    // 发请求：到服务端把完整 content 提取出来
    QJsonObject req;
    req.insert("action", "load_note_content");
    req.insert("user_id", current_user_id);
    req.insert("note_id", note_id);

    QJsonObject res = login::sendSyncRequest(req);
    if (res.value("status").toString() == "success")
    {
        QString content = res.value("content").toString();
        // 渲染到右侧的大文本框中让你随意读写和编辑
        ui->textEdit->setPlainText(content);
    }
    else
    {
        QMessageBox::warning(this, "打开失败", "无法读取该便签内容！");
    }
}

// 删除便签
void main_win::on_del_bt_clicked()
{
    if (current_edit_note_id == -1)
    {
        QMessageBox::warning(this, "提示", "请先在左侧双击选择要删除的便签！");
        return;
    }

    // 弹窗确认
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "确定要永久删除这条便签吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        return;
    }

    QJsonObject req;
    req.insert("action", "delete_note");
    req.insert("user_id", current_user_id);
    req.insert("note_id", current_edit_note_id);

    QJsonObject res = login::sendSyncRequest(req);
    if (res.value("status").toString() == "success")
    {
        QMessageBox::information(this, "成功", "便签已删除！");
        // 清理现场：重置为新建模式、清空右侧文本框、刷新左侧列表
        current_edit_note_id = -1;
        ui->textEdit->clear();
        loadUserNotes();
    }
    else
    {
        qDebug() << "删除便签失败";
        QMessageBox::critical(this, "错误", "删除失败！");
    }
}

void main_win::on_dateTimeEdit_dateChanged(const QDate &date)
{
    QString targetDate = date.toString("yyyy-MM-dd"); // 例如 "2026-04-07"

    // 清空现有列表
    ui->tableWidget->setRowCount(0);
    QJsonObject req;
    req.insert("action", "load_notes_by_date");
    req.insert("user_id", current_user_id);
    req.insert("date", targetDate);

    // 阻塞请求服务端
    QJsonObject res = login::sendSyncRequest(req);

    if (res.value("status").toString() == "success")
    {
        QJsonArray notesList = res.value("notes").toArray();// 这里的 notesList 是服务端根据日期过滤后的结果
        int row = 0;
        for(int i = 0; i < notesList.size(); ++i) {
            QJsonObject itemObj = notesList[i].toObject();// 每个 itemObj 里包含 note_id、title、update_time 字段
            int note_id = itemObj.value("note_id").toInt();
            QString title = itemObj.value("title").toString();
            QString time = itemObj.value("update_time").toString();

            ui->tableWidget->insertRow(row);// 每次插入新行
            QTableWidgetItem *item = new QTableWidgetItem(title + "\n" + time);
            item->setData(Qt::UserRole, note_id);
            ui->tableWidget->setItem(row, 0, item);
            row++;
        }
    }
}
