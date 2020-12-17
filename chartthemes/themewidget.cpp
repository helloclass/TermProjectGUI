#include "QT_INCLUDE.h"

QList<QChartView *> *MChart;

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(1),
    m_valueMax(5),
    m_valueCount(3),
    tasknum(5),
    TQ(2),

    m_ui(new Ui_ThemeWidgetForm)
    {
        m_ui->setupUi(this);
        populateThemeBox();
        //populateAnimationBox();
        //populateLegendBox();
        populateMethodBox();

        //init chart info
        QFile file("/home/seongbokjeon/GOODLUCK/purified1");

        if (!file.open(QFile::ReadOnly|QFile::Text))
        {
            if (!file.exists())
            {
                qDebug() << "Can't found file!!";
            }
            else
            {
                qDebug() << "Can't open file!!";
            }
        }

        // init
        for (int i = 0; i < 10; i++)
            for (int j = 0; j< 40; j++)
            {
                tSlice[i][j].pid = 0;
                tSlice[i][j].arr = 0;
                tSlice[i][j].exe = 0;
                tSlice[i][j].pri = 0;
                tSlice[i][j].wait = 0;
                tSlice[i][j].resp = 0;
                tSlice[i][j].ret = 0;
            }

        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 30; j++)
                taskpri[i][j] = 0;


        QTextStream strstr(&file);

        for (int met = 0; met< 7; met++){
            // pid, arr, exe, pri, wait, resp, ret
            QString context = strstr.readLine();
            qDebug() << "METTTT" << met;

            if (!context.contains("round"))
            {
                taskpri[met][0] = context.toInt();
                qDebug() << "rr" <<taskpri[met][0];
                for (int round=1; round<tasknum; round++)
                {
                    context = strstr.readLine();
                    taskpri[met][round] = context.toInt();
                    qDebug() << "rr" <<taskpri[met][round];
                }

                for (int i = 0; i < tasknum; i++)
                {
                    // pid, arr, exe, pri, wait, resp, ret
                    context = strstr.readLine();

                    QTextStream vocaStr(&context);

                    tSlice[met][i].pid = vocaStr.read(2).toInt();
                    tSlice[met][i].arr = vocaStr.read(2).toInt();
                    tSlice[met][i].exe = vocaStr.read(2).toInt();
                    tSlice[met][i].pri = vocaStr.read(2).toInt();
                    tSlice[met][i].wait = vocaStr.read(2).toInt();
                    tSlice[met][i].resp = vocaStr.read(2).toInt();
                    tSlice[met][i].ret = vocaStr.read(2).toInt();
                    qDebug() << "TEST: " << tSlice[met][i].pid << tSlice[met][i].arr << tSlice[met][i].exe << tSlice[met][i].pri <<tSlice[met][i].wait << tSlice[met][i].resp <<tSlice[met][i].ret;
                }

                // AVERAGE
                context = strstr.readLine();
                QTextStream vocaStr(&context);
                QString voca;

                //awt
                vocaStr >> voca;
                vocaStr >> voca;

                AWT[met] = voca.toFloat();

                //art
                vocaStr >> voca;
                vocaStr >> voca;

                ART[met] = voca.toFloat();

                //att
                vocaStr >> voca;
                vocaStr >> voca;

                ATT[met] = voca.toFloat();
            }
            else
            {
                int round = 0;
                int seqPID = 0;

                // PROCESSED ROUND
                while (true)
                {
                    // pid, arr, exe, pri, wait, resp, ret
                    context = strstr.readLine();
                    QTextStream vocaStr(&context);
                    QString voca;
                    int pid;

                    vocaStr >> voca;
                    if (voca == "round")
                    {
                        vocaStr >> voca;
                        round = voca.toInt();
                        continue;
                    }
                    else if (voca == "awt")
                    {
                        //awt
                        vocaStr >> voca;

                        AWT[met] = voca.toFloat();

                        //art
                        vocaStr >> voca;
                        vocaStr >> voca;

                        ART[met] = voca.toFloat();

                        //att
                        vocaStr >> voca;
                        vocaStr >> voca;

                        ATT[met] = voca.toFloat();

                        break;
                    }
                    pid = voca.toInt();

                    tSlice[met][pid].pid = pid;

                    tSlice[met][pid].round = round;
                    vocaStr >> voca;

                    tSlice[met][pid + round * tasknum].arr = voca.toInt();
                    vocaStr >> voca;

                    tSlice[met][pid + round * tasknum].exe = voca.toInt();
                    taskpri[met][seqPID + round * tasknum] = pid;

                    qDebug() << "TT$" << met << (seqPID++) + round * tasknum<< pid;
                    vocaStr >> voca;

                    tSlice[met][pid + round * tasknum].pri = voca.toInt();
                    vocaStr >> voca;

                    tSlice[met][pid + round * tasknum].wait = voca.toInt();
                    vocaStr >> voca;

                    tSlice[met][pid + round * tasknum].resp = voca.toInt();
                    vocaStr >> voca;

                    tSlice[met][pid + round * tasknum].ret = voca.toInt();

                    qDebug() << "RRTEST: " << pid + round * tasknum << tSlice[met][pid].pid << tSlice[met][pid].arr << tSlice[met][pid].exe\
                             << tSlice[met][pid].pri <<tSlice[met][pid].wait << tSlice[met][pid].resp <<tSlice[met][pid].ret;
                }

           }
        }

        chartView = new QChartView(createHBarChart(tasknum));
        m_ui->gridLayout->addWidget(chartView, 1, 0, 1, 4);
        m_charts << chartView;

        chartView = new QChartView(createLineChart());
        m_ui->gridLayout->addWidget(chartView, 2, 0);
        m_charts << chartView;

        chartView = new QChartView(createBarChart("AWT", AWT, QColor(255, 0, 0)));
        m_ui->gridLayout->addWidget(chartView, 2, 1);
        m_charts << chartView;

        chartView = new QChartView(createBarChart("ART", ART, QColor(0, 255, 0)));
        m_ui->gridLayout->addWidget(chartView, 2, 2);
        m_charts << chartView;

        chartView = new QChartView(createBarChart("ATT", ATT, QColor(0, 0, 255)));
        m_ui->gridLayout->addWidget(chartView, 2, 3);
        m_charts << chartView;

        chartView = new QChartView(createGanttChart());
        chartView->setFixedWidth(300);
        m_ui->gridLayout->addWidget(chartView, 1, 4, 2, 4);
        m_charts << chartView;

        // Set the colors from the light theme as default ones
        QPalette pal = qApp->palette();
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
        qApp->setPalette(pal);

        MChart = &m_charts;

        this->updateUI();
    }

