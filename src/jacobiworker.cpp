#include "JacobiWorker.h"
#include <QDebug>
#include <cmath>

/**
 * @class JacobiWorker
 * @brief A worker class that computes a portion of the Jacobi iteration.
 *
 * This class performs the Jacobi method computations for a subset of rows in the system
 * of linear equations. It runs in a separate thread to handle the parallel computation.
 */

/**
 * @brief Constructs a JacobiWorker object.
 *
 * Initializes the worker to process a subset of rows in the matrix. This includes
 * setting the start and end rows, as well as the matrix, right-hand side vector,
 * and old and new solution vectors for the computation.
 *
 * @param startRow The first row this worker should process.
 * @param endRow The last row this worker should process.
 * @param matrix A pointer to the matrix of the system.
 * @param b A pointer to the right-hand side vector.
 * @param xOld A pointer to the old solution vector.
 * @param xNew A pointer to the new solution vector where results will be stored.
 * @param parent The parent QObject (default is nullptr).
 */
JacobiWorker::JacobiWorker(int startRow, int endRow, const QVector<QVector<double>>* matrix,
                           const QVector<double>* b, const QVector<double>* xOld,
                           QVector<double>* xNew, QObject* parent)
    : QObject(parent), startRow(startRow), endRow(endRow), matrix(matrix),
    b(b), xOld(xOld), xNew(xNew), terminateFlag(false) {}

/**
 * @brief Computes a portion of the Jacobi iteration.
 *
 * This method computes the new values of the solution vector `xNew` for the rows
 * assigned to this worker. It performs the Jacobi iteration:
 *     xNew[i] = b[i] - sum(matrix[i][j] * xOld[j]) for all j != i
 * The result is stored in the `xNew` vector.
 *
 * If the `terminateFlag` is set to true, the computation is stopped early.
 */
void JacobiWorker::compute() {
    if (terminateFlag) return;  // If termination flag is set, stop computation

    // Perform Jacobi iteration for rows between startRow and endRow
    for (int i = startRow; i < endRow; ++i) {
        double sum = 0.0;
        for (int j = 0; j < matrix->size(); ++j) {
            if (i != j) {
                sum += (*matrix)[i][j] * (*xOld)[j];
            }
        }
        (*xNew)[i] = (*b)[i] - sum;
    }

    // Emit the finishedComputing signal to indicate that the worker has completed its task
    emit finishedComputing();
}

/**
 * @brief Stops the worker's computation.
 *
 * This method sets the `terminateFlag` to true, causing the worker to stop processing
 * when it checks the flag. It's typically used when we need to cancel or terminate
 * the worker's task early.
 */
void JacobiWorker::stop() {
    qDebug() << "Worker destroyed: Rows " << startRow << " to " << endRow;
    terminateFlag = true;  // Set the terminate flag to stop the computation
}
