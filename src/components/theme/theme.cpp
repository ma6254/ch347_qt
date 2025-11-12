#include "theme.h"

QList<Theme *> Theme::themes;

/*******************************************************************************
 * @brief 主题初始化
 ******************************************************************************/
void Theme::theme_init()
{

    // LIGHT主题
    Theme* light_theme= new Theme();
    light_theme->tabbar_btn_icon_color = QColor("#434343"); // 黑色
    light_theme->main_window_style = R""(
MainWindow {
    background-color: #FFFFFF;
}
)"";
    themes.append(light_theme);


    
    // DARK主题
    Theme *dark_theme = new Theme();
    dark_theme->tabbar_btn_icon_color = QColor("#F3F3F3"); // 白色
    dark_theme->main_window_style = R""(
    MainWindow {
        background-color: #434343;
    }
    )"";
    themes.append(dark_theme);
}

/*******************************************************************************
 * @brief 获取主题实例
 * @param theme_type 主题类型
 ******************************************************************************/
Theme *Theme::get_theme(Theme::THEME theme_type)
{
    int index = static_cast<int>(theme_type);
    Q_ASSERT_X(index >= 0 && index < themes.size(), "Theme::get_theme", "theme_type out of range");

    return Theme::themes[static_cast<int>(theme_type)];
}

/*******************************************************************************
 * @brief 主题构造函数
 ******************************************************************************/
Theme::Theme()
{
    // 默认参数，只作为故障检测

    tabbar_btn_icon_color = QColor("#FF0000");
    main_window_style = R""(
    MainWindow {
        background-color: #FF0000;
    }
    )"";
}

/*******************************************************************************
 * @brief 主题析构函数
 ******************************************************************************/
Theme::~Theme()
{
}
