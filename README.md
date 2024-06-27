# DataViz Project - Version 0.2

## Project Overview
This project aims to develop DataViz, a software for plotting and processing datasets. The goal is to enhance DataViz from version 0.0 to version 0.2 with several new features and improvements.

## Features Implemented
- **Curve Customization**: Users can change the width, color, and style of curves.
- **Help Window Redesign**: The help window is now organized into sections for easier navigation and future expansion.
- **Dataset Comments**: Users can add comments to describe each dataset.
- **Multiple Datasets in Single Graph**: Allows plotting multiple datasets in one graph window.
- **Mathematical Function Input**: Users can input mathematical expressions involving dataset names for analysis.
- **Histogram Plot**: Added a feature for plotting histograms using GSL and QCustomPlot.

## Technical Details
- **Language**: C++
- **Frameworks**: Qt for GUI, QCustomPlot for plotting, GSL for statistical computations.
- **Development Environment**: Personal computers/laptops, university remote computers.

## Core Components and Code

### Header Files
1. **dataset.h**: Defines the `DataSet` class to handle datasets, store them in GSL matrices, and manage dataset-related functionalities like saving and loading comments.
2. **datasetwindow.h**: Defines the `DataSetWindow` class to display datasets in a window, allowing users to interact with and plot datasets.
3. **functiondialog.h**: Defines the `FunctionDialog` class for a dialog window where users can input mathematical expressions for dataset analysis.
4. **graphwindow.h**: Defines the `GraphWindow` class to display graphs in a dedicated window, allowing multiple datasets to be plotted and customized.
5. **parentwindow.h**: Defines the `ParentWindow` class, which serves as the main application window managing datasets and graph windows.

### Source Files
1. **dataset.cpp**: Implements the `DataSet` class methods, including data loading, comment handling, and validation.
2. **datasetwindow.cpp**: Implements the `DataSetWindow` class methods, including the context menu for plotting datasets and handling user interactions.
3. **functiondialog.cpp**: Implements the `FunctionDialog` class methods for validating and retrieving user-inputted mathematical expressions.
4. **graphwindow.cpp**: Implements the `GraphWindow` class methods for plotting datasets, customizing graph appearances, and managing multiple datasets.
5. **parentwindow.cpp**: Implements the `ParentWindow` class methods for loading datasets, displaying help and about dialogs, and managing multiple graph windows.

## Instructions for Use
1. **Curve Customization**: Access through the properties menu to adjust curve attributes.
2. **Help Window**: Navigate via the help menu for detailed instructions and FAQs.
3. **Dataset Comments**: Add comments by right-clicking on the dataset name in the data manager.
4. **Multiple Datasets**: Use the plotting menu to select and plot multiple datasets.
5. **Function Input**: Enter mathematical expressions in the analysis menu under the function option.
6. **Histogram Plot**: Generate and view histograms through the histogram option in the plot menu.

## Error Handling
- Comprehensive error handling for runtime and user input errors.
- Meaningful error messages to guide users in troubleshooting issues.
