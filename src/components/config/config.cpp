#include <QDir>
#include <QVariant>
#include <QCoreApplication>
#include <QMetaEnum>
#include "config.h"

Config *Config::global_config;

/*******************************************************************************
 * @brief 全局配置文件初始化
 * @param None
 * @return None
 ******************************************************************************/
void Config::global_init()
{
    int err;
    global_config = Config::get_default();

    err = global_config->from_file();
    if (err < 0)
    {
        if (err == Config::ErrorNotExist)
        {
            global_config->to_file();
        }
        else
        {
            Q_ASSERT_X(false, "Config::global_init", "load global config failed");
        }
    }
}

/*******************************************************************************
 * @brief 默认参数的配置文件
 * @param None
 * @return None
 ******************************************************************************/
Config *Config::get_default()
{
    Config *config = new Config();
    config->theme = Theme::LIGHT; // 默认主题为浅色主题

    config->font = QFont("Microsoft YaHei", 12);

    return config;
}

/*******************************************************************************
 * @brief 配置文件默认路径
 * @param None
 * @return None
 ******************************************************************************/
QString Config::get_default_path()
{
    // QStringList path;
    // path.append(QCoreApplication::applicationDirPath());
    // path.append("config.yml");
    // return path.join(QDir::separator());
    return QCoreApplication::applicationDirPath() + QDir::separator() + "config.yml";
}

/*******************************************************************************
 * @brief 构造函数
 * @param None
 * @return None
 ******************************************************************************/
Config::Config()
{
}

/*******************************************************************************
 * @brief 析构函数
 * @param None
 * @return None
 ******************************************************************************/
Config::~Config()
{
}

/*******************************************************************************
 * @brief 从文件中加载
 * @param None
 * @return None
 ******************************************************************************/
int Config::from_file(QString file_path)
{
    QFile file;
    QByteArray config_file_buf;
    bool ok;

    if (file_path.isEmpty())
    {
        file_path = get_default_path();
    }

    file.setFileName(file_path);
    if (file.exists() == false)
    {
        return ErrorNotExist;
    }

    ok = file.open(QIODevice::ReadOnly);
    config_file_buf = file.readAll();
    file.close();

    YAML::Node node = YAML::Load(config_file_buf.constData());
    return from_node(node);
}

/*******************************************************************************
 * @brief 从YAML节点中加载
 * @param None
 * @return None
 ******************************************************************************/
int Config::from_node(YAML::Node node)
{
    YAML::Node tmp_node;
    QString tmp_str;
    int tmp_int;
    int err;

    if (node.IsMap() == false)
        return -1;

    // 加载 latest 最后更新时间
    tmp_node = node["latest"];
    if (tmp_node.IsScalar() == false)
        return -1;
    tmp_str = QString(tmp_node.as<std::string>().c_str());
    latest = QDateTime::fromString(tmp_str, Qt::ISODate);
    if( latest.isValid() == false)
        return -1;

    // 加载 theme 主题
    tmp_node = node["theme"];
    if (tmp_node.IsScalar() == false)
        return -1;
    tmp_str = QString(tmp_node.as<std::string>().c_str());
    bool ok;
    tmp_int = QMetaEnum::fromType<Theme::THEME>().keyToValue(tmp_str.toUpper().toUtf8().constData(), &ok);
    if (ok == false)
        return -1;
    theme = static_cast<Theme::THEME>(tmp_int);

    qDebug("[Config] ===========================================================");
    qDebug("[Config] load ok");
    qDebug("[Config] latest: %s", qPrintable(latest.toString(Qt::ISODate)));
    qDebug("[Config] theme: %s", qPrintable(QMetaEnum::fromType<Theme::THEME>().valueToKey(theme)));
    qDebug("[Config] ===========================================================");
    return 0;
}

/*******************************************************************************
 * @brief 保存到文件中
 * @param None
 * @return None
 ******************************************************************************/
int Config::to_file(QString file_path)
{
    int err;
    YAML::Node node;

    err = to_node(&node);
    if (err < 0)
        return err;

    if (file_path.isEmpty())
    {
        file_path = get_default_path();
    }

    // qDebug("[cfg] emitter");
    YAML::Emitter emitter;
    // emitter.SetIndent(4);
    emitter << node;

    QFile file;
    file.setFileName(file_path);
    if (file.exists())
        file.remove();

    file.open(QIODevice::ReadWrite);
    file.write(emitter.c_str());
    file.flush();
    file.close();

    // qDebug("[cfg] config_save");
    return 0;
}

/*******************************************************************************
 * @brief 保存到YAML节点中
 * @param None
 * @return None
 ******************************************************************************/
int Config::to_node(YAML::Node *node)
{
    QString tmp_str;

    if (node == nullptr)
        return -1;

    // qDebug("[cfg] config_save start");

    // 保存 latest 最后更新时间
    latest = QDateTime::currentDateTime();
    tmp_str = latest.toString(Qt::ISODate);
    (*node)["latest"] = qPrintable(tmp_str);
    (*node)["theme"] = QMetaEnum::fromType<Theme::THEME>().valueToKey(theme);

    return 0;
}
