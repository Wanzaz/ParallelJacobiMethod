#include <QCoreApplication>
#include <QDebug>
#include "MatrixHandler.h"
#include "JacobiSolver.h"
#include "ArgumentParser.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ArgumentParser parser(argc, argv);
    if (!parser.parseArguments()) {
        qDebug() << "Chyba: Špatné argumenty. Použití: program <soubor> <epsilon>";
        return -1;
    }

    QString fileName = parser.getFileName();
    double epsilon = parser.getEpsilon();

    qDebug() << "Načítám soubor:" << fileName;
    qDebug() << "Epsilon:" << epsilon;

    MatrixHandler handler;
    QVector<QVector<double>> matrix;
    QVector<double> b;

    if (!handler.loadMatrixFromFile(fileName, matrix, b)) {
        qDebug() << "Chyba: Nelze načíst matici nebo vektor ze souboru.";
        return -1;
    }

    if (!handler.validateMatrix(matrix) || !handler.validateVector(b, matrix)) {
        qDebug() << "Chyba: Matice nebo vektor nejsou validní.";
        return -1;
    }

    int size = matrix.size();
    JacobiSolver solver(size);
    solver.setMatrix(matrix);
    solver.setB(b);

    QObject::connect(&solver, &JacobiSolver::finished, &a, &QCoreApplication::quit);
    solver.solve(epsilon);

    QVector<double> result = solver.getResult();
    handler.printResults(result);

    return a.exec();
}
