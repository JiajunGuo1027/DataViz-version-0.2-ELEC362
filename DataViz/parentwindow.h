#ifndef PARENTWINDOW_H
#define PARENTWINDOW_H

/********************************
 *
 *  The ParentWindow class serves as the main window and backbone of the entire application.
 *  An instance of this class represents the running state of the application.
 *
 *  All other windows and dialogs within the application are children of this window.
 *  This class is responsible for managing datasets and graphs within the application.
 *
 **********************************/

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QList>
#include <QMdiSubWindow>
#include <QAction>
#include <QMenu>
#include "dataset.h"
#include "datasetwindow.h"
#include "graphwindow.h"
#include "aboutdialog.h"
#include "helpdialog.h"
#include "functiondialog.h"
#include "atmsp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ParentWindow; }
QT_END_NAMESPACE

class ParentWindow : public QMainWindow
{
    Q_OBJECT

public:
    ParentWindow(QWidget *parent = nullptr);
    ~ParentWindow();

private slots:

    void on_actionLoad_Dataset_triggered();   // Slot for loading a new dataset
    void GraphWindowToBePlotted(DataSet *ptr);   // Slot to create and display a new graph window

    // Slots for triggering About and Help dialogs
    void on_actionAbout_triggered();
    void on_actionHelp_triggered();


    void on_actionSelect_a_dataset_triggered();   // Slot for selecting a specific dataset
    void on_actionDrawMultipleCharts_2_triggered();   // Slot for drawing multiple charts
    void on_actionFunction_triggered();   // Slot for invoking the function dialog

private:
    Ui::ParentWindow *ui;
    QList<DataSet*> AllDataSets;  //List storing all datasets loaded in the application
    QMdiSubWindow* subWindow;   //Temporary variable for subwindows
    DataSet *AddedDataSet;   //Temporary variable for the most recently added dataset
    DataSetWindow *AddedDataSetWindow;   //Temporary variable for the most recently created dataset window
    FunctionDialog *funcDialog;   //Member variable for the FunctionDialog
};

#endif // PARENTWINDOW_H
