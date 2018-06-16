/********************************************************************************
** Form generated from reading UI file 'exportWin.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTWIN_H
#define UI_EXPORTWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_exportWin
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QListWidget *lstModule;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *lstSignals;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *lstSignExp;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDateTimeEdit *dTimeFrom;
    QLabel *label_2;
    QDateTimeEdit *dTimeTo;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnExport;

    void setupUi(QDialog *exportWin)
    {
        if (exportWin->objectName().isEmpty())
            exportWin->setObjectName(QStringLiteral("exportWin"));
        exportWin->resize(726, 449);
        horizontalLayout_2 = new QHBoxLayout(exportWin);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox = new QGroupBox(exportWin);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lstModule = new QListWidget(groupBox);
        lstModule->setObjectName(QStringLiteral("lstModule"));

        verticalLayout->addWidget(lstModule);


        verticalLayout_4->addWidget(groupBox);

        groupBox_2 = new QGroupBox(exportWin);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        lstSignals = new QTableWidget(groupBox_2);
        if (lstSignals->columnCount() < 3)
            lstSignals->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        lstSignals->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        lstSignals->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        lstSignals->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        lstSignals->setObjectName(QStringLiteral("lstSignals"));

        verticalLayout_2->addWidget(lstSignals);


        verticalLayout_4->addWidget(groupBox_2);


        horizontalLayout_2->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        groupBox_3 = new QGroupBox(exportWin);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        lstSignExp = new QTableWidget(groupBox_3);
        if (lstSignExp->columnCount() < 3)
            lstSignExp->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        lstSignExp->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        lstSignExp->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        lstSignExp->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        lstSignExp->setObjectName(QStringLiteral("lstSignExp"));

        verticalLayout_3->addWidget(lstSignExp);


        verticalLayout_5->addWidget(groupBox_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(exportWin);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        dTimeFrom = new QDateTimeEdit(exportWin);
        dTimeFrom->setObjectName(QStringLiteral("dTimeFrom"));

        horizontalLayout->addWidget(dTimeFrom);

        label_2 = new QLabel(exportWin);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        dTimeTo = new QDateTimeEdit(exportWin);
        dTimeTo->setObjectName(QStringLiteral("dTimeTo"));

        horizontalLayout->addWidget(dTimeTo);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnExport = new QPushButton(exportWin);
        btnExport->setObjectName(QStringLiteral("btnExport"));

        horizontalLayout->addWidget(btnExport);


        verticalLayout_5->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_5);


        retranslateUi(exportWin);

        QMetaObject::connectSlotsByName(exportWin);
    } // setupUi

    void retranslateUi(QDialog *exportWin)
    {
        exportWin->setWindowTitle(QApplication::translate("exportWin", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202 \320\262 JSON", 0));
        groupBox->setTitle(QApplication::translate("exportWin", "\320\234\320\276\320\264\321\203\320\273\320\270", 0));
        groupBox_2->setTitle(QApplication::translate("exportWin", "\320\241\320\270\320\263\320\275\320\260\320\273\321\213", 0));
        QTableWidgetItem *___qtablewidgetitem = lstSignals->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("exportWin", "\320\230\320\274\321\217", 0));
        QTableWidgetItem *___qtablewidgetitem1 = lstSignals->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("exportWin", "\320\242\320\270\320\277", 0));
        QTableWidgetItem *___qtablewidgetitem2 = lstSignals->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("exportWin", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\271", 0));
        groupBox_3->setTitle(QApplication::translate("exportWin", "\320\241\320\270\320\263\320\275\320\260\320\273\321\213 \320\275\320\260 \321\215\320\272\321\201\320\277\320\276\321\200\321\202", 0));
        QTableWidgetItem *___qtablewidgetitem3 = lstSignExp->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("exportWin", "\320\230\320\274\321\217", 0));
        QTableWidgetItem *___qtablewidgetitem4 = lstSignExp->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("exportWin", "\320\242\320\270\320\277", 0));
        QTableWidgetItem *___qtablewidgetitem5 = lstSignExp->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("exportWin", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\271", 0));
        label->setText(QApplication::translate("exportWin", "\320\276\321\202", 0));
        dTimeFrom->setDisplayFormat(QApplication::translate("exportWin", "yy.MM.dd H:mm", 0));
        label_2->setText(QApplication::translate("exportWin", "\320\264\320\276", 0));
        dTimeTo->setDisplayFormat(QApplication::translate("exportWin", "yy.MM.dd H:mm", 0));
        btnExport->setText(QApplication::translate("exportWin", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214", 0));
    } // retranslateUi

};

namespace Ui {
    class exportWin: public Ui_exportWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTWIN_H
