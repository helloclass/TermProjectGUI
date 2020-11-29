/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "themewidget.h"
#include "ui_themewidget.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QHorizontalBarSeries>
#include <QtCharts/QHorizontalPercentBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>
#include <QDebug>
#include "setting.h"

QList<QChartView *> *MChart;

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(1),
    m_valueMax(5),
    m_valueCount(3),

    m_ui(new Ui_ThemeWidgetForm)
    {
        m_ui->setupUi(this);
        // comboBox init
        populateThemeBox();
        populateAnimationBox();
        populateLegendBox();
        populateMethodBox();

        maxAva = 5;

        int _method = 0;

        int pid;
        unsigned long start;
        unsigned long exe, wait, ret;

        QFile File("/home/seongbok/TermProject/TermProjectGUI/chartthemes/purified");

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

        // READ FCFS
        for (_method = 0; _method < 3; _method++)
        {

            OpenFile >> Buffer;
            OpenFile >> Buffer;
            _method = Buffer.toInt();
            qDebug() << "aa: " << _method;

            OpenFile >> Buffer;
            TaskNum = Buffer.toInt();
            qDebug() << "aa: " << TaskNum;

            for (int i = 0; i <= TaskNum+1; i++)
            {
                configText = OpenFile.readLine();
                qDebug() << configText;

                if (configText == ""){
                    continue;
                }

                QTextStream Stream(&configText);

                Stream >> Buffer;
                Stream >> Buffer;
                int pidx = Buffer.toInt();

                tSlice[_method][pidx].pid = Buffer.toInt();

                Stream >> Buffer;
                tSlice[_method][pidx].task = Buffer.toULong();

                Stream >> Buffer;
                tSlice[_method][pidx].arr = Buffer.toULong();
                if (tSlice[_method][pidx].pid == 0){
                    start = tSlice[_method][pidx].arr;
                }
                tSlice[_method][pidx].arr -= start;

                Stream >> Buffer;
                tSlice[_method][pidx].priority = Buffer.toInt();

            }

            for (int i = 0; i <= TaskNum+1; i++)
            {
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
                    ATT[_method] = Buffer.toULong() / 1000;

                    myteststream >> Buffer;
                    myteststream >> Buffer;
                    // insert AWT
                    AWT[_method] = Buffer.toULong() / 1000;
                    qDebug() << "AWT: " << AWT[_method] << "ATT: " << ATT[_method];
                    break;
                }

                    pid = Buffer.toInt();
                    _taskOrder[_method][i] = pid;

                    myteststream >> Buffer;
                    wait = Buffer.toULong();

                    myteststream >> Buffer;
                    exe = Buffer.toULong();

                    myteststream >> Buffer;
                    ret = Buffer.toULong();
                    ret -= start;

                    tSlice[_method][pid].pid = pid;
                    tSlice[_method][pid].exe = exe;
                    tSlice[_method][pid].wait = wait;
                    tSlice[_method][pid].ret = ret;

                    qDebug() << "TEST: " <<tSlice[_method][pid].arr << tSlice[_method][pid].wait << tSlice[_method][pid].exe << tSlice[_method][pid].ret;
            }
        }

        // READ RR
        for (int _m = 0; _m < 2; _m++)
        {
            OpenFile >> Buffer;
            OpenFile >> Buffer;
            _method = Buffer.toInt();
            qDebug() << "QQQQaa: " << _method;

            OpenFile >> Buffer;
            TaskNum = Buffer.toInt();

            // info
            for (int i = 0; i <= TaskNum; i++){
                configText = OpenFile.readLine();
                qDebug() << configText;

                if (configText == ""){
                    continue;
                }

                QTextStream Stream(&configText);

                //info
                Stream >> Buffer;
                //pid
                Stream >> Buffer;
                int pidx = Buffer.toInt();

                tSlice[_method][pidx].pid = Buffer.toInt();
                //arrival
                Stream >> Buffer;
                tSlice[_method][pidx].arr = Buffer.toULong();
                if (tSlice[_method][pidx].pid == 0){
                    start = tSlice[_method][pidx].arr;
                }
                tSlice[_method][pidx].arr -= start;
            }
            // ~info

            for (int i = 0; i <= TaskNum+1; i++)
            {
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
                    ATT[_method] = Buffer.toULong() / 1000;

                    myteststream >> Buffer;
                    myteststream >> Buffer;
                    // insert AWT
                    AWT[_method] = Buffer.toULong() / 1000;

                    qDebug() << "AWT: " << AWT[_method] << "ATT: " << ATT[_method];
                    qDebug() <<"??";
                    break;
                    qDebug() <<"??";
                }

                pid = Buffer.toInt();
                _taskOrder[_method][i] = pid;

                myteststream >> Buffer;
                wait = Buffer.toULong();

                myteststream >> Buffer;
                exe = Buffer.toULong();

                myteststream >> Buffer;
                ret = Buffer.toULong();
                ret -= start;

                tSlice[_method][pid].pid = pid;
                tSlice[_method][pid].exe = exe;
                tSlice[_method][pid].wait = wait;
                tSlice[_method][pid].ret = ret;

                qDebug() << "TEST: " << _method <<tSlice[_method][pid].arr << tSlice[_method][pid].wait << tSlice[_method][pid].exe << tSlice[_method][pid].ret;
            }
            qDebug() <<"??" << _m;
        }
            // ~RR

        qDebug() <<"??" << "123214321321";
        File.close();
        qDebug() <<"??" << "1232143fef21";
        //create chart

        //QChartView *chartView;

        chartView = new QChartView(createHBarChart(TaskNum));
        m_ui->gridLayout->addWidget(chartView, 1, 0, 1, 3);
        m_charts << chartView;

        chartView = new QChartView(createPieChart1());
        m_ui->gridLayout->addWidget(chartView, 1, 3);
        m_charts << chartView;

        chartView = new QChartView(createLineChart());
        m_ui->gridLayout->addWidget(chartView, 2, 0);
        m_charts << chartView;

        chartView = new QChartView(createBarChart("AWT", AWT));
        m_ui->gridLayout->addWidget(chartView, 2, 1);
        m_charts << chartView;

        chartView = new QChartView(createBarChart("ATT", ATT));
        m_ui->gridLayout->addWidget(chartView, 2, 2);
        m_charts << chartView;

        chartView = new QChartView(createPieChart());
        m_ui->gridLayout->addWidget(chartView, 2, 3);
        m_charts << chartView;

        m_ui->antialiasCheckBox->setChecked(true);

        // Set the colors from the light theme as default ones
        QPalette pal = qApp->palette();
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
        qApp->setPalette(pal);

        QChart::ChartThemeBlueCerulean;

        MChart = &m_charts;

        updateUI();
    }

