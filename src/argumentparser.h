#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <QString>
#include <QStringList>
#include <QDebug>

class ArgumentParser
{
public:
    ArgumentParser(int argc, char *argv[]);

    bool parseArguments();  // Načtení argumentů
    QString getFileName() const;  // Vrátí jméno souboru
    double getEpsilon() const;  // Vrátí hodnotu epsilon
    bool isValid() const;  // Vrátí zda jsou argumenty platné

private:
    int argc;
    char **argv;
    QString fileName;
    double epsilon;
    bool valid;  // Kontrola platnosti argumentů
};

#endif // ARGUMENTPARSER_H
