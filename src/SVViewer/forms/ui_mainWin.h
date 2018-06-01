/********************************************************************************
** Form generated from reading UI file 'mainWin.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIN_H
#define UI_MAINWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
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
    QAction *actionStat;
    QAction *actionProgram;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnSortByModule;
    QPushButton *btnSortByGroup;
    treeWidgetExt *treeSignals;
    QProgressBar *progressBar;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QLabel *lbAllSignCnt;
    QFrame *line;
    QLabel *lbSignCnt;
    QFrame *line_2;
    QLabel *lbStatusMess;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWin)
    {
        if (MainWin->objectName().isEmpty())
            MainWin->setObjectName(QStringLiteral("MainWin"));
        MainWin->resize(362, 708);
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVViewer/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWin->setWindowIcon(icon);
        actionOpen = new QAction(MainWin);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/SVViewer/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionExit = new QAction(MainWin);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionStat = new QAction(MainWin);
        actionStat->setObjectName(QStringLiteral("actionStat"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/SVViewer/images/Graph.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStat->setIcon(icon2);
        actionProgram = new QAction(MainWin);
        actionProgram->setObjectName(QStringLiteral("actionProgram"));
        centralWidget = new QWidget(MainWin);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btnSortByModule = new QPushButton(groupBox);
        btnSortByModule->setObjectName(QStringLiteral("btnSortByModule"));
        btnSortByModule->setCheckable(true);
        btnSortByModule->setChecked(true);

        horizontalLayout_2->addWidget(btnSortByModule);

        btnSortByGroup = new QPushButton(groupBox);
        btnSortByGroup->setObjectName(QStringLiteral("btnSortByGroup"));
        btnSortByGroup->setCheckable(true);

        horizontalLayout_2->addWidget(btnSortByGroup);


        verticalLayout_2->addLayout(horizontalLayout_2);

        treeSignals = new treeWidgetExt(groupBox);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(3, QString::fromUtf8("\320\232\320\276\320\274\320\274\320\265\320\275\321\202\320\260\321\200\320\270\320\271"));
        __qtreewidgetitem->setText(2, QString::fromUtf8("\320\223\321\200\321\203\320\277\320\277\320\260"));
        treeSignals->setHeaderItem(__qtreewidgetitem);
        treeSignals->setObjectName(QStringLiteral("treeSignals"));
        treeSignals->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeSignals->setSortingEnabled(true);
        treeSignals->setAnimated(true);
        treeSignals->setAllColumnsShowFocus(true);
        treeSignals->setHeaderHidden(false);
        treeSignals->setColumnCount(4);
        treeSignals->header()->setProperty("showSortIndicator", QVariant(true));

        verticalLayout_2->addWidget(treeSignals);

        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMaximum(0);
        progressBar->setValue(-1);
        progressBar->setTextVisible(false);

        verticalLayout_2->addWidget(progressBar);

        splitter->addWidget(groupBox);

        verticalLayout->addWidget(splitter);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMaximumSize(QSize(16777215, 30));
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 0, 0, 0);
        lbAllSignCnt = new QLabel(groupBox_2);
        lbAllSignCnt->setObjectName(QStringLiteral("lbAllSignCnt"));

        horizontalLayout->addWidget(lbAllSignCnt);

        line = new QFrame(groupBox_2);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        lbSignCnt = new QLabel(groupBox_2);
        lbSignCnt->setObjectName(QStringLiteral("lbSignCnt"));

        horizontalLayout->addWidget(lbSignCnt);

        line_2 = new QFrame(groupBox_2);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_2);

        lbStatusMess = new QLabel(groupBox_2);
        lbStatusMess->setObjectName(QStringLiteral("lbStatusMess"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbStatusMess->sizePolicy().hasHeightForWidth());
        lbStatusMess->setSizePolicy(sizePolicy);
        lbStatusMess->setScaledContents(false);

        horizontalLayout->addWidget(lbStatusMess);

        horizontalSpacer = new QSpacerItem(850, 15, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(groupBox_2);

        MainWin->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWin);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 362, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        MainWin->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWin);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWin->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(actionExit);
        menu_2->addAction(actionProgram);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionStat);

        retranslateUi(MainWin);

        QMetaObject::connectSlotsByName(MainWin);
    } // setupUi

    void retranslateUi(QMainWindow *MainWin)
    {
        MainWin->setWindowTitle(QApplication::translate("MainWin", "SVViewer", 0));
        actionOpen->setText(QString());
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("MainWin", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("MainWin", "Ctrl+O", 0));
        actionExit->setText(QApplication::translate("MainWin", "\320\222\321\213\321\205\320\276\320\264", 0));
        actionStat->setText(QApplication::translate("MainWin", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", 0));
        actionProgram->setText(QApplication::translate("MainWin", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", 0));
        groupBox->setTitle(QString());
        btnSortByModule->setText(QApplication::translate("MainWin", "\320\234\320\276\320\264\321\203\320\273\320\270", 0));
        btnSortByGroup->setText(QApplication::translate("MainWin", "\320\223\321\200\321\203\320\277\320\277\321\213", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeSignals->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWin", "\320\242\320\270\320\277", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWin", "\320\230\320\274\321\217", 0));
        groupBox_2->setTitle(QString());
        lbAllSignCnt->setText(QString());
        lbSignCnt->setText(QString());
        lbStatusMess->setText(QString());
        menu->setTitle(QApplication::translate("MainWin", "\320\244\320\260\320\271\320\273", 0));
        menu_2->setTitle(QApplication::translate("MainWin", "\320\237\320\276\320\274\320\276\321\211\321\214", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWin: public Ui_MainWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIN_H
