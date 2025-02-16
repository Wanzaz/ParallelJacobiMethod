#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <QString>
#include <QStringList>
#include <QDebug>


/**
 * @class ArgumentParser
 * @brief A class for parsing command-line arguments.
 *
 * This class processes command-line arguments to extract an input file name
 * and an epsilon value for numerical computations. It also performs basic validation.
 */
class ArgumentParser
{
public:

    /**
     * @brief Constructs an ArgumentParser object.
     *
     * Initializes the parser with command-line arguments and sets default values
     * for epsilon and validation status.
     *
     * @param argc The number of command-line arguments.
     * @param argv The array of command-line argument strings.
     */
    ArgumentParser(int argc, char *argv[]);


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
    bool parseArguments();


    /**
     * @brief Gets the parsed file name.
     *
     * @return The file name as a QString.
     */
    QString getFileName() const;


   /**
     * @brief Gets the parsed epsilon value.
     *
     * @return The epsilon value as a double.
     */
    double getEpsilon() const;


    /**
     * @brief Checks if the parsed arguments are valid.
     *
     * @return true if arguments were successfully parsed and valid, false otherwise.
     */
    bool isValid() const;

private:
    int argc;
    char **argv;
    QString fileName;
    double epsilon;
    bool valid;
};

#endif // ARGUMENTPARSER_H
