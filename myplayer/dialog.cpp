#include "dialog.h"
#include "ui_dialog.h"
#include <iostream>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->btnnum=0;
}
void Dialog::SetItem(QStringList listname){
    ui->listWidget->addItems(listname);
}
Dialog::~Dialog()
{
    this->Reset();
    delete ui;
}
void Dialog::Reset(){
    ui->listWidget->clear();
}

void Dialog::on_buttonBox_accepted()
{
    std::cout<<"yes"<<std::endl;
}

void Dialog::on_buttonBox_rejected()
{
    std::cout<<"true"<<std::endl;
}
int Dialog::GetSelectIndex(){
    return ui->listWidget->currentRow();
}
