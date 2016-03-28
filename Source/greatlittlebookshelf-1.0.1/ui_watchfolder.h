/********************************************************************************
** Form generated from reading UI file 'watchfolder.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WATCHFOLDER_H
#define UI_WATCHFOLDER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_watchfolder
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QCheckBox *checkBox;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *watchfolder)
    {
        if (watchfolder->objectName().isEmpty())
            watchfolder->setObjectName(QString::fromUtf8("watchfolder"));
        watchfolder->resize(520, 165);
        watchfolder->setMinimumSize(QSize(520, 165));
        watchfolder->setMaximumSize(QSize(600, 200));
        gridLayout_2 = new QGridLayout(watchfolder);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        widget = new QWidget(watchfolder);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        checkBox = new QCheckBox(widget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        verticalLayout->addWidget(checkBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("folder")));
        pushButton->setIcon(icon);

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        verticalLayout_2->addWidget(widget);

        buttonBox = new QDialogButtonBox(watchfolder);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout_2->addWidget(buttonBox);


        gridLayout_2->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(watchfolder);
        QObject::connect(buttonBox, SIGNAL(accepted()), watchfolder, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), watchfolder, SLOT(reject()));

        QMetaObject::connectSlotsByName(watchfolder);
    } // setupUi

    void retranslateUi(QDialog *watchfolder)
    {
        watchfolder->setWindowTitle(QApplication::translate("watchfolder", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("watchfolder", "<html><head/><body><p><span style=\" font-weight:600;\">Note:</span> Monitoring directory for modifications consumes system resources. This implies there is a limit to the number of files and directories your process can monitor simultaneously. It is not advisable that you monitor large folders or drives.</p></body></html>", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("watchfolder", "Enable folder watching", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class watchfolder: public Ui_watchfolder {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WATCHFOLDER_H
