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
    QCoreApplication a(argc, argv);  // Create a Qt application instance

    // Create an ArgumentParser object to parse command-line arguments
    ArgumentParser parser(argc, argv);
    if (!parser.parseArguments()) {
        qDebug() << "Error: Incorrect arguments. Usage: program <file> <epsilon>";
        return -1;
    }

    // Get the parsed file name and epsilon value
    QString fileName = parser.getFileName();
    double epsilon = parser.getEpsilon();

    qDebug() << "Loading file:" << fileName;
    qDebug() << "Epsilon:" << epsilon;

    // Create a MatrixHandler object to handle the matrix and vector
    MatrixHandler handler;
    QVector<QVector<double>> matrix;
    QVector<double> b;

    // Attempt to load the matrix and vector from the file
    if (!handler.loadMatrixFromFile(fileName, matrix, b)) {
        qDebug() << "Error: Unable to load matrix or vector from file.";
        return -1;
    }

    // Validate the matrix and vector
    if (!handler.validateMatrix(matrix) || !handler.validateVector(b, matrix)) {
        qDebug() << "Error: Matrix or vector is not valid.";
        return -1;
    }

    // Get the size of the matrix (assuming square matrix)
    int size = matrix.size();

    // Create a JacobiSolver object and initialize it with the matrix and vector
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

