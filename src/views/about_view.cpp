#include "about_view.h"
#include "ui_about_viewiew.h"

AboutView::AboutView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AboutView)
{
    ui->setupUi(this);
}

AboutView::~AboutView()
{
    delete ui;
}
