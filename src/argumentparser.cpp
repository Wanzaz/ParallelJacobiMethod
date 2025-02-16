#include "ArgumentParser.h"

ArgumentParser::ArgumentParser(int argc, char *argv[])
    : argc(argc), argv(argv), epsilon(0.0), valid(true)
{
}

bool ArgumentParser::parseArguments()
{
    for (int i = 1; i < argc; ++i) {
        QString arg = QString(argv[i]);

        if (arg == "-f" && i + 1 < argc) {
            fileName = QString(argv[i + 1]);
            i++;  // Skipping the next argument because it's the file name
        } else if (arg == "-e" && i + 1 < argc) {
            bool epsilonOk = false;
            epsilon = QString(argv[i + 1]).toDouble(&epsilonOk);
            if (!epsilonOk || epsilon <= 0) {
                qDebug() << "Chyba: Neplatná hodnota pro epsilon.";
                valid = false;
                return false;
            }
            i++;  // Skipping the next argument because it's the epsilon value
        }
    }

    // Kontrola, zda jsou argumenty kompletní a validní
    if (fileName.isEmpty() || epsilon == 0.0) {
        qDebug() << "Chyba: Nezadali jste soubor nebo epsilon.";
        valid = false;
        return false;
    }

    return true;
}

QString ArgumentParser::getFileName() const
{
    return fileName;
}

double ArgumentParser::getEpsilon() const
{
    return epsilon;
}

bool ArgumentParser::isValid() const
{
    return valid;
}
