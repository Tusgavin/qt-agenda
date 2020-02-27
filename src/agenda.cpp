#include "agenda.h"
#include "ui_agenda.h"

static QSqlDatabase DataBase = QSqlDatabase::addDatabase("QSQLITE");

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    statusBar = new QStatusBar(this);
    ui->statusBarLayout->addWidget(statusBar);

    ui->deleteTaskButton->setEnabled(false);
    ui->updateTaskButton->setEnabled(false);
    ui->doneButton->setEnabled(false);

    setupWidget();
    setupDataBase();
    setGroupBoxTitles();
    setCalendarDates();

}

Widget::~Widget()
{
    delete ui;
}

QDate Widget::getCalendarDate() const
{
    return ui->calendarWidget->selectedDate();
}

QString Widget::getSelectedDesc() const
{
    int row = ui->tableWidget->currentRow();

    return ui->tableWidget->item(row, 1)->text();
}

QString Widget::getSelectedType() const
{
    int row = ui->tableWidget->currentRow();

    return ui->tableWidget->item(row, 0)->text();
}

int Widget::getSelectedDone() const
{
    int row = ui->tableWidget->currentRow();

    return ui->tableWidget->item(row, 3)->text().toInt();
}

int Widget::getSelectedId() const
{
    int row = ui->tableWidget->currentRow();

    return ui->tableWidget->item(row, 2)->text().toInt();
}

void Widget::setGroupBoxTitles() const
{
    ui->actionsGroupBox->setTitle("Ações");

    ui->addTaskButton->setText("Adicionar Tarefa");
    ui->updateTaskButton->setText("Editar Tarefa");
    ui->deleteTaskButton->setText("Deletar Tarefa");
    ui->doneButton->setText("Feita / Desfeita");

    ui->calendarGroupBox->setTitle("Calendario");
    ui->tasksGroupBox->setTitle("Tarefas do dia " + ui->calendarWidget->selectedDate().toString("dd.MM.yyyy"));
}

void Widget::setupDataBase() const
{
    /*
     * Criação e abertura do banco de dados do tipo "QSQLITE"
     */
    QString dir = qApp->applicationDirPath();
    QString path = dir + "/database/database.db";

    DataBase.setDatabaseName(path);

    if(!DataBase.open()) {
        qDebug() << "Cannot open DataBase";
        this->statusBar->showMessage("Não foi possível abrir o Banco de Dados", 3000);
    }
    else {
        qDebug() << "DataBase opened with success";
        this->statusBar->showMessage("Banco de Dados aberto com sucesso", 3000);
    }

}

void Widget::setupWidget()
{
    this->setWindowTitle("Agenda");

    this->setMinimumSize(650, 680);
    this->setMaximumSize(650, 680);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}

void Widget::clearTableWidget() const
{
    /*
     * Clear table Widget when clicked away from selected date
     */
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setRowCount(0);

}

void Widget::formatTableWidget() const
{
    QStringList agendaHeader = {"Tipo", "Descrição"};

    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, 600);

    ui->tableWidget->setHorizontalHeaderLabels(agendaHeader);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->verticalHeader()->setVisible(false);

    ui->tableWidget->setColumnHidden(2, true);
    ui->tableWidget->setColumnHidden(3, true);
    ui->tableWidget->setColumnHidden(4, true);
    ui->tableWidget->setColumnHidden(5, true);
}

void Widget::selectTasks()
{
    int day = ui->calendarWidget->selectedDate().day();
    int month = ui->calendarWidget->selectedDate().month();

    QSqlQuery query;
    query.prepare("select * from agenda where (day = :day) and (month = :month)");
    query.bindValue(":day", day);
    query.bindValue(":month", month);

    if(query.exec()) {
        int row = 0;

        ui->tableWidget->setColumnCount(6);

        while(query.next()) {
            ui->tableWidget->insertRow(row);

            /*
             * Selecting elements from each column (total of 2)
             */
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(3).toString().toUpper()));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(2).toString()));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(4).toString()));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(5).toString()));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(0).toString()));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(1).toString()));

            ui->tableWidget->setRowHeight(row, 40);

            if(query.value(5) == false) {
                QFont f = ui->tableWidget->item(row, 0)->font();
                f.setStrikeOut(false);

                ui->tableWidget->item(row, 0)->setFont(f);
                ui->tableWidget->item(row, 1)->setFont(f);
            }
            else if(query.value(5) == true) {
                QFont f = ui->tableWidget->item(row, 0)->font();
                f.setStrikeOut(true);

                ui->tableWidget->item(row, 0)->setFont(f);
                ui->tableWidget->item(row, 1)->setFont(f);
            }

            row++;
        }

        formatTableWidget();

    }
    else {
        qDebug() << "Problem while selecting from database";
        QMessageBox::warning(this, "Falha", "Um problema ocorreu ao selecionar itens do Banco de Dados");
    }
}

