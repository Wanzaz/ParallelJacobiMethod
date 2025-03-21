#ifndef JACOBISOLVER_H
#define JACOBISOLVER_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include "JacobiWorker.h"

class JacobiSolver : public QObject {
    Q_OBJECT

public:
    explicit JacobiSolver(int size, QObject* parent = nullptr);
    ~JacobiSolver();

    void setMatrix(const QVector<QVector<double>>& m);
    void setB(const QVector<double>& rhs);
    QVector<double> getResult();
    void solve(double epsilon);

signals:
    void startIteration();
    void stopWorkers();
    void finished();

private slots:
    void workerFinished();

private:
    int size;
    QVector<QVector<double>> matrix;
    QVector<double> b;
    QVector<double> x;
    QVector<double> xNew;
    QVector<double> xOld;

    QMutex mutex;
    QWaitCondition condition;
    int finishedThreads;
    bool ready;

    QVector<QThread*> threads;
    QVector<JacobiWorker*> workers;

    void normalizeMatrix(QVector<QVector<double>>& matrix, QVector<double>& b);
};

#endif
