#ifndef BASE_CONFIG_H
#define BASE_CONFIG_H

#include <yaml-cpp/yaml.h>
#include <QObject>

class BaseConfig : public QObject
{
    Q_OBJECT

public:
    BaseConfig();
    ~BaseConfig();

    virtual int from_node(YAML::Node root_node) = 0;
    virtual int to_node(YAML::Node *root_node) = 0;

protected:
    int from_node_QString(YAML::Node root_node, QString key, QString *value);
    int from_node_bool(YAML::Node root_node, QString key, bool *value);
    // int from_node_QString(YAML::Node *root_node, const char *k, QString *v);
};

#endif // CONFIG_H