void Widget::setupTaskCalendarDate(int day, int month)
{
    QSqlQuery query;

    QTextCharFormat format;
    format.setForeground(Qt::green);

    QTextCharFormat original;
    original.setForeground(Qt::black);

    query.prepare("select day, month from agenda group by day, month");


    if((day == 0) && (month == 0)) {
        if(query.exec()) {

            while(query.next()) {

                int _day = query.value(0).toString().toInt();
                int _month = query.value(1).toString().toInt();

                ui->calendarWidget->setDateTextFormat(QDate(2020, _month, _day), format);
            }

        }
    }
    else {
        if(ui->tableWidget->rowCount() == 1) {
             ui->calendarWidget->setDateTextFormat(QDate(2020, month, day), original);
        }
    }
}

void Widget::setCalendarDates()
{
    QDate currentDate = QDate::currentDate();

    ui->calendarWidget->setMinimumDate(QDate(2020, 1, 1));
    ui->calendarWidget->setMaximumDate(QDate(2020, 12, 31));
    ui->calendarWidget->setSelectedDate(currentDate);

    setupTaskCalendarDate();
    selectTasks();
}

void Widget::on_addTaskButton_clicked()
{
    QSqlQuery query;

    task *newTask = new task(this);

    int dialogReturn = newTask->exec();

    if(dialogReturn == QDialog::Accepted) {
        qDebug() << "Dialog Accepted";

        int day = newTask->getTaskDate().day();
        int month = newTask->getTaskDate().month();
        QString description = newTask->getTaskDesc();
        QString type = newTask->getTaskType();

        query.prepare("insert into agenda (day, month, description, type) values (:day, :month, :description, :type)");
        query.bindValue(":day", day);
        query.bindValue(":month", month);
        query.bindValue(":description", description);
        query.bindValue(":type", type);

        if(query.exec()) {
            qDebug() << "Task added with success";
            QMessageBox::information(this, "Tarefa adicionada", "Tarefa adicionada com sucesso !");
        }
        else {
            qDebug() << "Problem adding a task";
            QMessageBox::warning(this, "Falha", "Um problema ocorreu ao adicionar a tarefa !");
        }

    }
    else if(dialogReturn == QDialog::Rejected) {
        qDebug() << "Dialog Rejected";
    }

    clearTableWidget();
    selectTasks();
    setupTaskCalendarDate();

}

void Widget::on_calendarWidget_selectionChanged()
{
    ui->tasksGroupBox->setTitle("Tarefas do dia " + ui->calendarWidget->selectedDate().toString("dd.MM.yyyy"));

    clearTableWidget();

    selectTasks();

    ui->updateTaskButton->setEnabled(false);
    ui->deleteTaskButton->setEnabled(false);
    ui->doneButton->setEnabled(false);
}

