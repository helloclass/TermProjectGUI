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

#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class Ui_ThemeWidgetForm;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = 0);
    QList<QChartView *> m_charts;
    Ui_ThemeWidgetForm *m_ui;

    float ATT[7];
    float ART[7];
    float AWT[7];

    QChartView *chartView;

    int TaskNum;
    typedef struct timeSlice{
        int round;
        int pid;
        int arr;
        int exe;
        int pri;
        int wait;
        int resp;
        int ret;
    }timeSlice;
    timeSlice tSlice[10][40];

    void Reload();

    ~ThemeWidget();

private Q_SLOTS:
    void updateUI();
    void on_pushButton_clicked();

private:
    void populateThemeBox();
    //void populateAnimationBox();
    void populateLegendBox();
    void populateMethodBox();
    void connectSignals();
    QChart *createBarChart(QString name, float* valueCount, QColor color) const;
    QChart *createGanttChart() const;
    QChart *createHBarChart(int valueCount) const;
    QChart *createATTChart() const;
    QChart *createPieChart1() const;
    QChart *createLineChart() const;


private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;
    const int tasknum;
    int TQ;

    // save TASK PRIORITY
    int taskpri[10][30];
};

#endif /* THEMEWIDGET_H */
