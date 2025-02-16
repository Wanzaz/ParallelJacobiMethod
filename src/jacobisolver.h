#ifndef JACOBISOLVER_H
#define JACOBISOLVER_H

#include <QVector>
#include <QMutex>
#include <QWaitCondition>
#include "JacobiWorker.h"

class JacobiSolver {
public:
    JacobiSolver(int size);
    ~JacobiSolver();

    void setMatrix(const QVector<QVector<double>>& m);
    void setB(const QVector<double>& rhs);
    QVector<double> getResult();

    void solve(double epsilon);

private:
    int size;
    QVector<QVector<double>> matrix;
    QVector<double> b;
    QVector<double> x;
    QVector<double> xNew;
    QVector<double> xOld; // Kopie pro čtení

    QMutex mutex;
    QWaitCondition condition;
    int finishedThreads;
    bool ready;

    QVector<JacobiWorker*> workers;

    void normalizeMatrix(QVector<QVector<double>>& matrix, QVector<double>& b);
};

#endif
