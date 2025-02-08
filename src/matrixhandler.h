#ifndef MATRIXHANDLER_H
#define MATRIXHANDLER_H

#include <QString>
#include <QVector>

class MatrixHandler
{
public:
    MatrixHandler();

    // Funkce pro načtení matice a vektoru b ze souboru
    bool loadMatrixFromFile(const QString& fileName, QVector<QVector<double>>& matrix, QVector<double>& b);

    // Funkce pro validaci matice
    bool validateMatrix(const QVector<QVector<double>>& matrix);

    // Funkce pro validaci vektoru b
    bool validateVector(const QVector<double>& b, const QVector<QVector<double>>& matrix);

    // Funkce pro výpis výsledků
    void printResults(const QVector<double>& results);

private:
    // Funkce pro kontrolu, zda je index v platném rozsahu
    bool isValidIndex(int index, int size);
};

#endif // MATRIXHANDLER_H
