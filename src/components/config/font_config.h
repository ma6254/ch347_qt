#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QFont>
#include <yaml-cpp/yaml.h>
#include "base_config.h"


class FontConfig : public BaseConfig
{
    Q_OBJECT

public:
    FontConfig();
    ~FontConfig();


    static FontConfig *get_default();

    int from_node(YAML::Node node) override;
    int to_node(YAML::Node *node) override;

    QFont font;

private:

};

#endif // CONFIG_H
