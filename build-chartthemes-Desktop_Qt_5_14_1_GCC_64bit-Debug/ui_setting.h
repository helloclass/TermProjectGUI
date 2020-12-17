/********************************************************************************
** Form generated from reading UI file 'setting.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTING_H
#define UI_SETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Setting
{
public:
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *comboBox_3;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout_6;
    QLineEdit *CoreEdit;
    QLineEdit *ResolEdit;
    QLineEdit *Ps_numEdit;
    QLabel *label_13;
    QWidget *layoutWidget_3;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_14;
    QLabel *label_15;
    QWidget *layoutWidget_4;
    QVBoxLayout *verticalLayout_8;
    QLineEdit *MinEdit;
    QLineEdit *MaxEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_2;
    QComboBox *comboBox;
    QLineEdit *TaskEdit;
    QComboBox *comboBox_2;
    QPushButton *pushButton_3;

    void setupUi(QDialog *Setting)
    {
        if (Setting->objectName().isEmpty())
            Setting->setObjectName(QString::fromUtf8("Setting"));
        Setting->resize(479, 607);
        label_4 = new QLabel(Setting);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(60, 30, 101, 20));
        label_4->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(Setting);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(66, 170, 91, 31));
        label_5->setAlignment(Qt::AlignCenter);
        comboBox_3 = new QComboBox(Setting);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));
        comboBox_3->setGeometry(QRect(170, 170, 261, 31));
        layoutWidget = new QWidget(Setting);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 250, 101, 91));
        verticalLayout_5 = new QVBoxLayout(layoutWidget);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_9);

        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_10);

        label_11 = new QLabel(layoutWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setLayoutDirection(Qt::LeftToRight);
        label_11->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_11);

        label_12 = new QLabel(Setting);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(60, 220, 101, 20));
        label_12->setAlignment(Qt::AlignCenter);
        layoutWidget_2 = new QWidget(Setting);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(170, 250, 261, 91));
        verticalLayout_6 = new QVBoxLayout(layoutWidget_2);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        CoreEdit = new QLineEdit(layoutWidget_2);
        CoreEdit->setObjectName(QString::fromUtf8("CoreEdit"));

        verticalLayout_6->addWidget(CoreEdit);

        ResolEdit = new QLineEdit(layoutWidget_2);
        ResolEdit->setObjectName(QString::fromUtf8("ResolEdit"));

        verticalLayout_6->addWidget(ResolEdit);

        Ps_numEdit = new QLineEdit(layoutWidget_2);
        Ps_numEdit->setObjectName(QString::fromUtf8("Ps_numEdit"));

        verticalLayout_6->addWidget(Ps_numEdit);

        label_13 = new QLabel(Setting);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(60, 370, 101, 20));
        label_13->setAlignment(Qt::AlignCenter);
        layoutWidget_3 = new QWidget(Setting);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(60, 400, 101, 61));
        verticalLayout_7 = new QVBoxLayout(layoutWidget_3);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        label_14 = new QLabel(layoutWidget_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_14);

        label_15 = new QLabel(layoutWidget_3);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_15);

        layoutWidget_4 = new QWidget(Setting);
        layoutWidget_4->setObjectName(QString::fromUtf8("layoutWidget_4"));
        layoutWidget_4->setGeometry(QRect(170, 400, 261, 61));
        verticalLayout_8 = new QVBoxLayout(layoutWidget_4);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        MinEdit = new QLineEdit(layoutWidget_4);
        MinEdit->setObjectName(QString::fromUtf8("MinEdit"));

        verticalLayout_8->addWidget(MinEdit);

        MaxEdit = new QLineEdit(layoutWidget_4);
        MaxEdit->setObjectName(QString::fromUtf8("MaxEdit"));

        verticalLayout_8->addWidget(MaxEdit);

        pushButton = new QPushButton(Setting);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(340, 510, 89, 41));
        pushButton_2 = new QPushButton(Setting);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(230, 510, 89, 41));
        layoutWidget1 = new QWidget(Setting);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(60, 60, 101, 91));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget1);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setLayoutDirection(Qt::LeftToRight);
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        layoutWidget2 = new QWidget(Setting);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(170, 60, 261, 91));
        verticalLayout_2 = new QVBoxLayout(layoutWidget2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        comboBox = new QComboBox(layoutWidget2);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout_2->addWidget(comboBox);

        TaskEdit = new QLineEdit(layoutWidget2);
        TaskEdit->setObjectName(QString::fromUtf8("TaskEdit"));

        verticalLayout_2->addWidget(TaskEdit);

        comboBox_2 = new QComboBox(layoutWidget2);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        verticalLayout_2->addWidget(comboBox_2);

        pushButton_3 = new QPushButton(Setting);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(120, 510, 89, 41));

        retranslateUi(Setting);

        QMetaObject::connectSlotsByName(Setting);
    } // setupUi

    void retranslateUi(QDialog *Setting)
    {
        Setting->setWindowTitle(QCoreApplication::translate("Setting", "Dialog", nullptr));
        label_4->setText(QCoreApplication::translate("Setting", "<Process>", nullptr));
        label_5->setText(QCoreApplication::translate("Setting", "Scheduler", nullptr));
        label_9->setText(QCoreApplication::translate("Setting", "Core", nullptr));
        label_10->setText(QCoreApplication::translate("Setting", "RESOL", nullptr));
        label_11->setText(QCoreApplication::translate("Setting", "PS_NUM", nullptr));
        label_12->setText(QCoreApplication::translate("Setting", "<CPU>", nullptr));
        label_13->setText(QCoreApplication::translate("Setting", "<Queue>", nullptr));
        label_14->setText(QCoreApplication::translate("Setting", "MIN", nullptr));
        label_15->setText(QCoreApplication::translate("Setting", "MAX", nullptr));
        pushButton->setText(QCoreApplication::translate("Setting", "Pause", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Setting", "Execute", nullptr));
        label->setText(QCoreApplication::translate("Setting", "PID", nullptr));
        label_2->setText(QCoreApplication::translate("Setting", "TASK", nullptr));
        label_3->setText(QCoreApplication::translate("Setting", "PRIORITY", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Setting", "Exhibition", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Setting: public Ui_Setting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTING_H
