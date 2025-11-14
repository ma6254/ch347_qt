#include <QPalette>
#include <QClipboard>
#include "about_view.h"
#include "ui_about_viewiew.h"
#include "config.h"
#include "utils.h"

AboutView::AboutView(QWidget *parent)
    : QWidget(parent), ui(new Ui::AboutView)
{
    ui->setupUi(this);
    set_theme();

    QString build_time_str = QString("%1-%2-%3T%4").arg(__YEAR__).arg(__MONTH__).arg(__DAY__).arg(__TIME__);

    copy_str = QString("version: %1\n"
                       "commit: %2\n"
                       "build_time: %3\n"
                       "qt_version: %4")
                   .arg("")
                   .arg(GIT_COMMIT)
                   .arg(build_time_str)
                   .arg(QT_VERSION_STR);

    ui->commit_label->setText(GIT_COMMIT);
    ui->build_time_label->setText(build_time_str);
    ui->qt_version_label->setText(QT_VERSION_STR);

    connect(ui->copy_btn, &QToolButton::clicked, this, &AboutView::copy_btn_clicked_cb);
}

AboutView::~AboutView()
{
    delete ui;
}

void AboutView::set_theme()
{
    auto cur_theme = Theme::get_theme(Config::get_global()->theme);

    QPalette p(palette());
    p.setColor(QPalette::Window, cur_theme->get_background_color());
    setAutoFillBackground(true);
    setPalette(p);

    //     // 设置页面样式
    //     setStyleSheet(QString(R""(
    // background-color: %1;
    // )"")
    //                       .arg(bg_color_str));

    QString top_widget_style = QString(R""(
AboutView {
    border: none;
}
)"");
    setStyleSheet(top_widget_style);

    QString scrollArea_style = QString(R""(
QScrollArea#scrollArea {
    background-color: %1;
    border: none;
}
)"")
                                   .arg(cur_theme->get_background_color().name(QColor::HexArgb));

    ui->scrollArea->setStyleSheet(scrollArea_style);

    QString scrollAreaWidgetContents_style = QString(R""(
QWidget#scrollAreaWidgetContents {
    background-color: %1;
    border: none;
}
)"")
                                                 .arg(cur_theme->get_background_color().name(QColor::HexArgb));

    ui->scrollAreaWidgetContents->setStyleSheet(scrollAreaWidgetContents_style);

    QString label_style = QString(R""(
QLabel {
    color: %1;
    background-color: transparent;
    border: none;
}
)"")

                              .arg(cur_theme->get_foreground_color().name(QColor::HexArgb));

    ui->title_label->setStyleSheet(label_style);
    ui->version_label->setStyleSheet(label_style);
    ui->commit_label->setStyleSheet(label_style);
    ui->build_time_label->setStyleSheet(label_style);
    ui->qt_version_label->setStyleSheet(label_style);
    ui->version_title_label->setStyleSheet(label_style);
    ui->commit_title_label->setStyleSheet(label_style);
    ui->build_time_title_label->setStyleSheet(label_style);
    ui->qt_version_title_label->setStyleSheet(label_style);

    ui->title_label->setFont(Config::get_global()->font);
    ui->version_label->setFont(Config::get_global()->font);
    ui->commit_label->setFont(Config::get_global()->font);
    ui->build_time_label->setFont(Config::get_global()->font);
    ui->qt_version_label->setFont(Config::get_global()->font);
    ui->version_title_label->setFont(Config::get_global()->font);
    ui->commit_title_label->setFont(Config::get_global()->font);
    ui->build_time_title_label->setFont(Config::get_global()->font);
    ui->qt_version_title_label->setFont(Config::get_global()->font);

    QString groupbox_style = QString(R""(
QGroupBox {
    color: %2;
    background-color: %1;
    border: 2px solid %3;
    border-radius: 5px;
}
)"")

                                 .arg(cur_theme->get_background_color().name(QColor::HexArgb))
                                 .arg(cur_theme->get_foreground_color().name(QColor::HexArgb))
                                 .arg(cur_theme->get_accent_color().name(QColor::HexArgb));

    ui->software_info_groupbox->setStyleSheet(groupbox_style);
    ui->software_info_groupbox->setFont(Config::get_global()->font);

    auto copy_btn_style_tmplate = QString(R""(
QToolButton {
    color: %1;
    background-color: %2;

    border-style: solid;
    border-color: %1;
    border-width: 1px;
    border-radius: 5px;
}

)"");

    auto copy_btn_style = QString(copy_btn_style_tmplate)
                              .arg(cur_theme->get_foreground_color().name(QColor::HexArgb))
                              .arg(cur_theme->get_accent_color().name(QColor::HexArgb));

    ui->copy_btn->setStyleSheet(copy_btn_style);
    ui->copy_btn->setFont(Config::get_global()->font);

    //     QString scrollAreaWidgetContents_style = QString(R""(
    // QWidget#scrollAreaWidgetContents {
    //     background-color: %1;
    //     border:none;
    // }
    // )"")
    //                                    .arg(cur_theme->get_background_color().name(QColor::HexArgb));

    // 设置滚动区组件样式

    // ui->scrollAreaWidgetContents->setStyleSheet(scrollAreaWidgetContents_style);

    // qDebug("[AboutView] set_theme %s", qPrintable(widget_style));
    // setStyleSheet(widget_style);
}

void AboutView::copy_btn_clicked_cb()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(copy_str);
}
