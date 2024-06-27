#include "parentwindow.h"
#include "ui_parentwindow.h"
#include <QActionGroup>
#include "functiondialog.h"
#include <QMessageBox>
#include <QVector>


// Constructor for the main window of the application
ParentWindow::ParentWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ParentWindow)
{ // This block is called when the app is started
    ui->setupUi(this);

    setCentralWidget(ui->WindowsManager);     // Set the central widget of the MainWindow to manage multiple document interface
    funcDialog = new FunctionDialog(this);  // Initialize the FunctionDialog object

    //Connect signals to slot functions
    connect(ui->actionDrawMultipleCharts, &QAction::triggered,this, &ParentWindow::on_actionDrawMultipleCharts_2_triggered);

}

// Destructor for ParentWindow.
ParentWindow::~ParentWindow()
{ // This block is called when the app is closing (to clean up all memory allocation used when tha app started)
    delete AddedDataSetWindow;
    delete subWindow;
    delete AddedDataSet;
    delete ui;

    delete funcDialog;
}


// Slot function called when "Load Dataset" action is triggered.
void ParentWindow::on_actionLoad_Dataset_triggered()
{ // This block is called when the user triggers Load action to load a file

    // Open a file dialog for the user to select a dataset
    QString curPath=QDir::currentPath(); // Directs the "open file" to the current directory
    QString FileName=QFileDialog::getOpenFileName(this,tr("Open file"),curPath,tr("Text files (*.txt);;Images (*.png *.xpm *.jpg);;All files(*.*)"));


    if (FileName.isEmpty())
        return; //If no file is selected don't do anything further

    // Create and load a new dataset
    AddedDataSet= new DataSet(FileName);
    if (AddedDataSet->IsDataSetValid)
    {

        AllDataSets.push_back(AddedDataSet); // Addding a pointer to the new dataset so that it can be accessed by the rest of the app

        // Creat a subWindow for the loaded DataSet:
        AddedDataSetWindow=new DataSetWindow(AddedDataSet,this);
        subWindow=ui->WindowsManager->addSubWindow(AddedDataSetWindow);
        AddedDataSetWindow->show(); // showing the new dataset window to the user (when it is added for the first time)

        // To enable the ParentWindow to plot the dataset when the user clicks on XYPlot option in the context menu
        // of an already displayed DataSetWidnow
        connect(AddedDataSetWindow,SIGNAL(Plot_XYPlot_SIGNAL(DataSet*)),this,SLOT(GraphWindowToBePlotted(DataSet*)));

    }

}

void ParentWindow::on_actionHelp_triggered()
{// This function is called when the user clicks on "Help" option under "Help" menu
    HelpDialog* Help_dlg=new HelpDialog(this);
    Help_dlg->exec();  // Show the help dialog
    delete Help_dlg;  // Clean up
}

void ParentWindow::on_actionAbout_triggered()
{ // This function is called when the user clicks on "About" option under "About" menu
    AboutDialog* About_dlg=new AboutDialog(this);
    About_dlg->exec();
    delete About_dlg;
}

// Slot function called to select datasets for plotting
void ParentWindow::on_actionSelect_a_dataset_triggered() {
    if (AllDataSets.isEmpty()) {
        QMessageBox::information(this, tr("No Data"), tr("No datasets are loaded."));
        return;
    }

    // Create a menu for selecting dataset
    QMenu selectMenu(this);
    QActionGroup actionGroup(&selectMenu);
    actionGroup.setExclusive(false); // Allow multiple selection

     // Add actions for each dataset to the menu
    for (int i = 0; i < AllDataSets.size(); ++i) {
        QAction *action = selectMenu.addAction(AllDataSets[i]->getName());
        action->setData(i);
        action->setCheckable(true);
        actionGroup.addAction(action);
    }

    // Display the menu and get user selections
    selectMenu.exec(QCursor::pos());

    //Use the first data set in the AllDataSets list as an argument to the constructor
    GraphWindow *graphWindow = new GraphWindow(AllDataSets.first(), this);
    for (QAction *action : actionGroup.actions()) {
        if (action->isChecked()) {
            int dataSetIndex = action->data().toInt();
            graphWindow->addDataSet(AllDataSets.value(dataSetIndex));
        }
    }


    // Show the graph window if datasets are selected
    if (graphWindow->hasDataSets()) {
        subWindow = ui->WindowsManager->addSubWindow(graphWindow);
        graphWindow->show();
    } else {
        delete graphWindow;
    }
}

