#include "main_view.h"
#include "ui_main_view.h"

MainView::MainView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);
}

MainView::~MainView()
{
    delete ui;
}
