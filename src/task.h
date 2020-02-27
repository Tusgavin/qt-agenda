#ifndef TASK_H
#define TASK_H

#include <QDialog>

#include <QDate>
#include <QDebug>
#include <QApplication>

#include <QMessageBox>

namespace Ui {
class task;
}

class task : public QDialog
{
    Q_OBJECT

public:
    explicit task(QWidget *parent = nullptr, int flag = 0);
    ~task();


    QDate getTaskDate() const;

    QString getTaskType() const;

    QString getTaskDesc() const;

private slots:
    void on_saveButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::task *ui;

    // flag == 1 : update ? add
    int flag;

    QDate taskDate;
    QString taskType;
    QString taskDesc;

    void setupWindow();
};

#endif // TASK_H