ThemeWidget::~ThemeWidget()
{    
    delete m_ui;
}

void ThemeWidget::Reload(){

    //QChartView *chartView;

    chartView = new QChartView(createHBarChart(TaskNum));
    m_ui->gridLayout->addWidget(chartView, 1, 0, 1, 3);
    m_charts << chartView;

    chartView = new QChartView(createPieChart1());
    m_ui->gridLayout->addWidget(chartView, 1, 3);
    m_charts << chartView;

    chartView = new QChartView(createLineChart());
    m_ui->gridLayout->addWidget(chartView, 2, 0);
    m_charts << chartView;

    chartView = new QChartView(createBarChart("AWT", AWT));
    m_ui->gridLayout->addWidget(chartView, 2, 1);
    m_charts << chartView;

    chartView = new QChartView(createBarChart("ATT", ATT));
    m_ui->gridLayout->addWidget(chartView, 2, 2);
    m_charts << chartView;

    chartView = new QChartView(createPieChart());
    m_ui->gridLayout->addWidget(chartView, 2, 3);
    m_charts << chartView;

    m_ui->antialiasCheckBox->setChecked(true);

    // Set the colors from the light theme as default ones
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
    pal.setColor(QPalette::WindowText, QRgb(0x404044));
    qApp->setPalette(pal);

    MChart = &m_charts;

    this->updateUI();
}

void ThemeWidget::on_pushButton_clicked()
{
    this->Reload();
}

//
void ThemeWidget::populateThemeBox()
{
    // add items to theme combobox
    m_ui->themeComboBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    m_ui->themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    m_ui->themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    m_ui->themeComboBox->addItem("Light", QChart::ChartThemeLight);
    m_ui->themeComboBox->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    m_ui->themeComboBox->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    m_ui->themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
    m_ui->themeComboBox->addItem("Qt", QChart::ChartThemeQt);
}

void ThemeWidget::populateAnimationBox()
{
    // add items to animation combobox
    m_ui->animatedComboBox->addItem("All Animations", QChart::AllAnimations);
    m_ui->animatedComboBox->addItem("No Animations", QChart::NoAnimation);
    m_ui->animatedComboBox->addItem("GridAxis Animations", QChart::GridAxisAnimations);
    m_ui->animatedComboBox->addItem("Series Animations", QChart::SeriesAnimations);
}

