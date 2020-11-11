#include "setting.h"
#include "ui_setting.h"
#include "QDebug"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::on_pushButton_clicked()
{
    qDebug() << "PAUSE";
}

void Setting::on_pushButton_2_clicked()
{
    qDebug() << "EXECUTE";
}
