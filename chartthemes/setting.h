#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);

    static bool Update;

    int TaskNum;
    typedef struct timeSlice{
        int pid;
        unsigned long task;
        int priority;

        unsigned long arr;
        unsigned long exe;
        unsigned long wait;
        unsigned long ret;
    }timeSlice;
    timeSlice tSlice[40];

    ~Setting();

public slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void init_Line_Edit();

    void populate_PID_Box();
    void populate_PRIORITY_Box();

    void populate_Scheduler_Box();
    void set_Task();

private slots:

    void on_pushButton_3_clicked();

private:
    int Task;

    int Core;
    int Resol;
    int Ps_num;

    int Min;
    int Max;

    unsigned long AWT, ATT;

    Ui::Setting *ui;
};

#endif // SETTING_H
