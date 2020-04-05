/********************************************************************************
** Form generated from reading UI file 'signScriptPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNSCRIPTPANEL_H
#define UI_SIGNSCRIPTPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_signScriptPanelClass
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lbTop;
    QTextEdit *txtScript;
    QHBoxLayout *horizontalLayout;
    QLabel *lbBottom;
    QLineEdit *txtNewSName;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lbChange;
    QPushButton *btnSave;
    QPushButton *btnActive;

    void setupUi(QDialog *signScriptPanelClass)
    {
        if (signScriptPanelClass->objectName().isEmpty())
            signScriptPanelClass->setObjectName(QStringLiteral("signScriptPanelClass"));
        signScriptPanelClass->resize(716, 433);
        verticalLayout = new QVBoxLayout(signScriptPanelClass);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lbTop = new QLabel(signScriptPanelClass);
        lbTop->setObjectName(QStringLiteral("lbTop"));

        verticalLayout->addWidget(lbTop);

        txtScript = new QTextEdit(signScriptPanelClass);
        txtScript->setObjectName(QStringLiteral("txtScript"));

        verticalLayout->addWidget(txtScript);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lbBottom = new QLabel(signScriptPanelClass);
        lbBottom->setObjectName(QStringLiteral("lbBottom"));
        lbBottom->setTextFormat(Qt::AutoText);

        horizontalLayout->addWidget(lbBottom);

        txtNewSName = new QLineEdit(signScriptPanelClass);
        txtNewSName->setObjectName(QStringLiteral("txtNewSName"));
        txtNewSName->setMaximumSize(QSize(150, 16777215));

        horizontalLayout->addWidget(txtNewSName);

        label_5 = new QLabel(signScriptPanelClass);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setTextFormat(Qt::AutoText);

        horizontalLayout->addWidget(label_5);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        lbChange = new QLabel(signScriptPanelClass);
        lbChange->setObjectName(QStringLiteral("lbChange"));

        horizontalLayout_2->addWidget(lbChange);

        btnSave = new QPushButton(signScriptPanelClass);
        btnSave->setObjectName(QStringLiteral("btnSave"));

        horizontalLayout_2->addWidget(btnSave);

        btnActive = new QPushButton(signScriptPanelClass);
        btnActive->setObjectName(QStringLiteral("btnActive"));

        horizontalLayout_2->addWidget(btnActive);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(signScriptPanelClass);

        QMetaObject::connectSlotsByName(signScriptPanelClass);
    } // setupUi

    void retranslateUi(QDialog *signScriptPanelClass)
    {
        signScriptPanelClass->setWindowTitle(QApplication::translate("signScriptPanelClass", "\320\241\320\272\321\200\320\270\320\277\321\202 \320\264\320\273\321\217 \321\201\320\270\320\263\320\275\320\260\320\273\320\260", 0));
        lbTop->setText(QApplication::translate("signScriptPanelClass", "ctm = getTimeValue(\"client\", \"sin\"); \n"
"cval = getIntValue(\"client\", \"sin\"); ", 0));
        lbBottom->setText(QApplication::translate("signScriptPanelClass", "setIntValue(\"", 0));
        label_5->setText(QApplication::translate("signScriptPanelClass", "\", nval, ctm + offs);", 0));
        lbChange->setText(QString());
        btnSave->setText(QApplication::translate("signScriptPanelClass", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0));
        btnActive->setText(QApplication::translate("signScriptPanelClass", "\320\222\320\272\320\273\321\216\321\207\320\270\321\202\321\214", 0));
    } // retranslateUi

};

namespace Ui {
    class signScriptPanelClass: public Ui_signScriptPanelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNSCRIPTPANEL_H
