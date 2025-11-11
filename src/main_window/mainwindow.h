#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QButtonGroup>
#include "main_view.h"
#include "settings_view.h"
#include "about_view.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum TABBAR
    {
        TABBAR_HOME = 0,
        TABBAR_SETTINGS,
        TABBAR_ABOUT,

    };
    Q_ENUM(TABBAR);

    enum THEME
    {
        THEME_LIGHT = 0,
        THEME_DARK,
    };
    Q_ENUM(THEME);

private slots:

    void tabbar_nav_btn_clicked_cb(void);
    void tabbar_btn_group_clicked_cb(QAbstractButton *button);
    void tabbar_theme_btn_clicked_cb(void);

private:
    Ui::MainWindow *ui;
    QButtonGroup *tabbar_buttonGroup;
    bool tabbar_is_expanded = false;
    int theme;
    int tabbar_btn_icon_size;
    QFont tabbar_btn_font;


    MainView *main_view;
    SettingsView *settings_view;
    AboutView *about_view;

    QIcon render_svg_icon(const QString &path, const QColor &color);
    QColor get_tabbar_btn_theme_color(int theme);
    void set_navigate_expand(bool expanded);
    QIcon get_tabbar_group_btn_icon(int index, int theme);
    void set_tabbar_nav_btn_icon(bool expanded, int theme);
    void set_tabbar_theme_btn_icon(int theme);
    void set_tabbar_btn_theme(int theme);
};
#endif // MAINWINDOW_H
