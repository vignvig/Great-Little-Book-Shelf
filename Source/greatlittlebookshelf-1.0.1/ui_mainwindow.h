/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_5;
    QWidget *main_widget;
    QGridLayout *gridLayout_6;
    QVBoxLayout *verticalLayout_3;
    QWidget *top_widget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *BackButton;
    QPushButton *BackToThisCatalogButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *FavoritePushButton;
    QWidget *middle_widget;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout;
    QListWidget *CatalogWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *progress_widget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *LabelPath;
    QProgressBar *progressBar;
    QWidget *toolBar_pdf;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pdfOriginalSize;
    QPushButton *pdfZoomIn;
    QPushButton *pdfZoomOut;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pdfFirst;
    QPushButton *pdfBack;
    QSpinBox *aSpinBox;
    QPushButton *pdfNext;
    QPushButton *pdfLast;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(611, 314);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/IMG/glebc_icon32.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setIconSize(QSize(24, 24));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_5 = new QGridLayout(centralWidget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        main_widget = new QWidget(centralWidget);
        main_widget->setObjectName(QString::fromUtf8("main_widget"));
        gridLayout_6 = new QGridLayout(main_widget);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        top_widget = new QWidget(main_widget);
        top_widget->setObjectName(QString::fromUtf8("top_widget"));
        top_widget->setMinimumSize(QSize(0, 28));
        gridLayout_2 = new QGridLayout(top_widget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 5, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        BackButton = new QPushButton(top_widget);
        BackButton->setObjectName(QString::fromUtf8("BackButton"));

        horizontalLayout_3->addWidget(BackButton);

        BackToThisCatalogButton = new QPushButton(top_widget);
        BackToThisCatalogButton->setObjectName(QString::fromUtf8("BackToThisCatalogButton"));

        horizontalLayout_3->addWidget(BackToThisCatalogButton);

        horizontalSpacer = new QSpacerItem(298, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        FavoritePushButton = new QPushButton(top_widget);
        FavoritePushButton->setObjectName(QString::fromUtf8("FavoritePushButton"));
        FavoritePushButton->setMinimumSize(QSize(153, 0));

        horizontalLayout_3->addWidget(FavoritePushButton);


        gridLayout_2->addLayout(horizontalLayout_3, 0, 0, 1, 1);


        verticalLayout_3->addWidget(top_widget);

        middle_widget = new QWidget(main_widget);
        middle_widget->setObjectName(QString::fromUtf8("middle_widget"));
        gridLayout_3 = new QGridLayout(middle_widget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 2);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        CatalogWidget = new QListWidget(middle_widget);
        CatalogWidget->setObjectName(QString::fromUtf8("CatalogWidget"));
        CatalogWidget->setMinimumSize(QSize(300, 0));
        QFont font;
        font.setPointSize(9);
        CatalogWidget->setFont(font);
        CatalogWidget->viewport()->setProperty("cursor", QVariant(QCursor(Qt::ArrowCursor)));
        CatalogWidget->setFocusPolicy(Qt::NoFocus);
        CatalogWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
        CatalogWidget->setFrameShape(QFrame::NoFrame);
        CatalogWidget->setFrameShadow(QFrame::Sunken);
        CatalogWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        CatalogWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        CatalogWidget->setEditTriggers(QAbstractItemView::SelectedClicked);
        CatalogWidget->setProperty("showDropIndicator", QVariant(false));
        CatalogWidget->setAlternatingRowColors(false);
        CatalogWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        CatalogWidget->setIconSize(QSize(130, 180));
        CatalogWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        CatalogWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        CatalogWidget->setMovement(QListView::Static);
        CatalogWidget->setFlow(QListView::LeftToRight);
        CatalogWidget->setProperty("isWrapping", QVariant(true));
        CatalogWidget->setResizeMode(QListView::Adjust);
        CatalogWidget->setLayoutMode(QListView::SinglePass);
        CatalogWidget->setSpacing(70);
        CatalogWidget->setGridSize(QSize(260, 300));
        CatalogWidget->setViewMode(QListView::IconMode);
        CatalogWidget->setUniformItemSizes(false);
        CatalogWidget->setBatchSize(100);
        CatalogWidget->setWordWrap(false);
        CatalogWidget->setSelectionRectVisible(true);
        CatalogWidget->setSortingEnabled(false);

        horizontalLayout->addWidget(CatalogWidget);

        scrollArea = new QScrollArea(middle_widget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setAutoFillBackground(true);
        scrollArea->setStyleSheet(QString::fromUtf8("background-color:transparent;"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 302, 212));
        scrollAreaWidgetContents->setStyleSheet(QString::fromUtf8("background-color:transparent;"));
        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout->addWidget(scrollArea);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 1);


        verticalLayout_3->addWidget(middle_widget);

        progress_widget = new QWidget(main_widget);
        progress_widget->setObjectName(QString::fromUtf8("progress_widget"));
        gridLayout = new QGridLayout(progress_widget);
        gridLayout->setSpacing(2);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 0);
        LabelPath = new QLabel(progress_widget);
        LabelPath->setObjectName(QString::fromUtf8("LabelPath"));

        verticalLayout->addWidget(LabelPath);

        progressBar = new QProgressBar(progress_widget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMinimumSize(QSize(0, 10));
        progressBar->setValue(50);
        progressBar->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(progressBar);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        verticalLayout_3->addWidget(progress_widget);

        toolBar_pdf = new QWidget(main_widget);
        toolBar_pdf->setObjectName(QString::fromUtf8("toolBar_pdf"));
        toolBar_pdf->setMinimumSize(QSize(0, 25));
        gridLayout_4 = new QGridLayout(toolBar_pdf);
        gridLayout_4->setSpacing(0);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pdfOriginalSize = new QPushButton(toolBar_pdf);
        pdfOriginalSize->setObjectName(QString::fromUtf8("pdfOriginalSize"));
        pdfOriginalSize->setMaximumSize(QSize(30, 30));
        QIcon icon1;
        QString iconThemeName = QString::fromUtf8("zoom-original");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        }
        pdfOriginalSize->setIcon(icon1);
        pdfOriginalSize->setFlat(true);

        horizontalLayout_4->addWidget(pdfOriginalSize);

        pdfZoomIn = new QPushButton(toolBar_pdf);
        pdfZoomIn->setObjectName(QString::fromUtf8("pdfZoomIn"));
        pdfZoomIn->setMaximumSize(QSize(30, 30));
        QIcon icon2;
        iconThemeName = QString::fromUtf8("zoom-in");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        }
        pdfZoomIn->setIcon(icon2);
        pdfZoomIn->setFlat(true);

        horizontalLayout_4->addWidget(pdfZoomIn);

        pdfZoomOut = new QPushButton(toolBar_pdf);
        pdfZoomOut->setObjectName(QString::fromUtf8("pdfZoomOut"));
        pdfZoomOut->setMaximumSize(QSize(30, 30));
        QIcon icon3;
        iconThemeName = QString::fromUtf8("zoom-out");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon3 = QIcon::fromTheme(iconThemeName);
        } else {
            icon3.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        }
        pdfZoomOut->setIcon(icon3);
        pdfZoomOut->setFlat(true);

        horizontalLayout_4->addWidget(pdfZoomOut);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        pdfFirst = new QPushButton(toolBar_pdf);
        pdfFirst->setObjectName(QString::fromUtf8("pdfFirst"));
        pdfFirst->setMaximumSize(QSize(30, 30));
        QIcon icon4;
        iconThemeName = QString::fromUtf8("go-first");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon4 = QIcon::fromTheme(iconThemeName);
        } else {
            icon4.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        }
        pdfFirst->setIcon(icon4);
        pdfFirst->setFlat(true);

        horizontalLayout_4->addWidget(pdfFirst);

        pdfBack = new QPushButton(toolBar_pdf);
        pdfBack->setObjectName(QString::fromUtf8("pdfBack"));
        pdfBack->setMinimumSize(QSize(108, 0));
        pdfBack->setMaximumSize(QSize(0, 30));
        QIcon icon5;
        iconThemeName = QString::fromUtf8("go-previous");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon5 = QIcon::fromTheme(iconThemeName);
        } else {
            icon5.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        }
        pdfBack->setIcon(icon5);
        pdfBack->setFlat(true);

        horizontalLayout_4->addWidget(pdfBack);

        aSpinBox = new QSpinBox(toolBar_pdf);
        aSpinBox->setObjectName(QString::fromUtf8("aSpinBox"));

        horizontalLayout_4->addWidget(aSpinBox);

        pdfNext = new QPushButton(toolBar_pdf);
        pdfNext->setObjectName(QString::fromUtf8("pdfNext"));
        pdfNext->setMinimumSize(QSize(108, 0));
        pdfNext->setMaximumSize(QSize(0, 30));
        QIcon icon6;
        iconThemeName = QString::fromUtf8("go-next");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon6 = QIcon::fromTheme(iconThemeName);
        } else {
            icon6.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        }
        pdfNext->setIcon(icon6);
        pdfNext->setFlat(true);

        horizontalLayout_4->addWidget(pdfNext);

        pdfLast = new QPushButton(toolBar_pdf);
        pdfLast->setObjectName(QString::fromUtf8("pdfLast"));
        pdfLast->setMaximumSize(QSize(30, 30));
        QIcon icon7;
        iconThemeName = QString::fromUtf8("go-last");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon7 = QIcon::fromTheme(iconThemeName);
        } else {
            icon7.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        }
        pdfLast->setIcon(icon7);
        pdfLast->setFlat(true);

        horizontalLayout_4->addWidget(pdfLast);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        gridLayout_4->addLayout(horizontalLayout_4, 0, 0, 1, 1);


        verticalLayout_3->addWidget(toolBar_pdf);


        gridLayout_6->addLayout(verticalLayout_3, 0, 0, 1, 1);


        gridLayout_5->addWidget(main_widget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        BackButton->setText(QApplication::translate("MainWindow", "PushButton", 0, QApplication::UnicodeUTF8));
        BackToThisCatalogButton->setText(QApplication::translate("MainWindow", "PushButton", 0, QApplication::UnicodeUTF8));
        FavoritePushButton->setText(QApplication::translate("MainWindow", "Favorites", 0, QApplication::UnicodeUTF8));
        LabelPath->setText(QApplication::translate("MainWindow", "Path...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pdfOriginalSize->setToolTip(QApplication::translate("MainWindow", "Original zoom", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pdfOriginalSize->setText(QString());
#ifndef QT_NO_TOOLTIP
        pdfZoomIn->setToolTip(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pdfZoomIn->setText(QString());
#ifndef QT_NO_TOOLTIP
        pdfZoomOut->setToolTip(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pdfZoomOut->setText(QString());
#ifndef QT_NO_TOOLTIP
        pdfFirst->setToolTip(QApplication::translate("MainWindow", "First", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pdfFirst->setText(QString());
#ifndef QT_NO_TOOLTIP
        pdfBack->setToolTip(QApplication::translate("MainWindow", "Back", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pdfBack->setText(QString());
        aSpinBox->setPrefix(QString());
#ifndef QT_NO_TOOLTIP
        pdfNext->setToolTip(QApplication::translate("MainWindow", "Next", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pdfNext->setText(QString());
#ifndef QT_NO_TOOLTIP
        pdfLast->setToolTip(QApplication::translate("MainWindow", "Last", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pdfLast->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
