#ifndef RECORDSELECTIONWIDGET_H
#define RECORDSELECTIONWIDGET_H
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

#include "RecordSelectionConfig.h"
#include "JsonSerializable.h"

#include <QWidget>

class QLineEdit;
class QLabel;

class SC_ComboBox;
class SC_DoubleLineEdit;
class SC_IntLineEdit;

class RecordSelectionWidget : public QWidget, JsonSerializable
{
    Q_OBJECT
public:
    explicit RecordSelectionWidget(RecordSelectionConfig& selectionConfig, QWidget *parent = nullptr);

    int getNumberOfGMPerSite(void);

    bool outputToJSON(QJsonObject& obj);
    bool inputFromJSON(QJsonObject& obj);
    void reset(void);

private slots:

    void handleDBSelection(const QString& selection);

private:
    void setScalingVisibility(bool val);

    RecordSelectionConfig& m_selectionConfig;

    SC_IntLineEdit* numGMLineEdit = nullptr;
    SC_DoubleLineEdit* scalingMin = nullptr;
    SC_DoubleLineEdit* scalingMax = nullptr;
    SC_ComboBox* m_dbBox = nullptr;

    QLabel* scalingMinLabel = nullptr;
    QLabel* scalingMaxLabel = nullptr;

};

#endif // RECORDSELECTIONWIDGET_H
