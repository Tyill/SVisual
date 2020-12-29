/********************************************************************************
** Form generated from reading UI file 'axis_setting_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AXIS_SETTING_DIALOG_H
#define UI_AXIS_SETTING_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AxisSettingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QCheckBox *chbAuto;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *txtMaxValue;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *txtMinValue;
    QLabel *label;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *txtStepValue;
    QLabel *label_3;

    void setupUi(QDialog *AxisSettingDialog)
    {
        if (AxisSettingDialog->objectName().isEmpty())
            AxisSettingDialog->setObjectName(QStringLiteral("AxisSettingDialog"));
        AxisSettingDialog->resize(411, 210);
        AxisSettingDialog->setMaximumSize(QSize(450, 250));
        verticalLayout = new QVBoxLayout(AxisSettingDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        chbAuto = new QCheckBox(AxisSettingDialog);
        chbAuto->setObjectName(QStringLiteral("chbAuto"));
        chbAuto->setChecked(true);

        verticalLayout->addWidget(chbAuto);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        txtMaxValue = new QLineEdit(AxisSettingDialog);
        txtMaxValue->setObjectName(QStringLiteral("txtMaxValue"));
        txtMaxValue->setMinimumSize(QSize(100, 0));
        txtMaxValue->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_2->addWidget(txtMaxValue);

        label_2 = new QLabel(AxisSettingDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        txtMinValue = new QLineEdit(AxisSettingDialog);
        txtMinValue->setObjectName(QStringLiteral("txtMinValue"));
        txtMinValue->setMinimumSize(QSize(100, 0));
        txtMinValue->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(txtMinValue);

        label = new QLabel(AxisSettingDialog);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        txtStepValue = new QLineEdit(AxisSettingDialog);
        txtStepValue->setObjectName(QStringLiteral("txtStepValue"));
        txtStepValue->setMinimumSize(QSize(100, 0));
        txtStepValue->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_3->addWidget(txtStepValue);

        label_3 = new QLabel(AxisSettingDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(AxisSettingDialog);

        QMetaObject::connectSlotsByName(AxisSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *AxisSettingDialog)
    {
        AxisSettingDialog->setWindowTitle(QApplication::translate("AxisSettingDialog", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \320\276\321\201\320\270 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\271", Q_NULLPTR));
        chbAuto->setText(QApplication::translate("AxisSettingDialog", "\320\220\320\262\321\202\320\276\320\275\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260", Q_NULLPTR));
        label_2->setText(QApplication::translate("AxisSettingDialog", "\320\234\320\260\320\272\321\201\320\270\320\274\321\203\320\274", Q_NULLPTR));
        label->setText(QApplication::translate("AxisSettingDialog", "\320\234\320\270\320\275\320\270\320\274\321\203\320\274", Q_NULLPTR));
        label_3->setText(QApplication::translate("AxisSettingDialog", "\320\250\320\260\320\263 \320\274\320\265\320\266\320\264\321\203 \320\264\320\265\320\273\320\265\320\275\320\270\321\217\320\274\320\270", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AxisSettingDialog: public Ui_AxisSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AXIS_SETTING_DIALOG_H
