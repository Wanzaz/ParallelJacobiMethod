#include "MatrixHandler.h"
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QDebug>

/**
 * @brief Default constructor for the MatrixHandler class.
 *
 * Initializes an instance of the MatrixHandler class.
 * Currently, no specific initialization is performed.
 */
MatrixHandler::MatrixHandler() {}


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
bool MatrixHandler::loadMatrixFromFile(const QString& fileName, QVector<QVector<double>>& matrix, QVector<double>& b) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open the file.";
        return false;
    }

    QTextStream in(&file);
    matrix.clear();
    b.clear();

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList values = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        int numCols = values.size();
        if (numCols < 2) {
            qDebug() << "Error: The row does not contain enough values.";
            return false;
        }

        QVector<double> row;
        bool ok = true; // indicator of transforming strings to doubles
        for (int i = 0; i < numCols - 1; ++i) {
            row.append(values[i].toDouble(&ok));
            if (!ok) {
                qDebug() << "Error: Invalid value in the row.";
                return false;
            }
        }

        matrix.append(row);
        b.append(values.last().toDouble(&ok));
        if (!ok) {
            qDebug() << "Error: Invalid value in vector b.";
            return false;
        }
    }

    file.close();

    // Check if the matrix has a consistent number of columns
    int expectedCols = matrix.first().size();
    for (const auto& row : matrix) {
        if (row.size() != expectedCols) {
            qDebug() << "Error: The matrix has an inconsistent number of columns.";
            return false;
        }
    }

    return true;
}


/**
 * @brief Validates whether a given matrix is diagonally dominant.
 *
 * A matrix is diagonally dominant if, for each row, the absolute value of the diagonal
 * element is greater than or equal to the sum of the absolute values of the non-diagonal elements.
 *
 * @param matrix The matrix to be validated.
 * @return true if the matrix is diagonally dominant, false otherwise.
 */
bool MatrixHandler::validateMatrix(const QVector<QVector<double>>& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        double diag = matrix[i][i];
        double sum = 0.0;
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (i != j) sum += matrix[i][j];
        }

        if (diag <= sum) {
            qDebug() << "Error: The matrix is not diagonally dominant in row" << i + 1;
            return false;
        }
    }
    return true;
}


/**
 * @brief Validates if the vector b has the correct size relative to the matrix.
 *
 * @param b The right-hand side vector.
 * @param matrix The coefficient matrix.
 * @return true if the size of vector b matches the number of rows in the matrix, false otherwise.
 */
bool MatrixHandler::validateVector(const QVector<double>& b, const QVector<QVector<double>>& matrix) {
    return (b.size() == matrix.size());
}


/**
 * @brief Prints the solution vector.
 *
 * @param results The solution vector to be printed.
 */
void MatrixHandler::printResults(const QVector<double>& results) {
    qDebug() << "Result:";
    for (int i = 0; i < results.size(); ++i) {
        qDebug() << "x_" + QString::number(i + 1) << "=" << results[i];
    }
}