ThemeWidget::~ThemeWidget()
{    
    delete m_ui;
}

void ThemeWidget::Reload(){

    //QChartView *chartView;

    chartView = new QChartView(createHBarChart(tasknum));
    m_ui->gridLayout->addWidget(chartView, 1, 0, 1, 4);
    m_charts << chartView;

    chartView = new QChartView(createLineChart());
    m_ui->gridLayout->addWidget(chartView, 2, 0);
    m_charts << chartView;

    chartView = new QChartView(createBarChart("AWT", AWT, QColor(255, 0, 0)));
    m_ui->gridLayout->addWidget(chartView, 2, 1);
    m_charts << chartView;

    chartView = new QChartView(createBarChart("ART", ART, QColor(0, 255, 0)));
    m_ui->gridLayout->addWidget(chartView, 2, 2);
    m_charts << chartView;

    chartView = new QChartView(createBarChart("ATT", ATT, QColor(0, 0, 255)));
    m_ui->gridLayout->addWidget(chartView, 2, 3);
    m_charts << chartView;

    chartView = new QChartView(createGanttChart());
    chartView->setFixedWidth(300);
    m_ui->gridLayout->addWidget(chartView, 1, 4, 2, 4);
    m_charts << chartView;

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
    m_ui->themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    m_ui->themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    m_ui->themeComboBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
}

void ThemeWidget::populateMethodBox()
{
    // add items to theme combobox
    m_ui->taskComboBox->addItem("FCFS");
    m_ui->taskComboBox->addItem("SJF");
    m_ui->taskComboBox->addItem("HRN");
    m_ui->taskComboBox->addItem("RR");
    m_ui->taskComboBox->addItem("SRT");
    m_ui->taskComboBox->addItem("PP");
}

