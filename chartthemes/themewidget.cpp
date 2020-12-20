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
        QFile file("/home/seongbok/GOODLUCK/purified1");

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
            int rstack = 0;

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

                tSlice[met][tasknum].pid = -1;
            }
            else
            {
                // PROCESSED ROUND
                while (true)
                {
                    // pid, arr, exe, pri, wait, resp, ret
                    context = strstr.readLine();
                    QTextStream vocaStr(&context);
                    QString voca;
                    int pid;

                    vocaStr >> voca;

                    if (voca == "awt")
                    {
                        tSlice[met][rstack].pid = -1;

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

                    tSlice[met][rstack].pid = pid;
                    vocaStr >> voca;

                    tSlice[met][rstack].arr = voca.toInt();
                    vocaStr >> voca;

                    tSlice[met][rstack].exe = voca.toInt();
                    vocaStr >> voca;

                    tSlice[met][rstack].pri = voca.toInt();
                    vocaStr >> voca;

                    tSlice[met][rstack].wait = voca.toInt();
                    vocaStr >> voca;

                    tSlice[met][rstack].resp = voca.toInt();
                    vocaStr >> voca;

                    tSlice[met][rstack].ret = voca.toInt();

                    qDebug() << "RRTEST: " << tSlice[met][rstack].pid << tSlice[met][rstack].arr << tSlice[met][rstack].exe\
                             << tSlice[met][rstack].pri <<tSlice[met][rstack].wait << tSlice[met][rstack].resp <<tSlice[met][rstack].ret;

                    rstack += 1;
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
        pal.setColor(QPalette::Window, QRgb(0xcee7f0));
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

//    chartView = new QChartView(createGanttChart());
//    chartView->setFixedWidth(300);
//    m_ui->gridLayout->addWidget(chartView, 1, 4, 2, 4);
//    m_charts << chartView;

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
    m_ui->taskComboBox->addItem("NPP");
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
    int TN = tasknum;

    for (int i = 0; i < 40; i++)
        if (tSlice[met][i].pid == -1)
            TN = i;

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
    for (int j(0); j<TN; j++){
        val = tSlice[met][j].exe;

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
    QChart *chart = new QChart();
    chart->setTitle("Gantt Chart");

    QColor waitColor = QColor(255, 0, 0);
    QColor exeColor = QColor(0, 255, 0);

    int preTime = 0;

    int met = this->m_ui->taskComboBox->currentIndex();
    int TN = tasknum;
    int doneTime = 0, d;

    for (int i = 0; i < 40; i++)
        if (tSlice[met][i].pid == -1)
            TN = i;

    for (int t = 0; t < tasknum; t++)
    {
        doneTime += tSlice[met][t].exe;
    }

    qDebug() << "TOT_TASK_NUM: " << doneTime;

    d = doneTime;
    doneTime = 15;

    QHorizontalStackedBarSeries *series = new QHorizontalStackedBarSeries(chart);
    series->setLabelsVisible(true);

    QBarSet *set0 = new QBarSet("_ARRIVAL_TIME");
    QBarSet *set1 = new QBarSet("_WAIT_TIME");
    QBarSet *set2 = new QBarSet("_EXECUTE_TIME");
    QBarSet *set3 = new QBarSet("_DONE_TIME");

    set1->setColor(waitColor);
    set2->setColor(exeColor);

    // arr
    for (int t = 0; t < tasknum; t++){
        int pos2arr = 0;
        for (int count = 0; count < TN; count++)
            if (tSlice[met][count].pid == t)
                pos2arr = tSlice[met][count].arr;

        *set0 << pos2arr;
    }
    series->append(set0);

    int stackCount[30] = {0};
    int complimentations[30] = {0};

    // other
    for (int t = 0; t < TN*2; t++){
        int pos = -1, c = 0, b = stackCount[t % tasknum];

        for (int count = 0; count < TN; count++){
            //qDebug() << "t " << t%tasknum << "p " << tSlice[met][count].pid << "count " << count;
            if ((t % tasknum) == tSlice[met][count].pid){
                if (c < b)
                {
                    c++;
                    continue;
                }

                pos = count;
                stackCount[t % tasknum]++;
                break;
            }
        }

        if (pos == -1)
        {
            *set1 << 0;
            *set2 << 0;
        }
        else
        {
            *set1 << (tSlice[met][pos].wait - complimentations[t % tasknum]);
            *set2 << tSlice[met][pos].exe;
            complimentations[t % tasknum] = (tSlice[met][pos].exe + tSlice[met][pos].wait);
        }

        if ((t + 1) % tasknum == 0)
        {
            series->append(set1);
            series->append(set2);

            set1 = new QBarSet("_WAIT_TIME");
            set2 = new QBarSet("_EXECUTE_TIME");

            set1->setColor(waitColor);
            set2->setColor(exeColor);
        }
    }

    //for (int i = 0; i<tasknum; i++){
        //*set3 << (d - tSlice[met][pos].ret - tSlice[met][pos].arr);
    //}
    series->append(set3);

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
    int TN = 0;

    for (int i = 0; i < 40; i++)
        if (tSlice[met][i].pid == -1)
            TN = i;

    chart->setTitle("TASK SERISE");

    int max(0), mBound(1);

    QString name("Task ");
    for (int i = 0; i<1; i++)
    {
        for (int tt = 0; tt<TN; tt++){
            if (max < tSlice[met][tSlice[met][tt].pri].exe)
                max = tSlice[met][tSlice[met][tt].pri].exe;

            series->append(tt, tSlice[met][tSlice[met][tt].pri].exe);
        }
    }

    series->setName(this->m_ui->taskComboBox->currentText());

    chart->addSeries(series);

    while(mBound < max)
        mBound *= 2;

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, TN);
    chart->axes(Qt::Vertical).first()->setRange(0, 8);

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
//        for (QChartView *chartView : charts)
//        {
//            chartView->chart()->setTheme(theme);
//        }
    }

    if (!ThemeWidget::m_charts.isEmpty()) {
        for (QChartView *chartView : charts) {
            chartView->chart()->setAnimationOptions(QChart::AllAnimations);
            chartView->chart()->legend()->setAlignment(Qt::AlignLeft);
            chartView->chart()->legend()->show();
        }
    }
}
