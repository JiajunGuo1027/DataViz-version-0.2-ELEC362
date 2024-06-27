#include "functiondialog.h"
#include "ui_functiondialog.h"
#include <QMessageBox>
#include <QVector>
#include <QRegularExpression>

// Constructor for the FunctionDialog class. Initializes the UI components
FunctionDialog::FunctionDialog(QWidget *parent) :
    QDialog(parent), // Call the base class constructor with the parent widget
    ui(new Ui::FunctionDialog) // Initialize the Ui::FunctionDialog object
{
    ui->setupUi(this); // Set up the user interface as defined in the UI file

    // Connect the Cancel button's clicked signal to the on_pushButtonCancel_clicked slot
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &FunctionDialog::on_pushButtonCancel_clicked);
}

// Destructor for the FunctionDialog class
FunctionDialog::~FunctionDialog()
{
    delete ui;  // Delete the UI object to free resources
}

// Returns the mathematical expression entered by the user in the dialog
QString FunctionDialog::getExpression() const
{
    return ui->lineEditExpression->text();  // Fetch the text from the lineEdit UI component
}

// Slot function called when the OK button is clicked
void FunctionDialog::on_pushButtonOK_clicked()
{
    QString userInput = getExpression();  // Retrieve the expression entered by the user
    std::string exps = userInput.toStdString();  // Convert QString to standard string

    // Validate the expression
    if (isValidExpression(userInput)) {
        // Display a success message if the expression is valid.
        QMessageBox::information(this, tr("Success"), tr("The expression was successfully evaluated."));
    } else {
        // Display an error message if the expression is invalid
        QMessageBox::critical(this, tr("Error"), tr("Invalid mathematical expression."));
    }
}

// Slot function called when the Cancel button is clicked.
void FunctionDialog::on_pushButtonCancel_clicked()
{
    reject();  //Close the dialog and signal that the operation was canceled.
}

// Validates if the given expression is valid by checking each word against known datasets and functions.
bool FunctionDialog::isValidExpression(const QString& expression) {
    // Split the expression into words.
    QStringList words = expression.split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);

    // Iterate through each word in the expression
    for (const QString& word : words) {
        // Check if the word is not a known dataset name, math function, or dataset file name.
        if (!datasetNames.contains(word) && !mathFunctions.contains(word) && !datasetFileNames.contains(word)) {
            return false; // Invalid word found
        }
    }

    return true; // All words in the expression are valid
}
