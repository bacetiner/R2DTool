#include "HazardsWidget.h"
#include "VisualizationWidget.h"
#include "UserInputGMWidget.h"
#include "GMWidget.h"
#include "ShakeMapWidget.h"
#include "WorkflowAppRDT.h"

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QStackedWidget>
#include <QCheckBox>
#include <QJsonObject>
#include <QJsonArray>

HazardsWidget::HazardsWidget(QWidget *parent, VisualizationWidget* visWidget, RandomVariablesContainer * RVContainer) : SimCenterAppWidget(parent), theRandomVariablesContainer(RVContainer), theVisualizationWidget(visWidget)
{
    theRootStackedWidget = nullptr;
    theShakeMapWidget = nullptr;
    theEQSSWidget = nullptr;
    theUserInputGMWidget = nullptr;
    hazardSelectionCombo = nullptr;

    this->createWidget();
}


HazardsWidget::~HazardsWidget()
{

}


bool HazardsWidget::outputToJSON(QJsonObject &jsonObj)
{

    QJsonArray arrayEvents;
    QJsonObject EQObj;

    EQObj.insert("EventClassification", "Earthquake");
    EQObj.insert("Application", "SimCenterEvent");

    QJsonObject appDataObj;

    auto currentSelection = hazardSelectionCombo->currentText();

    if(currentSelection.compare("Earthquake Scenario Simulation") == 0)
    {
        theEQSSWidget->outputToJSON(appDataObj);
    }
    else if(currentSelection.compare("ShakeMap Input") == 0)
    {
        theShakeMapWidget->outputToJSON(appDataObj);
    }
    else if(currentSelection.compare("User Specified Ground Motions") == 0)
    {
        theUserInputGMWidget->outputToJSON(appDataObj);
    }
    else
    {
        qDebug()<<"Warning, could not recognize the earthquake combobox selection of"<<hazardSelectionCombo->currentText();
    }

    EQObj.insert("ApplicationData",appDataObj);

    arrayEvents.append(EQObj);

    jsonObj.insert("Events",arrayEvents);

    return true;
}


bool HazardsWidget::inputFromJSON(QJsonObject &jsonObject)
{
    return true;
}


void HazardsWidget::createWidget(void)
{
    QGridLayout* gridLayout = new QGridLayout(this);

    auto smallVSpacer = new QSpacerItem(0,10);

    QLabel* selectionText = new QLabel();
    selectionText->setText("Hazard Type:");

    hazardSelectionCombo = new QComboBox();
    hazardSelectionCombo->addItem("Earthquake Scenario Simulation");
    hazardSelectionCombo->addItem("Earthquake ShakeMap");
    hazardSelectionCombo->addItem("User Specified Ground Motions");
    hazardSelectionCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    connect(hazardSelectionCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(handleEQTypeSelection(QString)));

    theRootStackedWidget = new QStackedWidget();

    // Add a vertical spacer at the bottom to push everything up
    auto vspacer = new QSpacerItem(0,0,QSizePolicy::Minimum, QSizePolicy::Expanding);
    auto hspacer = new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(smallVSpacer,0,0);
    gridLayout->addWidget(selectionText,1,0);
    gridLayout->addWidget(hazardSelectionCombo,1,1);
    gridLayout->addItem(hspacer,1,3);
    gridLayout->addWidget(theRootStackedWidget,2,0,1,3);
    gridLayout->addItem(vspacer, 3, 0,1,3);

    theEQSSWidget = new GMWidget(this, theVisualizationWidget);
    theShakeMapWidget = new ShakeMapWidget(theVisualizationWidget);
    theUserInputGMWidget = new UserInputGMWidget(theVisualizationWidget);

    connect(theShakeMapWidget, &ShakeMapWidget::loadingComplete, this, &HazardsWidget::shakeMapLoadingFinished);
    connect(theEQSSWidget, SIGNAL(outputDirectoryPathChanged(QString)), this,  SLOT(gridFileChangedSlot(QString)));
    connect(theUserInputGMWidget, SIGNAL(outputDirectoryPathChanged(QString)), this,  SLOT(gridFileChangedSlot(QString)));

    theRootStackedWidget->addWidget(theEQSSWidget);
    theRootStackedWidget->addWidget(theShakeMapWidget->getShakeMapWidget());
    theRootStackedWidget->addWidget(theUserInputGMWidget->getUserInputGMWidget());

    theRootStackedWidget->setCurrentWidget(theEQSSWidget);

    hazardSelectionCombo->setCurrentText("Earthquake Scenario Simulation");
}


void HazardsWidget::shakeMapLoadingFinished(const bool value)
{
    if(!value)
        return;

    // Shift the focus to the visualization widget
    auto mainWindowWidget = qobject_cast<WorkflowAppRDT*>(this->parent());

    if(!mainWindowWidget)
        return;

//    mainWindowWidget->setActiveWidget(theVisualizationWidget);

}


void HazardsWidget::handleEQTypeSelection(const QString& selection)
{
    if(selection == "Earthquake Scenario Simulation")
        theRootStackedWidget->setCurrentWidget(theEQSSWidget);
    else if(selection == "Earthquake ShakeMap")
        theRootStackedWidget->setCurrentWidget(theShakeMapWidget->getShakeMapWidget());
    else if(selection == "User Specified Ground Motions")
        theRootStackedWidget->setCurrentWidget(theUserInputGMWidget->getUserInputGMWidget());
}


void HazardsWidget::gridFileChangedSlot(QString newPath)
{
    qDebug() << "HazardsWidget = SLOT " << newPath;
    emit gridFileChangedSignal(newPath);
}

