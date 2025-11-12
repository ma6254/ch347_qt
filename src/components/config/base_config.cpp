#include "base_config.h"

/*******************************************************************************
 * @brief 构造函数
 * @param None
 * @return None
 ******************************************************************************/
BaseConfig::BaseConfig()
{
}

/*******************************************************************************
 * @brief 析构函数
 * @param None
 * @return None
 ******************************************************************************/
BaseConfig::~BaseConfig()
{
}

/*******************************************************************************
 * @brief 解码得到QString类型的值
 * @param None
 * @return None
 ******************************************************************************/
int BaseConfig::from_node_QString(YAML::Node root_node, QString k, QString *v)
{
    YAML::Node tmp_node;

    tmp_node = root_node[k.toLatin1().constData()];
    if (tmp_node.IsScalar() == false)
        return -1;

    *v = QString(tmp_node.as<std::string>().c_str());

    return 0;
}

/*******************************************************************************
 * @brief 解码得到bool类型的值
 * @param None
 * @return None
 ******************************************************************************/
int BaseConfig::from_node_bool(YAML::Node root_node, QString k, bool *v)
{
    YAML::Node tmp_node;

    tmp_node = root_node[k.toLatin1().constData()];
    if (tmp_node.IsScalar() == false)
        return -1;

    *v = tmp_node.as<bool>();

    return 0;
}
