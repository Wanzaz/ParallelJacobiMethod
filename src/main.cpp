#include <QCoreApplication>
#include <QDebug>
#include <QtConcurrent>
#include "MatrixHandler.h"
#include "JacobiSolver.h"
#include "ArgumentParser.h"

/**
 * @brief The main function that initializes the application, parses arguments,
 *        loads the matrix and vector, and solves the system using the Jacobi method.
 *
 * It performs the following:
 * 1. Parses command-line arguments for the input file and epsilon value.
 * 2. Loads the matrix and vector from the specified file.
 * 3. Validates the matrix and vector.
 * 4. Initializes the Jacobi solver and solves the system asynchronously.
 * 5. Displays the results once the computation is finished.
 */
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ArgumentParser parser(argc, argv);
    if (!parser.parseArguments()) {
        qDebug() << "Error: Incorrect arguments. Usage: program <file> <epsilon>";
        return -1;
    }

    QString fileName = parser.getFileName();
    double epsilon = parser.getEpsilon();

    qDebug() << "Loading file:" << fileName;
    qDebug() << "Epsilon:" << epsilon;

    MatrixHandler handler;
    QVector<QVector<double>> matrix;
    QVector<double> b;

    if (!handler.loadMatrixFromFile(fileName, matrix, b)) {
        qDebug() << "Error: Unable to load matrix or vector from file.";
        return -1;
    }

    if (!handler.validateMatrix(matrix) || !handler.validateVector(b, matrix)) {
        qDebug() << "Error: Matrix or vector is not valid.";
        return -1;
    }

    int size = matrix.size();

    JacobiSolver solver(size);
    solver.setMatrix(matrix);
    solver.setB(b);

    // Start the computation asynchronously using QtConcurrent
    QFuture<void> future = QtConcurrent::run([&solver, epsilon]() {
        solver.solve(epsilon);
    });

    // Once the computation is finished, retrieve the result and print it
    QObject::connect(&solver, &JacobiSolver::finished, [&]() {
        QVector<double> result = solver.getResult();
        handler.printResults(result);
        QCoreApplication::quit();
    });

    // Start the Qt event loop to keep the application running
    return a.exec();
}

