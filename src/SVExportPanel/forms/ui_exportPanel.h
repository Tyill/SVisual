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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
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
    QWidget *widget;
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
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *chbAllTime;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QDateTimeEdit *dTimeBegin;
    QLabel *label_5;
    QDateTimeEdit *dTimeEnd;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_6;
    QRadioButton *rbtnEverySec;
    QRadioButton *rbtnEveryMin;
    QRadioButton *rbtnEveryVal;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnExport;
    QLabel *lbMessage;

    void setupUi(QDialog *ExportPanelClass)
    {
        if (ExportPanelClass->objectName().isEmpty())
            ExportPanelClass->setObjectName(QStringLiteral("ExportPanelClass"));
        ExportPanelClass->resize(722, 551);
        ExportPanelClass->setMaximumSize(QSize(900, 900));
        verticalLayout_5 = new QVBoxLayout(ExportPanelClass);
        verticalLayout_5->setSpacing(5);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(5, 5, 5, 5);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        splitter = new QSplitter(ExportPanelClass);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
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
        widget = new QWidget(splitter);
        widget->setObjectName(QStringLiteral("widget"));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        tableSignal = new QTableWidget(widget);
        if (tableSignal->columnCount() < 3)
            tableSignal->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableSignal->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableSignal->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableSignal->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableSignal->setObjectName(QStringLiteral("tableSignal"));
        tableSignal->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_2->addWidget(tableSignal);

        splitter->addWidget(widget);

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
        tableExport->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_3->addWidget(tableExport);


        horizontalLayout_3->addLayout(verticalLayout_3);


        verticalLayout_5->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        groupBox_2 = new QGroupBox(ExportPanelClass);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_8 = new QVBoxLayout(groupBox_2);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_2->addWidget(label_6);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        chbAllTime = new QCheckBox(groupBox_2);
        chbAllTime->setObjectName(QStringLiteral("chbAllTime"));

        horizontalLayout_2->addWidget(chbAllTime);


        verticalLayout_8->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        dTimeBegin = new QDateTimeEdit(groupBox_2);
        dTimeBegin->setObjectName(QStringLiteral("dTimeBegin"));

        horizontalLayout->addWidget(dTimeBegin);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);

        dTimeEnd = new QDateTimeEdit(groupBox_2);
        dTimeEnd->setObjectName(QStringLiteral("dTimeEnd"));

        horizontalLayout->addWidget(dTimeEnd);


        verticalLayout_8->addLayout(horizontalLayout);


        horizontalLayout_4->addWidget(groupBox_2);

        groupBox = new QGroupBox(ExportPanelClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_6 = new QVBoxLayout(groupBox);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        rbtnEverySec = new QRadioButton(groupBox);
        rbtnEverySec->setObjectName(QStringLiteral("rbtnEverySec"));
        rbtnEverySec->setChecked(true);

        verticalLayout_6->addWidget(rbtnEverySec);

        rbtnEveryMin = new QRadioButton(groupBox);
        rbtnEveryMin->setObjectName(QStringLiteral("rbtnEveryMin"));

        verticalLayout_6->addWidget(rbtnEveryMin);

        rbtnEveryVal = new QRadioButton(groupBox);
        rbtnEveryVal->setObjectName(QStringLiteral("rbtnEveryVal"));

        verticalLayout_6->addWidget(rbtnEveryVal);


        horizontalLayout_4->addWidget(groupBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        btnExport = new QPushButton(ExportPanelClass);
        btnExport->setObjectName(QStringLiteral("btnExport"));

        horizontalLayout_4->addWidget(btnExport);


        verticalLayout_5->addLayout(horizontalLayout_4);

        lbMessage = new QLabel(ExportPanelClass);
        lbMessage->setObjectName(QStringLiteral("lbMessage"));
        lbMessage->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        lbMessage->setWordWrap(false);

        verticalLayout_5->addWidget(lbMessage);


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
        groupBox_2->setTitle(QString());
        label_6->setText(QApplication::translate("ExportPanelClass", "\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275 \320\262\321\200\320\265\320\274\320\265\320\275\320\270", 0));
        chbAllTime->setText(QApplication::translate("ExportPanelClass", "\320\222\321\201\320\265 \320\262\321\200\320\265\320\274\321\217", 0));
        label_4->setText(QApplication::translate("ExportPanelClass", "\320\276\321\202", 0));
        label_5->setText(QApplication::translate("ExportPanelClass", "\320\264\320\276", 0));
        groupBox->setTitle(QString());
        rbtnEverySec->setText(QApplication::translate("ExportPanelClass", "\320\272\320\260\320\266\320\264\321\203\321\216 \321\201\320\265\320\272\321\203\320\275\320\264\321\203", 0));
        rbtnEveryMin->setText(QApplication::translate("ExportPanelClass", "\320\272\320\260\320\266\320\264\321\203\321\216 \320\274\320\270\320\275\321\203\321\202\321\203", 0));
        rbtnEveryVal->setText(QApplication::translate("ExportPanelClass", "\320\272\320\260\320\266\320\264\320\276\320\265 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265", 0));
        btnExport->setText(QApplication::translate("ExportPanelClass", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202", 0));
        lbMessage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ExportPanelClass: public Ui_ExportPanelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTPANEL_H
