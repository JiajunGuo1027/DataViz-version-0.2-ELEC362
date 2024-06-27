#ifndef FUNCTIONDIALOG_H
#define FUNCTIONDIALOG_H

#include <QDialog>
#include <QSet>
#include <QStringList>

namespace Ui {
class FunctionDialog;
}

// The FunctionDialog class represents a dialog window where users can input mathematical expressions
class FunctionDialog : public QDialog {
    Q_OBJECT

public:
    // Constructor and destructor
    explicit FunctionDialog(QWidget *parent = nullptr);
    ~FunctionDialog();

    // Retrieves the entered mathematical expression from the dialog
    QString getExpression() const;

    // Methods to set the dataset names and mathematical functions available for expressions
    void setDatasetNames(const QSet<QString>& names) { datasetNames = names; }
    void setMathFunctions(const QSet<QString>& functions) { mathFunctions = functions; }

    // Checks if the given expression is valid based on available datasets and functions
    bool isValidExpression(const QString& expression);

    // Adds a method to set dataset file names for validation purposes
    void setDatasetFileNames(const QSet<QString>& names) {
        datasetFileNames = names;
    }

private slots:   // Slot functions for OK and Cancel buttons
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::FunctionDialog *ui;

    // Lists of dataset names, mathematical functions, and dataset file names
    QSet<QString> datasetNames;
    QSet<QString> mathFunctions;
    QSet<QString> datasetFileNames;    //Added member variable to store dataset file names
};

#endif // FUNCTIONDIALOG_H
