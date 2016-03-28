#include "watchfolder.h"
#include "ui_watchfolder.h"
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <qtreeview.h>
#include "QMessageBox"

watchfolder::watchfolder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::watchfolder)
{
    ui->setupUi(this);
    readSettings();

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(browseFolder()));
    connect(ui->checkBox,SIGNAL(released()),this,SLOT(EnableDisable()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(writeSettings()));

}
//---------------------------------------------------------------------
watchfolder::~watchfolder()
{
    delete ui;
}
//---------------------------------------------------------------------
void watchfolder::writeSettings()
{
    QSettings settings("glbs", QApplication::applicationName());
    settings.beginGroup("FolderWatch");
    settings.setValue("PathToWatch", ui->lineEdit->text());

    QDir aDir(ui->lineEdit->text());
    if (aDir.exists())
    settings.setValue("Enabled", ui->checkBox->isChecked());
    else
        settings.setValue("Enabled", false);

    settings.endGroup();
}
//---------------------------------------------------------------------
void watchfolder::readSettings()
{
    QSettings settings("glbs", QApplication::applicationName());
    settings.beginGroup("FolderWatch");
    PathToWatch = settings.value("PathToWatch", QDir::homePath()).toString();
    ui->checkBox->setChecked(settings.value("Enabled", false).toBool());

    settings.endGroup();

    //DO
    ui->lineEdit->setText(PathToWatch);
    EnableDisable();
}
//---------------------------------------------------------------------
void watchfolder::browseFolder()
{
    QString path = QFileDialog::getExistingDirectory(this, "Watch this folder", QDir::homePath(), QFileDialog::ShowDirsOnly  | QFileDialog::DontResolveSymlinks);
    QDir aDir(path);
    if (aDir.exists())
    {
    ui->lineEdit->setText(path);
    }
    else
    {
        QMessageBox mb;
        if (!path.isEmpty())
        mb.setText("Error! " + path + " is not valid directory");
        else
            mb.setText("Error! Directory information is empty, you need to set correct directory!");

    }
}
//---------------------------------------------------------------------
void watchfolder::EnableDisable()
{
    if (ui->checkBox->isChecked())
    {
        ui->lineEdit->setEnabled(true);
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->lineEdit->setEnabled(false);
        ui->pushButton->setEnabled(false);
    }

    writeSettings();
}
//---------------------------------------------------------------------
