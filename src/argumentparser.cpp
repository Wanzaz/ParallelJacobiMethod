#include "ArgumentParser.h"


/**
 * @brief Constructs an ArgumentParser object.
 *
 * Initializes the parser with command-line arguments and sets default values
 * for epsilon and validation status.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument strings.
 */
ArgumentParser::ArgumentParser(int argc, char *argv[])
    : argc(argc), argv(argv), epsilon(0.0), valid(true)
{
}


/**
 * @brief Parses the command-line arguments.
 *
 * Recognizes two options:
 * - `-f <fileName>`: Specifies the input file.
 * - `-e <epsilon>`: Specifies the epsilon value (must be positive).
 *
 * Validates that required arguments are provided and that epsilon is a valid positive number.
 *
 * @return true if arguments are successfully parsed and valid, false otherwise.
 */
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

    // CHeck if the arguments are complete and valid
    if (fileName.isEmpty() || epsilon == 0.0) {
        qDebug() << "Chyba: Nezadali jste soubor nebo epsilon.";
        valid = false;
        return false;
    }

    return true;
}


/**
 * @brief Gets the parsed file name.
 *
 * @return The file name as a QString.
 */
QString ArgumentParser::getFileName() const
{
    return fileName;
}


/**
 * @brief Gets the parsed epsilon value.
 *
 * @return The epsilon value as a double.
 */
double ArgumentParser::getEpsilon() const
{
    return epsilon;
}


/**
 * @brief Checks if the parsed arguments are valid.
 *
 * @return true if arguments were successfully parsed and valid, false otherwise.
 */
bool ArgumentParser::isValid() const
{
    return valid;
}
