/********************************************************************************
** Form generated from reading UI file 'settingsPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSPANEL_H
#define UI_SETTINGSPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_settingsPanelClass
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *rbtnConnectByEthernet;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QLineEdit *txtAddr;
    QLabel *label_5;
    QLineEdit *txtPort;
    QHBoxLayout *horizontalLayout;
    QRadioButton *rbtnConnectByCom;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_2;
    QComboBox *cmbComPort;
    QLabel *label_6;
    QLineEdit *txtComSpeed;
    QGroupBox *grbSaveData;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *rbtnCopyEna;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QPushButton *btnCopyPath;
    QLineEdit *txtCopyPath;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lbChange;
    QPushButton *btnSave;

    void setupUi(QDialog *settingsPanelClass)
    {
        if (settingsPanelClass->objectName().isEmpty())
            settingsPanelClass->setObjectName(QStringLiteral("settingsPanelClass"));
        settingsPanelClass->resize(512, 260);
        settingsPanelClass->setMaximumSize(QSize(512, 260));
        verticalLayout_3 = new QVBoxLayout(settingsPanelClass);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(settingsPanelClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        rbtnConnectByEthernet = new QRadioButton(groupBox);
        rbtnConnectByEthernet->setObjectName(QStringLiteral("rbtnConnectByEthernet"));

        horizontalLayout_3->addWidget(rbtnConnectByEthernet);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(50, 0));

        horizontalLayout_3->addWidget(label);

        txtAddr = new QLineEdit(groupBox);
        txtAddr->setObjectName(QStringLiteral("txtAddr"));
        txtAddr->setMinimumSize(QSize(100, 0));

        horizontalLayout_3->addWidget(txtAddr);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setMinimumSize(QSize(55, 0));

        horizontalLayout_3->addWidget(label_5);

        txtPort = new QLineEdit(groupBox);
        txtPort->setObjectName(QStringLiteral("txtPort"));
        txtPort->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_3->addWidget(txtPort);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        rbtnConnectByCom = new QRadioButton(groupBox);
        rbtnConnectByCom->setObjectName(QStringLiteral("rbtnConnectByCom"));
        rbtnConnectByCom->setMinimumSize(QSize(166, 0));
        rbtnConnectByCom->setChecked(true);

        horizontalLayout->addWidget(rbtnConnectByCom);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(50, 0));

        horizontalLayout->addWidget(label_2);

        cmbComPort = new QComboBox(groupBox);
        cmbComPort->setObjectName(QStringLiteral("cmbComPort"));
        cmbComPort->setMinimumSize(QSize(100, 0));

        horizontalLayout->addWidget(cmbComPort);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMinimumSize(QSize(55, 0));

        horizontalLayout->addWidget(label_6);

        txtComSpeed = new QLineEdit(groupBox);
        txtComSpeed->setObjectName(QStringLiteral("txtComSpeed"));
        txtComSpeed->setMaximumSize(QSize(70, 16777215));

        horizontalLayout->addWidget(txtComSpeed);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_3->addWidget(groupBox);

        grbSaveData = new QGroupBox(settingsPanelClass);
        grbSaveData->setObjectName(QStringLiteral("grbSaveData"));
        verticalLayout_2 = new QVBoxLayout(grbSaveData);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        rbtnCopyEna = new QRadioButton(grbSaveData);
        rbtnCopyEna->setObjectName(QStringLiteral("rbtnCopyEna"));
        rbtnCopyEna->setEnabled(true);
        rbtnCopyEna->setLayoutDirection(Qt::LeftToRight);

        verticalLayout_2->addWidget(rbtnCopyEna);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_3 = new QLabel(grbSaveData);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setEnabled(true);

        horizontalLayout_2->addWidget(label_3);

        btnCopyPath = new QPushButton(grbSaveData);
        btnCopyPath->setObjectName(QStringLiteral("btnCopyPath"));
        btnCopyPath->setEnabled(true);
        btnCopyPath->setMinimumSize(QSize(30, 30));
        btnCopyPath->setMaximumSize(QSize(30, 30));
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVMonitor/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCopyPath->setIcon(icon);

        horizontalLayout_2->addWidget(btnCopyPath);

        txtCopyPath = new QLineEdit(grbSaveData);
        txtCopyPath->setObjectName(QStringLiteral("txtCopyPath"));
        txtCopyPath->setEnabled(true);
        txtCopyPath->setMinimumSize(QSize(200, 0));

        horizontalLayout_2->addWidget(txtCopyPath);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(grbSaveData);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        lbChange = new QLabel(settingsPanelClass);
        lbChange->setObjectName(QStringLiteral("lbChange"));

        horizontalLayout_6->addWidget(lbChange);

        btnSave = new QPushButton(settingsPanelClass);
        btnSave->setObjectName(QStringLiteral("btnSave"));

        horizontalLayout_6->addWidget(btnSave);


        verticalLayout_3->addLayout(horizontalLayout_6);


        retranslateUi(settingsPanelClass);

        QMetaObject::connectSlotsByName(settingsPanelClass);
    } // setupUi

    void retranslateUi(QDialog *settingsPanelClass)
    {
        settingsPanelClass->setWindowTitle(QApplication::translate("settingsPanelClass", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", 0));
        groupBox->setTitle(QApplication::translate("settingsPanelClass", "\320\237\320\276\320\273\321\203\321\207\320\265\320\275\320\270\320\265 \320\264\320\260\320\275\320\275\321\213\321\205", 0));
        rbtnConnectByEthernet->setText(QApplication::translate("settingsPanelClass", "\321\201\320\276\320\265\320\264\320\270\320\275\320\265\320\275\320\270\320\265 \320\277\320\276 Ethernet", 0));
        label->setText(QApplication::translate("settingsPanelClass", "ip \320\260\320\264\321\200\320\265\321\201", 0));
        label_5->setText(QApplication::translate("settingsPanelClass", "\320\277\320\276\321\200\321\202", 0));
        rbtnConnectByCom->setText(QApplication::translate("settingsPanelClass", "\321\201\320\276\320\265\320\264\320\270\320\275\320\265\320\275\320\270\320\265 \320\277\320\276 COM", 0));
        label_2->setText(QApplication::translate("settingsPanelClass", "\320\277\320\276\321\200\321\202", 0));
        cmbComPort->clear();
        cmbComPort->insertItems(0, QStringList()
         << QApplication::translate("settingsPanelClass", "COM0", 0)
         << QApplication::translate("settingsPanelClass", "COM1", 0)
         << QApplication::translate("settingsPanelClass", "COM2", 0)
         << QApplication::translate("settingsPanelClass", "COM3", 0)
         << QApplication::translate("settingsPanelClass", "COM4", 0)
         << QApplication::translate("settingsPanelClass", "COM5", 0)
         << QApplication::translate("settingsPanelClass", "COM6", 0)
         << QApplication::translate("settingsPanelClass", "COM7", 0)
         << QApplication::translate("settingsPanelClass", "COM8", 0)
         << QApplication::translate("settingsPanelClass", "COM9", 0)
         << QApplication::translate("settingsPanelClass", "COM10", 0)
        );
        label_6->setText(QApplication::translate("settingsPanelClass", "\321\201\320\272\320\276\321\200\320\276\321\201\321\202\321\214", 0));
        grbSaveData->setTitle(QApplication::translate("settingsPanelClass", "\320\241\320\276\321\205\321\200\320\260\320\275\320\265\320\275\320\270\320\265 \320\264\320\260\320\275\320\275\321\213\321\205", 0));
#ifndef QT_NO_TOOLTIP
        rbtnCopyEna->setToolTip(QApplication::translate("settingsPanelClass", "\320\220\320\272\321\202\320\270\320\262\320\275\320\276", 0));
#endif // QT_NO_TOOLTIP
        rbtnCopyEna->setText(QApplication::translate("settingsPanelClass", "\320\220\320\272\321\202\320\270\320\262\320\275\320\276", 0));
        label_3->setText(QApplication::translate("settingsPanelClass", "\320\277\321\203\321\202\321\214", 0));
        btnCopyPath->setText(QString());
        lbChange->setText(QApplication::translate("settingsPanelClass", "*", 0));
        btnSave->setText(QApplication::translate("settingsPanelClass", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0));
    } // retranslateUi

};

namespace Ui {
    class settingsPanelClass: public Ui_settingsPanelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSPANEL_H
