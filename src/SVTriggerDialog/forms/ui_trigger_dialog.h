/********************************************************************************
** Form generated from reading UI file 'trigger_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRIGGER_DIALOG_H
#define UI_TRIGGER_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TriggerDialog
{
public:
    QHBoxLayout *horizontalLayout_4;
    QSplitter *splitter;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLabel *label_8;
    QListWidget *listModule;
    QLabel *label;
    QTableWidget *tableSignal;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QTableWidget *tableTrigger;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QLineEdit *txtTrigg;
    QLabel *lbIsChange;
    QPushButton *btnAddTrigger;
    QPushButton *btnChangeTrigger;
    QPushButton *btnDelTrigger;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *btnLess;
    QPushButton *btnEqual;
    QPushButton *btnMore;
    QPushButton *btnPosFront;
    QPushButton *btnNegFront;
    QLineEdit *txtCondition;
    QLabel *label_9;
    QLineEdit *txtTOut;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_8;
    QRadioButton *rbtnActiv;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QPushButton *btnOpenProc;
    QLineEdit *txtUserProcPath;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QLineEdit *txtUserProcArgs;

    void setupUi(QDialog *TriggerDialog)
    {
        if (TriggerDialog->objectName().isEmpty())
            TriggerDialog->setObjectName(QStringLiteral("TriggerDialog"));
        TriggerDialog->resize(680, 595);
        horizontalLayout_4 = new QHBoxLayout(TriggerDialog);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        splitter = new QSplitter(TriggerDialog);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout->addWidget(label_8);

        listModule = new QListWidget(layoutWidget1);
        listModule->setObjectName(QStringLiteral("listModule"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listModule->sizePolicy().hasHeightForWidth());
        listModule->setSizePolicy(sizePolicy);
        listModule->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listModule->setSelectionMode(QAbstractItemView::SingleSelection);
        listModule->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(listModule);

        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        tableSignal = new QTableWidget(layoutWidget1);
        if (tableSignal->columnCount() < 3)
            tableSignal->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableSignal->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableSignal->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableSignal->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableSignal->rowCount() < 15)
            tableSignal->setRowCount(15);
        tableSignal->setObjectName(QStringLiteral("tableSignal"));
        tableSignal->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableSignal->setSelectionMode(QAbstractItemView::SingleSelection);
        tableSignal->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableSignal->setRowCount(15);

        verticalLayout->addWidget(tableSignal);

        splitter->addWidget(layoutWidget1);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout_4 = new QVBoxLayout(layoutWidget);
        verticalLayout_4->setSpacing(7);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        tableTrigger = new QTableWidget(layoutWidget);
        if (tableTrigger->columnCount() < 3)
            tableTrigger->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableTrigger->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableTrigger->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        if (tableTrigger->rowCount() < 15)
            tableTrigger->setRowCount(15);
        tableTrigger->setObjectName(QStringLiteral("tableTrigger"));
        tableTrigger->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableTrigger->setSelectionMode(QAbstractItemView::SingleSelection);
        tableTrigger->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableTrigger->setRowCount(15);
        tableTrigger->setColumnCount(3);

        verticalLayout_2->addWidget(tableTrigger);


        verticalLayout_4->addLayout(verticalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        txtTrigg = new QLineEdit(layoutWidget);
        txtTrigg->setObjectName(QStringLiteral("txtTrigg"));

        horizontalLayout_3->addWidget(txtTrigg);

        lbIsChange = new QLabel(layoutWidget);
        lbIsChange->setObjectName(QStringLiteral("lbIsChange"));

        horizontalLayout_3->addWidget(lbIsChange);

        btnAddTrigger = new QPushButton(layoutWidget);
        btnAddTrigger->setObjectName(QStringLiteral("btnAddTrigger"));

        horizontalLayout_3->addWidget(btnAddTrigger);

        btnChangeTrigger = new QPushButton(layoutWidget);
        btnChangeTrigger->setObjectName(QStringLiteral("btnChangeTrigger"));

        horizontalLayout_3->addWidget(btnChangeTrigger);

        btnDelTrigger = new QPushButton(layoutWidget);
        btnDelTrigger->setObjectName(QStringLiteral("btnDelTrigger"));

        horizontalLayout_3->addWidget(btnDelTrigger);


        verticalLayout_4->addLayout(horizontalLayout_3);

        groupBox_3 = new QGroupBox(layoutWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        groupBox_3->setFont(font);
        verticalLayout_6 = new QVBoxLayout(groupBox_3);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        btnLess = new QPushButton(groupBox_3);
        btnLess->setObjectName(QStringLiteral("btnLess"));
        btnLess->setMinimumSize(QSize(30, 30));
        btnLess->setMaximumSize(QSize(30, 30));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        btnLess->setFont(font1);
        btnLess->setCheckable(true);
        btnLess->setChecked(true);

        horizontalLayout_6->addWidget(btnLess);

        btnEqual = new QPushButton(groupBox_3);
        btnEqual->setObjectName(QStringLiteral("btnEqual"));
        btnEqual->setMinimumSize(QSize(30, 30));
        btnEqual->setMaximumSize(QSize(30, 30));
        btnEqual->setFont(font1);
        btnEqual->setCheckable(true);

        horizontalLayout_6->addWidget(btnEqual);

        btnMore = new QPushButton(groupBox_3);
        btnMore->setObjectName(QStringLiteral("btnMore"));
        btnMore->setMinimumSize(QSize(30, 30));
        btnMore->setMaximumSize(QSize(30, 30));
        btnMore->setFont(font1);
        btnMore->setCheckable(true);

        horizontalLayout_6->addWidget(btnMore);

        btnPosFront = new QPushButton(groupBox_3);
        btnPosFront->setObjectName(QStringLiteral("btnPosFront"));
        btnPosFront->setMinimumSize(QSize(30, 30));
        btnPosFront->setMaximumSize(QSize(30, 30));
        btnPosFront->setFont(font1);
        btnPosFront->setCheckable(true);

        horizontalLayout_6->addWidget(btnPosFront);

        btnNegFront = new QPushButton(groupBox_3);
        btnNegFront->setObjectName(QStringLiteral("btnNegFront"));
        btnNegFront->setMinimumSize(QSize(30, 30));
        btnNegFront->setMaximumSize(QSize(30, 30));
        btnNegFront->setFont(font1);
        btnNegFront->setCheckable(true);

        horizontalLayout_6->addWidget(btnNegFront);

        txtCondition = new QLineEdit(groupBox_3);
        txtCondition->setObjectName(QStringLiteral("txtCondition"));
        txtCondition->setFont(font1);

        horizontalLayout_6->addWidget(txtCondition);

        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font1);

        horizontalLayout_6->addWidget(label_9);

        txtTOut = new QLineEdit(groupBox_3);
        txtTOut->setObjectName(QStringLiteral("txtTOut"));
        txtTOut->setFont(font1);

        horizontalLayout_6->addWidget(txtTOut);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout_6->addLayout(horizontalLayout_6);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        rbtnActiv = new QRadioButton(groupBox_3);
        rbtnActiv->setObjectName(QStringLiteral("rbtnActiv"));
        rbtnActiv->setFont(font1);
        rbtnActiv->setAutoExclusive(false);

        horizontalLayout_8->addWidget(rbtnActiv);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);


        verticalLayout_6->addLayout(horizontalLayout_8);


        verticalLayout_4->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(layoutWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setFont(font);
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font1);

        horizontalLayout_2->addWidget(label_5);

        btnOpenProc = new QPushButton(groupBox_2);
        btnOpenProc->setObjectName(QStringLiteral("btnOpenProc"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVTrigger/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnOpenProc->setIcon(icon);

        horizontalLayout_2->addWidget(btnOpenProc);

        txtUserProcPath = new QLineEdit(groupBox_2);
        txtUserProcPath->setObjectName(QStringLiteral("txtUserProcPath"));
        txtUserProcPath->setFont(font1);

        horizontalLayout_2->addWidget(txtUserProcPath);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMinimumSize(QSize(68, 0));
        label_7->setFont(font1);

        horizontalLayout_7->addWidget(label_7);

        txtUserProcArgs = new QLineEdit(groupBox_2);
        txtUserProcArgs->setObjectName(QStringLiteral("txtUserProcArgs"));
        txtUserProcArgs->setFont(font1);

        horizontalLayout_7->addWidget(txtUserProcArgs);


        verticalLayout_3->addLayout(horizontalLayout_7);


        verticalLayout_4->addWidget(groupBox_2);

        splitter->addWidget(layoutWidget);

        horizontalLayout_4->addWidget(splitter);


        retranslateUi(TriggerDialog);

        QMetaObject::connectSlotsByName(TriggerDialog);
    } // setupUi

    void retranslateUi(QDialog *TriggerDialog)
    {
        TriggerDialog->setWindowTitle(QApplication::translate("TriggerDialog", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \321\203\320\262\320\265\320\264\320\276\320\274\320\273\320\265\320\275\320\270\320\271", 0));
        label_8->setText(QApplication::translate("TriggerDialog", "\320\234\320\276\320\264\321\203\320\273\320\270", 0));
        label->setText(QApplication::translate("TriggerDialog", "\320\241\320\270\320\263\320\275\320\260\320\273\321\213", 0));
        QTableWidgetItem *___qtablewidgetitem = tableSignal->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("TriggerDialog", "\320\230\320\274\321\217", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableSignal->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("TriggerDialog", "\320\242\320\270\320\277", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableSignal->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("TriggerDialog", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\271", 0));
        label_2->setText(QApplication::translate("TriggerDialog", "\320\242\321\200\320\270\320\263\320\263\320\265\321\200\321\213", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableTrigger->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("TriggerDialog", "\320\230\320\274\321\217", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableTrigger->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("TriggerDialog", "\320\243\321\201\320\273\320\276\320\262\320\270\320\265", 0));
        lbIsChange->setText(QApplication::translate("TriggerDialog", "*", 0));
        btnAddTrigger->setText(QApplication::translate("TriggerDialog", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", 0));
        btnChangeTrigger->setText(QApplication::translate("TriggerDialog", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214", 0));
        btnDelTrigger->setText(QApplication::translate("TriggerDialog", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", 0));
        groupBox_3->setTitle(QApplication::translate("TriggerDialog", "\320\243\321\201\320\273\320\276\320\262\320\270\320\265 \321\201\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\275\320\270\321\217", 0));
#ifndef QT_NO_TOOLTIP
        btnLess->setToolTip(QApplication::translate("TriggerDialog", "\320\274\320\265\320\275\321\214\321\210\320\265", 0));
#endif // QT_NO_TOOLTIP
        btnLess->setText(QApplication::translate("TriggerDialog", "<", 0));
#ifndef QT_NO_TOOLTIP
        btnEqual->setToolTip(QApplication::translate("TriggerDialog", "\321\200\320\260\320\262\320\275\320\276", 0));
#endif // QT_NO_TOOLTIP
        btnEqual->setText(QApplication::translate("TriggerDialog", "==", 0));
#ifndef QT_NO_TOOLTIP
        btnMore->setToolTip(QApplication::translate("TriggerDialog", "\320\261\320\276\320\273\321\214\321\210\320\265", 0));
#endif // QT_NO_TOOLTIP
        btnMore->setText(QApplication::translate("TriggerDialog", ">", 0));
#ifndef QT_NO_TOOLTIP
        btnPosFront->setToolTip(QApplication::translate("TriggerDialog", "\320\277\320\276\320\273\320\276\320\266\320\270\321\202\320\265\320\273\321\214\320\275\321\213\320\271 \321\204\321\200\320\276\320\275\321\202", 0));
#endif // QT_NO_TOOLTIP
        btnPosFront->setText(QApplication::translate("TriggerDialog", "FP", 0));
#ifndef QT_NO_TOOLTIP
        btnNegFront->setToolTip(QApplication::translate("TriggerDialog", "\320\276\321\202\321\200\320\270\321\206\320\260\321\202\320\265\320\273\321\214\320\275\321\213\320\271 \321\204\321\200\320\276\320\275\321\202", 0));
#endif // QT_NO_TOOLTIP
        btnNegFront->setText(QApplication::translate("TriggerDialog", "FN", 0));
#ifndef QT_NO_TOOLTIP
        txtCondition->setToolTip(QApplication::translate("TriggerDialog", "\320\277\320\276\321\200\320\276\320\263\320\276\320\262\320\276\320\265 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265", 0));
#endif // QT_NO_TOOLTIP
        label_9->setText(QApplication::translate("TriggerDialog", "tout", 0));
#ifndef QT_NO_TOOLTIP
        txtTOut->setToolTip(QApplication::translate("TriggerDialog", "\320\267\320\260\320\264\320\265\321\200\320\266\320\272\320\260 \321\201\321\200\320\260\320\261\320\260\321\202\321\213\320\262\320\260\320\275\320\270\321\217, \321\201\320\265\320\272", 0));
#endif // QT_NO_TOOLTIP
        txtTOut->setText(QApplication::translate("TriggerDialog", "0", 0));
        rbtnActiv->setText(QApplication::translate("TriggerDialog", "\320\220\320\272\321\202\320\270\320\262\320\275\320\276", 0));
        groupBox_2->setTitle(QApplication::translate("TriggerDialog", "\320\227\320\260\320\277\321\203\321\201\320\272 \320\277\321\200\320\276\321\206\320\265\321\201\321\201\320\260", 0));
        label_5->setText(QApplication::translate("TriggerDialog", "\320\237\321\203\321\202\321\214", 0));
        btnOpenProc->setText(QString());
        label_7->setText(QApplication::translate("TriggerDialog", "\320\220\321\200\320\263\321\203\320\274\320\265\320\275\321\202\321\213", 0));
    } // retranslateUi

};

namespace Ui {
    class TriggerDialog: public Ui_TriggerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRIGGER_DIALOG_H