// Slot function to display multiple charts
void ParentWindow::on_actionDrawMultipleCharts_2_triggered() {
    // Display a message and return if no datasets are loaded
    if (AllDataSets.isEmpty()) {
        QMessageBox::information(this, tr("No Data"), tr("No datasets are loaded."));
        return;
    }

    // Create a new graph window and add all datasets to it
    GraphWindow *graphWindow = new GraphWindow(AllDataSets.first(), this);
    for (auto *dataSet : AllDataSets) {
        graphWindow->addDataSet(dataSet);
    }

    // Show the graph window if datasets are added
    if (graphWindow->hasDataSets()) {
        subWindow = ui->WindowsManager->addSubWindow(graphWindow);
        graphWindow->show();
    } else {
        delete graphWindow;
    }
}


// Slot function to plot a dataset in a new graph window
void ParentWindow::GraphWindowToBePlotted(DataSet *ptr) {
      // Display a message and return if no datasets are loaded
    if (AllDataSets.isEmpty()) {
        QMessageBox::information(this, tr("No Data"), tr("No datasets are loaded."));
        return;
    }

    // Create a new graph window for the selected dataset and display it
    GraphWindow *addedGraphWindow = new GraphWindow(AllDataSets.first(), this);
    addedGraphWindow->addDataSet(ptr);
    subWindow = ui->WindowsManager->addSubWindow(addedGraphWindow);
    addedGraphWindow->show();
}


// Slot function to handle the 'Function' action
void ParentWindow::on_actionFunction_triggered() {
      // Display a message and return if no datasets are loaded
    if (AllDataSets.isEmpty()) {
        QMessageBox::information(this, tr("No Data"), tr("No datasets are loaded."));
        return;
    }

    // Create and display the function dialog
    FunctionDialog funcDialog(this);

     // Set the dataset file names for the function dialog
    QSet<QString> fileNames;
    for (const auto& dataSet : AllDataSets) {
        fileNames.insert(dataSet->getName());
    }
    funcDialog.setDatasetFileNames(fileNames);

    // Set the mathematical functions for the function dialog
    QSet<QString> mathFunctions = {"sin", "cos", "tan", "log", "exp", /* other functions */};
    funcDialog.setMathFunctions(mathFunctions);

    if (funcDialog.exec() == QDialog::Accepted) {
        QString expression = funcDialog.getExpression();

       // Process the dialog result
        if (!funcDialog.isValidExpression(expression)) {
            QMessageBox::critical(this, tr("Invalid Expression"), tr("The entered expression is invalid."));
            return;
        }

         // Validate the expression
        ATMSP<double> parserObj;
        ATMSB<double> byteCodeObj;

        std::string exps = expression.toStdString();
        std::string vars;   // Get variable names from the datasets

        // Parse the input expression
        size_t err = parserObj.parse(byteCodeObj, exps, vars);
        if (err) {
            QString errorMessage = QString::fromStdString(parserObj.errMessage(err));
            QMessageBox::critical(this, tr("Parsing Error"), errorMessage);
            return;
        }

        QVector<double> Result;

        // Assume all datasets have the same number of data points
        int dataSize = AllDataSets.first()->Size();
        for (int i = 0; i < dataSize; ++i) {
            byteCodeObj.fltErr = 0;

            // Set variable values
            byteCodeObj.var[0] = AllDataSets.first()->getPoint(i)[1];  //y value of the first dataset

            // Calculate the expression
            double result = byteCodeObj.run();
            if (byteCodeObj.fltErr) {
                QMessageBox::critical(this, tr("Evaluation Error"), tr("There was an error in evaluating the expression."));
                return;
            }
            Result.push_back(result);
        }
    }
}
