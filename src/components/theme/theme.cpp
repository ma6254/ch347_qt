#include "theme.h"

QList<Theme *> Theme::themes;

/*******************************************************************************
 * @brief 主题初始化
 ******************************************************************************/
void Theme::theme_init()
{
    Theme *tmp_theme;

    themes.clear();

    // LIGHT主题
    tmp_theme = new Theme();
    tmp_theme->background_color = QColor("#EFECE3");
    tmp_theme->foreground_color = QColor("#000000");
    tmp_theme->accent_color = QColor("#8FABD4");
    tmp_theme->highlight_color = QColor("#4A70A9");
    themes.append(tmp_theme);

    // DARK主题
    tmp_theme = new Theme();
    tmp_theme->background_color = QColor("#0C2B4E");
    tmp_theme->foreground_color = QColor("#F4F4F4");
    tmp_theme->accent_color = QColor("#1A3D64");
    tmp_theme->highlight_color = QColor("#1D546C");
    themes.append(tmp_theme);
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
    background_color = QColor("#FF0000");
    foreground_color = QColor("#FF0000");
    accent_color = QColor("#FF0000");
    highlight_color = QColor("#FF0000");
}

/*******************************************************************************
 * @brief 主题析构函数
 ******************************************************************************/
Theme::~Theme()
{
}
