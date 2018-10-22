/********************************************************************************
** Form generated from reading UI file 'eventOrderWin.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENTORDERWIN_H
#define UI_EVENTORDERWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_eventOrderClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDateTimeEdit *txtBeginDate;
    QLabel *label_2;
    QDateTimeEdit *txtEndDate;
    QPushButton *btnShowOrder;
    QSpacerItem *horizontalSpacer;
    QTableWidget *tableEvents;

    void setupUi(QDialog *eventOrderClass)
    {
        if (eventOrderClass->objectName().isEmpty())
            eventOrderClass->setObjectName(QStringLiteral("eventOrderClass"));
        eventOrderClass->resize(624, 365);
        verticalLayout = new QVBoxLayout(eventOrderClass);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(eventOrderClass);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        txtBeginDate = new QDateTimeEdit(eventOrderClass);
        txtBeginDate->setObjectName(QStringLiteral("txtBeginDate"));

        horizontalLayout->addWidget(txtBeginDate);

        label_2 = new QLabel(eventOrderClass);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        txtEndDate = new QDateTimeEdit(eventOrderClass);
        txtEndDate->setObjectName(QStringLiteral("txtEndDate"));

        horizontalLayout->addWidget(txtEndDate);

        btnShowOrder = new QPushButton(eventOrderClass);
        btnShowOrder->setObjectName(QStringLiteral("btnShowOrder"));

        horizontalLayout->addWidget(btnShowOrder);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        tableEvents = new QTableWidget(eventOrderClass);
        if (tableEvents->columnCount() < 5)
            tableEvents->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableEvents->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableEvents->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableEvents->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableEvents->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableEvents->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        if (tableEvents->rowCount() < 20)
            tableEvents->setRowCount(20);
        tableEvents->setObjectName(QStringLiteral("tableEvents"));
        tableEvents->setSortingEnabled(true);
        tableEvents->setRowCount(20);
        tableEvents->horizontalHeader()->setDefaultSectionSize(175);

        verticalLayout->addWidget(tableEvents);


        retranslateUi(eventOrderClass);

        QMetaObject::connectSlotsByName(eventOrderClass);
    } // setupUi

    void retranslateUi(QDialog *eventOrderClass)
    {
        eventOrderClass->setWindowTitle(QApplication::translate("eventOrderClass", "\320\236\321\202\321\207\320\265\321\202 \321\201\320\276\320\261\321\213\321\202\320\270\320\271", 0));
        label->setText(QApplication::translate("eventOrderClass", "c", 0));
        label_2->setText(QApplication::translate("eventOrderClass", "\320\277\320\276", 0));
        btnShowOrder->setText(QApplication::translate("eventOrderClass", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214", 0));
        QTableWidgetItem *___qtablewidgetitem = tableEvents->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("eventOrderClass", "\320\222\321\200\320\265\320\274\321\217", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableEvents->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("eventOrderClass", "\320\242\321\200\320\270\320\263\320\263\320\265\321\200", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableEvents->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("eventOrderClass", "\320\234\320\276\320\264\321\203\320\273\321\214", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableEvents->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("eventOrderClass", "\320\241\320\270\320\263\320\275\320\260\320\273", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableEvents->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("eventOrderClass", "\320\243\321\201\320\273\320\276\320\262\320\270\320\265", 0));
    } // retranslateUi

};

namespace Ui {
    class eventOrderClass: public Ui_eventOrderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTORDERWIN_H
