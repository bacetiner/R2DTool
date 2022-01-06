#ifndef RegionalSiteResponseWidget_H
#define RegionalSiteResponseWidget_H
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

// Written by: Stevan Gavrilovic, Frank McKenna

#include "GroundMotionStation.h"
#include "SimCenterAppWidget.h"

#include <memory>

#include <QMap>

class VisualizationWidget;
class SimCenterUnitsWidget;

class ComponentInputWidget;
class QStackedWidget;
class QLineEdit;
class QProgressBar;
class QLabel;

namespace Esri
{
namespace ArcGISRuntime
{
class ArcGISMapImageLayer;
class GroupLayer;
class FeatureCollectionLayer;
class KmlLayer;
class Layer;
}
}


class RegionalSiteResponseWidget : public SimCenterAppWidget
{
    Q_OBJECT

public:
    RegionalSiteResponseWidget(VisualizationWidget* visWidget, QWidget *parent = nullptr);
    ~RegionalSiteResponseWidget();

    void showUserGMLayers(bool state);

    QStackedWidget* getRegionalSiteResponseWidget(void);

    bool inputFromJSON(QJsonObject &jsonObj);  
    bool outputToJSON(QJsonObject &jsonObj);
    bool inputAppDataFromJSON(QJsonObject &jsonObj);
    bool outputAppDataToJSON(QJsonObject &jsonObj);
  
    bool copyFiles(QString &destDir);

    void clear(void);

public slots:

    void showUserGMSelectDialog(void);

private slots:

    void loadUserGMData(void);
    void chooseEventFileDialog(void);
    void chooseMotionDirDialog(void);
    void soilParamaterFileDialog(void);
    void soilScriptFileDialog(void);


signals:
    void outputDirectoryPathChanged(QString motionDir, QString eventFile);
    void loadingComplete(const bool value);

private:

  void showProgressBar(void);
  void hideProgressBar(void);
  void setFilterString(const QString& filter);
  QString getFilterString(void);
  
  QStackedWidget* theStackedWidget;

    VisualizationWidget* theVisualizationWidget;

    QString eventFile;
    QString motionDir;

    QLineEdit *eventFileLineEdit;
    QLineEdit *motionDirLineEdit;
    QLineEdit *soilFileLineEdit;
    QLineEdit *siteResponseScriptLineEdit;
    QLineEdit *filterLineEdit;

    QLabel* progressLabel;
    QWidget* progressBarWidget;
    QWidget* inputWidget;
    QProgressBar* progressBar;

    QVector<GroundMotionStation> stationList;

    SimCenterUnitsWidget* unitsWidget;
  
    ComponentInputWidget *theInputMotions;
};

#endif // RegionalSiteResponseWidget_H