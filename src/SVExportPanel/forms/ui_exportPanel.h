/********************************************************************************
** Form generated from reading UI file 'exportPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTPANEL_H
#define UI_EXPORTPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExportPanelClass
{
public:
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *tableModule;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QTableWidget *tableSignal;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer;
    QPushButton *btnAddSignal;
    QSpacerItem *verticalSpacer_3;
    QPushButton *btnDelSignal;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QTableWidget *tableExport;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QCheckBox *chbAllTime;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QDateTimeEdit *dTimeBegin;
    QLabel *label_5;
    QDateTimeEdit *dTimeEnd;
    QSpacerItem *horizontalSpacer_3;
    QLabel *lbMessage;
    QPushButton *btnExport;

    void setupUi(QDialog *ExportPanelClass)
    {
        if (ExportPanelClass->objectName().isEmpty())
            ExportPanelClass->setObjectName(QStringLiteral("ExportPanelClass"));
        ExportPanelClass->resize(786, 400);
        ExportPanelClass->setMaximumSize(QSize(900, 900));
        verticalLayout_5 = new QVBoxLayout(ExportPanelClass);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        splitter = new QSplitter(ExportPanelClass);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        tableModule = new QListWidget(layoutWidget);
        tableModule->setObjectName(QStringLiteral("tableModule"));

        verticalLayout->addWidget(tableModule);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        tableSignal = new QTableWidget(layoutWidget1);
        if (tableSignal->columnCount() < 3)
            tableSignal->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableSignal->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableSignal->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableSignal->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableSignal->setObjectName(QStringLiteral("tableSignal"));

        verticalLayout_2->addWidget(tableSignal);

        splitter->addWidget(layoutWidget1);

        horizontalLayout_3->addWidget(splitter);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        btnAddSignal = new QPushButton(ExportPanelClass);
        btnAddSignal->setObjectName(QStringLiteral("btnAddSignal"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVExp/images/arrow-right.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAddSignal->setIcon(icon);
        btnAddSignal->setIconSize(QSize(30, 30));

        verticalLayout_4->addWidget(btnAddSignal);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_3);

        btnDelSignal = new QPushButton(ExportPanelClass);
        btnDelSignal->setObjectName(QStringLiteral("btnDelSignal"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/SVExp/images/arrow-left.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDelSignal->setIcon(icon1);
        btnDelSignal->setIconSize(QSize(30, 30));

        verticalLayout_4->addWidget(btnDelSignal);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);


        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_3 = new QLabel(ExportPanelClass);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3);

        tableExport = new QTableWidget(ExportPanelClass);
        if (tableExport->columnCount() < 4)
            tableExport->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableExport->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableExport->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableExport->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableExport->setHorizontalHeaderItem(3, __qtablewidgetitem6);
        tableExport->setObjectName(QStringLiteral("tableExport"));
        tableExport->setMinimumSize(QSize(300, 0));

        verticalLayout_3->addWidget(tableExport);


        horizontalLayout_3->addLayout(verticalLayout_3);


        verticalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_6 = new QLabel(ExportPanelClass);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMinimumSize(QSize(200, 0));

        horizontalLayout_2->addWidget(label_6);

        chbAllTime = new QCheckBox(ExportPanelClass);
        chbAllTime->setObjectName(QStringLiteral("chbAllTime"));

        horizontalLayout_2->addWidget(chbAllTime);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_5->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_4 = new QLabel(ExportPanelClass);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        dTimeBegin = new QDateTimeEdit(ExportPanelClass);
        dTimeBegin->setObjectName(QStringLiteral("dTimeBegin"));

        horizontalLayout->addWidget(dTimeBegin);

        label_5 = new QLabel(ExportPanelClass);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);

        dTimeEnd = new QDateTimeEdit(ExportPanelClass);
        dTimeEnd->setObjectName(QStringLiteral("dTimeEnd"));

        horizontalLayout->addWidget(dTimeEnd);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        lbMessage = new QLabel(ExportPanelClass);
        lbMessage->setObjectName(QStringLiteral("lbMessage"));

        horizontalLayout->addWidget(lbMessage);

        btnExport = new QPushButton(ExportPanelClass);
        btnExport->setObjectName(QStringLiteral("btnExport"));

        horizontalLayout->addWidget(btnExport);


        verticalLayout_5->addLayout(horizontalLayout);


        retranslateUi(ExportPanelClass);

        QMetaObject::connectSlotsByName(ExportPanelClass);
    } // setupUi

    void retranslateUi(QDialog *ExportPanelClass)
    {
        ExportPanelClass->setWindowTitle(QApplication::translate("ExportPanelClass", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202 \320\264\320\260\320\275\320\275\321\213\321\205", 0));
        label->setText(QApplication::translate("ExportPanelClass", "\320\234\320\276\320\264\321\203\320\273\320\270", 0));
        label_2->setText(QApplication::translate("ExportPanelClass", "\320\241\320\270\320\263\320\275\320\260\320\273\321\213", 0));
        QTableWidgetItem *___qtablewidgetitem = tableSignal->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ExportPanelClass", "\320\230\320\274\321\217", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableSignal->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ExportPanelClass", "\320\242\320\270\320\277", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableSignal->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ExportPanelClass", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\271", 0));
#ifndef QT_NO_TOOLTIP
        btnAddSignal->setToolTip(QApplication::translate("ExportPanelClass", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", 0));
#endif // QT_NO_TOOLTIP
        btnAddSignal->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnDelSignal->setToolTip(QApplication::translate("ExportPanelClass", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", 0));
#endif // QT_NO_TOOLTIP
        btnDelSignal->setText(QString());
        label_3->setText(QApplication::translate("ExportPanelClass", "\320\241\320\270\320\263\320\275\320\260\320\273\321\213 \320\264\320\273\321\217 \321\215\320\272\321\201\320\277\320\276\321\200\321\202\320\260", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableExport->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("ExportPanelClass", "\320\234\320\276\320\264\321\203\320\273\321\214", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableExport->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("ExportPanelClass", "\320\241\320\270\320\263\320\275\320\260\320\273", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableExport->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("ExportPanelClass", "\320\242\320\270\320\277", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tableExport->horizontalHeaderItem(3);
        ___qtablewidgetitem6->setText(QApplication::translate("ExportPanelClass", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\271", 0));
        label_6->setText(QApplication::translate("ExportPanelClass", "\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275 \320\262\321\200\320\265\320\274\320\265\320\275\320\270", 0));
        chbAllTime->setText(QApplication::translate("ExportPanelClass", "\320\222\321\201\320\265 \320\262\321\200\320\265\320\274\321\217", 0));
        label_4->setText(QApplication::translate("ExportPanelClass", "\320\276\321\202", 0));
        label_5->setText(QApplication::translate("ExportPanelClass", "\320\264\320\276", 0));
        lbMessage->setText(QString());
        btnExport->setText(QApplication::translate("ExportPanelClass", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202", 0));
    } // retranslateUi

};

namespace Ui {
    class ExportPanelClass: public Ui_ExportPanelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTPANEL_H
