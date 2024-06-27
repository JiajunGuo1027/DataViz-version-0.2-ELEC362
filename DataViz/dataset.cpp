#include "dataset.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

// Initializing the static variable to count the datasets
int DataSet::DataSetCounter = 0;

// Constructor for DataSet class
DataSet::DataSet(QString& FileName) {
    QFileInfo infoFile(FileName);
    QString fileInfoName = infoFile.baseName();
    commentName =  "The comment of " + fileInfoName + "description.txt";

    // Reading the data from the file
    QFile file(FileName);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        // Step 1: Count the number of rows to determine matrix size for GSL (Gnu Scientific Library)
        while (!in.atEnd()) {
            in.readLine();
            NumberOfRows++;
        }

        // Step 2: Allocate a GSL matrix to store the data for further processing
        Matrix = gsl_matrix_alloc(NumberOfRows, 2);

        // Step 3: Store the data from the file into the GSL matrix
        in.seek(0);
        for (int i = 0; i < NumberOfRows; i++) {
            QString x, y;
            in >> x >> y; // Assumes x and y data are separated by a tab or a comma

            double x_double, y_double;
            // Convert the strings to doubles and check if the dataset is valid (contains only numbers)
            x_double = x.toDouble(&IsDataSetValid);
            y_double = y.toDouble(&IsDataSetValid);

            if (IsDataSetValid) {
                // If valid, set the values in the matrix
                gsl_matrix_set(Matrix, i, 0, x_double); // First column is x coordinate
                gsl_matrix_set(Matrix, i, 1, y_double); // Second column is y coordinate
            } else {
                // If invalid, display an error message and stop reading the file
                QMessageBox errorMsgBox;
                errorMsgBox.setWindowTitle("Error");
                errorMsgBox.setWindowIcon(QIcon(":/icons/errorSymbol.svg"));
                errorMsgBox.setText("Error");
                errorMsgBox.setInformativeText("The app encountered a non-numeric character in the dataset.");
                errorMsgBox.setIcon(QMessageBox::Critical);
                errorMsgBox.exec();

                // Free allocated memory as reading the file failed
                gsl_matrix_free(Matrix);
                break;
            }
        }
    }

    // Increment the dataset counter and assign a default name (D1, D2, ...) if loading is successful
    if (IsDataSetValid) {
        DataSetCounter++;
        DataSetName = "D" + QString::number(DataSetCounter) + "--" + fileInfoName;
    }
}

// Function to return the size of the dataset (number of rows)
int DataSet::Size() {
    return NumberOfRows;
}

// Function to return the name of the dataset
QString DataSet::getName() {
    return DataSetName;
}

// Function to return the ith data point (x, y coordinates)
double *DataSet::getPoint(int i) {
    DataPoint[0] = gsl_matrix_get(Matrix, i, 0); // x-coordinate
    DataPoint[1] = gsl_matrix_get(Matrix, i, 1); // y-coordinate
    return DataPoint;
}

// Function to save a comment specific to the dataset
void DataSet::saveComment(const QString &newComment) {
    QFile file(commentName); // Create a separate comment file for each dataset
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << newComment;
        file.close();
    }
}

// Function to load a comment specific to the dataset
QString DataSet::loadComment() {
    QFile file(commentName); // Read from a dataset-specific comment file
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        comment = stream.readAll();
        file.close();
        return comment;
    }
    return QString(); // Return an empty string if no comment file is found
}
