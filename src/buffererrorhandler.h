#ifndef BUFFERERRORHANDLER_H
#define BUFFERERRORHANDLER_H

#include <QObject>

class BufferErrorHandler : public QObject
{
    Q_OBJECT
public:
    static void handle(QObject *receiver, const char *member);
    static int increaseBuffer();

signals:
    void handled(bool);

private:
    BufferErrorHandler();
    static BufferErrorHandler *singleton;

    void printRequest();
    bool bufferProblem();

    int state; // -2 : not done, -1 : doing, 0 : failed, 1 : succeeded
};

#endif // BUFFERERRORHANDLER_H
