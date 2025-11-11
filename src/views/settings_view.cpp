#include "settings_view.h"
#include "ui_settings_view.h"

SettingsView::SettingsView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsView)
{
    ui->setupUi(this);
}

SettingsView::~SettingsView()
{
    delete ui;
}
