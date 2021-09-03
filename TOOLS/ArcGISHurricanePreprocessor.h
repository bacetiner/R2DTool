#ifndef ArcGISHurricanePreprocessor_H
#define ArcGISHurricanePreprocessor_H
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

#include "HurricaneObject.h"

#include <QString>
#include <QStringList>
#include <QVector>
#include <QVariant>

class ArcGISVisualizationWidget;
class LayerTreeItem;

class QObject;
class QProgressBar;

namespace Esri
{
namespace ArcGISRuntime
{
class Layer;
class GroupLayer;
class Geometry;
}
}

class ArcGISHurricanePreprocessor
{
public:
    ArcGISHurricanePreprocessor(QProgressBar* pBar, ArcGISVisualizationWidget* visWidget, QObject* parent);

    int loadHurricaneTrackData(const QString &eventFile, QString &err);

    void clear(void);

    // Gets the hurricane of the given storm id
    HurricaneObject* getHurricane(const QString& SID);

    Esri::ArcGISRuntime::Layer *getAllHurricanesLayer() const;

    // Creates a hurricane visualization of the track and track points if desired
    LayerTreeItem* createTrackVisualization(HurricaneObject* hurricane, LayerTreeItem* parentItem, Esri::ArcGISRuntime::GroupLayer* parentLayer, QString& err);

    // Note that including track points may take a long time, moreover not all hurricanes have a landfall
    LayerTreeItem*  createTrackPointsVisualization(HurricaneObject* hurricane, LayerTreeItem* parentItem, Esri::ArcGISRuntime::GroupLayer* parentLayer, QString& err);

    LayerTreeItem* createLandfallVisualization(const double latitude,
                                               const double longitude,
                                               const QMap<QString, QVariant>& featureAttributes,
                                               LayerTreeItem* parentItem,
                                               Esri::ArcGISRuntime::GroupLayer* parentLayer);

private:

    Esri::ArcGISRuntime::Geometry getTrackGeometry(HurricaneObject* hurricane, QString& err);
    Esri::ArcGISRuntime::Layer* allHurricanesLayer;
    QProgressBar* theProgressBar;
    ArcGISVisualizationWidget* theVisualizationWidget;
    QObject* theParent;
    QVector<HurricaneObject> hurricanes;
};

#endif // ArcGISHurricanePreprocessor_H
