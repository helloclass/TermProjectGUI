#include "main.h"
#include "setting.h"
#include "ui_setting.h"
#include "themewidget.h"
#include "QDebug"
#include "QFile"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    Task(0),
    Core(1),
    Resol(1),
    Ps_num(1),
    Min(-1),
    Max(1),

    ui(new Ui::Setting)
{
    ui->setupUi(this);

    int pid;
    unsigned long start;
    unsigned long exe, wait, ret;

    QFile File("/home/seongbok/TermProject/TermProjectGUI/chartthemes/purified.json");

    if (!File.open(QFile::ReadOnly|QFile::Text))
    {
        if (!File.exists())
        {
            qDebug() << "File is not exist!" ;
        }
        else
        {
            qDebug() << "Can't open file";
        }
    }
    QTextStream OpenFile(&File);
    QString configText, Buffer;

    OpenFile >> Buffer;
    TaskNum = Buffer.toInt();

    for (int i = 0; i <= TaskNum+1; i++){
        configText = OpenFile.readLine();
        if (configText == ""){
            continue;
        }

        QTextStream Stream(&configText);

        Stream >> Buffer;
        Stream >> Buffer;
        int pidx = Buffer.toInt();

        tSlice[pidx].pid = Buffer.toInt();

        Stream >> Buffer;
        tSlice[pidx].task = Buffer.toULong();

        Stream >> Buffer;
        tSlice[pidx].arr = Buffer.toULong();
        if (tSlice[pidx].pid == 0){
            start = tSlice[pidx].arr;
        }
        tSlice[pidx].arr -= start;

        Stream >> Buffer;
        tSlice[pidx].priority = Buffer.toInt();
    }

    for (int i = 0; i <= TaskNum; i++){
            configText = OpenFile.readLine();

            if (configText == ""){
                continue;
            }

            QString configString = configText;
            QTextStream myteststream(&configString);

            myteststream >> Buffer;
            if (Buffer == "ATT"){
                myteststream >> Buffer;
                // insert ATT
                myteststream >> Buffer;
                myteststream >> Buffer;
                // insert AWT
                break;
            }

            pid = Buffer.toInt();

            myteststream >> Buffer;
            exe = Buffer.toULong();

            myteststream >> Buffer;
            wait = Buffer.toULong();

            myteststream >> Buffer;
            ret = Buffer.toULong();
            ret -= start;

            tSlice[pid].pid = pid;
            tSlice[pid].exe = exe;
            tSlice[pid].wait = wait;
            tSlice[pid].ret = ret;

            qDebug() << "TEST: " << tSlice[pid].pid << tSlice[pid].task << tSlice[pid].priority << tSlice[pid].arr << tSlice[pid].wait << tSlice[pid].exe << tSlice[pid].ret;
    }

    File.close();

    // Init ComboBox
    init_Line_Edit();

    populate_PID_Box();
    populate_PRIORITY_Box();

    populate_Scheduler_Box();
}

bool Setting::Update = false;

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
    system("pwd");
}

void Setting::init_Line_Edit()
{
    QString str;
    str = Task;

    ui->TaskEdit->setText(QString::number(Task));
    ui->CoreEdit->setText(QString::number(Core));
    ui->ResolEdit->setText(QString::number(Resol));
    ui->Ps_numEdit->setText(QString::number(Ps_num));
    ui->MinEdit->setText(QString::number(Min));
    ui->MaxEdit->setText(QString::number(Max));
}

void Setting::populate_PID_Box()
{
    // display each processor PID
    int taskNum = TaskNum;

    for (int pn = 0; pn < taskNum; pn++)
        ui->comboBox->addItem(QString::number(pn));
}

void Setting::populate_PRIORITY_Box()
{
    // Queue Priority List
    int taskNum = TaskNum;

    for (int p = 0; p < taskNum; p++)
        ui->comboBox_2->addItem(QString::number(p));
}

void Setting::populate_Scheduler_Box()
{

    ui->comboBox_3->addItem("FCFS");
    ui->comboBox_3->addItem("SJF");
    ui->comboBox_3->addItem("Non-preemption Priority");
    ui->comboBox_3->addItem("Preemption Priority");
    ui->comboBox_3->addItem("RR");
    ui->comboBox_3->addItem("SRT");
    ui->comboBox_3->addItem("HRN");
}

void Setting::set_Task(){
    ui->TaskEdit->setText(QString::number(Task));
}


void Setting::on_pushButton_3_clicked()
{

}