void ThemeWidget::populateLegendBox()
{
    // add items to legend combobox
    m_ui->legendComboBox->addItem("Legend Left", Qt::AlignLeft);
    m_ui->legendComboBox->addItem("No Legend ", 0);
    m_ui->legendComboBox->addItem("Legend Top", Qt::AlignTop);
    m_ui->legendComboBox->addItem("Legend Bottom", Qt::AlignBottom);
    m_ui->legendComboBox->addItem("Legend Right", Qt::AlignRight);
}

void ThemeWidget::populateMethodBox()
{
    // add items to theme combobox
    m_ui->taskComboBox->addItem("FCFS");
    m_ui->taskComboBox->addItem("SJF");
    m_ui->taskComboBox->addItem("HRN");
    m_ui->taskComboBox->addItem("RR");
    m_ui->taskComboBox->addItem("SRT");
}

QChart *ThemeWidget::createBarChart(QString name, unsigned long* value) const
{
    unsigned long max = 0;

    //Q_UNUSED(value);
    QChart *chart = new QChart();
    chart->setTitle(name);

    QBarSeries *series = new QBarSeries(chart);
    QBarSet *set[maxAva];
    // data list num
    for (int i(0); i < 1; i++) {
        // one data Length
        for (int j = 0; j < maxAva; j++){
            set[j] = new QBarSet("Time: " + QString::number(j));
            unsigned long val = value[this->m_ui->taskComboBox->currentIndex()] + (rand() % 100000);

            if (max < val)
                max = val;

            *set[j] << val;

            series->append(set[j]);
        }
    }
    chart->addSeries(series);

    // calc top value
    unsigned long long top = 1;
    while (max > top)
        top *= 2;

    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0, top);

    return chart;
}

QChart *ThemeWidget::createHBarChart(int valueCount) const
{
    Q_UNUSED(valueCount);
    QChart *chart = new QChart();
    chart->setTitle("Gantt Chart");

    int met = this->m_ui->taskComboBox->currentIndex();
    unsigned long doneTime = tSlice[met][TaskNum-1].ret;

    QHorizontalPercentBarSeries *series = new QHorizontalPercentBarSeries(chart);

    QBarSet *set0 = new QBarSet("_ARRIVAL_TIME");
    QBarSet *set1 = new QBarSet("_WAIT_TIME");
    QBarSet *set2 = new QBarSet("_EXECUTE_TIME");
    QBarSet *set3 = new QBarSet("_DONE_TIME");

    *set0 << tSlice[met][0].arr << tSlice[met][1].arr << tSlice[met][2].arr << tSlice[met][3].arr << tSlice[met][4].arr << tSlice[met][5].arr << tSlice[met][6].arr << tSlice[met][7].arr;
    *set1 << tSlice[met][0].wait << tSlice[met][1].wait << tSlice[met][2].wait << tSlice[met][3].wait << tSlice[met][4].wait << tSlice[met][5].wait << tSlice[met][6].wait << tSlice[met][7].wait;
    *set2 << tSlice[met][0].exe << tSlice[met][1].exe << tSlice[met][2].exe << tSlice[met][3].exe << tSlice[met][4].exe << tSlice[met][5].exe << tSlice[met][6].exe << tSlice[met][7].exe;
    *set3 << doneTime - tSlice[met][0].ret << doneTime - tSlice[met][1].ret << doneTime - tSlice[met][2].ret << doneTime - tSlice[met][3].ret << doneTime - tSlice[met][4].ret << doneTime - tSlice[met][5].ret << doneTime - tSlice[met][6].ret << doneTime - tSlice[met][7].ret;

    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);


    chart->addSeries(series);

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 100);

    return chart;
}

QChart *ThemeWidget::createLineChart() const
{
    QChart *chart = new QChart();

    QLineSeries* series[3];
    series[0] = new QLineSeries();
    series[1] = new QLineSeries();
    series[2] = new QLineSeries();

    chart->setTitle("TASK SERISE");

    unsigned long long max(0UL), mBound(1UL);

    QString name("Task ");
    int nameIndex = 0;
    for (int i = 0; i<1; i++) {
        for (int t = 0; t < TaskNum; t++)
        {
            if (max < tSlice[i][t].task)
                max = tSlice[i][t].task;

            series[0]->append(t, tSlice[i][t].task);
            series[1]->append(t, tSlice[i][_taskOrder[1][t]].task * 2);
            series[2]->append(t, tSlice[i][_taskOrder[2][t]].task * 2);
        }

        series[0]->setName(name + "0");
        series[1]->setName(name + "1");
        series[2]->setName(name + "2");

        nameIndex++;
        chart->addSeries(series[0]);
        chart->addSeries(series[1]);
        chart->addSeries(series[2]);
    }

    while(mBound < max)
        mBound *= 2;

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 8);
    chart->axes(Qt::Vertical).first()->setRange(0, mBound);

    return chart;
}

