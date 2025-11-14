#include <QTabBar>
#include <QPainter>
#include <QSvgRenderer>
#include <QMetaEnum>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "config.h"

static const QString red_border_style = R""(

border-style: solid;
border-color: #FF0000;
border-width: 2px;
border-radius: 0px;

)"";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tabbar_is_expanded = false;
    tabbar_btn_icon_size = 36;

    connect(ui->tabbar_nav_btn, &QToolButton::clicked, this, &MainWindow::tabbar_nav_btn_clicked_cb);     // 导航栏折叠按钮
    connect(ui->tabbar_theme_btn, &QToolButton::clicked, this, &MainWindow::tabbar_theme_btn_clicked_cb); // 主题切换按钮

    tabbar_buttonGroup = new QButtonGroup(this);
    tabbar_buttonGroup->setExclusive(true); // 互斥
    tabbar_buttonGroup->addButton(ui->tabbar_home_btn, TABBAR_HOME);
    tabbar_buttonGroup->addButton(ui->tabbar_setting_btn, TABBAR_SETTINGS);
    tabbar_buttonGroup->addButton(ui->tabbar_about_btn, TABBAR_ABOUT);

    connect(tabbar_buttonGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(tabbar_btn_group_clicked_cb(QAbstractButton *)));


    main_view = new MainView(this);
    settings_view = new SettingsView(this);
    about_view = new AboutView(this);

    // remove all tabs
    while (ui->tabWidget->count())
        ui->tabWidget->removeTab(0);

    ui->tabWidget->addTab(main_view, QMetaEnum::fromType<MainWindow::TABBAR>().valueToKey(TABBAR_HOME));
    ui->tabWidget->addTab(settings_view, QMetaEnum::fromType<MainWindow::TABBAR>().valueToKey(TABBAR_SETTINGS));
    ui->tabWidget->addTab(about_view, QMetaEnum::fromType<MainWindow::TABBAR>().valueToKey(TABBAR_ABOUT));

    ui->tabWidget->tabBar()->setVisible(false); // 隐藏tabbar

    set_navigate_expand(tabbar_is_expanded);
    set_tabbar_btn_theme();
    set_main_window_theme();

    auto cur_theme = Theme::get_theme(Config::get_global()->theme);

    ui->tabwidget_outline_widget->setStyleSheet(R""(
QWidget#tabwidget_outline_widget {
    background-color: transparent;
}
)"");

    ui->tabWidget->setStyleSheet(R""(
QTabWidget::pane{
    background: transparent;
    border:none;
}
)"");

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*******************************************************************************
 * @brief 回调函数：导航栏折叠按钮点击
 ******************************************************************************/
void MainWindow::tabbar_nav_btn_clicked_cb(void)
{
    tabbar_is_expanded = !tabbar_is_expanded;
    set_navigate_expand(tabbar_is_expanded);
}

/*******************************************************************************
 * @brief 回调函数：导航栏按钮组点击
 ******************************************************************************/
void MainWindow::tabbar_btn_group_clicked_cb(QAbstractButton *button)
{
    // qDebug("[MainWindow] tabbar_btn_group_clicked_cb name:%s", qUtf8Printable(button->text()));

    int btn_i = tabbar_buttonGroup->id(button);
    Q_ASSERT_X(btn_i >= 0, "MainWindow::tabbar_btn_group_clicked_cb", "Unknown button clicked");

    // qDebug("[MainWindow] tabbar_btn_group_clicked_cb index:%d", btn_i);

    ui->tabWidget->setCurrentIndex(btn_i);
}

/*******************************************************************************
 * @brief 回调函数：主题切换按钮点击
 ******************************************************************************/
void MainWindow::tabbar_theme_btn_clicked_cb(void)
{
    if (Config::get_global()->theme == Theme::LIGHT)
    {
        Config::get_global()->theme = Theme::DARK;
    }
    else
    {
        Config::get_global()->theme = Theme::LIGHT;
    }

    set_main_window_theme();
    set_tabbar_btn_theme();

    Config::get_global()->save_to_file();
}

/*******************************************************************************
 * @brief 设置导航栏展开状态
 * @param expanded true:展开 false:收缩
 * @return
 ******************************************************************************/
