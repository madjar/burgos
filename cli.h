#ifndef CLI_H
#define CLI_H

#include <QObject>
#include <QTextStream>
#include <QString>

class Cli : public QObject
{
    Q_OBJECT

public:
    Cli(QObject *parent = 0);

public slots:
    void read();

private:
    void command(QString cmd);

    QTextStream in;
    QTextStream out;
};

#endif // CLI_H
