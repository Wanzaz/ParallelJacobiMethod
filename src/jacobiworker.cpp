#include "JacobiWorker.h"
#include <QDebug>
#include <exception>

void JacobiWorker::run() {
    try {
        while (true) {
            mutex->lock();
            while (!(*ready)) {
                condition->wait(mutex);
            }
            mutex->unlock();

            // Provádění výpočtů
            for (int i = startRow; i < endRow; ++i) {
                double sum = 0.0;
                for (int j = 0; j < matrix->size(); ++j) {
                    if (i != j) {
                        sum += (*matrix)[i][j] * (*x)[j];
                    }
                }
                (*xNew)[i] = ((*b)[i] - sum) / (*matrix)[i][i];
            }

            mutex->lock();
            *ready = false;
            condition->wakeOne();
            mutex->unlock();
        }
    } catch (const std::exception& e) {
        qDebug() << "Došlo k chybě ve vlákně:" << e.what();
        errorFlag = true; // Nastavení příznaku chyby
    } catch (...) {
        qDebug() << "Došlo k neznámé chybě ve vlákně!";
        errorFlag = true; // Nastavení příznaku chyby
    }
}
