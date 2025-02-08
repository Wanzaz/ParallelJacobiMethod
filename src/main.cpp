#include <QCoreApplication>
#include <QDebug>
#include "MatrixHandler.h"
#include "JacobiSolver.h"
#include "ArgumentParser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Vytvoření instance ArgumentParser pro zpracování argumentů
    ArgumentParser parser(argc, argv);

    if (!parser.parseArguments()) {
        return -1;  // Ukončíme aplikaci v případě chyby
    }

    // Získání hodnot souboru a epsilonu
    QString fileName = parser.getFileName();
    double epsilon = parser.getEpsilon();

    // Výpis zadaných hodnot pro potvrzení
    qDebug() << "Zadaný soubor: " << fileName;
    qDebug() << "Zadaná hodnota epsilon je: " << epsilon;

    // Vytvoření instance pro načítání matice
    MatrixHandler handler;

    QVector<QVector<double>> matrix;
    QVector<double> b;

    if (!handler.loadMatrixFromFile(fileName, matrix, b)) {
        qDebug() << "Chyba při načítání souboru!";
        return -1; // Ukončíme aplikaci v případě chyby
    }

    // Předpokládáme, že velikost matice je správná
    int size = matrix.size();

    // Vytvoření instance pro řešení systému rovnic pomocí Jacobiho metody
    JacobiSolver solver(size);
    solver.setMatrix(matrix);
    solver.setB(b);

    // Řešení rovnic
    solver.solve(epsilon); // Použití epsilon místo iterací

    // Výpis výsledků
    QVector<double> result = solver.getResult();
    handler.printResults(result);

    return a.exec();
}
