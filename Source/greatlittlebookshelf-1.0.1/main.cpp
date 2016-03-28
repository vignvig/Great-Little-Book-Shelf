#include <QtGui/QApplication>
#include "mainwindow.h"
#include "QSharedMemory"
#include "QMessageBox"
#include <QStyleFactory>
bool m_isRunning;
QSharedMemory sharedMemory;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Great Little Book Shelf");
    a.setApplicationVersion("1.0.1");
    //------------------------------------------------------

    //STIL
    QStyle *styleOxygen = QStyleFactory::create("oxygen");
    QStyle *stylePlastique = QStyleFactory::create("plastique");
       if(styleOxygen) QApplication::setStyle(styleOxygen);
       else QApplication::setStyle(stylePlastique);
    //FONT STIL
    QFont useFont = a.font();
    useFont.setStyleStrategy(QFont::OpenGLCompatible);
    useFont.setStyleHint(QFont::SansSerif);
    useFont.setStretch(QFont::SemiCondensed);
    a.setFont(useFont);
    //-----------------------------------------------------

    sharedMemory.setKey("GreatLittleBookShelf");
    QMessageBox mb;

    if(sharedMemory.attach())
        m_isRunning = true;

    if (!sharedMemory.create(1))
        {
            mb.setWindowTitle(a.applicationName());
            mb.setIcon(QMessageBox::Warning);
            mb.setText("Already one instance of the program is running.\nIf this is happening more than once, kill this app with your task manager or reboot the system!");
            mb.exec();

            a.exit();
        }
            else
            {
            MainWindow w;
            return a.exec();
            }
    //----------------------------------------------------
}
