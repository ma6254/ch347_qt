#ifndef THEME_H
#define THEME_H

#include <QObject>
#include <QColor>

class Theme : public QObject
{
    Q_OBJECT

public:
    enum THEME
    {
        LIGHT = 0,
        DARK,
    };
    Q_ENUM(THEME);

    Theme();
    ~Theme();

    static void theme_init();
    static Theme *get_theme(Theme::THEME theme_type);

    /*******************************************************************************
     * @brief 获取导航栏按钮图标主题颜色
     ******************************************************************************/
    QColor get_tabbar_btn_icon_color() { return tabbar_btn_icon_color; };

    /*******************************************************************************
     * @brief 获取主窗口样式表
     ******************************************************************************/
    QString get_main_window_style() { return main_window_style; };

private:
    static QList<Theme *> themes;

    QColor tabbar_btn_icon_color;
    QString main_window_style;
};

#endif // THEME_H
