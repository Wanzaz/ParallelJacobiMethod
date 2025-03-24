#include "JacobiSolver.h"
#include <QDebug>
#include <cmath>
#include <cstdlib>
#include <QtConcurrent>

/**
 * @class JacobiSolver
 * @brief A class to solve a system of linear equations using the Jacobi method.
 *
 * This class implements the Jacobi iterative method to solve a system of linear
 * equations of the form Ax = b, where A is the coefficient matrix, x is the
 * solution vector, and b is the right-hand side vector.
 */

/**
 * @brief Constructs a JacobiSolver object.
 *
 * Initializes the JacobiSolver object, setting up the size of the system,
 * and initializing matrices and vectors to solve the system.
 *
 * @param size The size of the matrix (number of variables in the system).
 * @param parent The parent QObject (default is nullptr).
 */
JacobiSolver::JacobiSolver(int size, QObject* parent)
    : QObject(parent), size(size) {
    matrix.resize(size, QVector<double>(size, 0));
    b.resize(size, 0);
    x.resize(size, 0);
    xNew.resize(size, 0);
    xOld.resize(size, 0);

    srand(time(0));
    for (int i = 0; i < size; ++i) {
        x[i] = rand() % 10 + 1;
    }
}

/**
 * @brief Destructor for JacobiSolver.
 *
 * No special cleanup is needed in this case, as the solver doesn't allocate resources
 * that require manual deallocation.
 */
JacobiSolver::~JacobiSolver() {}

/**
 * @brief Normalizes the matrix and right-hand side vector.
 *
 * This function normalizes the matrix such that the diagonal elements are 1.0
 * and scales the right-hand side vector accordingly. It also sets the diagonal elements to 0.
 *
 * @param matrix The system coefficient matrix to be normalized.
 * @param b The right-hand side vector.
 */
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

/**
 * @brief Solves the system of equations using the Jacobi method.
 *
 * The solve function performs the iterative Jacobi method until the solution converges.
 * It computes the solution vector by updating it iteratively and checking for convergence.
 *
 * @param epsilon The tolerance for convergence. The iteration stops when the maximum change
 *                in the solution vector is less than this value.
 */
void JacobiSolver::solve(double epsilon) {
    bool converged = false;
    int iteration = 0;
    normalizeMatrix(matrix, b);

    while (!converged) {
        iteration++;
        qDebug() << "Iteration:" << iteration;

        xOld = x;

        QFutureSynchronizer<void> synchronizer;  // Synchronizer to manage multiple threads
        int numThreads = QThread::idealThreadCount();
        int rowsPerThread = size / numThreads;

        // Create threads and assign them to compute different parts of the matrix
        for (int t = 0; t < numThreads; ++t) {
            int startRow = t * rowsPerThread;  // Calculate the start row for this thread
            int endRow = (t == numThreads - 1) ? size : startRow + rowsPerThread;

            synchronizer.addFuture(QtConcurrent::run([this, startRow, endRow]() {
                // Perform computation for rows assigned to this thread
                for (int i = startRow; i < endRow; ++i) {
                    double sum = 0.0;
                    // Perform the Jacobi iteration: xNew[i] = b[i] - sum
                    for (int j = 0; j < matrix.size(); ++j) {
                        if (i != j) {
                            sum += matrix[i][j] * xOld[j];
                        }
                    }
                    xNew[i] = b[i] - sum;
                }
            }));
        }

        synchronizer.waitForFinished();

        double maxChange = 0.0;
        for (int i = 0; i < size; ++i) {
            maxChange = std::max(maxChange, std::abs(xNew[i] - x[i]));
        }

        qDebug() << "Max change:" << maxChange;

        if (maxChange < epsilon) {
            qDebug() << "Converged!";
            converged = true;
        } else {
            std::swap(x, xNew);
        }
    }

    emit finished();  // Emit finished signal when the solution has converged
}

/**
 * @brief Sets the coefficient matrix.
 *
 * This function sets the matrix of the system of linear equations.
 *
 * @param m The coefficient matrix.
 */
void JacobiSolver::setMatrix(const QVector<QVector<double>>& m) {
    matrix = m;
}

/**
 * @brief Sets the right-hand side vector.
 *
 * This function sets the right-hand side vector (b) for the system of linear equations.
 *
 * @param rhs The right-hand side vector.
 */
void JacobiSolver::setB(const QVector<double>& rhs) {
    b = rhs;
}

/**
 * @brief Gets the computed solution vector.
 *
 * This function returns the computed solution vector after the Jacobi method has converged.
 *
 * @return The solution vector.
 */
QVector<double> JacobiSolver::getResult() {
    return x;
}