void MainWindow::set_navigate_expand(bool expanded)
{
    set_tabbar_nav_btn_icon(expanded);

    if (expanded)
    {
        // 展开状态

        ui->tabbar_nav_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->tabbar_theme_btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        for (uint32_t i = 0; i < tabbar_buttonGroup->buttons().size(); i++)
        {
            QToolButton *btn = qobject_cast<QToolButton *>(tabbar_buttonGroup->buttons().at(i));
            if (btn == nullptr)
                continue;

            btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        }
    }
    else
    {
        // 收缩状态
        ui->tabbar_nav_btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        ui->tabbar_theme_btn->setToolButtonStyle(Qt::ToolButtonIconOnly);

        for (uint32_t i = 0; i < tabbar_buttonGroup->buttons().size(); i++)
        {
            QToolButton *btn = qobject_cast<QToolButton *>(tabbar_buttonGroup->buttons().at(i));
            if (btn == nullptr)
                continue;

            btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
        }
    }
}

QIcon MainWindow::get_tabbar_group_btn_icon(int index)
{
    QString name = "";

    auto cur_theme = Theme::get_theme(Config::get_global()->theme);

    switch (index)
    {
    case TABBAR_HOME:
        name += QString("home");
        break;
    case TABBAR_SETTINGS:
        name += QString("settings");
        break;
    case TABBAR_ABOUT:
        name += QString("info");
        break;
    default:
        Q_ASSERT_X(false, "MainWindow::get_tabbar_btn_group_icon", "Unknown TABBAR_BTN_INDEX");
    }

    Q_ASSERT_X(name.length() != 0, "MainWindow::get_tabbar_btn_group_icon", "name is empty");

    QString path = QString(":/%1_24dp_000000_FILL0_wght400_GRAD0_opsz24.svg").arg(name);

    // qDebug("[MainWindow] get_tabbar_group_btn_icon index:%d theme:%d path: \"%s\"", index, theme, qPrintable(path));

    return render_svg_icon(path, cur_theme->get_foreground_color());
}

/*******************************************************************************
 * @brief 渲染SVG图标
 * @param path  SVG文件路径
 * @param color 图标颜色
 * @return QIcon对象
 ******************************************************************************/
QIcon MainWindow::render_svg_icon(const QString &path, const QColor &color)
{
    QSize baseSize(tabbar_btn_icon_size, tabbar_btn_icon_size);
    QSize actualSize = baseSize * 1;

    QPixmap pixmap(actualSize);
    pixmap.setDevicePixelRatio(1);
    pixmap.fill(Qt::transparent);

    QSvgRenderer renderer(path);
    Q_ASSERT_X(renderer.isValid(), "MainWindow::render_svg_icon", "valid current document");

    QPainter painter(&pixmap);
    renderer.render(&painter);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(QRect(QPoint(0, 0), actualSize), color);
    painter.end();

    return QIcon(pixmap);
}

/*******************************************************************************
 * @brief 设置导航栏导航按钮的图标
 * @param expanded 导航栏是否展开
 * @param theme 主题类型
 ******************************************************************************/
void MainWindow::set_tabbar_nav_btn_icon(bool expanded)
{
    auto cur_theme = Theme::get_theme(Config::get_global()->theme);

    QString icon_path = QString(":/%1_24dp_000000_FILL0_wght400_GRAD0_opsz24.svg")
                            .arg(expanded ? "arrow_menu_close" : "arrow_menu_open");

    QIcon btn_icon = render_svg_icon(icon_path, cur_theme->get_foreground_color());

    ui->tabbar_nav_btn->setIcon(btn_icon);
    ui->tabbar_nav_btn->setIconSize(QSize(tabbar_btn_icon_size, tabbar_btn_icon_size));
    ui->tabbar_nav_btn->setFont(Config::get_global()->font);
}

/*******************************************************************************
 * @brief 设置导航栏主题切换按钮的图标
 * @param theme 主题类型
 ******************************************************************************/
