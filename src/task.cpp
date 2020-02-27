#include "task.h"
#include "ui_task.h"

#include "agenda.h"

task::task(QWidget *parent, int flag) :
    QDialog(parent),
    ui(new Ui::task)
{
    ui->setupUi(this);

    this->flag = flag;

    setupWindow();
}

task::~task()
{
    delete ui;
}

void task::setupWindow()
{
    setWindowTitle("Nova Tarefa");

    setMinimumSize(540, 230);
    setMaximumSize(540, 230);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    /*
     * Access methods from parent Widget by using qobject_cast to its type
     */
    Widget * parentWid = qobject_cast<Widget*>(this->parent());

    ui->dateEdit->setDate(parentWid->getCalendarDate());

    ui->dateEdit->setMinimumDate(QDate(2020, 1, 1));
    ui->dateEdit->setMaximumDate(QDate(2020, 12, 31));

    if(this->flag == 1) {
        ui->typeLineEdit->setText(parentWid->getSelectedType());
        ui->descriptionText->setPlainText(parentWid->getSelectedDesc());
    }


}

void task::on_saveButton_clicked()
{
    QDate userTaskDate = ui->dateEdit->date();
    QString userTaskType = ui->typeLineEdit->text();
    QString userTaskDesc = ui->descriptionText->toPlainText();

    if(userTaskDate.isValid() && !(userTaskDesc.isEmpty()) && !(userTaskType.isEmpty())) {
        qDebug() << "Date is: " << userTaskDate;
        qDebug() << "Type is: " << userTaskType;
        qDebug() << "Desc is: " << userTaskDesc;

        this->taskDate = userTaskDate;
        this->taskDesc = userTaskDesc;
        this->taskType = userTaskType;

        accept();
    }
    else {
        qDebug() << "One of the attributes is not valid!";

        QMessageBox::warning(this, "Atributos inválidos", "Um dos atributos não é válido!");
    }


}

void task::on_cancelButton_clicked()
{
    reject();
}

QString task::getTaskDesc() const
{
    return taskDesc;
}

QString task::getTaskType() const
{
    return taskType;
}

QDate task::getTaskDate() const
{
    return taskDate;
}
