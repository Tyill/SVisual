/********************************************************************************
** Form generated from reading UI file 'graphPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHPANEL_H
#define UI_GRAPHPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "src/wdgAxisTime.h"

QT_BEGIN_NAMESPACE

class Ui_graphPanelClass
{
public:
    QVBoxLayout *verticalLayout_4;
    QSplitter *splitter_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnResizeByAuto;
    QPushButton *btnResizeByTime;
    QPushButton *btnResizeByValue;
    QPushButton *btnUndo;
    QPushButton *btnColorUpdate;
    QPushButton *btnScalePos;
    QPushButton *btnScaleNeg;
    QPushButton *btnAScale;
    QPushButton *btnPlay;
    QSpacerItem *horizontalSpacer_3;
    QDateTimeEdit *dTimeBegin;
    QLabel *lbDTime;
    QDateTimeEdit *dTimeEnd;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_5;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    wdgAxisTime *axisTime;
    QSpacerItem *horizontalSpacer_2;
    QTableWidget *tblValues;

    void setupUi(QWidget *graphPanelClass)
    {
        if (graphPanelClass->objectName().isEmpty())
            graphPanelClass->setObjectName(QStringLiteral("graphPanelClass"));
        graphPanelClass->resize(796, 557);
        verticalLayout_4 = new QVBoxLayout(graphPanelClass);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(graphPanelClass);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        layoutWidget = new QWidget(splitter_2);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnResizeByAuto = new QPushButton(groupBox);
        btnResizeByAuto->setObjectName(QStringLiteral("btnResizeByAuto"));
        btnResizeByAuto->setMaximumSize(QSize(28, 28));
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVGp/images/iconResAuto.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnResizeByAuto->setIcon(icon);
        btnResizeByAuto->setIconSize(QSize(23, 23));

        horizontalLayout->addWidget(btnResizeByAuto);

        btnResizeByTime = new QPushButton(groupBox);
        btnResizeByTime->setObjectName(QStringLiteral("btnResizeByTime"));
        btnResizeByTime->setMaximumSize(QSize(28, 28));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/SVGp/images/iconResByTime.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnResizeByTime->setIcon(icon1);
        btnResizeByTime->setIconSize(QSize(23, 25));

        horizontalLayout->addWidget(btnResizeByTime);

        btnResizeByValue = new QPushButton(groupBox);
        btnResizeByValue->setObjectName(QStringLiteral("btnResizeByValue"));
        btnResizeByValue->setMaximumSize(QSize(28, 28));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/SVGp/images/iconResByValue.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnResizeByValue->setIcon(icon2);

        horizontalLayout->addWidget(btnResizeByValue);

        btnUndo = new QPushButton(groupBox);
        btnUndo->setObjectName(QStringLiteral("btnUndo"));
        btnUndo->setMaximumSize(QSize(28, 28));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/SVGp/images/bw.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnUndo->setIcon(icon3);

        horizontalLayout->addWidget(btnUndo);

        btnColorUpdate = new QPushButton(groupBox);
        btnColorUpdate->setObjectName(QStringLiteral("btnColorUpdate"));
        btnColorUpdate->setMaximumSize(QSize(28, 28));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/SVGp/images/color_wheel.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnColorUpdate->setIcon(icon4);

        horizontalLayout->addWidget(btnColorUpdate);

        btnScalePos = new QPushButton(groupBox);
        btnScalePos->setObjectName(QStringLiteral("btnScalePos"));
        btnScalePos->setMaximumSize(QSize(28, 28));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/SVGp/images/scalePos.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnScalePos->setIcon(icon5);
        btnScalePos->setIconSize(QSize(25, 25));
        btnScalePos->setAutoRepeat(true);

        horizontalLayout->addWidget(btnScalePos);

        btnScaleNeg = new QPushButton(groupBox);
        btnScaleNeg->setObjectName(QStringLiteral("btnScaleNeg"));
        btnScaleNeg->setMaximumSize(QSize(28, 28));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/SVGp/images/scaleNeg.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnScaleNeg->setIcon(icon6);
        btnScaleNeg->setIconSize(QSize(25, 25));
        btnScaleNeg->setAutoRepeat(true);

        horizontalLayout->addWidget(btnScaleNeg);

        btnAScale = new QPushButton(groupBox);
        btnAScale->setObjectName(QStringLiteral("btnAScale"));
        btnAScale->setMaximumSize(QSize(28, 28));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/SVGp/images/ascale.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAScale->setIcon(icon7);
        btnAScale->setIconSize(QSize(30, 30));
        btnAScale->setCheckable(true);
        btnAScale->setChecked(true);

        horizontalLayout->addWidget(btnAScale);

        btnPlay = new QPushButton(groupBox);
        btnPlay->setObjectName(QStringLiteral("btnPlay"));
        btnPlay->setMaximumSize(QSize(28, 28));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/SVGp/images/player_stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnPlay->setIcon(icon8);
        btnPlay->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(btnPlay);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        dTimeBegin = new QDateTimeEdit(groupBox);
        dTimeBegin->setObjectName(QStringLiteral("dTimeBegin"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dTimeBegin->sizePolicy().hasHeightForWidth());
        dTimeBegin->setSizePolicy(sizePolicy);
        dTimeBegin->setMaximumSize(QSize(130, 16777215));
        dTimeBegin->setFrame(true);
        dTimeBegin->setReadOnly(true);
        dTimeBegin->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout->addWidget(dTimeBegin);

        lbDTime = new QLabel(groupBox);
        lbDTime->setObjectName(QStringLiteral("lbDTime"));

        horizontalLayout->addWidget(lbDTime);

        dTimeEnd = new QDateTimeEdit(groupBox);
        dTimeEnd->setObjectName(QStringLiteral("dTimeEnd"));
        sizePolicy.setHeightForWidth(dTimeEnd->sizePolicy().hasHeightForWidth());
        dTimeEnd->setSizePolicy(sizePolicy);
        dTimeEnd->setMaximumSize(QSize(130, 16777215));
        dTimeEnd->setFrame(true);
        dTimeEnd->setReadOnly(true);
        dTimeEnd->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout->addWidget(dTimeEnd);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(100, 28));
        label->setPixmap(QPixmap(QString::fromUtf8(":/SVGp/images/SVLabel.png")));
        label->setScaledContents(true);

        horizontalLayout->addWidget(label);

        horizontalSpacer_5 = new QSpacerItem(25, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);


        verticalLayout->addWidget(groupBox);

        scrollArea = new QScrollArea(layoutWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setAcceptDrops(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 773, 174));
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, -1);
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(65, 50, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        axisTime = new wdgAxisTime(layoutWidget);
        axisTime->setObjectName(QStringLiteral("axisTime"));
        axisTime->setMinimumSize(QSize(50, 50));
        axisTime->setMaximumSize(QSize(16777215, 50));

        horizontalLayout_2->addWidget(axisTime);

        horizontalSpacer_2 = new QSpacerItem(25, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        splitter_2->addWidget(layoutWidget);
        tblValues = new QTableWidget(splitter_2);
        if (tblValues->columnCount() < 8)
            tblValues->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblValues->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblValues->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tblValues->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tblValues->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tblValues->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tblValues->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tblValues->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tblValues->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        if (tblValues->rowCount() < 5)
            tblValues->setRowCount(5);
        tblValues->setObjectName(QStringLiteral("tblValues"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tblValues->sizePolicy().hasHeightForWidth());
        tblValues->setSizePolicy(sizePolicy1);
        tblValues->setRowCount(5);
        splitter_2->addWidget(tblValues);

        verticalLayout_4->addWidget(splitter_2);


        retranslateUi(graphPanelClass);

        QMetaObject::connectSlotsByName(graphPanelClass);
    } // setupUi

    void retranslateUi(QWidget *graphPanelClass)
    {
        graphPanelClass->setWindowTitle(QApplication::translate("graphPanelClass", "graphPanel", 0));
        groupBox->setTitle(QString());
#ifndef QT_NO_TOOLTIP
        btnResizeByAuto->setToolTip(QApplication::translate("graphPanelClass", "\320\260\320\262\321\202\320\276\321\210\320\272\320\260\320\273\320\260 [CNTR+A]", 0));
#endif // QT_NO_TOOLTIP
        btnResizeByAuto->setText(QString());
        btnResizeByAuto->setShortcut(QApplication::translate("graphPanelClass", "Ctrl+A", 0));
#ifndef QT_NO_TOOLTIP
        btnResizeByTime->setToolTip(QApplication::translate("graphPanelClass", "\320\260\320\262\321\202\320\276\321\210\320\272\320\260\320\273\320\260 \320\277\320\276 \320\262\321\200\320\265\320\274\320\265\320\275\320\270  [CNTR+S]", 0));
#endif // QT_NO_TOOLTIP
        btnResizeByTime->setText(QString());
        btnResizeByTime->setShortcut(QApplication::translate("graphPanelClass", "Ctrl+S", 0));
#ifndef QT_NO_TOOLTIP
        btnResizeByValue->setToolTip(QApplication::translate("graphPanelClass", "\320\260\320\262\321\202\320\276\321\210\320\272\320\260\320\273\320\260 \320\277\320\276 \320\260\320\274\320\277\320\273\320\270\321\202\321\203\320\264\320\265 [CNTR+D]", 0));
#endif // QT_NO_TOOLTIP
        btnResizeByValue->setText(QString());
        btnResizeByValue->setShortcut(QApplication::translate("graphPanelClass", "Ctrl+D", 0));
#ifndef QT_NO_TOOLTIP
        btnUndo->setToolTip(QApplication::translate("graphPanelClass", "\320\262\320\265\321\200\320\275\321\203\321\202\321\214\321\201\321\217 [CNTR+Z]", 0));
#endif // QT_NO_TOOLTIP
        btnUndo->setText(QString());
        btnUndo->setShortcut(QApplication::translate("graphPanelClass", "Ctrl+Z", 0));
#ifndef QT_NO_TOOLTIP
        btnColorUpdate->setToolTip(QApplication::translate("graphPanelClass", "\321\201\320\274\320\265\320\275\320\270\321\202\321\214 \321\206\320\262\320\265\321\202\320\260  [CNTR+F]", 0));
#endif // QT_NO_TOOLTIP
        btnColorUpdate->setText(QString());
        btnColorUpdate->setShortcut(QApplication::translate("graphPanelClass", "Ctrl+F", 0));
#ifndef QT_NO_TOOLTIP
        btnScalePos->setToolTip(QApplication::translate("graphPanelClass", "\320\274\320\260\321\201\321\210\321\202\320\260\320\261 +", 0));
#endif // QT_NO_TOOLTIP
        btnScalePos->setText(QString());
        btnScalePos->setShortcut(QApplication::translate("graphPanelClass", "+", 0));
#ifndef QT_NO_TOOLTIP
        btnScaleNeg->setToolTip(QApplication::translate("graphPanelClass", "\320\274\320\260\321\201\321\210\321\202\320\260\320\261 -", 0));
#endif // QT_NO_TOOLTIP
        btnScaleNeg->setText(QString());
        btnScaleNeg->setShortcut(QApplication::translate("graphPanelClass", "-", 0));
#ifndef QT_NO_TOOLTIP
        btnAScale->setToolTip(QApplication::translate("graphPanelClass", "\320\260\320\262\321\202\320\276\321\210\320\272\320\260\320\273\320\260 [CTRL+G]", 0));
#endif // QT_NO_TOOLTIP
        btnAScale->setText(QString());
        btnAScale->setShortcut(QApplication::translate("graphPanelClass", "Ctrl+G", 0));
#ifndef QT_NO_TOOLTIP
        btnPlay->setToolTip(QApplication::translate("graphPanelClass", "\321\201\321\202\320\260\321\200\321\202-\321\201\321\202\320\276\320\277 [SPACE]", 0));
#endif // QT_NO_TOOLTIP
        btnPlay->setText(QString());
        btnPlay->setShortcut(QApplication::translate("graphPanelClass", "Space", 0));
        dTimeBegin->setDisplayFormat(QApplication::translate("graphPanelClass", "yy.MM.dd  HH:mm", 0));
        lbDTime->setText(QApplication::translate("graphPanelClass", "-", 0));
        dTimeEnd->setDisplayFormat(QApplication::translate("graphPanelClass", "yy.MM.dd  HH:mm", 0));
        label->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = tblValues->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("graphPanelClass", "\320\230\320\274\321\217", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tblValues->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("graphPanelClass", "\320\242\320\270\320\277", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tblValues->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("graphPanelClass", "X1", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tblValues->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("graphPanelClass", "X2", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tblValues->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("graphPanelClass", "X2 - X1", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tblValues->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("graphPanelClass", "Y1", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tblValues->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("graphPanelClass", "Y2", 0));
        QTableWidgetItem *___qtablewidgetitem7 = tblValues->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("graphPanelClass", "Y2 - Y1", 0));
    } // retranslateUi

};

namespace Ui {
    class graphPanelClass: public Ui_graphPanelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHPANEL_H
