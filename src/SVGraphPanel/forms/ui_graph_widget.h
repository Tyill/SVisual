/********************************************************************************
** Form generated from reading UI file 'graph_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPH_WIDGET_H
#define UI_GRAPH_WIDGET_H

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
#include "src/axis_value_widget.h"
#include "src/plot_widget.h"

QT_BEGIN_NAMESPACE

class Ui_GraphWidget
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
    AxisValueWidget *axisValue;
    PlotWidget *plot;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *vLayoutSignName;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *vLayoutAuxSignName;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *GraphWidget)
    {
        if (GraphWidget->objectName().isEmpty())
            GraphWidget->setObjectName(QStringLiteral("GraphWidget"));
        GraphWidget->resize(442, 400);
        GraphWidget->setStyleSheet(QStringLiteral(""));
        verticalLayout_2 = new QVBoxLayout(GraphWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(GraphWidget);
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
        axisValue = new AxisValueWidget(groupBox);
        axisValue->setObjectName(QStringLiteral("axisValue"));
        axisValue->setMinimumSize(QSize(60, 0));
        axisValue->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_2->addWidget(axisValue);

        plot = new PlotWidget(groupBox);
        plot->setObjectName(QStringLiteral("plot"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(plot->sizePolicy().hasHeightForWidth());
        plot->setSizePolicy(sizePolicy1);
        plot->setAutoFillBackground(true);
        verticalLayout_3 = new QVBoxLayout(plot);
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


        horizontalLayout_2->addWidget(plot);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(groupBox);


        retranslateUi(GraphWidget);

        QMetaObject::connectSlotsByName(GraphWidget);
    } // setupUi

    void retranslateUi(QWidget *GraphWidget)
    {
        GraphWidget->setWindowTitle(QApplication::translate("GraphWidget", "wGraph", Q_NULLPTR));
        groupBox->setTitle(QString());
#ifndef QT_NO_TOOLTIP
        btnAxisAttr->setToolTip(QApplication::translate("GraphWidget", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \320\276\321\201\320\270 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\271", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnAxisAttr->setText(QApplication::translate("GraphWidget", "Auto", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        btnUp->setToolTip(QApplication::translate("GraphWidget", "\320\237\320\265\321\200\320\265\320\274\320\265\321\201\321\202\320\270\321\202\321\214 \320\262\320\275\320\270\320\267", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnUp->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnDn->setToolTip(QApplication::translate("GraphWidget", "\320\237\320\265\321\200\320\265\320\274\320\265\321\201\321\202\320\270\321\202\321\214 \320\262\320\262\320\265\321\200\321\205", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnDn->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnClose->setToolTip(QApplication::translate("GraphWidget", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnClose->setText(QApplication::translate("GraphWidget", "X", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GraphWidget: public Ui_GraphWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPH_WIDGET_H
