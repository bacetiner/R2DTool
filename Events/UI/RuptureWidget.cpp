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
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
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

#include "RuptureWidget.h"
#include "UCERF2Widget.h"
#include "MeanUCERFWidget.h"

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QGroupBox>
#include <QComboBox>

RuptureWidget::RuptureWidget(QWidget *parent) : SimCenterAppWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    ruptureSelectionCombo = new QComboBox();
    mainStackedWidget = new QStackedWidget();
    mainStackedWidget->setContentsMargins(0,0,0,0);

    // Connect the combo box signal to the stacked widget slot
    QObject::connect(ruptureSelectionCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     mainStackedWidget, &QStackedWidget::setCurrentIndex);

    ucerfWidget = new UCERF2Widget();
    meanUcerfWidget = new MeanUCERFWidget();

    ruptureSelectionCombo->addItem("WGCEP (2007) UCERF2 - Single Branch");
    ruptureSelectionCombo->addItem("Mean UCERF3");

    mainStackedWidget->addWidget(ucerfWidget);
    mainStackedWidget->addWidget(meanUcerfWidget);

    layout->addWidget(ruptureSelectionCombo);
    layout->addWidget(mainStackedWidget);

}


bool RuptureWidget::outputToJSON(QJsonObject &jsonObject)
{
    ucerfWidget->outputToJSON(jsonObject);
}


bool RuptureWidget::inputFromJSON(QJsonObject &/*jsonObject*/)
{
    return true;
}


//QString RuptureWidget::getEQNum() const
//{
//    QString numEQ;
//    if (widgetType.compare("Hazard Occurrence")==0) {
//        numEQ = hoWidget->getRuptureSource()->getCandidateEQ();
//    } else {
//        //KZ: update the scenario number for OpenSHA ERF
//        //numEQ = "1";
//        if (widgetType.compare("OpenSHA ERF")==0) {
//            numEQ = erfWidget->getNumScen();
//        } else {
//            numEQ = "1";
//        }
//    }
//    return numEQ;
//}


//QString RuptureWidget::getGMPELogicTree() const
//{
//    QString gmpeLT = "";
//    if (widgetType.compare("OpenQuake Classical")==0)
//    {
//        gmpeLT = oqcpWidget->getRuptureSource()->getGMPEFilename();
//    }


//    return gmpeLT;
//}
