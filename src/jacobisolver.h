#ifndef JACOBISOLVER_H
#define JACOBISOLVER_H

#include <QVector>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>

class JacobiSolver
{
public:
    JacobiSolver(int size);

    // Metoda pro řešení systému rovnic s epsilonem (přesnost)
    void solve(double epsilon);

    // Nastavení matice a pravé strany
    void setMatrix(const QVector<QVector<double>>& m);
    void setB(const QVector<double>& rhs);

    // Získání výsledku (vektoru řešení)
    QVector<double> getResult();

private:
    // Metoda pro kontrolu diagonální dominance
    bool isDiagonallyDominant() const;

    int size;  // Velikost matice
    QVector<QVector<double>> matrix;  // Matice koeficientů
    QVector<double> b;  // Pravá strana
    QVector<double> x;  // Aktuální řešení
    QVector<double> xNew;  // Nové řešení
    bool ready;  // Flag pro indikaci, že vlákna jsou připravena na další iteraci

    QMutex mutex;  // Mutex pro synchronizaci vlákna
    QWaitCondition condition;  // Podmínka pro čekání ve vláknech
};

#endif // JACOBISOLVER_H
