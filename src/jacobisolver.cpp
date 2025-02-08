#include "JacobiSolver.h"
#include "JacobiWorker.h"
#include <QDebug>
#include <cmath>

JacobiSolver::JacobiSolver(int size) : size(size), ready(false) {
    matrix.resize(size, QVector<double>(size, 0));
    b.resize(size, 0);
    x.resize(size, 0);
    xNew.resize(size, 0);
}

void JacobiSolver::solve(double epsilon) {
    int numThreads = QThread::idealThreadCount();
    QVector<JacobiWorker*> workers;
    int rowsPerThread = size / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? size : startRow + rowsPerThread;
        JacobiWorker* worker = new JacobiWorker(startRow, endRow, &matrix, &b, &x, &xNew, &mutex, &condition, &ready);
        workers.append(worker);
        worker->start();
    }

    bool converged = false;
    while (!converged) {
        mutex.lock();
        ready = true;
        condition.wakeAll();
        mutex.unlock();

        mutex.lock();
        while (ready) {
            condition.wait(&mutex);
        }
        mutex.unlock();

        // Kontrola konvergence: zda změna mezi aktuálními a novými hodnotami je menší než epsilon
        double maxChange = 0.0;
        for (int i = 0; i < size; ++i) {
            maxChange = std::max(maxChange, std::abs(xNew[i] - x[i]));
        }

        if (maxChange < epsilon) {
            converged = true; // Ukončení, pokud změny mezi iteracemi jsou menší než epsilon
        } else {
            x = xNew; // Přenos nových hodnot do aktuálních
        }

        // Kontrola, zda došlo k chybám ve vláknech
        for (JacobiWorker* worker : workers) {
            if (worker->hasError()) {
                qDebug() << "Došlo k chybě ve vlákně během výpočtu!";
                for (JacobiWorker* w : workers) {
                    w->quit();
                    w->wait();
                    delete w;
                }
                return;
            }
        }
    }

    for (JacobiWorker* worker : workers) {
        worker->quit();
        worker->wait();
        delete worker;
    }
}

void JacobiSolver::setMatrix(const QVector<QVector<double>>& m) { matrix = m; }
void JacobiSolver::setB(const QVector<double>& rhs) { b = rhs; }
QVector<double> JacobiSolver::getResult() { return x; }
