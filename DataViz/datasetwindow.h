#ifndef DATASETWINDOW_H
#define DATASETWINDOW_H

/********************************
 *
 *  This class is defined to display a dataset in a window.
 *  An object of this class represents the window where a dataset is shown to the user.
 *
 *  A dataset should only be visible to the user through a DataSetWindow object.
 *
**********************************/

#include <QDialog>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include "dataset.h"

namespace Ui {
class DataSetWindow;
}

class DataSetWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DataSetWindow(DataSet* DataSet, QWidget *parent = nullptr);
    ~DataSetWindow();

    void contextMenuEvent(QContextMenuEvent *event);  //Override for handling context menu events
    void ConstructContextMenu(QMenu *);    //Function to construct the context menu

public slots:

    void DataSetToBePlotted();   //Slot to handle the action to plot the dataset
    void onSaveButtonClicked();   //Slot for save button click action

signals:

    void Plot_XYPlot_SIGNAL(DataSet *ptr);   //Signal to notify parent window to plot the dataset

private:
    Ui::DataSetWindow *ui;


    DataSet *DisplayedDataSet;   //Reference to the dataset displayed in this window


    QAction* XYPlot = new QAction("XY Plot", this);   // Action for plotting XY graph

    // Context menu and its sub-menu for plotting
    QMenu *ContextMenu = new QMenu(this);
    QMenu *PlotSubMenu = new QMenu("Plot"); // Plot section
};

#endif // DATASETWINDOW_H
