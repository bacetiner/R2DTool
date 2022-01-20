/* *****************************************************************************
Copyright (c) 2016-2021, The Regents of the University of California (Regents).
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS
PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

*************************************************************************** */

// Written by: Stevan Gavrilovic

#include "GISBuildingInputWidget.h"

#include "AssetInputDelegate.h"
#include "ComponentTableView.h"
#include "ComponentTableModel.h"
#include "ComponentDatabaseManager.h"
#include "CSVReaderWriter.h"

#include "QGISVisualizationWidget.h"

#include <QDir>
#include <QApplication>
#include <QLineEdit>
#include <QLabel>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>

#include <qgsfillsymbol.h>
#include <qgsmarkersymbol.h>
#include <qgslinesymbol.h>

// Test to remove
#include <chrono>
using namespace std::chrono;


GISBuildingInputWidget::GISBuildingInputWidget(QWidget *parent, VisualizationWidget* visWidget, QString componentType, QString appType) : ComponentInputWidget(parent, visWidget, componentType, appType)
{    

    this->setContentsMargins(0,0,0,0);

    QString txt1 = "Load buildings from a GIS file (.shp, .gdb., etc.)";
    this->setLabel1(txt1);

    QString txt2 = "Enter the IDs of one or more buildings to analyze."
                   "Define a range of buildings with a dash and separate multiple buildings with a comma.";
    this->setLabel2(txt2);

    QString txt3 = "Building Information";
    this->setLabel3(txt3);

    theComponentDb = ComponentDatabaseManager::getInstance()->getBuildingComponentDb();

    //    pathToComponentInputFile = "/Users/steve/Desktop/GalvestonTestbed/GalvestonGIS/GalvestonBuildings/galveston-bldg-v7.shp";
    //    componentFileLineEdit->setText(pathToComponentInputFile);
    //    this->loadComponentData();
}


GISBuildingInputWidget::~GISBuildingInputWidget()
{

}


int GISBuildingInputWidget::loadComponentVisualization()
{

    //    enum GeometryType
    //    {
    //      PointGeometry,
    //      LineGeometry,
    //      PolygonGeometry,
    //      UnknownGeometry,
    //      NullGeometry
    //    };
    // , "linestring", "polygon","multipoint","multilinestring","multipolygon"
    auto type = shapeFileLayer->geometryType();

    QString typeStr = "Null";
    if(type == QgsWkbTypes::PointGeometry)
        typeStr = "point";
    else if(type == QgsWkbTypes::LineGeometry)
        typeStr = "multilinestring";
    else if(type == QgsWkbTypes::PolygonGeometry)
        typeStr = "polygon";

    // Create the selected building layer
    auto selectedFeaturesLayer = theVisualizationWidget->addVectorLayer(typeStr,"Selected Buildings");

    if(selectedFeaturesLayer == nullptr)
    {
        this->errorMessage("Error adding the selected assets vector layer");
        return -1;
    }

    selectedFeaturesLayer->setCrs(shapeFileLayer->crs());

    QgsSymbol* selectFeatSymbol = nullptr;
    if(type == QgsWkbTypes::PointGeometry)
    {
        selectFeatSymbol = new QgsMarkerSymbol();
    }
    else if(type == QgsWkbTypes::LineGeometry)
    {
        selectFeatSymbol = new QgsLineSymbol();
    }
    else if(type == QgsWkbTypes::PolygonGeometry)
    {
        selectFeatSymbol = new QgsFillSymbol();
    }

    selectFeatSymbol->setColor(Qt::yellow);

    theVisualizationWidget->createSimpleRenderer(selectFeatSymbol,selectedFeaturesLayer);

    auto pr2 = selectedFeaturesLayer->dataProvider();

    auto fields = shapeFileLayer->dataProvider()->fields();

    auto res2 = pr2->addAttributes(fields.toList());

    if(!res2)
        this->errorMessage("Error adding attributes to the layer");

    selectedFeaturesLayer->updateFields(); // tell the vector layer to fetch changes from the provider

    QVector<QgsMapLayer*> mapLayers;
    mapLayers.push_back(selectedFeaturesLayer);
    mapLayers.push_back(shapeFileLayer);

    theVisualizationWidget->createLayerGroup(mapLayers,"Buildings");

    theComponentDb->setMainLayer(shapeFileLayer);
    theComponentDb->setSelectedLayer(selectedFeaturesLayer);

    return 0;
}