QChart *ThemeWidget::createBarChart(QString name, float* value, QColor color) const
{
    unsigned max = 1;

    //Q_UNUSED(value);
    QChart *chart = new QChart();
    chart->setTitle(name);

    QBarSeries *series = new QBarSeries(chart);
    QBarSet *set;

    series->setLabelsVisible(true);

    float val;
    // data list num
    for (int i(0); i < 1; i++) {
        // one data Length
        val = value[this->m_ui->taskComboBox->currentIndex()];
        set = new QBarSet(QString::number(val));
        set->setColor(color);

        *set << val;

        series->append(set);
    }
    chart->addSeries(series);

    while(max < val)
        max *= 3;

    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0, max);

    return chart;
}

QChart *ThemeWidget::createGanttChart() const
{
    int met = this->m_ui->taskComboBox->currentIndex();
    int top = 0, max = 1;


    //Q_UNUSED(value);
    QChart *chart = new QChart();
    chart->setTitle("GANTT");

    QStackedBarSeries *series = new QStackedBarSeries(chart);
    //QBarSet *set = new QBarSet("GANTT");
    QBarSet* set[tasknum * 2];
    for (int i = 0; i<tasknum * 2; i++){
        set[i] = new QBarSet("PID" + QString::number(taskpri[met][i % tasknum]));
    }

    series->setLabelsVisible(true);

    float val;
    // data list num
    for (int j(0); j<tasknum * 2; j++){
        if (j < tasknum)
            val = tSlice[met][taskpri[met][j%tasknum]].exe;
        else
            val = tSlice[met][taskpri[met][j%tasknum] + tasknum].exe;

        if (val <= 0)
            continue;

        top += val;

        *set[j] << val;
        series->append(set[j]);
    }
    chart->addSeries(series);

    max = top;

    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0, max);

    return chart;
}

QChart *ThemeWidget::createHBarChart(int valueCount) const
{
    Q_UNUSED(valueCount);
    QChart *chart = new QChart();
    chart->setTitle("Gantt Chart");

    int met = this->m_ui->taskComboBox->currentIndex();
    int doneTime = 0, d;

    for (int t = 0; t < tasknum; t++)
    {
        doneTime += tSlice[met][t].exe;
    }

    d = doneTime;
    doneTime = 15;

    QHorizontalStackedBarSeries *series = new QHorizontalStackedBarSeries(chart);
    series->setLabelsVisible(true);

    QBarSet *set0 = new QBarSet("_ARRIVAL_TIME");
    QBarSet *set1 = new QBarSet("_WAIT_TIME");
    QBarSet *set2 = new QBarSet("_EXECUTE_TIME");
    QBarSet *set3 = new QBarSet("_DONE_TIME");
    QBarSet *set4 = new QBarSet("NEXT");

    for (int t = 0; t < tasknum; t++){
        *set0 << tSlice[met][t].arr;
        *set1 << tSlice[met][t].wait;
        *set2 << tSlice[met][t].exe;
        *set3 << (d - tSlice[met][t].ret - tSlice[met][t].arr);

        int next = tSlice[met][t + tasknum].exe;
        *set4 << next;
        d += next;
    }

    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);

    chart->addSeries(series);

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, doneTime);

    return chart;
}

QChart *ThemeWidget::createLineChart() const
{
    QChart *chart = new QChart();
    int met = this->m_ui->taskComboBox->currentIndex();

    QLineSeries* series = new QLineSeries();


    chart->setTitle("TASK SERISE");

    int max(0), mBound(1);

    QString name("Task ");
    for (int i = 0; i<1; i++)
    {
        for (int tt = 0; tt<tasknum; tt++){
            if (max < tSlice[met][taskpri[met][tt]].exe)
                max = tSlice[met][taskpri[met][tt]].exe;

            series->append(tt, tSlice[met][taskpri[met][tt]].exe);
        }
    }

    series->setName(this->m_ui->taskComboBox->currentText());

    chart->addSeries(series);

    while(mBound < max)
        mBound *= 2;

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, tasknum - 1);
    chart->axes(Qt::Vertical).first()->setRange(0, mBound);

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
        for (QChartView *chartView : charts)
        {
            chartView->chart()->setTheme(theme);
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

    if (!ThemeWidget::m_charts.isEmpty()) {
        for (QChartView *chartView : charts) {
            chartView->chart()->setAnimationOptions(QChart::AllAnimations);
            chartView->chart()->legend()->setAlignment(Qt::AlignLeft);
            chartView->chart()->legend()->show();
        }
    }
}
