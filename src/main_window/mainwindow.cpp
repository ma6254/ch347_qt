#include <QTabBar>
#include <QPainter>
#include <QSvgRenderer>
#include <QMetaEnum>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

static const QString tabbar_nav_btn_light_style = R""(
QToolButton {

    color: rgba(0,0,0,1.0);

    border-style: solid;
    border-color: rgba(0,0,0,0);
    border-width: 0px;
    border-radius: 0px;
}
)"";

static const QString tabbar_nav_btn_dark_style = R""(
QToolButton {

    color: rgba(255,255,255,1.0);

    border-style: solid;
    border-color: rgba(0,0,0,0);
    border-width: 0px;
    border-radius: 0px;
}
)"";

static const QString red_border_style = R""(

border-style: solid;
border-color: #FF0000;
border-width: 2px;
border-radius: 0px;

)"";

static const QString style_btn_tabbar = R""(
QToolButton {
    color: rgba(255,255,255,1.0);

    border-style: solid;
    border-color: rgba(0,0,0,0);
    border-width: 0px;
    border-radius: 0px;
}

QToolButton::checked {
    background-color: rgba(0,147,221,0.7);

}

QToolButton::!checked {
    background-color: rgba(20,45,86,0.5);
}
)"";

static const QString main_window_light_style = R""(
MainWindow {
    background-color: #FFFFFF;
}
)"";

static const QString main_window_dark_style = R""(
MainWindow {
    background-color: #434343;
}
)"";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tabbar_is_expanded = false;
    theme = THEME_LIGHT;
    tabbar_btn_icon_size = 42;
    tabbar_btn_font = QFont("Microsoft YaHei", 12);

    // ui->tabbar_widget->setStyleSheet(red_border_style);
    ui->tabbar_theme_btn->setStyleSheet(style_btn_tabbar);

    connect(ui->tabbar_nav_btn, &QToolButton::clicked, this, &MainWindow::tabbar_nav_btn_clicked_cb);     // 导航栏折叠按钮
    connect(ui->tabbar_theme_btn, &QToolButton::clicked, this, &MainWindow::tabbar_theme_btn_clicked_cb); // 主题切换按钮

    tabbar_buttonGroup = new QButtonGroup(this);
    tabbar_buttonGroup->setExclusive(true); // 互斥
    tabbar_buttonGroup->addButton(ui->tabbar_home_btn, TABBAR_HOME);
    tabbar_buttonGroup->addButton(ui->tabbar_setting_btn, TABBAR_SETTINGS);
    tabbar_buttonGroup->addButton(ui->tabbar_about_btn, TABBAR_ABOUT);

    connect(tabbar_buttonGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(tabbar_btn_group_clicked_cb(QAbstractButton *)));

    for (uint32_t i = 0; i < tabbar_buttonGroup->buttons().size(); i++)
    {
        QToolButton *btn = qobject_cast<QToolButton *>(tabbar_buttonGroup->buttons().at(i));
        if (btn == nullptr)
            continue;

        btn->setStyleSheet(style_btn_tabbar);
    }

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
    set_tabbar_btn_theme(theme);
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
    if (theme == THEME_LIGHT)
    {
        theme = THEME_DARK;
    }
    else
    {
        theme = THEME_LIGHT;
    }

    switch (theme)
    {
    case THEME_LIGHT:
        setStyleSheet(main_window_light_style);
        ui->tabbar_nav_btn->setStyleSheet(tabbar_nav_btn_light_style);
        
        break;
        case THEME_DARK:
        setStyleSheet(main_window_dark_style);
        ui->tabbar_nav_btn->setStyleSheet(tabbar_nav_btn_dark_style);
        break;
    }

    set_tabbar_btn_theme(theme);
}

/*******************************************************************************
 * @brief 设置导航栏展开状态
 * @param expanded true:展开 false:收缩
 * @return
 ******************************************************************************/