bool GISBuildingInputWidget::loadComponentData(void)
{
    // Ask for the file path if the file path has not yet been set, and return if it is still null
    if(pathToComponentInputFile.compare("NULL") == 0)
        this->chooseComponentInfoFileDialog();

    if(pathToComponentInputFile.compare("NULL") == 0)
        return false;

    // Check if the directory exists
    QFileInfo file(pathToComponentInputFile);

    if (!file.exists())
    {
        auto relPathToComponentFile = QCoreApplication::applicationDirPath() + QDir::separator() + pathToComponentInputFile;

        if (!QFile(relPathToComponentFile).exists())
        {
            QString errMsg = "Cannot find the file: "+ pathToComponentInputFile + "\n" +"Check your directory and try again.";
            this->errorMessage(errMsg);
            return false;
        }
        else
        {
            pathToComponentInputFile = relPathToComponentFile;
            componentFileLineEdit->setText(pathToComponentInputFile);
        }
    }

    // Name the layer according to the filename
    auto fName = file.fileName();

    shapeFileLayer = theVisualizationWidget->addVectorLayer(pathToComponentInputFile, fName, "ogr");

    auto numFeat = shapeFileLayer->featureCount();

    if(numFeat == 0)
    {
        this->errorMessage("Input Shapefile does not have any features");
        return false;
    }
    else{
        this->statusMessage("Loading information for " + QString::number(numFeat)+ " assets");
        QApplication::processEvents();
    }

    auto layerId = shapeFileLayer->id();
    theVisualizationWidget->registerLayerForSelection(layerId,this);

    auto features = shapeFileLayer->getFeatures();

    auto fields = shapeFileLayer->fields();

    if(fields.size() == 0)
    {
        this->errorMessage("Input Shapefile layer does not have any fields. Will not show table");
        return false;
    }

    QStringList fieldsStrList;
    for(int i = 0; i<fields.size(); ++i)
    {
        auto field = fields[i];
        auto fieldName = field.name();

        fieldsStrList.push_back(fieldName);
    }

    // Add the ID column to the headers
    //fieldsStrList.push_front("ID");

    tableHorizontalHeadings = fieldsStrList;

    // Data containing the table
    QVector<QStringList> data(numFeat);

    // Test to remove
    // auto start = high_resolution_clock::now();

    QgsFeature feat;
    int i = 0;
    while (features.nextFeature(feat))
    {
        //QStringList attributeStrList = {QString::number(i+1)};
        QStringList attributeStrList;

        auto attributes = feat.attributes();
        for(int i = 0; i<attributes.size(); ++i)
        {
            auto attribute = attributes[i];
            auto attributeStr = attribute.toString();

            if(attributeStr.compare("ID")==0)
                continue;

            attributeStrList.push_back(attributeStr);
        }

        data[i] = attributeStrList;

        ++i;
    }

    // Test to remove
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<milliseconds>(stop - start);
    //    this->statusMessage("Done shapefile results "+QString::number(duration.count()));

    componentTableWidget->clear();
    componentTableWidget->getTableModel()->populateData(data, tableHorizontalHeadings);

    label3->show();
    componentTableWidget->show();
    componentTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);

    this->loadComponentVisualization();

    // Offset is 0 since QGIS always numbers components starting at 1
    offset = 0;

    theComponentDb->setOffset(offset);

    this->statusMessage("Done loading assets");
    QApplication::processEvents();

    return true;
}


bool GISBuildingInputWidget::outputAppDataToJSON(QJsonObject &jsonObject)
{

    // First get the default information and then modify
    ComponentInputWidget::outputAppDataToJSON(jsonObject);

    // Here we will export everything to a .csv so we can use the CSV to BIM app
    jsonObject["Application"]="GIS_to_BIM";

    // It assumes the file name stays the same, but we need to modify the extension so that it is in csv and not .gdb, or .shp, or whatever
    QFileInfo componentFile(componentFileLineEdit->text());

    QString baseNameCSV = componentFile.baseName() + ".csv";

    auto appData = jsonObject.value("ApplicationData").toObject();

    if(appData.isEmpty())
        return false;

    appData["buildingSourceFile"] = baseNameCSV;

    appData["buildingGISFile"] = componentFile.fileName();

    jsonObject["ApplicationData"] = appData;

    return true;
}


