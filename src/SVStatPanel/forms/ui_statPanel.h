/********************************************************************************
** Form generated from reading UI file 'statPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATPANEL_H
#define UI_STATPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "src/wdgGraphHist.h"

QT_BEGIN_NAMESPACE

class Ui_StatPanelClass
{
public:
    QVBoxLayout *verticalLayout;
    wdgGraphHist *wgtGraph;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout;
    QComboBox *cmbSignals;
    QLabel *label_6;
    QLineEdit *txtValue;
    QLabel *label_7;
    QLineEdit *txtHistValue;
    QSpacerItem *horizontalSpacer;
    QCheckBox *chbFront;
    QCheckBox *chbWinDiap;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_4;
    QLineEdit *txtCurrArea;
    QLineEdit *txtDiapMin;
    QLineEdit *txtDiapMax;
    QCheckBox *chbDiapEna;
    QLineEdit *txtMx;
    QLabel *label_5;
    QLineEdit *txtDurationMean;
    QLabel *label_3;
    QLineEdit *txtDurationFull;
    QLabel *label;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listSignTimeChange;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *StatPanelClass)
    {
        if (StatPanelClass->objectName().isEmpty())
            StatPanelClass->setObjectName(QStringLiteral("StatPanelClass"));
        StatPanelClass->resize(741, 464);
        StatPanelClass->setMaximumSize(QSize(900, 600));
        verticalLayout = new QVBoxLayout(StatPanelClass);
        verticalLayout->setSpacing(3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(3, 3, 3, 3);
        wgtGraph = new wdgGraphHist(StatPanelClass);
        wgtGraph->setObjectName(QStringLiteral("wgtGraph"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(wgtGraph->sizePolicy().hasHeightForWidth());
        wgtGraph->setSizePolicy(sizePolicy);
        wgtGraph->setMinimumSize(QSize(450, 200));

        verticalLayout->addWidget(wgtGraph);

        groupBox_3 = new QGroupBox(StatPanelClass);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        horizontalLayout = new QHBoxLayout(groupBox_3);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        cmbSignals = new QComboBox(groupBox_3);
        cmbSignals->setObjectName(QStringLiteral("cmbSignals"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cmbSignals->sizePolicy().hasHeightForWidth());
        cmbSignals->setSizePolicy(sizePolicy1);
        cmbSignals->setMinimumSize(QSize(150, 0));
        cmbSignals->setMaximumSize(QSize(200, 16777215));
        cmbSignals->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout->addWidget(cmbSignals);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout->addWidget(label_6);

        txtValue = new QLineEdit(groupBox_3);
        txtValue->setObjectName(QStringLiteral("txtValue"));
        txtValue->setMaximumSize(QSize(60, 16777215));

        horizontalLayout->addWidget(txtValue);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout->addWidget(label_7);

        txtHistValue = new QLineEdit(groupBox_3);
        txtHistValue->setObjectName(QStringLiteral("txtHistValue"));
        txtHistValue->setMaximumSize(QSize(60, 16777215));

        horizontalLayout->addWidget(txtHistValue);

        horizontalSpacer = new QSpacerItem(275, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        chbFront = new QCheckBox(groupBox_3);
        chbFront->setObjectName(QStringLiteral("chbFront"));

        horizontalLayout->addWidget(chbFront);

        chbWinDiap = new QCheckBox(groupBox_3);
        chbWinDiap->setObjectName(QStringLiteral("chbWinDiap"));
        chbWinDiap->setChecked(false);

        horizontalLayout->addWidget(chbWinDiap);


        verticalLayout->addWidget(groupBox_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(3);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        groupBox = new QGroupBox(StatPanelClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(315, 16777215));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        txtCurrArea = new QLineEdit(groupBox);
        txtCurrArea->setObjectName(QStringLiteral("txtCurrArea"));
        txtCurrArea->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(txtCurrArea, 2, 2, 1, 1);

        txtDiapMin = new QLineEdit(groupBox);
        txtDiapMin->setObjectName(QStringLiteral("txtDiapMin"));
        txtDiapMin->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(txtDiapMin, 1, 2, 1, 1);

        txtDiapMax = new QLineEdit(groupBox);
        txtDiapMax->setObjectName(QStringLiteral("txtDiapMax"));
        txtDiapMax->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(txtDiapMax, 1, 3, 1, 1);

        chbDiapEna = new QCheckBox(groupBox);
        chbDiapEna->setObjectName(QStringLiteral("chbDiapEna"));

        gridLayout->addWidget(chbDiapEna, 1, 5, 1, 1);

        txtMx = new QLineEdit(groupBox);
        txtMx->setObjectName(QStringLiteral("txtMx"));
        txtMx->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(txtMx, 2, 5, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 2, 3, 1, 1);

        txtDurationMean = new QLineEdit(groupBox);
        txtDurationMean->setObjectName(QStringLiteral("txtDurationMean"));
        txtDurationMean->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(txtDurationMean, 3, 5, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 3, 3, 1, 1);

        txtDurationFull = new QLineEdit(groupBox);
        txtDurationFull->setObjectName(QStringLiteral("txtDurationFull"));
        txtDurationFull->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(txtDurationFull, 3, 2, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 3, 0, 1, 1);


        horizontalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(StatPanelClass);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        listSignTimeChange = new QListWidget(groupBox_2);
        listSignTimeChange->setObjectName(QStringLiteral("listSignTimeChange"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(listSignTimeChange->sizePolicy().hasHeightForWidth());
        listSignTimeChange->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(listSignTimeChange);


        horizontalLayout_3->addWidget(groupBox_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(StatPanelClass);

        QMetaObject::connectSlotsByName(StatPanelClass);
    } // setupUi

    void retranslateUi(QDialog *StatPanelClass)
    {
        StatPanelClass->setWindowTitle(QApplication::translate("StatPanelClass", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", 0));
        groupBox_3->setTitle(QString());
        label_6->setText(QApplication::translate("StatPanelClass", "\320\227\320\275\320\260\321\207\320\265\320\275\320\270\320\265", 0));
        txtValue->setText(QString());
        label_7->setText(QApplication::translate("StatPanelClass", "\320\232\320\276\320\273-\320\262\320\276", 0));
        txtHistValue->setText(QString());
        chbFront->setText(QApplication::translate("StatPanelClass", "\320\244\321\200\320\276\320\275\321\202", 0));
        chbWinDiap->setText(QApplication::translate("StatPanelClass", "\320\236\320\272\320\275\320\276", 0));
        groupBox->setTitle(QString());
        label_2->setText(QApplication::translate("StatPanelClass", "\320\237\320\273\320\276\321\211\320\260\320\264\321\214", 0));
        label_4->setText(QApplication::translate("StatPanelClass", "\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275", 0));
        txtDiapMin->setText(QApplication::translate("StatPanelClass", "0", 0));
        txtDiapMax->setText(QApplication::translate("StatPanelClass", "255", 0));
        chbDiapEna->setText(QString());
        label_5->setText(QApplication::translate("StatPanelClass", "\320\241\321\200 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265", 0));
        label_3->setText(QApplication::translate("StatPanelClass", "\320\241\321\200 \320\264\320\273\320\270\321\202, \320\274\321\201", 0));
        label->setText(QApplication::translate("StatPanelClass", "\320\237\320\276\320\273\320\275 \320\264\320\273\320\270\321\202, \321\201", 0));
        groupBox_2->setTitle(QApplication::translate("StatPanelClass", "\320\222\321\200\320\265\320\274\321\217 \320\270\320\267\320\274\320\265\320\275\320\265\320\275\320\270\321\217", 0));
    } // retranslateUi

};

namespace Ui {
    class StatPanelClass: public Ui_StatPanelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATPANEL_H
