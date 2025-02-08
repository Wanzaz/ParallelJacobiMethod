#include "MatrixHandler.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

MatrixHandler::MatrixHandler() {}


bool MatrixHandler::loadMatrixFromFile(const QString& fileName, QVector<QVector<double>>& matrix, QVector<double>& b)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Chyba při otevírání souboru.";
        return false;
    }

    QTextStream in(&file);
    int rows, cols;
    in >> rows >> cols;

    // Nastavení velikosti matice a vektoru b
    matrix.resize(rows);
    b.resize(rows);

    // Načítání matice a vektoru b (poslední sloupec je vektor b)
    for (int i = 0; i < rows; ++i) {
        matrix[i].resize(cols - 1);  // Matice má o jeden sloupec méně než počet sloupců v souboru
        for (int j = 0; j < cols - 1; ++j) {
            in >> matrix[i][j];  // Načítání hodnot matice (bez posledního sloupce)
        }
        in >> b[i];  // Načítání hodnoty z posledního sloupce do vektoru b
    }


    // Validace matice a vektoru
    if (!validateMatrix(matrix) || !validateVector(b, matrix)) {
        return false;
    }

    return true;
}



bool MatrixHandler::validateMatrix(const QVector<QVector<double>>& matrix)
{
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = 0; i < rows; ++i) {
        double sum = 0.0;

        // Sum of all values except for the diagonal
        for (int j = 0; j < cols; ++j) {
            if (i != j) {
                sum += matrix[i][j];
            }
        }

        // Check diagonal dominance
        if (matrix[i][i] <= sum) {
            qDebug() << "Matrix is not diagonally dominant at row" << i + 1;
            return false; // If it's not diagonally dominant, return false
        }
    }

    return true; // Matrix is diagonally dominant
}






bool MatrixHandler::validateVector(const QVector<double>& b, const QVector<QVector<double>>& matrix)
{
    // Kontrola, že vektor b má správnou délku (stejný počet prvků jako matice)
    if (b.size() != matrix.size()) {
        qDebug() << "Vektor b musí mít stejný počet prvků jako matice.";
        return false;
    }
    return true;
}

bool MatrixHandler::isValidIndex(int index, int size)
{
    return index >= 0 && index < size;
}

void MatrixHandler::printResults(const QVector<double>& results)
{
    qDebug() << "Výsledek:";
    for (int i = 0; i < results.size(); ++i) {
        qDebug() << "x_" << i + 1 << "=" << results[i];
    }
}
