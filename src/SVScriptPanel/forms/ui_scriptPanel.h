/********************************************************************************
** Form generated from reading UI file 'scriptPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCRIPTPANEL_H
#define UI_SCRIPTPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScriptPanelClass
{
public:
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnNewScript;
    QSpacerItem *horizontalSpacer_4;
    QSplitter *splitter;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tblScripts;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnSetActive;
    QPushButton *btnResetActive;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QTableWidget *tblActiveScripts;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_4;
    QSplitter *splitter_2;
    QTabWidget *tabWidget;
    QTextEdit *txtStatusMess;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *lbChange;
    QPushButton *btnSave;
    QPushButton *btnClear;

    void setupUi(QDialog *ScriptPanelClass)
    {
        if (ScriptPanelClass->objectName().isEmpty())
            ScriptPanelClass->setObjectName(QStringLiteral("ScriptPanelClass"));
        ScriptPanelClass->resize(842, 540);
        verticalLayout_5 = new QVBoxLayout(ScriptPanelClass);
        verticalLayout_5->setSpacing(5);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(5, 5, 5, 5);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        btnNewScript = new QPushButton(ScriptPanelClass);
        btnNewScript->setObjectName(QStringLiteral("btnNewScript"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVScript/images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnNewScript->setIcon(icon);
        btnNewScript->setIconSize(QSize(30, 30));

        horizontalLayout_3->addWidget(btnNewScript);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout_5->addLayout(horizontalLayout_3);

        splitter = new QSplitter(ScriptPanelClass);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        verticalLayout_3 = new QVBoxLayout(layoutWidget1);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        groupBox_2 = new QGroupBox(layoutWidget1);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tblScripts = new QTableWidget(groupBox_2);
        if (tblScripts->columnCount() < 1)
            tblScripts->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblScripts->setHorizontalHeaderItem(0, __qtablewidgetitem);
        tblScripts->setObjectName(QStringLiteral("tblScripts"));

        verticalLayout_2->addWidget(tblScripts);


        verticalLayout_3->addWidget(groupBox_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btnSetActive = new QPushButton(layoutWidget1);
        btnSetActive->setObjectName(QStringLiteral("btnSetActive"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/SVScript/images/arrow-dn.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnSetActive->setIcon(icon1);
        btnSetActive->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(btnSetActive);

        btnResetActive = new QPushButton(layoutWidget1);
        btnResetActive->setObjectName(QStringLiteral("btnResetActive"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/SVScript/images/arrow-up.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnResetActive->setIcon(icon2);
        btnResetActive->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(btnResetActive);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout);

        groupBox = new QGroupBox(layoutWidget1);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tblActiveScripts = new QTableWidget(groupBox);
        if (tblActiveScripts->columnCount() < 1)
            tblActiveScripts->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblActiveScripts->setHorizontalHeaderItem(0, __qtablewidgetitem1);
        tblActiveScripts->setObjectName(QStringLiteral("tblActiveScripts"));

        verticalLayout->addWidget(tblActiveScripts);


        verticalLayout_3->addWidget(groupBox);

        splitter->addWidget(layoutWidget1);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout_4 = new QVBoxLayout(layoutWidget);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(layoutWidget);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        tabWidget = new QTabWidget(splitter_2);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(true);
        tabWidget->setMovable(false);
        tabWidget->setTabBarAutoHide(false);
        splitter_2->addWidget(tabWidget);
        txtStatusMess = new QTextEdit(splitter_2);
        txtStatusMess->setObjectName(QStringLiteral("txtStatusMess"));
        txtStatusMess->setFrameShape(QFrame::Box);
        splitter_2->addWidget(txtStatusMess);

        verticalLayout_4->addWidget(splitter_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        lbChange = new QLabel(layoutWidget);
        lbChange->setObjectName(QStringLiteral("lbChange"));

        horizontalLayout_2->addWidget(lbChange);

        btnSave = new QPushButton(layoutWidget);
        btnSave->setObjectName(QStringLiteral("btnSave"));

        horizontalLayout_2->addWidget(btnSave);

        btnClear = new QPushButton(layoutWidget);
        btnClear->setObjectName(QStringLiteral("btnClear"));

        horizontalLayout_2->addWidget(btnClear);


        verticalLayout_4->addLayout(horizontalLayout_2);

        splitter->addWidget(layoutWidget);

        verticalLayout_5->addWidget(splitter);


        retranslateUi(ScriptPanelClass);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(ScriptPanelClass);
    } // setupUi

    void retranslateUi(QDialog *ScriptPanelClass)
    {
        ScriptPanelClass->setWindowTitle(QApplication::translate("ScriptPanelClass", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214\321\201\320\272\320\270\320\265 \321\201\320\272\321\200\320\270\320\277\321\202\321\213", 0));
#ifndef QT_NO_TOOLTIP
        btnNewScript->setToolTip(QApplication::translate("ScriptPanelClass", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214 \321\201\320\272\321\200\320\270\320\277\321\202", 0));
#endif // QT_NO_TOOLTIP
        btnNewScript->setText(QString());
        groupBox_2->setTitle(QApplication::translate("ScriptPanelClass", "\320\222\321\201\320\265 \321\201\320\272\321\200\320\270\320\277\321\202\321\213", 0));
        QTableWidgetItem *___qtablewidgetitem = tblScripts->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ScriptPanelClass", "\320\230\320\274\321\217", 0));
#ifndef QT_NO_TOOLTIP
        btnSetActive->setToolTip(QApplication::translate("ScriptPanelClass", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\262 \320\260\320\272\321\202\320\270\320\262\320\275\321\213\320\265", 0));
#endif // QT_NO_TOOLTIP
        btnSetActive->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnResetActive->setToolTip(QApplication::translate("ScriptPanelClass", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\270\320\267 \320\260\320\272\321\202\320\270\320\262\320\275\321\213\321\205", 0));
#endif // QT_NO_TOOLTIP
        btnResetActive->setText(QString());
        groupBox->setTitle(QApplication::translate("ScriptPanelClass", "\320\220\320\272\321\202\320\270\320\262\320\275\321\213\320\265 \321\201\320\272\321\200\320\270\320\277\321\202\321\213", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tblActiveScripts->horizontalHeaderItem(0);
        ___qtablewidgetitem1->setText(QApplication::translate("ScriptPanelClass", "\320\230\320\274\321\217", 0));
        lbChange->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnSave->setToolTip(QApplication::translate("ScriptPanelClass", "CTRL + S", 0));
#endif // QT_NO_TOOLTIP
        btnSave->setText(QApplication::translate("ScriptPanelClass", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0));
        btnSave->setShortcut(QApplication::translate("ScriptPanelClass", "Ctrl+S", 0));
#ifndef QT_NO_TOOLTIP
        btnClear->setToolTip(QApplication::translate("ScriptPanelClass", "CTRL + D", 0));
#endif // QT_NO_TOOLTIP
        btnClear->setText(QApplication::translate("ScriptPanelClass", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", 0));
        btnClear->setShortcut(QApplication::translate("ScriptPanelClass", "Ctrl+D", 0));
    } // retranslateUi

};

namespace Ui {
    class ScriptPanelClass: public Ui_ScriptPanelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCRIPTPANEL_H