QChart *ThemeWidget::createPieChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Avarege Task Time");

    QPieSeries *series = new QPieSeries(chart);
    QPieSlice *slice;
    for (int i = 0; i < 5; i++) {
        slice = series->append("ATT" + QString::number(i), ATT[i]);
        // show Detail current ATT
        if (this->m_ui->taskComboBox->currentIndex() == i){
            slice->setLabelVisible();
            slice->setExploded();
            slice->setExplodeDistanceFactor(0.3);
        }
    }
    series->setPieSize(0.6);
    chart->addSeries(series);

    return chart;
}

QChart *ThemeWidget::createPieChart1() const
{
    QChart *chart = new QChart();
    chart->setTitle("Avarege Wait Time");

    QPieSeries *series = new QPieSeries(chart);
    QPieSlice *slice;
    for (int i = 0; i < 5; i++) {
        slice = series->append("AWT" + QString::number(i), AWT[i]);
        slice->setBrush(QColor(0, 255, 0));
        slice->setColor(QColor(255, 0, 0));

        // show Detail current ATT
        if (this->m_ui->taskComboBox->currentIndex() == i){
            slice->setLabelVisible();
            slice->setExploded();
            slice->setExplodeDistanceFactor(0.3);
        }
    }
    series->setPieSize(0.6);
    chart->addSeries(series);

    return chart;
}

void ThemeWidget::updateUI()
{
    //![6]
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
                m_ui->themeComboBox->itemData(m_ui->themeComboBox->currentIndex()).toInt());

    //![6]
    const auto charts = m_charts;
    if (!m_charts.isEmpty()) {
        for (QChartView *chartView : charts) {
            //![7]
            chartView->chart()->setTheme(theme);
            //![7]
        }

        // Set palette colors based on selected theme
        //![8]
        QPalette pal = window()->palette();

        if (theme == QChart::ChartThemeLight) {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        //![8]
        } else if (theme == QChart::ChartThemeDark) {
            pal.setColor(QPalette::Window, QRgb(0x121218));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        } else if (theme == QChart::ChartThemeBlueCerulean) {
            pal.setColor(QPalette::Window, QRgb(0x40434a));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        } else if (theme == QChart::ChartThemeBrownSand) {
            pal.setColor(QPalette::Window, QRgb(0x9e8965));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeBlueNcs) {
            pal.setColor(QPalette::Window, QRgb(0x018bba));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeHighContrast) {
            pal.setColor(QPalette::Window, QRgb(0xffab03));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
        } else if (theme == QChart::ChartThemeBlueIcy) {
            pal.setColor(QPalette::Window, QRgb(0xcee7f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        window()->setPalette(pal);
    }

    // Update antialiasing
    //![11]
    bool checked = m_ui->antialiasCheckBox->isChecked();
    for (QChartView *chart : charts)
        chart->setRenderHint(QPainter::Antialiasing, checked);
    //![11]

    // Update animation options
    //![9]
    QChart::AnimationOptions options(
                m_ui->animatedComboBox->itemData(m_ui->animatedComboBox->currentIndex()).toInt());

    if (!ThemeWidget::m_charts.isEmpty()) {
        for (QChartView *chartView : charts)
            chartView->chart()->setAnimationOptions(options);
    }
    //![9]

    // Update legend alignment
    //![10]
    Qt::Alignment alignment(
                m_ui->legendComboBox->itemData(m_ui->legendComboBox->currentIndex()).toInt());

    if (!alignment) {
        for (QChartView *chartView : charts)
            chartView->chart()->legend()->hide();
    } else {
        for (QChartView *chartView : charts) {
            chartView->chart()->legend()->setAlignment(alignment);
            chartView->chart()->legend()->show();
        }
    }
    //![10]
}

int ThemeWidget::num_m_chart()
{
    return MChart->count();
}

QChartView* ThemeWidget::get_m_chart(int idx)
{
    if (idx < MChart->count()){
        qDebug() << "WA!";
        return MChart->at(idx);
    }
    return nullptr;
}

void ThemeWidget::set_m_chart(int idx, QChartView* widget)
{
    if (idx < MChart->count())
        (*MChart)[idx] = widget;
}

