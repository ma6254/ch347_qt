#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include <QWidget>

namespace Ui {
class MainView;
}

class MainView : public QWidget
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = nullptr);
    ~MainView();

private:
    Ui::MainView *ui;
};

#endif // MAIN_VIEW_H
