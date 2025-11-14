#ifndef ABOUT_VIEW_H
#define ABOUT_VIEW_H

#include <QWidget>
#include "theme.h"

namespace Ui {
class AboutView;
}

class AboutView : public QWidget
{
    Q_OBJECT

public:
    explicit AboutView(QWidget *parent = nullptr);
    ~AboutView();

    void set_theme();

private slots:
    void copy_btn_clicked_cb();

private:
    Ui::AboutView *ui;
    QString copy_str;
};

#endif // ABOUT_VIEW_H
