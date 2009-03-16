#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QtGui/QWidget>

namespace Ui {
    class LogWidget;
}

class LogWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(LogWidget)
public:
    explicit LogWidget(QWidget *parent = 0);
    virtual ~LogWidget();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::LogWidget *m_ui;
};

#endif // LOGWIDGET_H
