#include "JacobiSolver.h"
#include <QDebug>
#include <cmath>
#include <cstdlib>

JacobiSolver::JacobiSolver(int size, QObject* parent)
    : QObject(parent), size(size), ready(false), finishedThreads(0) {
    matrix.resize(size, QVector<double>(size, 0));
    b.resize(size, 0);
    x.resize(size, 0);
    xNew.resize(size, 0);
    xOld.resize(size, 0);

    srand(time(0));
    for (int i = 0; i < size; ++i) {
        x[i] = rand() % 10 + 1;
    }

    int numThreads = QThread::idealThreadCount();
    int rowsPerThread = size / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * rowsPerThread;
        int endRow = (i == numThreads - 1) ? size : startRow + rowsPerThread;

        QThread* thread = new QThread();
        JacobiWorker* worker = new JacobiWorker(startRow, endRow, &matrix, &b, &xOld, &xNew);
        worker->moveToThread(thread);

        connect(this, &JacobiSolver::startIteration, worker, &JacobiWorker::compute);
        connect(this, &JacobiSolver::stopWorkers, worker, &JacobiWorker::stop);
        connect(worker, &JacobiWorker::finishedComputing, this, &JacobiSolver::workerFinished);
        connect(worker, &JacobiWorker::destroyed, thread, &QThread::quit);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        threads.append(thread);
        workers.append(worker);

        thread->start();
    }
}

JacobiSolver::~JacobiSolver() {
    emit stopWorkers();
    for (QThread* thread : threads) {
        thread->quit();
        thread->wait();
    }
}

void JacobiSolver::normalizeMatrix(QVector<QVector<double>>& matrix, QVector<double>& b) {
    for (int r = 0; r < matrix.size(); ++r) {
        double diag = matrix[r][r];
        if (qFuzzyIsNull(diag)) {
            qFatal("Error: Zero diagonal element at row %d!", r);
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
        qDebug() << "Iteration:" << iteration;  // Sleduj iterace

        mutex.lock();
        xOld = x;
        ready = true;
        finishedThreads = 0;
        mutex.unlock();

        emit startIteration();

        mutex.lock();
        while (finishedThreads < workers.size()) {
            condition.wait(&mutex);
        }
        mutex.unlock();

        double maxChange = 0.0;
        for (int i = 0; i < size; ++i) {
            maxChange = std::max(maxChange, std::abs(xNew[i] - x[i]));
        }

        qDebug() << "Max change:" << maxChange;  // Sleduj hodnotu změny

        if (maxChange < epsilon) {
            qDebug() << "Converged!";
            converged = true;
        } else {
            std::swap(x, xNew);
        }
    }

    emit finished();
}


void JacobiSolver::workerFinished() {
    mutex.lock();
    finishedThreads++;
    qDebug() << "Worker finished. Total finished:" << finishedThreads << "/" << workers.size();
    if (finishedThreads == workers.size()) {
        condition.wakeAll();
    }
    mutex.unlock();
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
