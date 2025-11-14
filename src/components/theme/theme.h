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

    QColor get_background_color() { return background_color; } // 背景色
    QColor get_foreground_color() { return foreground_color; } // 前景色
    QColor get_accent_color() { return accent_color; }         // 强调色
    QColor get_highlight_color() { return highlight_color; }   // 高亮色

private:
    static QList<Theme *> themes;

    QColor background_color; // 背景色
    QColor foreground_color; // 前景色，文本和线条颜色
    QColor accent_color;     // 强调色，用于突出重要元素
    QColor highlight_color;  // 高亮色，用于选中元素
};

#endif // THEME_H
