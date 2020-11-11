#include "settingh.h"
#include "ui_Setting.h"

Setting::Setting(QWidget *parent) :
    m_listCount(1),
    m_ui(new Setting::Ui_ThemeWidgetForm))
    {
        m_ui->setupUi(this);
    }

Setting::~Setting()
{
    delete m_ui;
}
