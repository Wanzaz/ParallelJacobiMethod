#ifndef JACOBISOLVER_H
#define JACOBISOLVER_H

#include <QObject>
#include <QVector>
#include <QtConcurrent>
#include <QFuture>

/**
 * @class JacobiSolver
 * @brief A class for solving linear systems using the Jacobi method.
 *
 * This class solves a system of linear equations using the Jacobi iterative method.
 * It supports parallel execution of the method across multiple threads to speed up the computation.
 */
class JacobiSolver : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs a JacobiSolver object.
     *
     * Initializes the solver with the matrix size and allocates memory for vectors.
     * This constructor does not start the computation, it only sets up the solver.
     *
     * @param size The size of the system (number of rows and columns).
     * @param parent The parent object for this QObject (default is nullptr).
     */
    explicit JacobiSolver(int size, QObject* parent = nullptr);

    /**
     * @brief Destructor for the JacobiSolver class.
     *
     * Cleans up any allocated resources when the solver is destroyed.
     */
    ~JacobiSolver();

    /**
     * @brief Sets the matrix for the system of equations.
     *
     * This function takes a matrix as input and sets it as the system's matrix.
     *
     * @param m The matrix representing the coefficients of the system.
     */
    void setMatrix(const QVector<QVector<double>>& m);

    /**
     * @brief Sets the right-hand side vector (b).
     *
     * This function sets the right-hand side vector for the system of equations.
     *
     * @param rhs The right-hand side vector representing the constants.
     */
    void setB(const QVector<double>& rhs);

    /**
     * @brief Gets the result vector after solving the system.
     *
     * This function returns the solution vector that holds the results of the system of equations.
     *
     * @return A QVector<double> containing the solution to the system.
     */
    QVector<double> getResult();

    /**
     * @brief Solves the system of equations using the Jacobi method.
     *
     * This method performs the Jacobi iterative method until convergence is reached.
     * It uses a specified epsilon value to determine when the solution has converged.
     *
     * @param epsilon The convergence threshold (stopping criterion).
     */
    void solve(double epsilon);

signals:
    /**
     * @brief Signal emitted when the Jacobi solver finishes the computation.
     *
     * This signal is emitted when the solver completes the iterative process and has found a solution.
     */
    void finished();

private:
    int size;  ///< The size of the system (number of rows and columns).
    QVector<QVector<double>> matrix;  ///< The matrix of coefficients for the system of equations.
    QVector<double> b;  ///< The right-hand side vector (constants).
    QVector<double> x;  ///< The current approximation of the solution.
    QVector<double> xNew;  ///< The updated approximation of the solution after an iteration.
    QVector<double> xOld;  ///< The previous approximation of the solution.

    /**
     * @brief Normalizes the matrix and the right-hand side vector (b).
     *
     * This function ensures that the diagonal elements of the matrix are non-zero by normalizing them.
     * It modifies the matrix and the vector in place, making the diagonal elements 1 and adjusting the rest.
     *
     * @param matrix The matrix to normalize.
     * @param b The right-hand side vector to normalize.
     */
    void normalizeMatrix(QVector<QVector<double>>& matrix, QVector<double>& b);
};

#endif // JACOBISOLVER_H
