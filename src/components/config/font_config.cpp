#include "font_config.h"

FontConfig *FontConfig::get_default()
{
    FontConfig *config = new FontConfig();

    config->font = QFont("Microsoft YaHei", 12);

    return config;
}

FontConfig::FontConfig()
{
}

FontConfig::~FontConfig()
{
}

/*******************************************************************************
 * @brief 从YAML节点中加载
 * @param None
 * @return None
 ******************************************************************************/
int FontConfig::from_node(YAML::Node node)
{
    YAML::Node tmp_node;

    if (node.IsMap() == false)
        return -1;

    // 字体族
    tmp_node = node["family"];
    if (tmp_node.IsScalar() == false)
        return -1;
    QString font_family = QString::fromStdString(tmp_node.as<std::string>());
    font.setFamily(font_family);

    // 字体字高
    tmp_node = node["size"];
    if (tmp_node.IsScalar() == false)
        return -1;
    int font_size = tmp_node.as<int>();

    font.setPointSize(font_size);
}

/*******************************************************************************
 * @brief 保存到YAML节点中
 * @param None
 * @return None
 ******************************************************************************/
int FontConfig::to_node(YAML::Node *node)
{
    (*node)["family"] = qPrintable(font.family());
    (*node)["size"] = font.pointSize();
}
