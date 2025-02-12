#ifndef MATRIXHANDLER_H
#define MATRIXHANDLER_H

#include <QString>
#include <QVector>

class MatrixHandler
{
public:
    MatrixHandler();

    // Load matrix and vector b from file
    bool loadMatrixFromFile(const QString& fileName, QVector<QVector<double>>& matrix, QVector<double>& b);

    // Validate matrix (check for diagonal dominance)
    bool validateMatrix(const QVector<QVector<double>>& matrix);

    // Validate vector b (check if it matches the matrix size)
    bool validateVector(const QVector<double>& b, const QVector<QVector<double>>& matrix);

    // Print results
    void printResults(const QVector<double>& results);

private:
    // Check if an index is valid
    bool isValidIndex(int index, int size);
};

#endif // MATRIXHANDLER_H
