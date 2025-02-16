#include "JacobiWorker.h"

JacobiWorker::JacobiWorker(int startRow, int endRow, const QVector<QVector<double>>* matrix,
                           const QVector<double>* b, const QVector<double>* xOld,
                           QVector<double>* xNew, QMutex* mutex, QWaitCondition* condition,
                           int* finishedThreads, bool* ready)
    : startRow(startRow), endRow(endRow), matrix(matrix), b(b), xOld(xOld), xNew(xNew),
    mutex(mutex), condition(condition), finishedThreads(finishedThreads), ready(ready),
    terminateFlag(false) {}

void JacobiWorker::run() {
    while (!terminateFlag) {
        mutex->lock();
        while (!(*ready)) {
            condition->wait(mutex);
        }
        mutex->unlock();

        // Calculate new values for xNew
        for (int i = startRow; i < endRow; ++i) {
            double sum = 0.0;
            for (int j = 0; j < matrix->size(); ++j) {
                if (i != j) {
                    sum += (*matrix)[i][j] * (*xOld)[j];
                }
            }
            (*xNew)[i] = (*b)[i] - sum;
        }

        mutex->lock();
        (*finishedThreads)++;
        if (*finishedThreads == matrix->size()) {
            *ready = false;
            *finishedThreads = 0;
            condition->wakeAll();
        }
        mutex->unlock();
    }
}

void JacobiWorker::stop() {
    terminateFlag = true;
}
