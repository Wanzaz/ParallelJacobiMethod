#include "JacobiSolver.h"
#include <QDebug>
#include <cmath>
#include <cstdlib>  // pro srand a rand

JacobiSolver::JacobiSolver(int size) : size(size), ready(false), finishedThreads(0) {
    matrix.resize(size, QVector<double>(size, 0));
    b.resize(size, 0);
    x.resize(size, 0);
    xNew.resize(size, 0);
    xOld.resize(size, 0);  // Initialize xOld for safe reading

    // Incialization of vector x with random values
    srand(time(0));
    for (int i = 0; i < size; ++i) {
        x[i] = rand() % 10 + 1;
    }

    int numThreads = QThread::idealThreadCount();
    int rowsPerThread = size / numThreads;

    // Create workers for parallel computation
    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? size : startRow + rowsPerThread;
        JacobiWorker* worker = new JacobiWorker(startRow, endRow, &matrix, &b, &xOld, &xNew, &mutex, &condition, &finishedThreads, &ready);
        workers.append(worker);
        worker->start();
    }
}

JacobiSolver::~JacobiSolver() {
    // Free all working threads
    for (JacobiWorker* worker : workers) {
        worker->stop();  // Stop worker thread
        worker->wait();  // Wait until thread finishes
        delete worker;   // Delete worker
    }
}

void JacobiSolver::normalizeMatrix(QVector<QVector<double>>& matrix, QVector<double>& b)
{
    for (int r = 0; r < matrix.size(); ++r) {
        double diag = matrix[r][r];

        if (qFuzzyIsNull(diag)) {
            qFatal("Chyba: Nulový diagonální prvek v řádku %d!", r);
        }

        for (int s = 0; s < matrix[r].size(); ++s) {
            if (r != s) {
                matrix[r][s] /= diag;
            }
        }

        b[r] /= diag;
        matrix[r][r] = 0.0;
    }
}


void JacobiSolver::solve(double epsilon) {
    bool converged = false;
    int iteration = 0;

    normalizeMatrix(matrix, b);

    while (!converged) {
        iteration++;

        // Synchronize and signal workers to start
        mutex.lock();
        xOld = x;  // Assign current x to xOld for safe reading
        ready = true;
        finishedThreads = 0;  // Reset finished threads
        condition.wakeAll();
        mutex.unlock();

        // Wait for all workers to finish
        mutex.lock();
        while (ready) {
            condition.wait(&mutex);
        }
        mutex.unlock();

        // Debug output for the current iteration
        qDebug() << "Iteration" << iteration;
        for (int i = 0; i < size; ++i) {
            qDebug() << "xNew[" << i << "] = " << xNew[i];
        }

        // Check for convergence
        double maxChange = 0.0;
        for (int i = 0; i < size; ++i) {
            maxChange = std::max(maxChange, std::abs(xNew[i] - x[i]));
        }

        qDebug() << "Max change: " << maxChange;

        if (maxChange < epsilon) {
            converged = true;
        } else {
            std::swap(x, xNew);  // Efficient swap of pointers instead of copying
        }
    }
}

void JacobiSolver::setMatrix(const QVector<QVector<double>>& m) {
    matrix = m;
}

void JacobiSolver::setB(const QVector<double>& rhs) {
    b = rhs;
}

QVector<double> JacobiSolver::getResult() {
    return x;
}
