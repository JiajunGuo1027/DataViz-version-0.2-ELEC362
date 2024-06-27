#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

/********************************
 *
 *  The GraphWindow class is responsible for displaying graphs in a dedicated window.
 *  An instance of this class represents a window where one or more graphs can be plotted.
 *
 *  Within this class, a "figure" refers to the overall plot frame, including axes, grid, title, and legend.
 *  A "graph" refers to the individual curve plotted within the figure. It includes properties like line width, style, and color.
 *
 **********************************/

#include <QDialog>
#include <QPen>
#include "dataset.h"
#include <QColorDialog>
#include <QMap>

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphWindow(DataSet *DataSet, QWidget *parent = nullptr);
    ~GraphWindow();

    void addDataSet(DataSet *dataSet);   // Adds a new dataset to the graph window

    bool hasDataSets() const { return !dataSets.isEmpty(); }   // Checks if the graph window contains any datasets


    void SetGraphSetting(DataSet *DataSet);   // Configures graph settings for a given dataset
    void SetFigureSetting();   // Sets up the overall figure settings (like legend, axes labels)

public slots:

    void selectColor();   //Slot function for color selection using QColorDialog

    // Slot functions for changing line style and width
    void changeLineStyle(int index);
    void changeLineWidth(int width);

private:

    void SetGraphSetting();  // Internal function to update graph settings
    void updateDataSetComboBox();   // Updates the dataset combo box with available datasets
    void plotAllDataSets();   // Plots all datasets added to the graph window

    Ui::GraphWindow *ui;
    QList<DataSet*> dataSets; // List to hold multiple datasets
    static int FigureCounter; // Counter for the number of figures created in the application

    QPen currentPen; // Current pen for graph line settings
    QMap<QString, QPen> dataSetPens; // Maps dataset names to their respective QPen settings
};

#endif // GRAPHWINDOW_H
