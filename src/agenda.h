#ifndef AGENDA_H
#define AGENDA_H

#include <QWidget>

#include <QDebug>
#include <QtSql>
#include <QFileInfo>
#include <QDate>
#include <QStatusBar>
#include <QTableWidget>
#include <QTextCharFormat>
#include <QApplication>
#include <QFont>

#include "task.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QDate getCalendarDate() const;

    QString getSelectedDesc() const;

    QString getSelectedType() const;

    int getSelectedDone() const;

    int getSelectedId() const;

private slots:
    void on_addTaskButton_clicked();

    void on_calendarWidget_selectionChanged();

    void on_updateTaskButton_clicked();

    void on_deleteTaskButton_clicked();

    void on_tableWidget_itemClicked();

    void on_quitButton_clicked();

    void on_doneButton_clicked();

private:
    Ui::Widget *ui;
    QStatusBar *statusBar;

    void setCalendarDates();
    void setGroupBoxTitles() const;
    void setupDataBase() const;
    void setupWidget();
    void clearTableWidget() const;
    void formatTableWidget() const;
    void selectTasks();
    void setupTaskCalendarDate(int day = 0, int month = 0);


};
#endif // AGENDA_H