void MainWindow::set_tabbar_theme_btn_icon()
{
    auto cur_theme = Theme::get_theme(Config::get_global()->theme);
    QString name = "";

    switch (Config::get_global()->theme)
    {
    case Theme::LIGHT:
        name = QString("dark_mode");
        break;
    case Theme::DARK:
        name = QString("light_mode");
        break;
    default:
        Q_ASSERT_X(false, "MainWindow::set_tabbar_theme_btn_icon", "Unknown THEME");
    }

    QString icon_path = QString(":/%1_24dp_000000_FILL0_wght400_GRAD0_opsz24.svg").arg(name);

    QIcon btn_icon = render_svg_icon(icon_path, cur_theme->get_foreground_color());
    ui->tabbar_theme_btn->setIcon(btn_icon);
    ui->tabbar_theme_btn->setIconSize(QSize(tabbar_btn_icon_size, tabbar_btn_icon_size));
    ui->tabbar_theme_btn->setFont(Config::get_global()->font);
}

/*******************************************************************************
 * @brief 设置导航栏所有按钮的主题
 * @param theme 主题类型
 ******************************************************************************/
void MainWindow::set_tabbar_btn_theme()
{
    
    set_tabbar_nav_btn_icon(tabbar_is_expanded);
    set_tabbar_theme_btn_icon();

    auto cur_theme = Theme::get_theme(Config::get_global()->theme);

    for (uint32_t btn_i = 0; btn_i < tabbar_buttonGroup->buttons().size(); btn_i++)
    {
        QToolButton *cur_btn = qobject_cast<QToolButton *>(tabbar_buttonGroup->buttons().at(btn_i));
        if (cur_btn == nullptr)
            continue;

        QIcon btn_icon = get_tabbar_group_btn_icon(btn_i);

        cur_btn->setIcon(btn_icon);
        cur_btn->setIconSize(QSize(tabbar_btn_icon_size, tabbar_btn_icon_size));
        cur_btn->setFont(Config::get_global()->font);
    }
}

void MainWindow::set_main_window_theme()
{
    auto cur_theme = Theme::get_theme(Config::get_global()->theme);


    // setStyleSheet(cur_theme->get_main_window_style());

    setStyleSheet(QString(R""(
QMainWindow {
    background-color: %1;
}
)"").arg(cur_theme->get_background_color().name(QColor::HexArgb)));

//     ui->tabWidget->setStyleSheet(QString(R""(
// QTabWidget {
//     background-color: #%1;
// }
// )"").arg(cur_theme->get_background_color().rgb() & 0xFFFFF, 6, 16, QChar('0')));

    auto tarbar_nav_btn_style_template = QString(R""(
QToolButton {
    color: %1;
    background-color: transparent;

    border-style: solid;
    border-color: rgba(0,0,0,0);
    border-width: 0px;
    border-radius: 0px;
}
)"");

    auto tarbar_nav_btn_style = QString(tarbar_nav_btn_style_template)
                                    .arg(cur_theme->get_foreground_color().name(QColor::HexArgb));

    auto tarbar_btn_style_tmplate = QString(R""(
QToolButton {
    color: %1;
    background-color: transparent;

    border-style: solid;
    border-color: rgba(0,0,0,0);
    border-width: 0px;
    border-radius: 0px;
}

QToolButton::checked {
    border-left: 2px solid %2;
}

)"");

    auto tarbar_btn_style = QString(tarbar_btn_style_tmplate)
                                .arg(cur_theme->get_foreground_color().name(QColor::HexArgb))
                                .arg(cur_theme->get_highlight_color().name(QColor::HexArgb));

    ui->tabbar_nav_btn->setStyleSheet(tarbar_nav_btn_style);
    ui->tabbar_theme_btn->setStyleSheet(tarbar_btn_style);

    for (uint32_t i = 0; i < tabbar_buttonGroup->buttons().size(); i++)
    {
        QToolButton *btn = qobject_cast<QToolButton *>(tabbar_buttonGroup->buttons().at(i));
        if (btn == nullptr)
            continue;

        btn->setStyleSheet(tarbar_btn_style);
    }

    QString tabbar_widget_style = QString(R""(
QWidget#tabbar_widget {
    background-color: %2;
    border-right: 0px solid %1;
}
)"")
                                      .arg(cur_theme->get_foreground_color().name(QColor::HexArgb))
                                      .arg(cur_theme->get_accent_color().name(QColor::HexArgb));

    ui->tabbar_widget->setStyleSheet(tabbar_widget_style);

    about_view->set_theme();
}
