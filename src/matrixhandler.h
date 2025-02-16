#ifndef MATRIXHANDLER_H
#define MATRIXHANDLER_H

#include <QString>
#include <QVector>

/**
 * @class MatrixHandler
 * @brief A class for handling matrix operations, including loading from a file, validation, and result output.
 */
class MatrixHandler
{
public:
    MatrixHandler();

    /**
     * @brief Loads a matrix and a vector from a text file.
     *
     * Reads a file where each row represents a row of the matrix, with the last value stored separately in vector b.
     * Ensures the values are valid numbers and checks for matrix consistency.
     *
     * @param fileName The name of the file to be loaded.
     * @param matrix Reference to a 2D vector where the matrix will be stored.
     * @param b Reference to a vector where the last column (right-hand side) will be stored.
     * @return true if the matrix and vector were successfully loaded, false otherwise.
     */
    bool loadMatrixFromFile(const QString& fileName, QVector<QVector<double>>& matrix, QVector<double>& b);


    /**
     * @brief Validates whether a given matrix is diagonally dominant.
     *
     * A matrix is diagonally dominant if, for each row, the absolute value of the diagonal
     * element is greater than or equal to the sum of the absolute values of the non-diagonal elements.
     *
     * @param matrix The matrix to be validated.
     * @return true if the matrix is diagonally dominant, false otherwise.
     */
    bool validateMatrix(const QVector<QVector<double>>& matrix);


    /**
     * @brief Validates if the vector b has the correct size relative to the matrix.
     *
     * @param b The right-hand side vector.
     * @param matrix The coefficient matrix.
     * @return true if the size of vector b matches the number of rows in the matrix, false otherwise.
     */
    bool validateVector(const QVector<double>& b, const QVector<QVector<double>>& matrix);


    /**
     * @brief Prints the solution vector.
     *
     * @param results The solution vector to be printed.
     */
    void printResults(const QVector<double>& results);

private:
};

#endif // MATRIXHANDLER_H
