#include "MatrixHandler.h"
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>
#include <QDebug>

MatrixHandler::MatrixHandler() {}

bool MatrixHandler::loadMatrixFromFile(const QString& fileName, QVector<QVector<double>>& matrix, QVector<double>& b) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Chyba: Nelze otevřít soubor.";
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
            qDebug() << "Chyba: Řádka neobsahuje dostatek hodnot.";
            return false;
        }

        QVector<double> row;
        bool ok = true;
        for (int i = 0; i < numCols - 1; ++i) {
            row.append(values[i].toDouble(&ok));
            if (!ok) {
                qDebug() << "Chyba: Neplatná hodnota v řádku.";
                return false;
            }
        }

        matrix.append(row);
        b.append(values.last().toDouble(&ok));
        if (!ok) {
            qDebug() << "Chyba: Neplatná hodnota vektoru b.";
            return false;
        }
    }

    file.close();

    // Kontrola správné velikosti matice
    int expectedCols = matrix.first().size();
    for (const auto& row : matrix) {
        if (row.size() != expectedCols) {
            qDebug() << "Chyba: Matice má nekonzistentní počet sloupců.";
            return false;
        }
    }

    return true;
}

bool MatrixHandler::validateMatrix(const QVector<QVector<double>>& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        double diag = matrix[i][i];
        double sum = 0.0;
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (i != j) sum += matrix[i][j];
        }

        if (diag <= sum) {
            qDebug() << "Chyba: Matice není diagonálně dominantní v řádku" << i + 1;
            return false;
        }
    }
    return true;
}

bool MatrixHandler::validateVector(const QVector<double>& b, const QVector<QVector<double>>& matrix) {
    return (b.size() == matrix.size());
}

void MatrixHandler::printResults(const QVector<double>& results) {
    qDebug() << "Výsledek:";
    for (int i = 0; i < results.size(); ++i) {
        qDebug() << "x_" + QString::number(i + 1) << "=" << results[i];
    }
}