void MainWindow::set_navigate_expand(bool expanded)
{
    set_tabbar_nav_btn_icon(expanded, theme);

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

QIcon MainWindow::get_tabbar_group_btn_icon(int index, int theme)
{
    QString name = "";

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

    return render_svg_icon(path, get_tabbar_btn_theme_color(theme));
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
    QPainter painter(&pixmap);
    renderer.render(&painter);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(QRect(QPoint(0, 0), actualSize), color);
    painter.end();

    return QIcon(pixmap);
}

/*******************************************************************************
 * @brief 获取导航栏按钮主题颜色
 * @param theme 主题类型
 * @return QColor对象
 ******************************************************************************/
QColor MainWindow::get_tabbar_btn_theme_color(int theme)
{
    QColor color = QColor(0, 0, 0);

    switch (theme)
    {
    case THEME_LIGHT:
        color = QColor("#434343");
        break;
    case THEME_DARK:
        color = QColor("#F3F3F3");
        break;
    default:
        Q_ASSERT_X(false, "MainWindow::set_theme", "Unknown THEME");
    }

    return color;
}

/*******************************************************************************
 * @brief 设置导航栏导航按钮的图标
 * @param expanded 导航栏是否展开
 * @param theme 主题类型
 ******************************************************************************/
void MainWindow::set_tabbar_nav_btn_icon(bool expanded, int theme)
{
    QString icon_path = QString(":/%1_24dp_000000_FILL0_wght400_GRAD0_opsz24.svg")
                            .arg(expanded ? "arrow_menu_close" : "arrow_menu_open");

    QIcon btn_icon = render_svg_icon(icon_path, get_tabbar_btn_theme_color(theme));

    ui->tabbar_nav_btn->setIcon(btn_icon);
    ui->tabbar_nav_btn->setIconSize(QSize(tabbar_btn_icon_size, tabbar_btn_icon_size));
    ui->tabbar_nav_btn->setFont(tabbar_btn_font);
}

/*******************************************************************************
 * @brief 设置导航栏主题切换按钮的图标
 * @param theme 主题类型
 ******************************************************************************/
void MainWindow::set_tabbar_theme_btn_icon(int theme)
{
    QString name = "";

    switch (theme)
    {
    case THEME_LIGHT:
        name = QString("dark_mode");
        break;
    case THEME_DARK:
        name = QString("light_mode");
        break;
    default:
        Q_ASSERT_X(false, "MainWindow::set_tabbar_theme_btn_icon", "Unknown THEME");
    }

    QString icon_path = QString(":/%1_24dp_000000_FILL0_wght400_GRAD0_opsz24.svg").arg(name);

    QIcon btn_icon = render_svg_icon(icon_path, get_tabbar_btn_theme_color(theme));
    ui->tabbar_theme_btn->setIcon(btn_icon);
    ui->tabbar_theme_btn->setIconSize(QSize(tabbar_btn_icon_size, tabbar_btn_icon_size));
    ui->tabbar_theme_btn->setFont(tabbar_btn_font);
}

/*******************************************************************************
 * @brief 设置导航栏所有按钮的主题
 * @param theme 主题类型
 ******************************************************************************/
void MainWindow::set_tabbar_btn_theme(int theme)
{
    set_tabbar_nav_btn_icon(tabbar_is_expanded, theme);
    set_tabbar_theme_btn_icon(theme);

    for (uint32_t btn_i = 0; btn_i < tabbar_buttonGroup->buttons().size(); btn_i++)
    {
        QToolButton *cur_btn = qobject_cast<QToolButton *>(tabbar_buttonGroup->buttons().at(btn_i));
        if (cur_btn == nullptr)
            continue;

        QIcon btn_icon = get_tabbar_group_btn_icon(btn_i, theme);

        cur_btn->setIcon(btn_icon);
        cur_btn->setIconSize(QSize(tabbar_btn_icon_size, tabbar_btn_icon_size));
        cur_btn->setFont(tabbar_btn_font);
    }
}
