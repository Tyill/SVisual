/********************************************************************************
** Form generated from reading UI file 'graphSetting.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHSETTING_H
#define UI_GRAPHSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_graphSettingClass
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSlider *slrColorTransparent;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QSlider *slrPenWidth;
    QCheckBox *chbDarkTheme;

    void setupUi(QDialog *graphSettingClass)
    {
        if (graphSettingClass->objectName().isEmpty())
            graphSettingClass->setObjectName(QStringLiteral("graphSettingClass"));
        graphSettingClass->resize(400, 235);
        graphSettingClass->setMaximumSize(QSize(400, 235));
        verticalLayout_3 = new QVBoxLayout(graphSettingClass);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(graphSettingClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 5, -1, 5);
        slrColorTransparent = new QSlider(groupBox);
        slrColorTransparent->setObjectName(QStringLiteral("slrColorTransparent"));
        slrColorTransparent->setMaximum(255);
        slrColorTransparent->setOrientation(Qt::Horizontal);
        slrColorTransparent->setTickPosition(QSlider::TicksAbove);

        horizontalLayout->addWidget(slrColorTransparent);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(graphSettingClass);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 5, -1, 5);
        slrPenWidth = new QSlider(groupBox_2);
        slrPenWidth->setObjectName(QStringLiteral("slrPenWidth"));
        slrPenWidth->setMinimum(1);
        slrPenWidth->setMaximum(5);
        slrPenWidth->setOrientation(Qt::Horizontal);
        slrPenWidth->setTickPosition(QSlider::TicksAbove);

        horizontalLayout_2->addWidget(slrPenWidth);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(groupBox_2);

        chbDarkTheme = new QCheckBox(graphSettingClass);
        chbDarkTheme->setObjectName(QStringLiteral("chbDarkTheme"));

        verticalLayout_3->addWidget(chbDarkTheme);


        retranslateUi(graphSettingClass);

        QMetaObject::connectSlotsByName(graphSettingClass);
    } // setupUi

    void retranslateUi(QDialog *graphSettingClass)
    {
        graphSettingClass->setWindowTitle(QApplication::translate("graphSettingClass", "\320\222\320\270\320\264 \320\263\321\200\320\260\321\204\320\270\320\272\320\260", 0));
        groupBox->setTitle(QApplication::translate("graphSettingClass", "\320\237\321\200\320\276\320\267\321\200\320\260\321\207\320\275\320\276\321\201\321\202\321\214", 0));
        groupBox_2->setTitle(QApplication::translate("graphSettingClass", "\320\242\320\276\320\273\321\211\320\270\320\275\320\260 \320\273\320\270\320\275\320\270\320\270", 0));
        chbDarkTheme->setText(QApplication::translate("graphSettingClass", "\320\247\320\265\321\200\320\275\321\213\320\271 \321\204\320\276\320\275", 0));
    } // retranslateUi

};

namespace Ui {
    class graphSettingClass: public Ui_graphSettingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHSETTING_H
