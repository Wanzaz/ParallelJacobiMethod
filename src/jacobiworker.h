#ifndef JACOBIWORKER_H
#define JACOBIWORKER_H

#include <QThread>
#include <QVector>
#include <QMutex>
#include <QWaitCondition>

class JacobiWorker : public QThread {
    Q_OBJECT

public:
    JacobiWorker(int startRow, int endRow, QVector<QVector<double>>* matrix, QVector<double>* b, QVector<double>* x, QVector<double>* xNew, QMutex* mutex, QWaitCondition* condition, bool* ready)
        : startRow(startRow), endRow(endRow), matrix(matrix), b(b), x(x), xNew(xNew), mutex(mutex), condition(condition), ready(ready), errorFlag(false) {}

    void run() override;
    bool hasError() const { return errorFlag; }  // Funkce pro detekci chyby

private:
    int startRow, endRow;
    QVector<QVector<double>>* matrix;
    QVector<double>* b;
    QVector<double>* x;
    QVector<double>* xNew;
    QMutex* mutex;
    QWaitCondition* condition;
    bool* ready;
    bool errorFlag;  // Příznak chyby
};

#endif // JACOBIWORKER_H
