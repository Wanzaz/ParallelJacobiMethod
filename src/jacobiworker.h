#ifndef JACOBIWORKER_H
#define JACOBIWORKER_H

#include <QObject>
#include <QVector>

/**
 * @class JacobiWorker
 * @brief A class for performing the computation of a portion of the Jacobi method.
 *
 * This class represents a worker responsible for computing a portion of the Jacobi iteration
 * for solving a system of linear equations. Each worker is assigned a range of rows to compute in parallel.
 */
class JacobiWorker : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs a JacobiWorker object.
     *
     * Initializes the worker with the range of rows to process and the necessary data for computation.
     * This constructor does not perform any computations, it just sets up the worker.
     *
     * @param startRow The starting row for this worker to compute.
     * @param endRow The ending row for this worker to compute.
     * @param matrix A pointer to the system's coefficient matrix.
     * @param b A pointer to the right-hand side vector (constants).
     * @param xOld A pointer to the previous approximation of the solution.
     * @param xNew A pointer to the new approximation of the solution (computed by the worker).
     * @param parent The parent object for this QObject (default is nullptr).
     */
    explicit JacobiWorker(int startRow, int endRow, const QVector<QVector<double>>* matrix,
                          const QVector<double>* b, const QVector<double>* xOld,
                          QVector<double>* xNew, QObject* parent = nullptr);

public slots:
    /**
     * @brief Performs the computation of the Jacobi iteration for the assigned rows.
     *
     * This function performs the Jacobi iteration for the rows assigned to the worker.
     * It calculates the new values for the solution vector based on the current approximation.
     */
    void compute();

    /**
     * @brief Stops the worker's execution.
     *
     * This function sets a flag to terminate the worker's computation.
     * It can be used to stop a worker before it finishes its computation.
     */
    void stop();

signals:
    /**
     * @brief Signal emitted when the worker finishes its computation.
     *
     * This signal is emitted when the worker has finished processing its assigned rows.
     */
    void finishedComputing();

private:
    int startRow, endRow;  ///< The range of rows assigned to this worker for computation.
    const QVector<QVector<double>>* matrix;  ///< Pointer to the system's coefficient matrix.
    const QVector<double>* b;  ///< Pointer to the right-hand side vector (constants).
    const QVector<double>* xOld;  ///< Pointer to the previous approximation of the solution.
    QVector<double>* xNew;  ///< Pointer to the new approximation of the solution (computed by the worker).
    bool terminateFlag;  ///< Flag indicating whether the worker should stop its computation.
};

#endif // JACOBIWORKER_H
