#include <QCoreApplication>
#include <QDebug>
#include "MatrixHandler.h"
#include "JacobiSolver.h"
#include "ArgumentParser.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Parse command-line arguments
    ArgumentParser parser(argc, argv);
    if (!parser.parseArguments()) {
        qDebug() << "Chyba: Špatné argumenty. Použití: program <soubor> <epsilon>";
        return -1;
    }

    QString fileName = parser.getFileName();
    double epsilon = parser.getEpsilon();

    qDebug() << "Načítám soubor:" << fileName;
    qDebug() << "Epsilon:" << epsilon;

    // Load matrix and vector from file
    MatrixHandler handler;
    QVector<QVector<double>> matrix;
    QVector<double> b;

    if (!handler.loadMatrixFromFile(fileName, matrix, b)) {
        qDebug() << "Chyba: Nelze načíst matici nebo vektor ze souboru.";
        return -1;
    }

    // Validate matrix and vector
    if (!handler.validateMatrix(matrix)) {
        return -1;
    }

    if (!handler.validateVector(b, matrix)) {
        qDebug() << "Chyba: Vektor b má nesprávnou velikost.";
        return -1;
    }

    // Solve the system using Jacobi method
    int size = matrix.size();
    JacobiSolver solver(size);
    solver.setMatrix(matrix);
    solver.setB(b);

    solver.solve(epsilon);

    // Print results
    QVector<double> result = solver.getResult();
    handler.printResults(result);

    return a.exec();
}
