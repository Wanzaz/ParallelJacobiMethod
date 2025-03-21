#ifndef JACOBIWORKER_H
#define JACOBIWORKER_H

#include <QObject>
#include <QVector>

class JacobiWorker : public QObject {
    Q_OBJECT

public:
    explicit JacobiWorker(int startRow, int endRow, const QVector<QVector<double>>* matrix,
                          const QVector<double>* b, const QVector<double>* xOld,
                          QVector<double>* xNew, QObject* parent = nullptr);

public slots:
    void compute();
    void stop();

signals:
    void finishedComputing();

private:
    int startRow, endRow;
    const QVector<QVector<double>>* matrix;
    const QVector<double>* b;
    const QVector<double>* xOld;
    QVector<double>* xNew;
    bool terminateFlag;
};

#endif
