/********************************************************************************
** Form generated from reading UI file 'mainwin.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWIN_H
#define MAINWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "src/treeWidgetExt.h"

QT_BEGIN_NAMESPACE

class Ui_MainWin
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionAdd;
    QAction *actionSignals;
    QAction *actionTrgPanel;
    QAction *actionSettings;
    QAction *actionEventOrder;
    QAction *actionProgram;
    QAction *actionCheckUpdate;
    QAction *actionPrint;
    QAction *actionExport;
    QAction *actionNewWin;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnSlowPlay;
    QSplitter *splitter_2;
    treeWidgetExt *treeSignals;
    QTextEdit *txtStatusMess;
    QMenuBar *menuBar;
    QMenu *menuMain;
    QMenu *menuHelp;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWin)
    {
        if (MainWin->objectName().isEmpty())
            MainWin->setObjectName(QStringLiteral("MainWin"));
        MainWin->resize(289, 454);
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVMonitor/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWin->setWindowIcon(icon);
        actionOpen = new QAction(MainWin);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/SVMonitor/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionExit = new QAction(MainWin);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionAdd = new QAction(MainWin);
        actionAdd->setObjectName(QStringLiteral("actionAdd"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/SVMonitor/resources/images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd->setIcon(icon2);
        actionSignals = new QAction(MainWin);
        actionSignals->setObjectName(QStringLiteral("actionSignals"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/SVMonitor/resources/images/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSignals->setIcon(icon3);
        actionTrgPanel = new QAction(MainWin);
        actionTrgPanel->setObjectName(QStringLiteral("actionTrgPanel"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/SVMonitor/images/flag-red.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTrgPanel->setIcon(icon4);
        actionSettings = new QAction(MainWin);
        actionSettings->setObjectName(QStringLiteral("actionSettings"));
        actionEventOrder = new QAction(MainWin);
        actionEventOrder->setObjectName(QStringLiteral("actionEventOrder"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/SVMonitor/images/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEventOrder->setIcon(icon5);
        actionProgram = new QAction(MainWin);
        actionProgram->setObjectName(QStringLiteral("actionProgram"));
        actionCheckUpdate = new QAction(MainWin);
        actionCheckUpdate->setObjectName(QStringLiteral("actionCheckUpdate"));
        actionPrint = new QAction(MainWin);
        actionPrint->setObjectName(QStringLiteral("actionPrint"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/SVMonitor/images/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon6);
        actionExport = new QAction(MainWin);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/SVMonitor/images/export.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExport->setIcon(icon7);
        actionNewWin = new QAction(MainWin);
        actionNewWin->setObjectName(QStringLiteral("actionNewWin"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/SVMonitor/images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNewWin->setIcon(icon8);
        centralWidget = new QWidget(MainWin);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMaximumSize(QSize(350, 16777215));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnSlowPlay = new QPushButton(centralWidget);
        btnSlowPlay->setObjectName(QStringLiteral("btnSlowPlay"));

        horizontalLayout_2->addWidget(btnSlowPlay);


        verticalLayout->addLayout(horizontalLayout_2);

        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        treeSignals = new treeWidgetExt(splitter_2);
        treeSignals->setObjectName(QStringLiteral("treeSignals"));
        treeSignals->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeSignals->setTabKeyNavigation(true);
        treeSignals->setSortingEnabled(true);
        treeSignals->setWordWrap(true);
        treeSignals->setColumnCount(4);
        splitter_2->addWidget(treeSignals);
        treeSignals->header()->setDefaultSectionSize(125);
        treeSignals->header()->setProperty("showSortIndicator", QVariant(true));
        txtStatusMess = new QTextEdit(splitter_2);
        txtStatusMess->setObjectName(QStringLiteral("txtStatusMess"));
        splitter_2->addWidget(txtStatusMess);

        verticalLayout->addWidget(splitter_2);

        MainWin->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWin);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 289, 21));
        menuMain = new QMenu(menuBar);
        menuMain->setObjectName(QStringLiteral("menuMain"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWin->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWin);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWin->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuMain->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuMain->addAction(actionSettings);
        menuMain->addAction(actionExit);
        menuHelp->addAction(actionProgram);
        mainToolBar->addAction(actionTrgPanel);
        mainToolBar->addAction(actionEventOrder);
        mainToolBar->addAction(actionPrint);
        mainToolBar->addAction(actionExport);
        mainToolBar->addAction(actionNewWin);

        retranslateUi(MainWin);

        QMetaObject::connectSlotsByName(MainWin);
    } // setupUi

    void retranslateUi(QMainWindow *MainWin)
    {
        MainWin->setWindowTitle(QApplication::translate("MainWin", "SVMonitor", Q_NULLPTR));
        actionOpen->setText(QString());
        actionExit->setText(QApplication::translate("MainWin", "\320\222\321\213\321\205\320\276\320\264", Q_NULLPTR));
        actionAdd->setText(QApplication::translate("MainWin", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", Q_NULLPTR));
        actionSignals->setText(QApplication::translate("MainWin", "\320\241\320\270\320\263\320\275\320\260\320\273\321\213", Q_NULLPTR));
        actionTrgPanel->setText(QApplication::translate("MainWin", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \321\203\320\262\320\265\320\264\320\276\320\274\320\273\320\265\320\275\320\270\320\271", Q_NULLPTR));
        actionSettings->setText(QApplication::translate("MainWin", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", Q_NULLPTR));
        actionEventOrder->setText(QApplication::translate("MainWin", "\320\236\321\202\321\207\320\265\321\202 \321\201\320\276\320\261\321\213\321\202\320\270\320\271", Q_NULLPTR));
        actionProgram->setText(QApplication::translate("MainWin", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", Q_NULLPTR));
        actionCheckUpdate->setText(QApplication::translate("MainWin", "\320\237\321\200\320\276\320\262\320\265\321\200\320\270\321\202\321\214 \320\276\320\261\320\275\320\276\320\262\320\273\320\265\320\275\320\270\321\217", Q_NULLPTR));
        actionPrint->setText(QApplication::translate("MainWin", "\320\237\320\265\321\207\320\260\321\202\321\214", Q_NULLPTR));
        actionExport->setText(QApplication::translate("MainWin", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202", Q_NULLPTR));
        actionNewWin->setText(QApplication::translate("MainWin", "\320\235\320\276\320\262\320\276\320\265 \320\276\320\272\320\275\320\276", Q_NULLPTR));
        label->setText(QApplication::translate("MainWin", "\320\234\320\276\320\264\321\203\320\273\320\270", Q_NULLPTR));
        btnSlowPlay->setText(QApplication::translate("MainWin", "\320\242\320\270\321\205\320\270\320\271 \321\200\320\265\320\266\320\270\320\274", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeSignals->headerItem();
        ___qtreewidgetitem->setText(3, QApplication::translate("MainWin", "\320\223\321\200\321\203\320\277\320\277\320\260", Q_NULLPTR));
        ___qtreewidgetitem->setText(2, QApplication::translate("MainWin", "\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\271", Q_NULLPTR));
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWin", "\320\242\320\270\320\277", Q_NULLPTR));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWin", "\320\230\320\274\321\217", Q_NULLPTR));
        menuMain->setTitle(QApplication::translate("MainWin", "\320\244\320\260\320\271\320\273", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWin", "\320\237\320\276\320\274\320\276\321\211\321\214", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWin: public Ui_MainWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWIN_H
