#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QButtonGroup>
#include "main_view.h"
#include "settings_view.h"
#include "about_view.h"
#include "theme.h"

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

private slots:

    void tabbar_nav_btn_clicked_cb(void);
    void tabbar_btn_group_clicked_cb(QAbstractButton *button);
    void tabbar_theme_btn_clicked_cb(void);

private:
    Ui::MainWindow *ui;
    QButtonGroup *tabbar_buttonGroup;
    bool tabbar_is_expanded = false;
    int tabbar_btn_icon_size;

    MainView *main_view;
    SettingsView *settings_view;
    AboutView *about_view;

    QIcon render_svg_icon(const QString &path, const QColor &color);
    void set_navigate_expand(bool expanded);
    QIcon get_tabbar_group_btn_icon(int index);
    void set_tabbar_nav_btn_icon(bool expanded);
    void set_tabbar_theme_btn_icon();
    void set_tabbar_btn_theme();

    void set_main_window_theme();
};
#endif // MAINWINDOW_H
