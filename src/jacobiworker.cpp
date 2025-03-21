#include "JacobiWorker.h"
#include <QDebug>
#include <cmath>

JacobiWorker::JacobiWorker(int startRow, int endRow, const QVector<QVector<double>>* matrix,
                           const QVector<double>* b, const QVector<double>* xOld,
                           QVector<double>* xNew, QObject* parent)
    : QObject(parent), startRow(startRow), endRow(endRow), matrix(matrix),
    b(b), xOld(xOld), xNew(xNew), terminateFlag(false) {}

void JacobiWorker::compute() {
    if (terminateFlag) return;

    qDebug() << "Worker computing rows:" << startRow << "to" << endRow;

    for (int i = startRow; i < endRow; ++i) {
        double sum = 0.0;
        for (int j = 0; j < matrix->size(); ++j) {
            if (i != j) {
                sum += (*matrix)[i][j] * (*xOld)[j];
            }
        }
        (*xNew)[i] = (*b)[i] - sum;
    }

    qDebug() << "Worker finished rows:" << startRow << "to" << endRow;
    emit finishedComputing();
}


void JacobiWorker::stop() {
    terminateFlag = true;
}