void Widget::on_updateTaskButton_clicked()
{
    QSqlQuery query;

    int _day = getCalendarDate().day();
    int _month = getCalendarDate().month();


    task *newTask = new task(this, 1);

    int dialogReturn = newTask->exec();

    if(dialogReturn == QDialog::Accepted) {
        qDebug() << "Dialog Accepted";

        int id = this->getSelectedId();
        int day = newTask->getTaskDate().day();
        int month = newTask->getTaskDate().month();
        QString description = newTask->getTaskDesc();
        QString type = newTask->getTaskType();

        query.prepare("update agenda set description = :description, type = :type, day = :day, month = :month where id = :id");
        query.bindValue(":description", description);
        query.bindValue(":type", type);
        query.bindValue(":id", id);
        query.bindValue(":day", day);
        query.bindValue(":month", month);


        if(query.exec()) {
            qDebug() << "Task updated with success";
            QMessageBox::information(this, "Tarefa atualizada", "Tarefa atualizada com sucesso !");

            if(!((_day == day) && (_month == month))) {
                setupTaskCalendarDate(_day, _month);
            }

        }
        else {
            qDebug() << "Problem updating a task";
            QMessageBox::warning(this, "Falha", "Um problema ocorreu ao atualizar a tarefa !");
        }

    }
    else if(dialogReturn == QDialog::Rejected) {
        qDebug() << "Dialog Rejected";
    }

    ui->updateTaskButton->setEnabled(false);
    ui->deleteTaskButton->setEnabled(false);
    ui->doneButton->setEnabled(false);

    clearTableWidget();
    setupTaskCalendarDate();
    selectTasks();
}

void Widget::on_deleteTaskButton_clicked()
{
    QSqlQuery query;

    int row = ui->tableWidget->currentRow();
    int id_delete = ui->tableWidget->item(row, 2)->text().toInt();
    int day_delete = ui->tableWidget->item(row, 4)->text().toInt();
    int month_delete = ui->tableWidget->item(row, 5)->text().toInt();


    query.prepare("delete from agenda where id = :id");
    query.bindValue(":id", id_delete);

    if(query.exec()) {
        ui->tableWidget->removeRow(row);
        qDebug() << "Item deleted from database";
        QMessageBox::information(this, "Sucesso", "Tarefa deletada com sucesso !");
        setupTaskCalendarDate(day_delete, month_delete);
    }
    else {
        qDebug() << "Problem while deleting from database";
        QMessageBox::warning(this, "Falha", "Um problema ocorreu ao deletar itens do Banco de Dados");
    }

    ui->updateTaskButton->setEnabled(false);
    ui->deleteTaskButton->setEnabled(false);
    ui->doneButton->setEnabled(false);
}

void Widget::on_tableWidget_itemClicked()
{
    ui->updateTaskButton->setEnabled(true);
    ui->deleteTaskButton->setEnabled(true);
    ui->doneButton->setEnabled(true);

}

void Widget::on_quitButton_clicked()
{
    QApplication::quit();
}

void Widget::on_doneButton_clicked()
{
    QSqlQuery query;

    QList<QTableWidgetItem *> lista = ui->tableWidget->selectedItems();

    try {
        if(lista.isEmpty()) {
            throw 10;
        }
        else {
            if(getSelectedDone() == 0) {
                query.prepare("update agenda set done = :done where id = :id");
                query.bindValue(":done", true);
                query.bindValue(":id", getSelectedId());

                if(query.exec()) {
                    int row = ui->tableWidget->currentRow();
                    QFont f = ui->tableWidget->item(row, 0)->font();
                    f.setStrikeOut(true);

                    clearTableWidget();
                    selectTasks();

                    ui->tableWidget->item(row, 0)->setFont(f);
                    ui->tableWidget->item(row, 1)->setFont(f);
                }
            }
            else if(getSelectedDone() == 1) {
                query.prepare("update agenda set done = :done where id = :id");
                query.bindValue(":done", false);
                query.bindValue(":id", getSelectedId());

                if(query.exec()) {
                    int row = ui->tableWidget->currentRow();
                    QFont f = ui->tableWidget->item(row, 0)->font();
                    f.setStrikeOut(false);

                    clearTableWidget();
                    selectTasks();

                    ui->tableWidget->item(row, 0)->setFont(f);
                    ui->tableWidget->item(row, 1)->setFont(f);
                }
            }

            ui->doneButton->setEnabled(false);
            ui->deleteTaskButton->setEnabled(false);
            ui->updateTaskButton->setEnabled(false);

        }

    } catch (int e) {
        qDebug() << "Erro tipo: " << e;
        QMessageBox::warning(this, "ERRO", "Nenhuma tarefa selecionada");
    }

}