bool GISBuildingInputWidget::inputAppDataFromJSON(QJsonObject &jsonObject)
{

    //jsonObject["Application"]=appType;
    if (jsonObject.contains("Application")) {
        if (appType != jsonObject["Application"].toString()) {
            this->errorMessage("GISBuildingInputWidget::inputFRommJSON app name conflict");
            return false;
        }
    }


    if (jsonObject.contains("ApplicationData")) {
        QJsonObject appData = jsonObject["ApplicationData"].toObject();

        QFileInfo fileInfo;
        QString fileName;
        QString pathToFile;
        bool foundFile = false;
        if (appData.contains("buildingGISFile"))
            fileName = appData["buildingGISFile"].toString();

        if (fileInfo.exists(fileName)) {

            selectComponentsLineEdit->setText(fileName);

            this->loadComponentData();
            foundFile = true;

        } else {

            if (appData.contains("pathToGIS"))
                pathToFile = appData["pathToGIS"].toString();
            else
                pathToFile=QDir::currentPath();

            pathToComponentInputFile = pathToFile + QDir::separator() + fileName;

            if (fileInfo.exists(pathToComponentInputFile)) {
                componentFileLineEdit->setText(pathToComponentInputFile);
                foundFile = true;
                this->loadComponentData();

            } else {
                // adam .. adam .. adam
                pathToComponentInputFile = pathToFile + QDir::separator()
                        + "input_data" + QDir::separator() + fileName;
                if (fileInfo.exists(pathToComponentInputFile)) {
                    componentFileLineEdit->setText(pathToComponentInputFile);
                    foundFile = true;
                    this->loadComponentData();
                }
                else
                {
                    QString errMessage = appType + " no file found at: " + fileName;
                    this->errorMessage(errMessage);
                    return false;
                }
            }
        }

        if(foundFile == false)
        {
            QString errMessage = appType + " no file found: " + fileName;
            this->errorMessage(errMessage);
            return false;
        }

        if (appData.contains("filter"))
            this->setFilterString(appData["filter"].toString());

    }

    return true;
}


bool GISBuildingInputWidget::copyFiles(QString &destName)
{
    auto compLineEditText = componentFileLineEdit->text();

    QFileInfo componentFile(compLineEditText);

    if (!componentFile.exists())
        return false;

    if (!QFile::copy(compLineEditText, destName + QDir::separator() + componentFile.fileName()))
        return false;

    auto fileName = componentFile.baseName();

    // Do not copy the file, output a new csv which will have the changes that the user makes in the table
    //        if (componentFile.exists()) {
    //            return this->copyFile(componentFileLineEdit->text(), destName);
    //        }

    auto pathToSaveFile = destName + QDir::separator() + fileName + ".csv";

    auto nRows = componentTableWidget->rowCount();

    if(nRows == 0)
        return false;

    auto data = componentTableWidget->getTableModel()->getTableData();

    auto headerValues = componentTableWidget->getTableModel()->getHeaderStringList();

    data.push_front(headerValues);

    CSVReaderWriter csvTool;

    QString err;
    csvTool.saveCSVFile(data,pathToSaveFile,err);

    if(!err.isEmpty())
        return false;


    // For testing, creates a csv file of only the selected components
    //    qDebug()<<"Saving selected components to .csv";
    //    auto selectedIDs = selectComponentsLineEdit->getSelectedComponentIDs();

    //    QVector<QStringList> selectedData(selectedIDs.size()+1);

    //    selectedData[0] = headerValues;

    //    auto nCols = componentTableWidget->columnCount();

    //    int i = 0;
    //    for(auto&& rowID : selectedIDs)
    //    {
    //        QStringList rowData;
    //        rowData.reserve(nCols);

    //        for(int j = 0; j<nCols; ++j)
    //        {
    //            auto item = componentTableWidget->item(rowID-1,j).toString();

    //            rowData<<item;
    //        }
    //        selectedData[i+1] = rowData;

    //        ++i;
    //    }

    //    csvTool.saveCSVFile(selectedData,"/Users/steve/Desktop/Selected.csv",err);
    // For testing end

    return true;
}
