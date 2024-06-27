#include "graphwindow.h"
#include "ui_graphwindow.h"

// Initialize the static variable to track the number of figures created
int GraphWindow::FigureCounter = 0;

// Constructor for GraphWindow. Initializes the UI and sets up graph settings
GraphWindow::GraphWindow(DataSet *DataSet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphWindow) {
    ui->setupUi(this); // Setup UI components

    FigureCounter++; // Increment the counter for each new figure

    // Set up graph settings for the provided dataset
    SetGraphSetting(DataSet);
    SetFigureSetting(); // Set up figure properties

    // Set the window title with the figure number
    this->setWindowTitle("Figure " + QString::number(FigureCounter));

    // Initialize default pen properties
    currentPen.setColor(Qt::blue);
    currentPen.setStyle(Qt::SolidLine);
    currentPen.setWidth(2);

    // Initialize the dataset combo box with available datasets
    updateDataSetComboBox();

    // Populate line style choices in the combo box
    ui->comboBoxLineStyle->addItem("Solid Line", QVariant(static_cast<int>(Qt::SolidLine)));
    ui->comboBoxLineStyle->addItem("Dash Line", QVariant(static_cast<int>(Qt::DashLine)));
    ui->comboBoxLineStyle->addItem("Dot Line", QVariant(static_cast<int>(Qt::DotLine)));
    ui->comboBoxLineStyle->addItem("Dash Dot Line", QVariant(static_cast<int>(Qt::DashDotLine)));
    ui->comboBoxLineStyle->addItem("Dash Dot Dot Line", QVariant(static_cast<int>(Qt::DashDotDotLine)));

    // Connect signals and slots for UI interactions
    connect(ui->comboBoxLineStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GraphWindow::changeLineStyle);
    connect(ui->spinBoxLineWidth, QOverload<int>::of(&QSpinBox::valueChanged), this, &GraphWindow::changeLineWidth);
    connect(ui->pushButtonSelectColor, &QPushButton::clicked, this, &GraphWindow::selectColor);
}

// Destructor for GraphWindow. Cleans up the UI
GraphWindow::~GraphWindow() {
    delete ui;
}

// Method to add a new dataset to the graph window
void GraphWindow::addDataSet(DataSet *dataSet) {
    if (dataSet && dataSet->IsDataSetValid) {
        dataSets.append(dataSet); // Add the valid dataset.
        updateDataSetComboBox(); // Update the combo box with the new dataset
        // Assign default pen for the new dataset.
        QPen defaultPen(Qt::blue, 2, Qt::SolidLine);
        dataSetPens[dataSet->getName()] = defaultPen;
        plotAllDataSets(); // Plot all datasets in the graph
    }
}

// Method to update the dataset combo box with current datasets
void GraphWindow::updateDataSetComboBox() {
    ui->comboBoxDataSets->clear();
    for (auto *dataSet : dataSets) {
        ui->comboBoxDataSets->addItem(dataSet->getName());
    }
}

// Method to set graph settings for a single dataset
void GraphWindow::SetGraphSetting(DataSet *DataSet) {
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->addData(DataSet);
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setName(DataSet->getName());
    ui->customPlot->graph(0)->rescaleAxes();
}

// Method to set the properties of the figure containing the graph
void GraphWindow::SetFigureSetting()
{ // Sets up the properties of the figure (that contains the curve)

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

}

// Slot for selecting color. Opens a color dialog and sets the selected color
void GraphWindow::selectColor() {
    QString selectedDataSetName = ui->comboBoxDataSets->currentText();
    QColor color = QColorDialog::getColor(dataSetPens[selectedDataSetName].color(), this, "Select Line Color");
    if (color.isValid()) {
        dataSetPens[selectedDataSetName].setColor(color);
        plotAllDataSets(); // Redraw the graph with new color settings
    }
}

// Slot for changing line style. Updates the pen style for the selected dataset
void GraphWindow::changeLineStyle(int index) {
    QString selectedDataSetName = ui->comboBoxDataSets->currentText();
    Qt::PenStyle style = static_cast<Qt::PenStyle>(ui->comboBoxLineStyle->itemData(index).toInt());
    dataSetPens[selectedDataSetName].setStyle(style);
    plotAllDataSets(); // Redraw the graph with new line style settings
}

// Slot for changing line width. Updates the pen width for the selected dataset
void GraphWindow::changeLineWidth(int width) {
    QString selectedDataSetName = ui->comboBoxDataSets->currentText();
    dataSetPens[selectedDataSetName].setWidth(width);
    plotAllDataSets(); // Redraw the graph with new line width settings
}

// Method to plot all datasets in the graph
void GraphWindow::plotAllDataSets() {
    ui->customPlot->clearGraphs(); // Clear existing graphs
    for (auto *dataSet : dataSets) {
        ui->customPlot->addGraph();
        int graphIndex = ui->customPlot->graphCount() - 1;
        ui->customPlot->graph(graphIndex)->addData(dataSet);
        ui->customPlot->graph(graphIndex)->setName(dataSet->getName());
        ui->customPlot->graph(graphIndex)->setPen(dataSetPens[dataSet->getName()]); // Set custom pen for each dataset
        ui->customPlot->graph(graphIndex)->rescaleAxes();
    }
    ui->customPlot->replot(); // Redraw the graph with all datasets
}
