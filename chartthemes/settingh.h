#ifndef SETTINGH_H
#define SETTINGH_H

#include <QtWidgets/QDialog>

class Ui_ThemeWidgetForm;

// Setting header
class Setting: public QDialog
{
    Q_OBJECT
public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();

private Q_SLOTS:
    void updateUI();

private:
    Ui_ThemeWidgetForm* m_ui;
    int m_listCount;

};

#endif // SETTINGH_H
