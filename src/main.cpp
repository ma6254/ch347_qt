#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "theme.h"
#include "config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ch347_qt_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    
    // 配置文件初始化
    Config::global_init();

    // 主题初始化
    Theme::theme_init();

    MainWindow w;
    w.show();
    return a.exec();
}
