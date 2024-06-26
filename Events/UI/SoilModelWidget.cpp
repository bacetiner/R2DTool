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

// Written by: Kuanshi Zhong

#include "SoilModelWidget.h"

#include <QHBoxLayout>
#include <QGroupBox>
#include <QStringListModel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

SoilModelWidget::SoilModelWidget(SoilModel& soilModel, SiteConfig& siteConfig, QWidget *parent): QWidget(parent), m_soilModel(soilModel)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QGroupBox* soilModelGroupBox = new QGroupBox(this);
    soilModelGroupBox->setTitle("Soil Model Type (if not defined by users)");

    QHBoxLayout* formLayout = new QHBoxLayout(soilModelGroupBox);
    m_typeBox = new QComboBox(this);

    m_typeBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);

    QLabel* typeLabel = new QLabel(tr("Type:"),this);

    formLayout->addWidget(typeLabel);
    formLayout->addWidget(m_typeBox);
//    formLayout->addStretch(1);

    soilModelGroupBox->setLayout(formLayout);

    // user model box
    m_userModelBox = new QGroupBox(this);
    m_userModelBox->setTitle("User-Defined Soil Model");

    QHBoxLayout* userFormLayout = new QHBoxLayout(m_userModelBox);
    QLabel* userTypeLabel = new QLabel(tr("User model script:"),this);

    m_userModelFile = new QLineEdit("");
    m_userModelButton = new QPushButton(tr("Browser"));

    userFormLayout->addWidget(userTypeLabel);
    userFormLayout->addWidget(m_userModelFile);
    userFormLayout->addWidget(m_userModelButton);
    m_userModelBox->setVisible(false);

    //
    layout->addWidget(soilModelGroupBox);
    layout->addWidget(m_userModelBox);
    this->setLayout(layout);

    QStringList validType;
    validType = this->m_soilModel.validTypes();

    QStringListModel* typeModel = new QStringListModel(validType);
    m_typeBox->setModel(typeModel);
    m_typeBox->setCurrentIndex(validType.indexOf(m_soilModel.type()));
    this->setupConnections();
}


void SoilModelWidget::setupConnections()
{
    connect(this->m_typeBox, &QComboBox::currentTextChanged,
            &this->m_soilModel, &SoilModel::setType);

    connect(&this->m_soilModel, &SoilModel::typeChanged,
            this->m_typeBox, &QComboBox::setCurrentText);

    connect(&this->m_soilModel, &SoilModel::typeChanged, this, [this](QString type) {
        if (type.compare("User")==0)
            m_userModelBox->setVisible(true);
        else
        {
            m_userModelBox->setVisible(false);
            m_userModelFile->setText("");
        }
    });

    connect(m_userModelButton, &QPushButton::clicked,
            this, &SoilModelWidget::loadUserModelFile);

    connect(this, SIGNAL(userModelFileChanged(QString)), &this->m_soilModel, SLOT(setUserModelPath(QString)));
}

void SoilModelWidget::loadUserModelFile()
{
    userModelFilePath=QFileDialog::getOpenFileName(this,tr("Model File (.py)"));
    if(userModelFilePath.isEmpty())
    {
        userModelFilePath = "NULL";
        return;
    }
    this->setModelFile(userModelFilePath);

    //Connecting the filename
    if (userModelFilePath.compare("Null") == 0 && userModelFilePath.contains(".py", Qt::CaseInsensitive))
    {
        QString errMsg = "Please choose a Model File (.py)";
        qDebug() << errMsg;
        return;
    }
}

void SoilModelWidget::setModelFile(QString dirPath)
{
    m_userModelFile->setText(dirPath);
    emit userModelFileChanged(dirPath);
    return;
}

QString SoilModelWidget::getModelPathFile()
{
    return m_userModelFile->text();
}
