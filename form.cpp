#include "form.h"
#include "ui_form.h"
#include <QMessageBox>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    if ( !ui->loginEdit->text().isEmpty() && !ui->passwordEdit->text().isEmpty() )
    {
        emit click( ui->loginEdit->text(), ui->passwordEdit->text() );
        close();
    }
    else
    {
        QMessageBox *box = new QMessageBox( this );
        box->setText( "Login and password must not empty!" );
        box->show();
    }

}
