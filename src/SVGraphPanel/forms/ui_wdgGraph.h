/********************************************************************************
** Form generated from reading UI file 'wdgGraph.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WDGGRAPH_H
#define UI_WDGGRAPH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "src/wdgAxisValue.h"
#include "src/wdgPlot.h"

QT_BEGIN_NAMESPACE

class Ui_wdgGraphClass
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAxisAttr;
    QSpacerItem *horizontalSpacer_2;
    QFrame *line;
    QPushButton *btnUp;
    QPushButton *btnDn;
    QPushButton *btnClose;
    QHBoxLayout *horizontalLayout_2;
    wdgAxisValue *wAxisValue;
    wdgPlot *wPlot;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *vLayoutSignName;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *vLayoutAuxSignName;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *wdgGraphClass)
    {
        if (wdgGraphClass->objectName().isEmpty())
            wdgGraphClass->setObjectName(QStringLiteral("wdgGraphClass"));
        wdgGraphClass->resize(442, 400);
        wdgGraphClass->setStyleSheet(QStringLiteral(""));
        verticalLayout_2 = new QVBoxLayout(wdgGraphClass);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(wdgGraphClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QStringLiteral(""));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, -1, -1, -1);
        btnAxisAttr = new QPushButton(groupBox);
        btnAxisAttr->setObjectName(QStringLiteral("btnAxisAttr"));
        btnAxisAttr->setMinimumSize(QSize(40, 20));
        btnAxisAttr->setMaximumSize(QSize(40, 20));
        QFont font;
        font.setPointSize(6);
        btnAxisAttr->setFont(font);
        btnAxisAttr->setCheckable(false);

        horizontalLayout->addWidget(btnAxisAttr);

        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        line = new QFrame(groupBox);
        line->setObjectName(QStringLiteral("line"));
        line->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        line->setFrameShadow(QFrame::Raised);
        line->setFrameShape(QFrame::HLine);

        horizontalLayout->addWidget(line);

        btnUp = new QPushButton(groupBox);
        btnUp->setObjectName(QStringLiteral("btnUp"));
        btnUp->setMaximumSize(QSize(20, 20));
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVGp/images/arrowDn.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnUp->setIcon(icon);
        btnUp->setIconSize(QSize(15, 15));

        horizontalLayout->addWidget(btnUp);

        btnDn = new QPushButton(groupBox);
        btnDn->setObjectName(QStringLiteral("btnDn"));
        btnDn->setMaximumSize(QSize(20, 20));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/SVGp/images/arrowUp.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDn->setIcon(icon1);
        btnDn->setIconSize(QSize(15, 15));

        horizontalLayout->addWidget(btnDn);

        btnClose = new QPushButton(groupBox);
        btnClose->setObjectName(QStringLiteral("btnClose"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnClose->sizePolicy().hasHeightForWidth());
        btnClose->setSizePolicy(sizePolicy);
        btnClose->setMaximumSize(QSize(20, 20));

        horizontalLayout->addWidget(btnClose);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, -1);
        wAxisValue = new wdgAxisValue(groupBox);
        wAxisValue->setObjectName(QStringLiteral("wAxisValue"));
        wAxisValue->setMinimumSize(QSize(60, 0));
        wAxisValue->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_2->addWidget(wAxisValue);

        wPlot = new wdgPlot(groupBox);
        wPlot->setObjectName(QStringLiteral("wPlot"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(wPlot->sizePolicy().hasHeightForWidth());
        wPlot->setSizePolicy(sizePolicy1);
        wPlot->setAutoFillBackground(true);
        verticalLayout_3 = new QVBoxLayout(wPlot);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        vLayoutSignName = new QVBoxLayout();
        vLayoutSignName->setSpacing(1);
        vLayoutSignName->setObjectName(QStringLiteral("vLayoutSignName"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vLayoutSignName->addItem(verticalSpacer_3);


        horizontalLayout_3->addLayout(vLayoutSignName);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        vLayoutAuxSignName = new QVBoxLayout();
        vLayoutAuxSignName->setSpacing(1);
        vLayoutAuxSignName->setObjectName(QStringLiteral("vLayoutAuxSignName"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vLayoutAuxSignName->addItem(verticalSpacer_2);


        horizontalLayout_3->addLayout(vLayoutAuxSignName);


        verticalLayout_3->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 229, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout_2->addWidget(wPlot);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(groupBox);


        retranslateUi(wdgGraphClass);

        QMetaObject::connectSlotsByName(wdgGraphClass);
    } // setupUi

    void retranslateUi(QWidget *wdgGraphClass)
    {
        wdgGraphClass->setWindowTitle(QApplication::translate("wdgGraphClass", "wGraph", 0));
        groupBox->setTitle(QString());
        btnAxisAttr->setText(QApplication::translate("wdgGraphClass", "Auto", 0));
#ifndef QT_NO_TOOLTIP
        btnUp->setToolTip(QApplication::translate("wdgGraphClass", "\320\237\320\265\321\200\320\265\320\274\320\265\321\201\321\202\320\270\321\202\321\214 \320\262\320\275\320\270\320\267", 0));
#endif // QT_NO_TOOLTIP
        btnUp->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnDn->setToolTip(QApplication::translate("wdgGraphClass", "\320\237\320\265\321\200\320\265\320\274\320\265\321\201\321\202\320\270\321\202\321\214 \320\262\320\262\320\265\321\200\321\205", 0));
#endif // QT_NO_TOOLTIP
        btnDn->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnClose->setToolTip(QApplication::translate("wdgGraphClass", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", 0));
#endif // QT_NO_TOOLTIP
        btnClose->setText(QApplication::translate("wdgGraphClass", "X", 0));
    } // retranslateUi

};

namespace Ui {
    class wdgGraphClass: public Ui_wdgGraphClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WDGGRAPH_H
