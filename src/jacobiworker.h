#ifndef JACOBIWORKER_H
#define JACOBIWORKER_H

#include <QThread>
#include <QVector>
#include <QMutex>
#include <QWaitCondition>

class JacobiWorker : public QThread {
    Q_OBJECT
public:
    JacobiWorker(int startRow, int endRow, const QVector<QVector<double>>* matrix,
                 const QVector<double>* b, const QVector<double>* xOld,
                 QVector<double>* xNew, QMutex* mutex, QWaitCondition* condition,
                 int* finishedThreads, bool* ready);

    void run() override;
    void stop();

private:
    int startRow, endRow;
    const QVector<QVector<double>>* matrix;
    const QVector<double>* b;
    const QVector<double>* xOld;
    QVector<double>* xNew;
    QMutex* mutex;
    QWaitCondition* condition;
    int* finishedThreads;
    bool* ready;
    bool terminateFlag;
};

#endif
