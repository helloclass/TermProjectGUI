QT += charts
requires(qtConfig(combobox))

HEADERS += \
    main.h \
    setting.h \
    themewidget.h

SOURCES += \
    main.cpp \
    setting.cpp \
    themewidget.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/chartthemes
INSTALLS += target

FORMS += \
    setting.ui \
    themewidget.ui

DISTFILES +=
