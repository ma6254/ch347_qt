#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QDateTime>
#include <yaml-cpp/yaml.h>
#include "base_config.h"
#include "theme.h"

class Config : public BaseConfig
{
    Q_OBJECT

public:
    enum Error
    {
        ErrorUnknow = -1,   // 未知的错误
        ErrorNotExist = -2, // 配置文件不存在
    };
    Q_ENUM(Error)

    Config();
    ~Config();

    static Config *get_global() { return Config::global_config; };
    static void global_init();

    static QString get_default_path();
    static Config *get_default();

    int from_file(QString file_path = "");
    int from_node(YAML::Node node) override;

    int save_to_file(QString file_path = "")
    {
        latest = QDateTime::currentDateTime();
        return to_file(file_path);
    };

    int to_file(QString file_path = "");
    int to_node(YAML::Node *node) override;



    QDateTime latest;
    Theme::THEME theme;

private:

    static Config *global_config;
};

#endif // CONFIG_H
