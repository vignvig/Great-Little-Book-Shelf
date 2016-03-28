#include <QApplication>

#include "mainwindow.h"
#include "watchfolder.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore/QFile>
#include <QDir>
#include <QtCore/QTextStream>
#include "QTableWidgetItem"
#include "QMessageBox"
#include <poppler-qt4.h>
#include <QLabel>
#include <QDirIterator>
#include <QtDebug>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QMenu>
#include <QRegExp>
#include <QInputDialog>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QFont>
#include <QPainterPath>
#include <QDesktopServices>
#include <QUrl>
#include <QFileSystemModel>
#include <QSettings>
#include <qscrollbar.h>
#include "QStringRef"
#include <QString>
#include <QAction>
#include <QList>
#include <QStringList>
#include <QShortcut>
#include <QFileSystemWatcher>

//------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    testmsg.setText("Sorry!");//Brisati posle zadnjeg testiranja
    //testmsg.exec();

    show();
    readSettings();

    ui->CatalogWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    DeepnessLevel = 0;
    restoreX = 10;
    restoreY = 10;
    restoreW = 880;
    restoreH = 660;

    SetProgressWidget();
    SetToolBar1();

    currentPage = 1;
    scaleFactor = 0.20;
    scaleFactorForPreView = 1.0;

    tableWidget = new QTableWidget();
    myeBookDocument = new eBookDocument();
    eBookPage = 1;

    SetAppStyle();

    SloveIfCatalogFileNotExists();
    LoadCatalog(PathToCatalogFile);
    ShowCatalogs();
    CheckIfCatalogFilesStillExists();

    FolderWatcher = new QFileSystemWatcher(QStringList() << PathToWatch, this);


    Connecting();
    //---------------------------------------------------------
    FavoriteIsON = false;
    CheckIfThereIsFavoritesOrNot();

    ui->CatalogWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    installEventFilter(this);

    WatchingFolderDirectoryChanged();
}
//------------------------------------------------------------------------------------
void MainWindow::wheelEvent(QWheelEvent* event)
{
ui->CatalogWidget->verticalScrollBar()->setSingleStep(10);

    if (DeepnessLevel == 2) //sto znaci da je otvoren dokument
    {
        if (QApplication::keyboardModifiers() == Qt::CTRL )
        {
            if (event->delta() > 0)
                myeBookDocument->setScale(myeBookDocument->FactorSkaliranja + 0.075);
            else if (event->delta() < 0)
                myeBookDocument->setScale(myeBookDocument->FactorSkaliranja - 0.075);
        }
    }  
}

//------------------------------------------------------------------------------------

void MainWindow::GetAvaiableSkins()
{
try
    {
    QStringList nameFilter("*.qss");
    QString PathToSkinDir = "/opt/extras.ubuntu.com/glbs/styles/";
    QDir directory(PathToSkinDir);
    QStringList FilesAndDirectories = directory.entryList(nameFilter);

    QMenu *skins_menu = new QMenu;
    skins_menu->setTitle("Shelfs");
    skins_menu->setStyleSheet(SkinStyle);

    for(int i = 0; i < FilesAndDirectories.count(); i++)
        {
        QString aSkinFileString(FilesAndDirectories.at(i));
        aSkinFileString.chop(4);


        QAction *SkinItem = new QAction(skins_menu);
        QString SkinItemText(aSkinFileString);
        SkinItem->setText(QString::fromUtf8(SkinItemText.toStdString().c_str()));
        SkinItem->setVisible(true);
        connect(SkinItem, SIGNAL(triggered()), this, SLOT(SkinFromMenu()));
        connect(SkinItem, SIGNAL(hovered()), this, SLOT(SkinFromMenu()));
        skins_menu->addAction(SkinItem);
        }
    settings_menu->addMenu(skins_menu);
    }
    catch(...){}
}
//------------------------------------------------------------------------------------
void MainWindow::FontByNumber(int number)
{
ui->CatalogWidget->setStyleSheet("font:" + QString::number(number) + "px;");
fontsize = number;
}
//------------------------------------------------------------------------------------
void MainWindow::FontFromMenu()
{
    int number;
    QAction *A;
    QObject *Y = QObject::sender();
    A = qobject_cast<QAction *>(Y);
    if (Y)
        number = A->text().toInt();

    ui->CatalogWidget->setStyleSheet("font:" + QString::number(number) + "px;");
    ui->CatalogWidget->repaint();

    fontsize = number;
}
//------------------------------------------------------------------------------------
void MainWindow::SkinByName(QString skin)
{
    QString PathToSkinDir = "/opt/extras.ubuntu.com/glbs/styles/";
    try
    {
        QFile SkinFile(PathToSkinDir + QDir::separator() + skin + QString(".qss"));

        if (SkinFile.exists())
        {
        SkinFile.open(QFile::ReadOnly);

        QString ClearStyleSheet;
        ClearStyleSheet.clear();
        setStyleSheet(ClearStyleSheet);

        SkinStyle.clear();

        SkinStyle = QString(SkinFile.readAll());
        setStyleSheet(SkinStyle);
        SkinStyleName = skin;
        }
    }
    catch(...)
        {
        testmsg.setText("Can't load skin: " + skin);
        testmsg.exec();
        }
}

//------------------------------------------------------------------------------------

void MainWindow::SkinFromMenu()
{
QString skin;
    QAction *A;
    QObject *Y = QObject::sender();
    A = qobject_cast<QAction *>(Y);
    if (Y)
        skin = A->text();

    try
    {
        QString PathToSkinDir = "/opt/extras.ubuntu.com/glbs/styles/";
        QFile SkinFile(PathToSkinDir + QDir::separator() + skin + QString(".qss"));

        if (SkinFile.exists())
        {
        SkinFile.open(QFile::ReadOnly);

        QString ClearStyleSheet;
        ClearStyleSheet.clear();
        setStyleSheet(ClearStyleSheet);

        SkinStyle.clear();

        SkinStyle = QString(SkinFile.readAll());
        setStyleSheet(SkinStyle);
        SkinStyleName = skin;
        }
    }
    catch(...)
        {
        testmsg.setText("Can't load skin: " + skin);
        testmsg.exec();
        }
}
//------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    delete ui;
    writeSettings();
    QApplication::exit();
}

//------------------------------------------------------------------------------------
void MainWindow::CheckIfCatalogFilesStillExists()
{
    int oldRowNumber = tableWidget->rowCount();
    for (int i = tableWidget->rowCount()-1; i > 0; i--)
    {
        QFile aFile(tableWidget->item(i,1)->text());
        if (!aFile.exists())
        {
            tableWidget->removeRow(i);
        }
    }
    if (oldRowNumber != tableWidget->rowCount())
        {
        int difference = oldRowNumber-tableWidget->rowCount();

        QMessageBox msgBox;
        msgBox.setWindowTitle(QApplication::applicationName());
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Books missing!");
        if (difference > 1)
            msgBox.setInformativeText(QString::number(difference) + " books don't exists anymore, they were probably deleted or moved from previous locations. We will remove them from our database!");
        else if (difference == 1)
            msgBox.setInformativeText(QString::number(difference) + " book don't exists anymore, it is probably deleted or moved from previous location. We will remove it from our local database!");

        int ret = msgBox.exec();
        ShowCatalogs();
        }

    SaveCatalog();
}



//------------------------------------------------------------------------------------
void MainWindow::SetProgressWidget()
{
    ProgressWidget = new QWidget();
    ProgressWidget->setParent(this);
    ProgressWidget->setWindowFlags(Qt::ToolTip);
    ProgressWidget->move(500,200);
    Label = new QLabel(ProgressWidget);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(Label);
    ProgressWidget->setLayout(layout);
}

//------------------------------------------------------------------------------------

void MainWindow::SetToolBar1()
{
    ui->BackButton->setDisabled(true);
    ui->BackToThisCatalogButton->setDisabled(true);

    QActionGroup *ViewGroup = new QActionGroup(this);

    IconStyleOne = new QAction(this);
    IconStyleOne->setIconText("One (thrown)");
    IconStyleOne->setCheckable(true);
    IconStyleOne->setActionGroup(ViewGroup);

    IconStyleScattered = new QAction(this);
    IconStyleScattered->setIconText("Four (scattered)");
    IconStyleScattered->setCheckable(true);
    IconStyleScattered->setActionGroup(ViewGroup);

    IconStyleExt = new QAction(this);
    IconStyleExt->setIconText("Five (extracted)");
    IconStyleExt->setCheckable(true);
    IconStyleExt->setActionGroup(ViewGroup);

    IconStyleTwo = new QAction(this);
    IconStyleTwo->setIconText("Two (organized)");
    IconStyleTwo->setCheckable(true);
    IconStyleTwo->setActionGroup(ViewGroup);

    IconStyleTwoScattered = new QAction(this);
    IconStyleTwoScattered->setIconText("Two (scattered)");
    IconStyleTwoScattered->setCheckable(true);
    IconStyleTwoScattered->setActionGroup(ViewGroup);

    IconStyleThree = new QAction(this);
    IconStyleThree->setIconText("Three (organized)");
    IconStyleThree->setCheckable(true);
    IconStyleThree->setActionGroup(ViewGroup);

    IconStyleFiveInOrder = new QAction(this);
    IconStyleFiveInOrder->setIconText("Five (organized)");
    IconStyleFiveInOrder->setCheckable(true);
    IconStyleFiveInOrder->setActionGroup(ViewGroup);

    IconStyleInOrder = new QAction(this);
    IconStyleInOrder->setIconText("Four (organized)");
    IconStyleInOrder->setCheckable(true);
    IconStyleInOrder->setActionGroup(ViewGroup);
    //IconStyleInOrder->setChecked(true);

ui->CatalogWidget->setFocus();
}
//------------------------------------------------------------------------------------

void MainWindow::Connecting()
{
    connect(ui->CatalogWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
    connect(ui->BackButton,SIGNAL(clicked()),this,SLOT(BackToAllCatalogsButtonClick()));
    connect(ui->BackToThisCatalogButton,SIGNAL(clicked()),this,SLOT(BackToThisCatalogsButtonClick()));
    connect(ui->CatalogWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(CatalogBookClick(QListWidgetItem*)));
    connect(ui->pdfBack,SIGNAL(clicked()),this,SLOT(eBookGoBack()));
    connect(ui->pdfNext,SIGNAL(clicked()),this,SLOT(eBookGoNext()));
    connect(ui->pdfFirst,SIGNAL(clicked()),this,SLOT(eBookGoFirst()));
    connect(ui->pdfLast,SIGNAL(clicked()),this,SLOT(eBookGoLast()));
    connect(ui->aSpinBox,SIGNAL(valueChanged(int)),this,SLOT(eBookGoToPage()));
    connect(ui->pdfOriginalSize,SIGNAL(clicked()),this,SLOT(eBookOriginalSize()));
    connect(ui->pdfZoomIn,SIGNAL(clicked()),this,SLOT(eBookZoomIn()));
    connect(ui->pdfZoomOut,SIGNAL(clicked()),this,SLOT(eBookZoomOut()));

    connect(IconStyleOne,SIGNAL(triggered()),this,SLOT(SetOneStyle()));
    connect(IconStyleScattered,SIGNAL(triggered()),this,SLOT(SetScatteredStyle()));
    connect(IconStyleTwo,SIGNAL(triggered()),this,SLOT(SetTwoStyle()));
    connect(IconStyleTwoScattered,SIGNAL(triggered()),this,SLOT(SetTwoScatteredStyle()));
    connect(IconStyleThree,SIGNAL(triggered()),this,SLOT(SetThreeStyle()));
    connect(IconStyleInOrder,SIGNAL(triggered()),this,SLOT(SetInOrderStyle()));
    connect(IconStyleFiveInOrder,SIGNAL(triggered()),this,SLOT(SetFiveInOrderStyle()));
    connect(IconStyleExt,SIGNAL(triggered()),this,SLOT(SetExtStyle()));
    connect(IconStyleOne,SIGNAL(hovered()),this,SLOT(SetOneStyle()));
    connect(IconStyleScattered,SIGNAL(hovered()),this,SLOT(SetScatteredStyle()));
    connect(IconStyleTwo,SIGNAL(hovered()),this,SLOT(SetTwoStyle()));
    connect(IconStyleTwoScattered,SIGNAL(hovered()),this,SLOT(SetTwoScatteredStyle()));
    connect(IconStyleThree,SIGNAL(hovered()),this,SLOT(SetThreeStyle()));
    connect(IconStyleInOrder,SIGNAL(hovered()),this,SLOT(SetInOrderStyle()));
    connect(IconStyleFiveInOrder,SIGNAL(hovered()),this,SLOT(SetFiveInOrderStyle()));
    connect(IconStyleExt,SIGNAL(hovered()),this,SLOT(SetExtStyle()));

    connect(ui->FavoritePushButton,SIGNAL(clicked()),this,SLOT(FavoritesButtonClick()));

    connect(FolderWatcher,SIGNAL(directoryChanged(QString)),this,SLOT(WatchingFolderDirectoryChanged()));
}

//------------------------------------------------------------------------------------
void MainWindow::hit()
{
    myeBookDocument->showPageFit(currentPage);
}

//------------------------------------------------------------------------------------
void MainWindow::SettingsDialog()
{

}
//------------------------------------------------------------------------------------

void MainWindow::SetTwoStyle()
{
    ThumbStyle = "two";
    ShowCatalogs();
}

//------------------------------------------------------------------------------------

void MainWindow::SetTwoScatteredStyle()
{
    ThumbStyle = "twoscattered";
    ShowCatalogs();
}

//------------------------------------------------------------------------------------

void MainWindow::SetOneStyle()
{
    ThumbStyle = "one";
    ShowCatalogs();
}

//------------------------------------------------------------------------------------

void MainWindow::SetScatteredStyle()
{
    ThumbStyle = "scattered";
    ShowCatalogs();
}

//------------------------------------------------------------------------------------

void MainWindow::SetFiveInOrderStyle()
{
    ThumbStyle = "fiveInOrder";
    ShowCatalogs();
}

//------------------------------------------------------------------------------------

void MainWindow::SetInOrderStyle()
{
    ThumbStyle = "four";
    ShowCatalogs();
}

//------------------------------------------------------------------------------------
void MainWindow::SetExtStyle()
{
    ThumbStyle = "ext";
    ShowCatalogs();
}

void MainWindow::SetThreeStyle()
{
    ThumbStyle = "three";
    ShowCatalogs();
}

//------------------------------------------------------------------------------------

//---------------------------------SAVE---------------------------------------------------

void MainWindow::SaveCatalog()
{
    tableWidget->sortByColumn(0,Qt::AscendingOrder);

    QFile SaveFile(PathToCatalogFile);
    if (SaveFile.exists())
    {
             if(SaveFile.open(QIODevice::WriteOnly))
             {
                 QTextStream ts(&SaveFile);
                 QStringList strList;
                 for( int r = 0; r < tableWidget->rowCount(); ++r )
                    {
                     strList.clear();
                     for( int c = 0; c < tableWidget->columnCount(); ++c )
                        {
                        strList << "\"" + tableWidget->item(r, c)->text() + "\"";
                        }
                    ts << strList.join("|") + "\n";
                    }
                SaveFile.close();
             }
    }
}

//--------------------------------LOAD----------------------------------------------------

void MainWindow::LoadCatalog(QString PathToCatalogFile)
{
    QFile OpenFile(PathToCatalogFile);
    if (OpenFile.exists())
    {
            if(OpenFile.open(QIODevice::ReadOnly))
                {
                QTextStream ts( &OpenFile );
                QList<QStringList> list;
                int row = 0, col = 0;

                while( !ts.atEnd() )
                    {
                    list << ts.readLine().split( "|" );
                    }
                OpenFile.close();

                tableWidget->setRowCount( list.count() );
                tableWidget->setColumnCount( list[0].count() );

                tableWidget->setUpdatesEnabled( false );
                foreach( QStringList l, list )
                {
                    foreach( QString str, l )
                    {
                        if( str.endsWith( '"' ) ) str.chop(1);
                        if( str.startsWith( '"' ) ) str.remove(0,1);
                        tableWidget->setItem( row, col++, new QTableWidgetItem( str ));
                    }
                    row++; col=0;
                }
                tableWidget->setUpdatesEnabled( true );
            }
       }
}
//-------------------------------Solve if catalog file not exists-----------------------------------------------------
void MainWindow::SloveIfCatalogFileNotExists()
{
    //Provera da li postoji fajl sa katalogom i ako ne postoji, pravljenje jednog takvog fajla
    PathToCatalogDir = QDir::homePath() + "/.config/glbs/";
    PathToCatalogFile = PathToCatalogDir + "catalog.csv";

try
    {
    //Lokalni dir
    QDir D(PathToCatalogDir);
    if (!D.exists()) //Ako ne postoji lokalni dir
    {
    if (D.mkpath(PathToCatalogDir))
        {
        //
        }
        else
            {
            QMessageBox msgBoxDir;
            QString S1 = "ERROR! We did not made local 'Great Little PDF Catalog' path: ";
            QString S2 = ". Sorry!";
            msgBoxDir.setText(QString::fromUtf8(S1.toStdString().c_str()) + PathToCatalogDir + QString::fromUtf8(S2.toStdString().c_str()));
            msgBoxDir.exec();
            }
    }
    //Lokalni fajl
    QFile F(PathToCatalogFile);
    if (!F.exists())//Ako NE postoji lokalni falj
            {
            QFile InstaliraniFajlStanica("/opt/extras.ubuntu.com/glbs/catalog.csv");
            if (InstaliraniFajlStanica.exists())//Ako postoji instalirani fajl
                {
                F.copy(InstaliraniFajlStanica.fileName(),PathToCatalogFile);
                }
            }
    }
    catch(...){}
}
//------------------------------------------------------------------------------------

void MainWindow::BackToAllCatalogsButtonClick()
{
    DeepnessLevel = 0;
    ShowCatalogs();
    CheckIfThereIsFavoritesOrNot();
    repaint();
    ui->CatalogWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}
//------------------------------------------------------------------------------------

void MainWindow::BackToThisCatalogsButtonClick()
{
    DeepnessLevel = 1;
    ui->scrollArea->hide();


    //QString backtocatalogs("All Catalogs");

    ui->BackButton->setEnabled(true);
    ui->BackButton->setText("All Catalogs");
    ui->BackToThisCatalogButton->setDisabled(true);

    IconStyleScattered->setVisible(false);
    IconStyleTwo->setVisible(false);
    IconStyleFiveInOrder->setVisible(false);
    IconStyleInOrder->setVisible(false);
    IconStyleExt->setVisible(false);
    ui->toolBar_pdf->setVisible(false);
    currentPage = 1;
    ui->aSpinBox->setValue(1);

    //Sada se pravi privremeni itek koji bi simulisao kao da je kliknuta kategorija, upravo ona kojoj pripada otvorena knjiga.
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(CurrentOpenedCatalog);
    item->setStatusTip("Catalog");
    CatalogBookClick(item);

    ui->CatalogWidget->show();
    ui->CatalogWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    setWindowTitle("Catalog: " + CurrentOpenedCatalog);
    ui->FavoritePushButton->setText("Favorites");
}

//------------------------------------------------------------------------------------

void MainWindow::ShowCatalogs()
{
    ui->CatalogWidget->setSortingEnabled(true);
    ui->CatalogWidget->sortItems(Qt::AscendingOrder);

setWindowTitle("Catalog View");
ui->CatalogWidget->setSelectionMode(QAbstractItemView::SingleSelection);

if ((DeepnessLevel == 0) || (DeepnessLevel == 2))
{
    ui->scrollArea->hide();
    ui->CatalogWidget->show();

    IconStyleOne->setVisible(true);
    IconStyleScattered->setVisible(true);
    IconStyleTwo->setVisible(true);
    IconStyleFiveInOrder->setVisible(true);
    IconStyleInOrder->setVisible(true);
    IconStyleExt->setVisible(true);
    IconStyleTwoScattered->setVisible(true);
    IconStyleThree->setVisible(true);

    ui->BackButton->setDisabled(true);
    ui->BackButton->setText("");
    ui->BackToThisCatalogButton->setDisabled(true);
    ui->BackToThisCatalogButton->setText("");


    if (DeepnessLevel == 0)
    {
        ui->toolBar_pdf->setVisible(false);
    }

    //ui->toolBar->repaint();
    repaint();
    QDir FileThumbsDir(PathToCatalogDir + "FileThumbs");
    //QDir CatalogThumbsDir(PathToCatalogDir +  "CatalogThumbs");//QDir::separator()
        ui->CatalogWidget->clear();
        ui->CatalogWidget->setIconSize(QSize(262,350));

        QStringList CatalogStringList;
        CatalogStringList.clear();

        //Прављење листе каталога и фаворита
        int row = tableWidget->rowCount();
        int numFavorites = 0;
        //---------------------------
        for (int n = 0; n < row; n++)
            {
            if (!tableWidget->item(n,4)->text().isEmpty())
            numFavorites++;

            CatalogStringList << tableWidget->item(n,3)->text();
            }

        CatalogStringList.sort();
        CatalogStringList.removeDuplicates();
        //---------------------------
        //Убацивање каталога у Widget као item
        for (int i = 0; i < CatalogStringList.count(); i++)
            {
            QListWidgetItem *itemX;
            itemX = new QListWidgetItem(QIcon(":/image/IMG/empty_logo.png"), CatalogStringList.at(i), ui->CatalogWidget, QListWidgetItem::Type);
            if (ui->CatalogWidget->viewMode() == QListView::IconMode)
            itemX->setTextAlignment(Qt::AlignCenter); 
            itemX->setStatusTip("Catalog");
            }
        //--------------------

        for (int d = 0; d < ui->CatalogWidget->count(); d++)
        {
            QStringList FirstFourItems, FirstFourFavoriteItems;
            FirstFourItems.clear();
            FirstFourFavoriteItems.clear();

            for (int n = 0; n < tableWidget->rowCount(); n++) //Napravi listu prvih cetiri knjiga koji pripadaju toj kategoriji
                {
                //Obicna kategorija
                if (ui->CatalogWidget->item(d)->text() == tableWidget->item(n,3)->text())
                    {
                    FirstFourItems << tableWidget->item(n,1)->text();
                    }
                /*
                //Favoriti
                if (ui->CatalogWidget->item(d)->text() == tableWidget->item(n,4)->text())
                    {
                    FirstFourFavoriteItems << tableWidget->item(n,1)->text();
                    }*/

                if (FirstFourItems.count() > 4) break;
                }

            try
                {
                QImage *FinishedCatalogImage = new QImage;
                FinishedCatalogImage->load(QString(":/image/IMG/empty.png"), 0);
                QPainter paint;
                QTransform trans;
                QRect rectangle(-200,-200,600,600);//-200,-200,600,600);
                //paint.resetTransform();
                paint.eraseRect(rectangle);
                paint.fillRect(rectangle,QColor(0,0,0,0));
                paint.setRenderHint(QPainter::SmoothPixmapTransform);
                //paint.setBackground(QColor(0, 0, 0, 0));

                paint.begin(FinishedCatalogImage);
                //Obicna kategorija
                for(int R = 0; R < FirstFourItems.count(); R++)
                    {
                    QFile CheckIfFileExists(FirstFourItems.at(R));

                    if (CheckIfFileExists.exists())
                    {
                    QFile NonExistingFile(FileThumbsDir.path() + FirstFourItems.at(R) + ".png");
                    if (NonExistingFile.exists())
                        {

                        paint.setCompositionMode(QPainter::CompositionMode_Source);
                        paint.setBackground(QColor(0, 0, 0, 0));
                        int w = FinishedCatalogImage->width();
                        int w2 = FinishedCatalogImage->width()/2;
                        int h = FinishedCatalogImage->height();
                        int h2 = FinishedCatalogImage->height()/2;
                        //-----------------------------------------------------------
                        if (ThumbStyle == "scattered")
                        {
                        //
                        switch( R )
                        {
                        case 0:
                            {
                            QImage image0;
                            image0.load(NonExistingFile.fileName());
                            paint.rotate(-15);
                            paint.setRenderHint(QPainter::Antialiasing);
                            if (image0.width() < image0.height())
                                {
                                if (image0.width() < (image0.height()/2))
                                    paint.drawPixmap(QRect(15,10,25,60),QPixmap::fromImage(image0.scaled(QSize(40,60),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                else
                                paint.drawPixmap(QRect(10,10,40,60),QPixmap::fromImage(image0.scaled(QSize(40,60),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                }
                                else
                                {
                                paint.drawPixmap(QRect(5,15,60,40),QPixmap::fromImage(image0.scaled(QSize(60,40),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                }
                            }
                            break ;
                        case 1:
                            {
                            QImage image1;
                            image1.load(NonExistingFile.fileName());
                            paint.rotate(18);
                            paint.setRenderHint(QPainter::Antialiasing);
                            if (image1.width() < image1.height())
                                {
                                if (image1.width() < (image1.height()/2))
                                    paint.drawPixmap(QRect(45,5,30,70),QPixmap::fromImage(image1.scaled(QSize(50,70),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                else
                                paint.drawPixmap(QRect(40,5,50,70),QPixmap::fromImage(image1.scaled(QSize(50,70),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                }
                                else
                                {
                                paint.drawPixmap(QRect(25,10,70,50),QPixmap::fromImage(image1.scaled(QSize(70,50),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                }
                            }
                            break ;
                        case 2:
                            {
                            QImage image2;
                            image2.load(NonExistingFile.fileName());
                            paint.rotate(14);
                            paint.setRenderHint(QPainter::Antialiasing);
                            if (image2.width() < image2.height())
                                {
                                if (image2.width() < (image2.height()/2))
                                    paint.drawPixmap(QRect(70,10,35,85),QPixmap::fromImage(image2.scaled(QSize(60,80),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                else
                                paint.drawPixmap(QRect(65,10,60,80),QPixmap::fromImage(image2.scaled(QSize(60,80),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                }
                                else
                                {
                                paint.drawPixmap(QRect(50,15,80,60),QPixmap::fromImage(image2.scaled(QSize(80,60),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                }
                            }
                            break ;
                        case 3:
                            {
                            QImage image3;
                            image3.load(NonExistingFile.fileName());
                            paint.rotate(15);
                            paint.setRenderHint(QPainter::Antialiasing);
                            if (image3.width() < image3.height())
                                {
                                if (image3.width() < (image3.height()/2))
                                    paint.drawPixmap(QRect(95,15,40,100),QPixmap::fromImage(image3.scaled(QSize(75,100),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                else
                                paint.drawPixmap(QRect(80,15,75,100),QPixmap::fromImage(image3.scaled(QSize(75,100),Qt::KeepAspectRatio,Qt::SmoothTransformation)));
                                }
                                else
                                {
                                paint.drawPixmap(QRect(60,25,100,75),QPixmap::fromImage(image3.scaled(QSize(100,75),Qt::KeepAspectRatio,Qt::SmoothTransformation)));
                                }
                            }
                            break ;
                        }
                    }
                        //-----------------------------------------------------------
                        else if (ThumbStyle == "four")
                        {
                            switch( R )
                            {
                            case 0:
                                {
                                QImage image0;
                                image0.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image0.width() < image0.height())
                                    {
                                    if (image0.width() < (image0.height()/2))
                                        paint.drawPixmap(QRect((w2/2)-14,0,30,78),QPixmap::fromImage(image0.scaled(QSize(58,78),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect((w2/2)-29,0,58,78), QPixmap::fromImage(image0.scaled(QSize(58,78),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect((w2/2)-29,0,58,48), QPixmap::fromImage(image0.scaled(QSize(48,58),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 1:
                                {
                                QImage image1;
                                image1.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image1.width() < image1.height())
                                    {
                                    if (image1.width() < (image1.height()/2))
                                        paint.drawPixmap(QRect((w2+(w2/2))-14,0,30,78),QPixmap::fromImage(image1.scaled(QSize(58,78),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect((w2+w2)-58,0,58,78),QPixmap::fromImage(image1.scaled(QSize(58,78),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect(w2+(w2-58),0,58,48),QPixmap::fromImage(image1.scaled(QSize(48,58),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                }
                                }
                                break ;
                            case 2:
                                {
                                QImage image2;
                                image2.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image2.width() < image2.height())
                                    {
                                    if (image2.width() < (image2.height()/2))
                                        paint.drawPixmap(QRect((w2/2)-14,82,30,78),QPixmap::fromImage(image2.scaled(QSize(58,78),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect((w2/2)-29,82,58,78),QPixmap::fromImage(image2.scaled(QSize(58,78),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect((w2/2)-29,82,58,48),QPixmap::fromImage(image2.scaled(QSize(48,58),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                }
                                }
                                break ;
                            case 3:
                                {
                                QImage image3;
                                image3.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image3.width() < image3.height())
                                    {
                                    if (image3.width() < (image3.height()/2))
                                        paint.drawPixmap(QRect(w2+(w2/2)-14,82,30,78),QPixmap::fromImage(image3.scaled(QSize(58,78),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(w2+(w2-58),82,58,78),QPixmap::fromImage(image3.scaled(QSize(58,78),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect(w2+(w2-58),82,58,48),QPixmap::fromImage(image3.scaled(QSize(48,58),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                }
                                }
                                break ;
                            }
                        }
                        //-----------------------------------------------------------
                        else if (ThumbStyle == "fiveInOrder")
                        {

                         QPen Pen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                            switch( R )
                            {
                            case 0:
                                {
                                QImage image0;
                                image0.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image0.width() < image0.height())
                                    {
                                    if (image0.width() < (image0.height()/2))
                                        paint.drawPixmap(QRect((w2/2)-14,0,30,70),QPixmap::fromImage(image0.scaled(QSize(50,70),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect((w2-50)-5,0,50,70),QPixmap::fromImage(image0.scaled(QSize(50,70),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                else
                                    {
                                    paint.drawPixmap(QRect((w2-55)-5,0,55,40),QPixmap::fromImage(image0.scaled(QSize(55,40),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 1:
                                {
                                QImage image1;
                                image1.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image1.width() < image1.height())
                                    {
                                    if (image1.width() < (image1.height()/2))
                                        paint.drawPixmap(QRect((w2+(w2/2))-14,0,30,70),QPixmap::fromImage(image1.scaled(QSize(50,70),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(((w2*2)-50)-5,0,50,70),QPixmap::fromImage(image1.scaled(QSize(50,70),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                else
                                    {
                                    paint.drawPixmap(QRect(((w2*2)-55)-5,0,55,40),QPixmap::fromImage(image1.scaled(QSize(55,40),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 2:
                                {
                                QImage image2;
                                image2.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image2.width() < image2.height())
                                    {
                                    if (image2.width() < (image2.height()/2))
                                        paint.drawPixmap(QRect((w2/2)-14,92,30,70),QPixmap::fromImage(image2.scaled(QSize(50,70),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect((w2-50)-5,92,50,70),QPixmap::fromImage(image2.scaled(QSize(50,70),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                else
                                    {
                                    paint.drawPixmap(QRect((w2-55)-5,122,55,40),QPixmap::fromImage(image2.scaled(QSize(55,40),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 3:
                                {
                                QImage image3;
                                image3.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image3.width() < image3.height())
                                    {
                                    if (image3.width() < (image3.height()/2))
                                        paint.drawPixmap(QRect(w2+(w2/2)-14,92,30,70),QPixmap::fromImage(image3.scaled(QSize(50,70),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(((w2*2)-50)-5,92,50,70),QPixmap::fromImage(image3.scaled(QSize(50,70),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                else
                                    {
                                    paint.drawPixmap(QRect(((w2*2)-55)-5,122,55,40),QPixmap::fromImage(image3.scaled(QSize(55,40),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 4:
                                {
                                QImage image4;
                                image4.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                QRect aRect1;
                                if (image4.width() < image4.height())
                                    {
                                    if (image4.width() < (image4.height()/2))
                                        {
                                        aRect1 = QRect(w2-18,h2-48,36,95);
                                        paint.drawPixmap(QRect(w2-18,h2-48,36,95),QPixmap::fromImage(image4.scaled(QSize(75,95),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                        }
                                        else
                                        {
                                        aRect1 = QRect(w2-37,h2-48,75,95);
                                        paint.drawPixmap(QRect(w2-37,h2-48,75,95),QPixmap::fromImage(image4.scaled(QSize(75,95),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                        }
                                    }
                                else
                                    {
                                    aRect1 = QRect(w2-50,h2-37,100,75);
                                    paint.drawPixmap(QRect(w2-50,h2-37,100,75),QPixmap::fromImage(image4.scaled(QSize(100,75),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }

                                Pen.setWidth(1);
                                paint.setPen(Pen);
                                paint.drawRect(aRect1);
                                }
                                break ;
                            }
                        }
                        else if (ThumbStyle == "two")
                        {
                            switch( R )
                            {
                            case 0:
                                {
                                QImage image0;
                                image0.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image0.width() < image0.height())
                                    {
                                    if (image0.width() < (image0.height()/2))
                                        paint.drawPixmap(QRect((w2/2)-15,0,30,90),QPixmap::fromImage(image0.scaled(QSize(30,90),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(0,0,68,90), QPixmap::fromImage(image0.scaled(QSize(68,90),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect(0,0,90,60), QPixmap::fromImage(image0.scaled(QSize(90,60),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 1:
                                {
                                QImage image1;
                                image1.load(NonExistingFile.fileName());
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image1.width() < image1.height())
                                    {
                                    if (image1.width() < (image1.height()/2))
                                        paint.drawPixmap(QRect(w2+(w2/2)-20,h-130,40,130),QPixmap::fromImage(image1.scaled(QSize(40,130),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(w-95,h-125,95,125),QPixmap::fromImage(image1.scaled(QSize(95,125),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect(w-120,h-88,120,88),QPixmap::fromImage(image1.scaled(QSize(120,88),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            }
                        }
                        else if (ThumbStyle == "twoscattered")
                        {
                            switch( R )
                            {
                            case 0:
                                {
                                QImage image0;
                                image0.load(NonExistingFile.fileName());
                                paint.rotate(-9);
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image0.width() < image0.height())
                                    {
                                    if (image0.width() < (image0.height()/2))
                                        paint.drawPixmap(QRect(20,15,30,90),QPixmap::fromImage(image0.scaled(QSize(30,90),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(-5,10,90,120), QPixmap::fromImage(image0.scaled(QSize(90,120),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect(-5,15,125,95), QPixmap::fromImage(image0.scaled(QSize(125,95),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 1:
                                {
                                QImage image1;
                                image1.load(NonExistingFile.fileName());
                                paint.rotate(22);
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image1.width() < image1.height())
                                    {
                                    if (image1.width() < (image1.height()/2))
                                        paint.drawPixmap(QRect(75,25,40,120),QPixmap::fromImage(image1.scaled(QSize(40,120),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(45,30,90,120),QPixmap::fromImage(image1.scaled(QSize(90,120),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect(30,55,120,88),QPixmap::fromImage(image1.scaled(QSize(120,88),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            }
                        }
                        else if (ThumbStyle == "one")
                        {
                            switch( R )
                            {
                            case 0:
                                {
                                QImage image0;
                                image0.load(NonExistingFile.fileName());
                                paint.rotate(-5);
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image0.width() < image0.height())
                                    {
                                    if (image0.width() < (image0.height()/2))
                                        paint.drawPixmap(QRect(30,13,50,155),QPixmap::fromImage(image0.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(-3,13,110,155), QPixmap::fromImage(image0.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect(-13,25,155,110), QPixmap::fromImage(image0.scaled(QSize(350,262),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;

                            }
                        }
                        else if (ThumbStyle == "three")
                        {
                            trans.reset();
                            paint.resetTransform();
                            switch( R )
                            {
                            case 0:
                                {
                                QImage image0;
                                image0.load(NonExistingFile.fileName());
                                paint.resetTransform();
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image0.width() < image0.height())
                                    {
                                    if (image0.width() < (image0.height()/2))
                                        paint.drawPixmap(QRect(25,0,25,80),QPixmap::fromImage(image0.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(5,0,55,80), QPixmap::fromImage(image0.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect(0,0,75,50), QPixmap::fromImage(image0.scaled(QSize(350,262),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 1:
                                {
                                QImage image1;
                                image1.load(NonExistingFile.fileName());
                                paint.resetTransform();
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image1.width() < image1.height())
                                    {
                                    if (image1.width() < (image1.height()/2))
                                        paint.drawPixmap(QRect(((w/3)*2)-16,15,32,100),QPixmap::fromImage(image1.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(50,15,70,100), QPixmap::fromImage(image1.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    int w = (FinishedCatalogImage->width()/2);
                                    paint.drawPixmap(QRect(30,30,100,70), QPixmap::fromImage(image1.scaled(QSize(350,262),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 2:
                                {
                                QImage image2;
                                image2.load(NonExistingFile.fileName());
                                paint.resetTransform();
                                paint.setRenderHint(QPainter::Antialiasing);
                                if (image2.width() < image2.height())
                                    {
                                    if (image2.width() < (image2.height()/2))
                                        paint.drawPixmap(QRect(w2-22,h-120,44,120),QPixmap::fromImage(image2.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(w2-40,h-120,80,120), QPixmap::fromImage(image2.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    paint.drawPixmap(QRect(w2-60,(h-90),120,80), QPixmap::fromImage(image2.scaled(QSize(350,262),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            }
                        }
                        else if (ThumbStyle == "ext")
                        {
                            trans.reset();

                            switch( R )
                            {
                            case 0:
                                {
                                QImage image0;
                                image0.load(NonExistingFile.fileName());
                                paint.resetTransform();
                                paint.setRenderHint(QPainter::Antialiasing);

                                if (image0.width() < image0.height())
                                    {
                                    trans.rotate(-35,Qt::YAxis);
                                    paint.setTransform(trans);
                                    if (image0.width() < (image0.height()/2))
                                        paint.drawPixmap(QRect(5,0,50,150),QPixmap::fromImage(image0.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(0,0,110,150),QPixmap::fromImage(image0.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    trans.rotate(-40,Qt::YAxis);
                                    paint.setTransform(trans);
                                    paint.drawPixmap(QRect(0,5,155,110), QPixmap::fromImage(image0.scaled(QSize(350,262),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 1:
                                {
                                QImage image1;
                                image1.load(NonExistingFile.fileName());
                                paint.resetTransform();
                                paint.setRenderHint(QPainter::Antialiasing);
                                paint.setTransform(trans);
                                if (image1.width() < image1.height())
                                    {
                                    trans.rotate(-35,Qt::YAxis);
                                    paint.setTransform(trans);
                                    if (image1.width() < (image1.height()/2))
                                        paint.drawPixmap(QRect(28,5,50,150),QPixmap::fromImage(image1.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(15,8,110,150),QPixmap::fromImage(image1.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    trans.rotate(-40,Qt::YAxis);
                                    paint.setTransform(trans);
                                    paint.drawPixmap(QRect(10,20,155,110), QPixmap::fromImage(image1.scaled(QSize(350,262),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 2:
                                {
                                QImage image2;
                                image2.load(NonExistingFile.fileName());
                                paint.resetTransform();
                                paint.setRenderHint(QPainter::Antialiasing);
                                paint.setTransform(trans);
                                if (image2.width() < image2.height())
                                    {
                                    trans.rotate(-35,Qt::YAxis);
                                    paint.setTransform(trans);
                                    if (image2.width() < (image2.height()/2))
                                        paint.drawPixmap(QRect(51,10,50,150),QPixmap::fromImage(image2.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(25,16,110,150),QPixmap::fromImage(image2.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    trans.rotate(-40,Qt::YAxis);
                                    paint.setTransform(trans);
                                    paint.drawPixmap(QRect(20,35,155,110), QPixmap::fromImage(image2.scaled(QSize(350,262),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 3:
                                {
                                QImage image3;
                                image3.load(NonExistingFile.fileName());
                                paint.resetTransform();
                                paint.setTransform(trans);
                                if (image3.width() < image3.height())
                                    {
                                    trans.rotate(-35,Qt::YAxis);
                                    paint.setTransform(trans);
                                    if (image3.width() < (image3.height()/2))
                                        paint.drawPixmap(QRect(74,15,50,150),QPixmap::fromImage(image3.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(35,24,110,150),QPixmap::fromImage(image3.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    trans.rotate(-40,Qt::YAxis);
                                    paint.setTransform(trans);
                                    paint.drawPixmap(QRect(30,50,155,110), QPixmap::fromImage(image3.scaled(QSize(350,262),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;
                            case 4:
                                {
                                QImage image4;
                                image4.load(NonExistingFile.fileName());
                                paint.resetTransform();
                                paint.setTransform(trans);
                                if (image4.width() < image4.height())
                                    {
                                    trans.rotate(-35,Qt::YAxis);
                                    paint.setTransform(trans);
                                    if (image4.width() < (image4.height()/2))
                                        paint.drawPixmap(QRect(97,20,50,150),QPixmap::fromImage(image4.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    else
                                    paint.drawPixmap(QRect(45,32,110,150),QPixmap::fromImage(image4.scaled(QSize(262,350),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                    else
                                    {
                                    trans.rotate(-40,Qt::YAxis);
                                    paint.setTransform(trans);
                                    paint.drawPixmap(QRect(40,65,155,110), QPixmap::fromImage(image4.scaled(QSize(350,262),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
                                    }
                                }
                                break ;

                            }
                        }
                        }
                        else
                        {

                            //------------------------------AKO NEMA THUMBS-----------------------------
                            QColor ProvidnoBeloNoThumb(255,255,255,10);
                            QColor BeloNoThumb(255,255,255,220);
                            QPen krugNoThumb(ProvidnoBeloNoThumb, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

                            paint.begin(FinishedCatalogImage);
                            paint.setCompositionMode(QPainter::CompositionMode_Source);
                            //paint.setBackground(QColor(0, 0, 0, 0));
                            //paint.resetTransform();

                            QBrush PopunaNoThumb(ProvidnoBeloNoThumb, Qt::SolidPattern);
                            paint.setPen(krugNoThumb);
                            paint.setBrush(PopunaNoThumb);
                            paint.setRenderHint(QPainter::Antialiasing);
                            paint.drawEllipse(QRect(1,(FinishedCatalogImage->height()-FinishedCatalogImage->width())/2,FinishedCatalogImage->width()-2,FinishedCatalogImage->width()-2));

                            QPen CircleNoThumb(BeloNoThumb, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
                            CircleNoThumb.setWidth(1);
                            paint.setPen(CircleNoThumb);
                            paint.setBrush(Qt::NoBrush);
                            paint.setRenderHint(QPainter::Antialiasing);
                            paint.drawEllipse(QRect(1,(FinishedCatalogImage->height()-FinishedCatalogImage->width())/2,FinishedCatalogImage->width()-2,FinishedCatalogImage->width()-2));

                            QPen PenNoThumb(BeloNoThumb, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                            PenNoThumb.setWidth(2);
                            paint.setPen(PenNoThumb);
                            QFont serifFont("Ariel", 12, QFont::Light);
                            paint.setFont(serifFont);
                            paint.setRenderHint(QPainter::Antialiasing);
                            paint.drawText(QRect(1,(FinishedCatalogImage->height()-FinishedCatalogImage->width())/2,FinishedCatalogImage->width()-2,FinishedCatalogImage->width()-2),Qt::AlignCenter, "NO PREVIEW\nopen to generate");
                            //--------------------------------------------------------------------------
                        }
                    }
                }
                int knjigaukatalogu = 0;
                //---------------------------
                for (int n = 0; n < tableWidget->rowCount(); n++)
                    {
                    if (tableWidget->item(n,3)->text() == ui->CatalogWidget->item(d)->text())
                        knjigaukatalogu++;
                    }
                //------------------------------------------
                QColor Belo1(255,255,255,220);
                QColor Belo2(255,255,255,160);
                QColor ProvidnoBelo(255,255,255,10);
                QBrush Popuna(ProvidnoBelo, Qt::SolidPattern);
                QString knjigaukataloguString(QString::number(knjigaukatalogu));
                QRect myrect(FinishedCatalogImage->width()-30,FinishedCatalogImage->height()-30, 27, 27);//
                QPen krug(ProvidnoBelo, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                QPen krug2(Belo2, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);

                paint.resetTransform();

                    //Pun krug
                    paint.setPen(krug);
                    paint.setBrush(Popuna);
                    paint.drawEllipse(myrect);

                    //Prsten kruga
                    krug2.setWidth(1);
                    paint.setPen(krug2);
                    paint.setBrush(Qt::NoBrush);
                    paint.drawEllipse(myrect);

                    //broj u krugu
                    QPen Pen2(Belo1, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                    Pen2.setWidth(2);
                    paint.setPen(Pen2);
                    QFont serifFont("Ariel", 12, QFont::Light);
                    paint.setFont(serifFont);
                    paint.drawText(myrect,Qt::AlignCenter, knjigaukataloguString);

                    QIcon ItemIcon;
                    ItemIcon.addPixmap(QPixmap::fromImage( *FinishedCatalogImage, Qt::AutoColor));
                    ui->CatalogWidget->item(d)->setIcon(ItemIcon);

                //------------------------------------------
                paint.resetTransform();
                paint.eraseRect(rectangle);
                paint.fillRect(rectangle,QColor(0,0,0,0));
                paint.end();
                }catch(...){}
        }
        //--------------------

setWindowTitle(QApplication::applicationName());
}
//ui->FavoritePushButton->setText("Favorites");
FavoriteIsON = false;
}

//------------------------------------------------------------------------------------

void MainWindow::CatalogBookClick(QListWidgetItem *item)
{
    ClickedItemText = item->text();
    ClickedItemWhatsThis = item->whatsThis();
    ClickedItemStatusTip = item->statusTip();
    ClickedItemToolTip = item->toolTip();

int itemcount = 0;
    for (int i = 0; i < tableWidget->rowCount(); i++)
        {
        if (tableWidget->item(i,3)->text() == item->text())
            {
            itemcount++;
            }
        }
    ui->progressBar->setMaximum(itemcount);
    ui->progressBar->setValue(0);
    ui->progress_widget->show();

    if(!item->text().isEmpty())
        {
        ui->CatalogWidget->setIconSize(QSize(262,350));

        if (ClickedItemStatusTip == "Catalog")
            {
            CatalogClick(item);
            }
            else if (ClickedItemStatusTip == "Book")
                {
                BookClick(item);
                }
        }
ui->progress_widget->hide();
repaint();
}

//------------------------------------------------------------------------------------
void MainWindow::LoadThumbsOrGenerateOne()
{
    ui->progressBar->setMaximum(ui->CatalogWidget->count());
    ui->progressBar->setValue(0);
    ui->progressBar->repaint();
ui->CatalogWidget->repaint();
for (int i = 0; i < ui->CatalogWidget->count(); i++)
    {
    QDir ThumbsDir(PathToCatalogDir + "FileThumbs");
    QFile NonExistingFile(ThumbsDir.path() + ui->CatalogWidget->item(i)->whatsThis());
    QFileInfo FileInfo(NonExistingFile.fileName());
    QFile ThumbFile(FileInfo.absoluteFilePath() + ".png");
    QFileInfo filenameFileInfo(ThumbFile);

    QPainter paint;
    QRect rectangle(-200,-200,600,600);

    bool isFavorite = false;
    for (int r = 0; r < tableWidget->rowCount(); r++)
        {
        if (ui->CatalogWidget->item(i)->whatsThis() == tableWidget->item(r,1)->text())
            {
            if (tableWidget->item(r,4)->text() == "Favorite")
                {
                isFavorite = true;
                break;
                }
            }
        }

    if (ThumbFile.exists()) //AKO VEC POSTOJI THUMB
        {
        //------------------------------------------
        paint.resetTransform();
        paint.eraseRect(rectangle);
        paint.fillRect(rectangle,QColor(0,0,0,0));
        paint.setBackground(QColor(0, 0, 0, 0));
        paint.setRenderHint(QPainter::Antialiasing);
        QImage *FinishedCatalogImage = new QImage;
        FinishedCatalogImage->load(QString(FileInfo.absoluteFilePath() + ".png"), 0);
        paint.begin(FinishedCatalogImage);
        paint.resetTransform();
        QRect myrect(FinishedCatalogImage->width()-34,FinishedCatalogImage->height()-34, 32, 32);
        QRect myFavoriterect(FinishedCatalogImage->width()-64,FinishedCatalogImage->height()-34, 32, 32);
        paint.setRenderHint(QPainter::SmoothPixmapTransform);

        QImage pdfImage(":/image/IMG/pdf.png");
        paint.drawPixmap(myrect,QPixmap::fromImage(pdfImage));

        //Status
        if (isFavorite)
        {
        QImage FavoriteImage(":/image/IMG/favorite.png");
        paint.drawPixmap(myFavoriterect,QPixmap::fromImage(FavoriteImage));
        }

        //------------------------------------------

        QIcon ItemIcon;
        ItemIcon.addPixmap(QPixmap::fromImage( *FinishedCatalogImage, Qt::AutoColor));
        ui->CatalogWidget->item(i)->setIcon(ItemIcon);

        ui->LabelPath->setText("Loading thumbnails for: " + filenameFileInfo.absoluteFilePath());
        ui->LabelPath->repaint();
        }
        else
        {
        QImage *FinishedThumbnailLESSImage = new QImage;

        //GENERISI THUMB ILI PRIKAZI DRUGO
        try
            {
            QImage image;
            Poppler::Document* documentT = Poppler::Document::load(ui->CatalogWidget->item(i)->whatsThis());

            if (documentT)
                {
                if (!documentT->isLocked())
                    {
                    documentT->setRenderHint(Poppler::Document::TextAntialiasing);
                    image = documentT->page(0)->renderToImage(scaleFactor * physicalDpiX(), scaleFactor * physicalDpiY());

                    QImage FinishedImage;
                    if (image.width() > image.height())
                       {
                        ui->CatalogWidget->setIconSize(QSize(350,262));
                        FinishedImage = image.scaledToWidth(167, Qt::SmoothTransformation);
                        }
                    else
                       {
                       ui->CatalogWidget->setIconSize(QSize(262,350));
                       FinishedImage = image.scaledToHeight(167, Qt::SmoothTransformation);
                       }

                    QDir newfile(FileInfo.path());
                    newfile.mkpath(FileInfo.path());

                    FinishedImage.save(FileInfo.absoluteFilePath() + ".png");

                    ui->LabelPath->setText("Generating thumbnails for: " + filenameFileInfo.absoluteFilePath());
                    ui->LabelPath->repaint();
                    }
                delete documentT;
                }
                else
                    {
                        //------------------------------AKO NE MOZE DA SE GENERISE THUMBS-----------------------------
                        FinishedThumbnailLESSImage->load(QString(":/image/IMG/empty.png"), 0);

                        QPainter paint;
                        QRect rectangle(-200,-200,600,600);
                        paint.resetTransform();
                        paint.eraseRect(rectangle);
                        paint.fillRect(rectangle,QColor(0,0,0,0));
                        paint.setBackground(QColor(0, 0, 0, 0));

                        paint.begin(FinishedThumbnailLESSImage);
                        paint.setCompositionMode(QPainter::CompositionMode_Source);
                        paint.setBackground(QColor(0, 0, 0, 0));
                        paint.setRenderHint(QPainter::Antialiasing);

                        QColor ProvidnoBeloNoThumb(255,255,255,200);
                        QColor BeloNoThumb(255,0,0,255);
                        QPen krugNoThumb(ProvidnoBeloNoThumb, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

                        paint.begin(FinishedThumbnailLESSImage);
                        paint.setCompositionMode(QPainter::CompositionMode_Source);
                        paint.setBackground(QColor(0, 0, 0, 0));
                        paint.resetTransform();

                        QBrush PopunaNoThumb(ProvidnoBeloNoThumb, Qt::SolidPattern);
                        paint.setPen(krugNoThumb);
                        paint.setBrush(PopunaNoThumb);
                        paint.setRenderHint(QPainter::Antialiasing);
                        paint.drawEllipse(QRect(1,(FinishedThumbnailLESSImage->height()-FinishedThumbnailLESSImage->width())/2,FinishedThumbnailLESSImage->width()-2,FinishedThumbnailLESSImage->width()-2));

                        QPen CircleNoThumb(BeloNoThumb, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
                        CircleNoThumb.setWidth(1);
                        paint.setPen(CircleNoThumb);
                        paint.setBrush(Qt::NoBrush);
                        paint.setRenderHint(QPainter::Antialiasing);
                        paint.drawEllipse(QRect(1,(FinishedThumbnailLESSImage->height()-FinishedThumbnailLESSImage->width())/2,FinishedThumbnailLESSImage->width()-2,FinishedThumbnailLESSImage->width()-2));

                        QPen PenNoThumb(BeloNoThumb, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                        PenNoThumb.setWidth(2);
                        paint.setPen(PenNoThumb);
                        QFont serifFont("Ariel", 12, QFont::Light);
                        paint.setFont(serifFont);
                        paint.setRenderHint(QPainter::Antialiasing);
                        paint.drawText(QRect(1,(FinishedThumbnailLESSImage->height()-FinishedThumbnailLESSImage->width())/2,FinishedThumbnailLESSImage->width()-2,FinishedThumbnailLESSImage->width()-2),Qt::AlignCenter, "ERROR\nreading file\n\nwe don't advice\nopening this file");
                        //--------------------------------------------------------------------------

                    }
            }
            catch(...){}
        if (ThumbFile.exists())
            ui->CatalogWidget->item(i)->setIcon(QIcon(ThumbFile.fileName()));
        else
            ui->CatalogWidget->item(i)->setIcon(QPixmap::fromImage( *FinishedThumbnailLESSImage, Qt::AutoColor));
        //-------------------
        ui->progressBar->setValue(i);
        ui->progressBar->repaint();
        }

    paint.resetTransform();
    paint.eraseRect(rectangle);
    paint.fillRect(rectangle,QColor(0,0,0,0));

    paint.end();
    }

}
//--------------------------------------------------------------------------
void MainWindow::CatalogClick(QListWidgetItem *item)
{
    setWindowTitle("Catalog: " + ClickedItemText);//
    //
    if (QApplication::keyboardModifiers() != Qt::CTRL)
    {
        ui->CatalogWidget->setSelectionMode(QAbstractItemView::MultiSelection);

        ui->scrollArea->hide();
        ui->CatalogWidget->show();

        ui->BackButton->setEnabled(true);
        ui->BackButton->setText("All Catalogs");

        IconStyleScattered->setVisible(false);
        IconStyleTwo->setVisible(false);
        IconStyleFiveInOrder->setVisible(false);
        IconStyleInOrder->setVisible(false);
        IconStyleOne->setVisible(false);
        IconStyleTwoScattered->setVisible(false);
        IconStyleExt->setVisible(false);
        IconStyleThree->setVisible(false);
        repaint();

    try
        {
        QString S(ClickedItemText); //Ovo ne sme biti posle ui->CatalogWidget->clear(); samo pre njega
        ui->CatalogWidget->clear();

        ui->CatalogWidget->repaint();
        ui->CatalogWidget->setIconSize(QSize(262,350));
        //---------------------------------------------------------

        //Dodavanje itema
        //if (ClickedItemText != "Favorite")
        //{
        for (int i = 0; i < tableWidget->rowCount(); i++)
            {
            if (tableWidget->item(i,3)->text() == S)
                {
                QFile filename(tableWidget->item(i,1)->text());
                if (filename.exists())
                    {
                    QListWidgetItem *itemX = new QListWidgetItem(ui->CatalogWidget);
                    QFont f = itemX->font();
                    QFontMetrics fm = QFontMetrics(f);

                    int maxWidth;
                    switch (fontsize) {
                    case 8:
                        maxWidth = 170;
                        break;
                    case 9:
                        maxWidth = 160;
                        break;
                    case 10:
                        maxWidth = 150;
                        break;
                    case 11:
                        maxWidth = 140;
                        break;
                    case 12:
                        maxWidth = 130;
                        break;
                    case 13:
                        maxWidth = 120;
                        break;
                    case 14:
                        maxWidth = 110;
                        break;
                    case 15:
                        maxWidth = 100;
                        break;
                    case 16:
                        maxWidth = 90;
                        break;
                    default:
                        maxWidth = 130;
                        break;
                    }

                    QString elidedString = fm.elidedText(tableWidget->item(i,0)->text(),Qt::ElideRight, maxWidth);
                    CurrentOpenedCatalog = tableWidget->item(i,3)->text();
                    itemX->setText(elidedString);
                    itemX->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);

                    QImage *FinishedThumbnailLESSImage = new QImage;
                    FinishedThumbnailLESSImage->load(QString(":/image/IMG/empty_logo.png"), 0);
                    itemX->setIcon(QPixmap::fromImage( *FinishedThumbnailLESSImage, Qt::AutoColor));

                    itemX->setStatusTip("Book"); //Kategorija itema, NE katalog
                    itemX->setWhatsThis(tableWidget->item(i,1)->text()); //Ovde se nalazi prava adresa fajla
                    itemX->setToolTip(tableWidget->item(i,0)->text()); //Sta se prikaze korisniku

                    ui->CatalogWidget->addItem(itemX);
                    ui->progressBar->setValue(ui->CatalogWidget->count());
                    ui->LabelPath->setText("Loading: " + tableWidget->item(i,1)->text());
                    ui->LabelPath->repaint();
                    }
                }
            }
        //}

        }
        catch(...){}
        DeepnessLevel = 1;

        ui->CatalogWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->CatalogWidget->repaint();
        }
LoadThumbsOrGenerateOne();
}
//---------------------------------------------------------
void MainWindow::FavoritesButtonClick()
{
    if (!FavoriteIsON)
    {
    Favorites();
    FavoriteIsON = true;

    ui->BackButton->setDisabled(true);
    ui->BackButton->setText("");
    ui->BackToThisCatalogButton->setDisabled(true);
    ui->BackToThisCatalogButton->setText("");

    IconStyleScattered->setVisible(false);
    IconStyleTwo->setVisible(false);
    IconStyleFiveInOrder->setVisible(false);
    IconStyleInOrder->setVisible(false);
    IconStyleOne->setVisible(false);
    IconStyleTwoScattered->setVisible(false);
    IconStyleExt->setVisible(false);
    IconStyleThree->setVisible(false);
    ui->toolBar_pdf->setVisible(false);
    }
    else
    {
        BackToAllCatalogsButtonClick();
        FavoriteIsON = false;
    }



    CheckIfThereIsFavoritesOrNot();
}
//---------------------------------------------------------
void MainWindow::Favorites()
{
    setWindowTitle("Favorites");//
    //
    if (QApplication::keyboardModifiers() != Qt::CTRL)
    {
        ui->CatalogWidget->setSelectionMode(QAbstractItemView::MultiSelection);

        ui->scrollArea->hide();
        ui->CatalogWidget->show();

        IconStyleScattered->setVisible(false);
        IconStyleTwo->setVisible(false);
        IconStyleFiveInOrder->setVisible(false);
        IconStyleInOrder->setVisible(false);
        IconStyleOne->setVisible(false);
        IconStyleTwoScattered->setVisible(false);
        IconStyleExt->setVisible(false);
        IconStyleThree->setVisible(false);
        repaint();

    try
        {
        QString S("Favorite"); //Ovo ne sme biti posle ui->CatalogWidget->clear(); samo pre njega
        ui->CatalogWidget->clear();

        ui->CatalogWidget->repaint();
        ui->CatalogWidget->setIconSize(QSize(262,350));
        //---------------------------------------------------------
    for (int i = 0; i < tableWidget->rowCount(); i++)
        {
        if (tableWidget->item(i,4)->text() == S)
            {
            QFile filename(tableWidget->item(i,1)->text());
            if (filename.exists())
                {
                QListWidgetItem *itemX = new QListWidgetItem(ui->CatalogWidget);
                QFont f = itemX->font();
                QFontMetrics fm = QFontMetrics(f);

                int maxWidth;
                switch (fontsize) {
                case 8:
                    maxWidth = 170;
                    break;
                case 9:
                    maxWidth = 160;
                    break;
                case 10:
                    maxWidth = 150;
                    break;
                case 11:
                    maxWidth = 140;
                    break;
                case 12:
                    maxWidth = 130;
                    break;
                case 13:
                    maxWidth = 120;
                    break;
                case 14:
                    maxWidth = 110;
                    break;
                case 15:
                    maxWidth = 100;
                    break;
                case 16:
                    maxWidth = 90;
                    break;
                default:
                    maxWidth = 130;
                    break;
                }

                QString elidedString = fm.elidedText(tableWidget->item(i,0)->text(),Qt::ElideRight, maxWidth);
                CurrentOpenedCatalog = tableWidget->item(i,3)->text();
                itemX->setText(elidedString);
                itemX->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);

                QImage *FinishedThumbnailLESSImage = new QImage;
                FinishedThumbnailLESSImage->load(QString(":/image/IMG/empty_logo.png"), 0);
                itemX->setIcon(QPixmap::fromImage( *FinishedThumbnailLESSImage, Qt::AutoColor));

                itemX->setStatusTip("Book"); //Kategorija itema, NE katalog
                itemX->setWhatsThis(tableWidget->item(i,1)->text()); //Ovde se nalazi prava adresa fajla
                itemX->setToolTip(tableWidget->item(i,0)->text()); //Sta se prikaze korisniku

                ui->CatalogWidget->addItem(itemX);
                ui->progressBar->setValue(ui->CatalogWidget->count());
                ui->LabelPath->setText("Loading: " + tableWidget->item(i,1)->text());
                ui->LabelPath->repaint();
                }
            }
        }
        }
        catch(...){}
        DeepnessLevel = 1;

        ui->CatalogWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->CatalogWidget->repaint();
        }

LoadThumbsOrGenerateOne();
CheckIfThereIsFavoritesOrNot();
//ui->FavoritePushButton->setText("All Catalogs");
}
//---------------------------------------------------------

void MainWindow::SetAppStyle()
{
setWindowIcon(QIcon(":/image/IMG/glebc_icon48.png"));
this->setWindowState(Qt::WindowMaximized);

ui->progress_widget->hide();
ui->LabelPath->clear();

ui->toolBar_pdf->setVisible(false);

this->repaint();

}

//------------------------------------------------------------------------------------

void MainWindow::ScanFolderForEbooks()
{

QString DirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),QDir::homePath(),QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

QDirIterator dirIt(DirPath,QDirIterator::Subdirectories);
while (dirIt.hasNext())
{
dirIt.next();
if (QFileInfo(dirIt.filePath()).isFile())
    {
    if ((QFileInfo(dirIt.filePath()).suffix() == "pdf")  || (QFileInfo(dirIt.filePath()).suffix() == "PDF"))
            {
            bool FileFound = false;
            for (int i = 0; i < tableWidget->rowCount(); i++)
                {
                if (tableWidget->item(i,1)->text() == dirIt.fileInfo().absoluteFilePath())
                    {
                    FileFound = true;
                    break;
                    }
                }
                if (!FileFound)
                    {
                    int row = tableWidget->rowCount();
                    tableWidget->insertRow(row);

                    QTableWidgetItem *eBookName = new QTableWidgetItem;
                    eBookName->setText(dirIt.fileInfo().baseName());

                    QTableWidgetItem *eBookPath = new QTableWidgetItem;
                    eBookPath->setText(dirIt.fileInfo().absoluteFilePath());

                    QTableWidgetItem *eBookLastPage = new QTableWidgetItem;
                    eBookLastPage->setText("");

                    QTableWidgetItem *eBookCategory = new QTableWidgetItem;
                    eBookCategory->setText("All Books");

                    QTableWidgetItem *eBookFavorite = new QTableWidgetItem;
                    eBookFavorite->setText("");

                    tableWidget->setItem(row, 0, eBookName);
                    tableWidget->setItem(row, 1, eBookPath);
                    tableWidget->setItem(row, 2, eBookLastPage);
                    tableWidget->setItem(row, 3, eBookCategory);
                    tableWidget->setItem(row, 4, eBookFavorite);
                    }
                }
            }


}
SaveCatalog();
ShowCatalogs();

//Sada Otvori 'All Books' katalog kako bi se odmah generisali thumbnails i korisnik odmah video sve knjige
QListWidgetItem *AllItems_item = new QListWidgetItem;
AllItems_item->setText("All Books");
AllItems_item->setStatusTip("Catalog");
CatalogBookClick(AllItems_item);
}

//------------------------------------------------------------------------------------



void MainWindow::AddNewBooks()
{
QString fileName = QFileDialog::getOpenFileName(this, tr("Open PDF"), QDir::homePath(), tr("PDF Books (*.pdf)"));
QFileInfo NewFile(fileName);
QFileInfo OldFiles;

if (NewFile.exists())
{
    if ((NewFile.suffix() == "pdf")  || (NewFile.suffix() == "PDF"))
    {
        int FileFound = 0;
        for (int i = 0; i < tableWidget->rowCount(); i++)
        {
        OldFiles.setFile(tableWidget->item(i,1)->text());

        if (NewFile.absoluteFilePath() == OldFiles.absoluteFilePath())
            {
            FileFound = 0;
            break;
            }
            else if ((NewFile.baseName() == OldFiles.baseName()) && (NewFile.absoluteFilePath() != OldFiles.absoluteFilePath()))
                {
                FileFound = 1;
                break;
                }
                else if ((NewFile.baseName() != OldFiles.baseName()) && (NewFile.absoluteFilePath() != OldFiles.absoluteFilePath()))
                    {
                    FileFound = 2;
                    }
        }

        //----
        switch (FileFound) {
            case 2:
                    {
                    //NOVI FAJL UBACI GA
                    int row = tableWidget->rowCount();
                    tableWidget->insertRow(row);

                    QTableWidgetItem *eBookName = new QTableWidgetItem;
                    eBookName->setText(NewFile.baseName());

                    QTableWidgetItem *eBookPath = new QTableWidgetItem;
                    eBookPath->setText(NewFile.absoluteFilePath());

                    QTableWidgetItem *eBookLastPage = new QTableWidgetItem;
                    eBookLastPage->setText("");

                    QTableWidgetItem *eBookCategory = new QTableWidgetItem;
                    eBookCategory->setText("All Books");

                    QTableWidgetItem *eBookFavorite = new QTableWidgetItem;
                    eBookFavorite->setText("");


                    tableWidget->setItem(row, 0, eBookName);
                    tableWidget->setItem(row, 1, eBookPath);
                    tableWidget->setItem(row, 2, eBookLastPage);
                    tableWidget->setItem(row, 3, eBookCategory);
                    tableWidget->setItem(row, 4, eBookFavorite);

                    SaveCatalog();
                    ShowCatalogs();

                    //Sada Otvori 'All Books' katalog kako bi se odmah generisali thumbnails i korisnik odmah video sve knjige
                    QListWidgetItem *AllItems_item = new QListWidgetItem;
                    AllItems_item->setText("All Books");
                    AllItems_item->setStatusTip("Catalog");
                    CatalogBookClick(AllItems_item);

                    break;
                    }
            break;
            case 1:
                    {
                    //Isto ime, na drugom mestu
                    //ISTI ITEM RAZLICITA ADRESA, PITAJ STA DA SE RADI, DA DODA, DA ZAMENI, NISTA DA NE RADI

                    QString info1 = "Same or similar file already existing";

                    QString info2 = QString("<html><body>Now we have two same or similar pdf documents in diferent locations, here you can compare them. What you want to do now, with new file that you wanted to add?<ul><li>You can add this new file an you will have this two files in shelf</li><li>You can replace old file with new one.</li><li>You don't have to do anything.</li></ul><br>")
                            + QString("<table width='100%' border='0' cellpadding='2'>")
                            + QString("<tr><td align='left'><b>File to add</b></td><td></td><td align='right'><b>File in shelf</b></td></tr>")
                            //+ QString("<tr><td align='left'>" + QString("<img src='") + image1 + QString("/>'") + "</td><td align='center'></td><td align='right'>" + OldFiles.baseName() + "</td></tr>")
                            + QString("<font size=1><tr><td align='left'>" + NewFile.baseName() + "</td><td align='center'><b>name</b></td><td align='right'>" + OldFiles.baseName() + "</td></tr>")
                            + QString("<tr><td align='left'>" + NewFile.path() + "</td><td align='center'><b>path</b></td><td align='right'>" + OldFiles.path() + "</td></tr>")
                            + QString("<tr><td align='left'>" + QString::number(NewFile.size()/1024) + "KB</td><td align='center'><b>size</b></td><td align='right'>" + QString::number(OldFiles.size()/1024) + "KB</td></tr>")
                            + QString("<tr><td align='left'>" + NewFile.created().toString() + "</td><td align='center'><b>created</b></td><td align='right'>" + OldFiles.created().toString() + "</td></tr>")
                            + QString("<tr><td align='left'>" + NewFile.owner() + "</td><td align='center'><b>owner</b></td><td align='right'>" + OldFiles.owner() + "</td></tr>")
                            + QString("</table><body><html>");

                        QMessageBox m;
                        m.setWindowTitle(QApplication::applicationName());
                        m.setWindowIcon(windowIcon());

                        //QDialogButtonBox buttonBox = new QDialogButtonBox;
                        QPushButton* ButtonDontDoAnything = m.addButton(tr("&Cancel"), QMessageBox::NoRole);
                        //QPushButton* ButtonReplaceOldFile = m.addButton(tr("&Replace"), QMessageBox::YesRole);
                        //QPushButton* ButtonAddNewFile = m.addButton(tr("&Add"), QMessageBox::AcceptRole);

                        m.setDefaultButton(ButtonDontDoAnything);
                        m.setText("<font size=5><b>" + QString::fromUtf8(info1.toStdString().c_str()) + "</b></font>");
                        m.setInformativeText(QString("<table width='600' border='0' cellpadding='6'><tr><td align='left'>") + QString(info2) + "</td></tr></table>");
                    ShowCatalogs();
                    }
            break;
            default: 0;
                    {
                    //Apsolutno isti fajl, nista neradi samo OBAVESTI
                    testmsg.setText("You are trying to add the file that you already have in your shelf. Nothing will be done!");
                    testmsg.exec();
                    ShowCatalogs();
                    }
            break;
        }
    }
}
}

//------------------------------------------------------------------------------------
void MainWindow::RenameBook()
{
    QString OldName;
    for (int i = 0; i < tableWidget->rowCount(); i++)
        {
        if (tableWidget->item(i,1)->text() == ui->CatalogWidget->item(ui->CatalogWidget->currentIndex().row())->whatsThis())
            {
            OldName = tableWidget->item(i,0)->text();
            }
        }

bool ok;
QString text = QInputDialog::getText(this, tr("Rename book"),tr("New book name:"), QLineEdit::Normal, OldName, &ok);
if (ok && !text.isEmpty())
    {
    ui->CatalogWidget->item(ui->CatalogWidget->currentIndex().row())->setText(text);

    for (int i = 0; i < tableWidget->rowCount(); i++)
        {
        if (tableWidget->item(i,1)->text() == ui->CatalogWidget->item(ui->CatalogWidget->currentIndex().row())->whatsThis())
            {
            tableWidget->item(i,0)->setText(text);
            }
        }
    }
SaveCatalog();
QListWidgetItem *item = new QListWidgetItem();
item->setText(CurrentOpenedCatalog);
item->setStatusTip("Catalog");
CatalogBookClick(item);
//RebuildCategoryList();
}

//------------------------------------------------------------------------------------

void MainWindow::MoveToNewCatalog()
{
    QStringList SelectedItems;

    for (int l = 0; l < ui->CatalogWidget->count(); l++)
    {
        if (ui->CatalogWidget->item(l)->isSelected())
        {
        SelectedItems << ui->CatalogWidget->item(l)->whatsThis();
        }
    }

    QString CatalogName;
    for (int i = 0; i < tableWidget->rowCount(); i++)
        {
        if (tableWidget->item(i,1)->text() == ui->CatalogWidget->item(ui->CatalogWidget->currentIndex().row())->whatsThis())
            {
            CatalogName = tableWidget->item(i,3)->text();
            }
        }

bool ok;
QString text = QInputDialog::getText(this, tr("Move to..."),tr("New Catalog name:"), QLineEdit::Normal, CatalogName, &ok);
if (ok && !text.isEmpty())
    {
    for (int n = 0; n < SelectedItems.count(); n++)
    {
    for (int i = 0; i < tableWidget->rowCount(); i++)
        {
        if (tableWidget->item(i,1)->text() == SelectedItems.at(n))
            {
            tableWidget->item(i,3)->setText(text);
            break;
            }
        }
    }
    }
SaveCatalog();
DeepnessLevel = 0;

QListWidgetItem *item = new QListWidgetItem();
item->setText(CurrentOpenedCatalog);
item->setStatusTip("Catalog");
CatalogBookClick(item);
//RebuildCategoryList();
}

//------------------------------------------------------------------------------------

void MainWindow::MoveToExistingCatalog()
{
QStringList SelectedItems;

for (int l = 0; l < ui->CatalogWidget->count(); l++)
{
    if (ui->CatalogWidget->item(l)->isSelected())
    {
    SelectedItems << ui->CatalogWidget->item(l)->whatsThis();
    }
}

for (int n = 0; n < SelectedItems.count(); n++)
{
for (int i = 0; i < tableWidget->rowCount(); i++)
        {
        if (tableWidget->item(i,1)->text() == SelectedItems.at(n))
            {

            tableWidget->item(i,3)->setText(sender()->objectName());
            break;
            }
        }
}

SaveCatalog();
DeepnessLevel = 0;

QListWidgetItem *item = new QListWidgetItem();
item->setText(CurrentOpenedCatalog);
item->setStatusTip("Catalog");
CatalogBookClick(item);
//RebuildCategoryList();
}

//------------------------------------------------------------------------------------

void MainWindow::RenameCatalog()
{
QString OldCatalaogName = ui->CatalogWidget->item(ui->CatalogWidget->currentIndex().row())->text();

bool ok;
QString text = QInputDialog::getText(this, tr("Rename catalog"),tr("New Catalog name:"), QLineEdit::Normal, OldCatalaogName, &ok);
if (ok && !text.isEmpty())
{
    for (int i = 0; i < tableWidget->rowCount(); i++)
        {
        if (tableWidget->item(i,3)->text() == OldCatalaogName)
            {
            tableWidget->item(i,3)->setText(text);
            }
        }
}

SaveCatalog();
DeepnessLevel = 0;
//RebuildCategoryList();

ShowCatalogs();
}
//------------------------------------------------------------------------------------

void MainWindow::DeleteBook()
{
    QStringList SelectedItems;
    QString DocsForDeletionInfoString;

    for (int l = 0; l < ui->CatalogWidget->count(); l++)
    {
        if (ui->CatalogWidget->item(l)->isSelected())
        {
        SelectedItems << ui->CatalogWidget->item(l)->whatsThis();
        DocsForDeletionInfoString = DocsForDeletionInfoString + ui->CatalogWidget->item(l)->text() + "\n";
        }
    }
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Removing a document from catalog", "This operation will remove this document ONLY from the your catalog, but NOT from your hard disk. Are you sure you want to do this?\n\nDocument(s): \n" + DocsForDeletionInfoString, QMessageBox::Yes|QMessageBox::No).exec())
    {
    for (int n = 0; n < SelectedItems.count(); n++)
    {
    for (int i = tableWidget->rowCount()-1; i >= 0; i--)
            {
            if (tableWidget->item(i,1)->text() == SelectedItems.at(n))
                {
                tableWidget->removeRow(i);

                QDir ThumbsDir(PathToCatalogDir + "FileThumbs");
                QFile ThumbsFile(ThumbsDir.path() + SelectedItems.at(n)  + ".png");
                if (ThumbsFile.exists())
                    {
                        ThumbsFile.remove();
                    }
                break;
                }
            }
    }
    }

    SaveCatalog();
    DeepnessLevel = 0;

    QListWidgetItem *item = new QListWidgetItem();
    item->setText(CurrentOpenedCatalog);
    item->setStatusTip("Catalog");
    CatalogBookClick(item);
    //RebuildCategoryList();
}
//------------------------------------------------------------------------------------

void MainWindow::DeleteBookDisk()
{
    QStringList SelectedItems;
    QString FilesForDeletionInfoString;
    for (int l = 0; l < ui->CatalogWidget->count(); l++)
    {
        if (ui->CatalogWidget->item(l)->isSelected())
        {
        SelectedItems << ui->CatalogWidget->item(l)->whatsThis();
        FilesForDeletionInfoString = FilesForDeletionInfoString + ui->CatalogWidget->item(l)->whatsThis() + "\n";
        }
    }
    if (QMessageBox::Yes == QMessageBox(QMessageBox::Information, "Removing a document from disk", "This operation will remove this document not only from the your catalog, but from hard disk as well. Are you sure you want to do this?\n\nFile(s): \n" + FilesForDeletionInfoString, QMessageBox::Yes|QMessageBox::No).exec())
    {
    for (int n = 0; n < SelectedItems.count(); n++)
    {
    for (int i = tableWidget->rowCount()-1; i >= 0; i--)
            {
            if (tableWidget->item(i,1)->text() == SelectedItems.at(n))
                {
                QFile FileToDelete(SelectedItems.at(n));
                if (FileToDelete.exists())
                    {
                    bool deleted = FileToDelete.remove();
                        if (deleted)
                                {
                                tableWidget->removeRow(i);
                                }

                        QDir ThumbsDir(PathToCatalogDir + "FileThumbs");
                        QFile ThumbsFile(ThumbsDir.path() + SelectedItems.at(n)  + ".png");
                        if (ThumbsFile.exists())
                            {
                                ThumbsFile.remove();
                            }
                    break;
                    }
                }
            }
    }
    }

    SaveCatalog();
    DeepnessLevel = 0;

    QListWidgetItem *item = new QListWidgetItem();
    item->setText(CurrentOpenedCatalog);
    item->setStatusTip("Catalog");
    CatalogBookClick(item);
    //RebuildCategoryList();
}

//------------------------------------------------------------------------------------
void MainWindow::BookProperties()
{
qint64 FilesSize = 0;
QFileInfo FileInfo(ui->CatalogWidget->currentItem()->whatsThis());
if (FileInfo.exists())
    {
    FilesSize = FileInfo.size();
    }

QString FileTitle;
for (int i = 0; i < tableWidget->rowCount(); i++)
{
    if (tableWidget->item(i,1)->text() == FileInfo.absoluteFilePath())
    {
        FileTitle = tableWidget->item(i,0)->text();
        break;
    }
}

const quint64 kb = 1024;
const quint64 mb = 1024 * kb;
const quint64 gb = 1024 * mb;
const quint64 tb = 1024 * gb;

QString Fin;

if (FilesSize >= tb)
    Fin = QFileSystemModel::tr("%1 TB").arg(QLocale().toString(qreal(FilesSize) / tb, 'f', 2));
else if (FilesSize >= gb)
    Fin =   QFileSystemModel::tr("%1 GB").arg(QLocale().toString(qreal(FilesSize) / gb, 'f', 2));
else if (FilesSize >= mb)
    Fin =   QFileSystemModel::tr("%1 MB").arg(QLocale().toString(qreal(FilesSize) / mb, 'f', 2));
else if (FilesSize >= kb)
    Fin =   QFileSystemModel::tr("%1 KB").arg(QLocale().toString(FilesSize / kb));

QString info2 = QString("<table width='100%' border='0' cellpadding='2'>")
        + QString("<tr><td><b>Name:</b></td><td>") + FileInfo.fileName() + QString("</td></tr>")
        + QString("<tr><td><b>Path:</b></td><td>") + FileInfo.absolutePath() + QString("</td></tr>")
        + QString("<tr><td><b>Size:</b></td><td>") + Fin + QString("</td></tr>")
        + QString("<tr><td></td><td></td></tr>")
        + QString("<tr><td><b>Created:</b></td><td>") + FileInfo.created().toString() + QString("</td></tr>")
        + QString("<tr><td><b>Read:</b></td><td>") + FileInfo.lastRead().toString() + QString("</td></tr>")
        + QString("<tr><td><b>Modified:</b></td><td>") + FileInfo.lastModified().toString() + QString("</td></tr>");

QDir ThumbsDir(PathToCatalogDir + "FileThumbs");
QFile NonExistingFile(ThumbsDir.path() + FileInfo.absoluteFilePath());

QFileInfo FileInfo1(NonExistingFile.fileName());
QFile ThumbFile(FileInfo1.absoluteFilePath() + ".png");

QMessageBox m;
m.setWindowTitle(QApplication::applicationName());
m.setWindowIcon(windowIcon());
m.setText("<font size=5><b>" + FileTitle + "</b></font>");
m.setInformativeText(QString("<table width='500' border='0' cellpadding='6'><tr><td align='center' width='15%'><img src='" + QString(ThumbFile.fileName() + "'/>")) + QString("</td><td>" + QString(info2) + "</td></tr></table>"));
m.exec();
}

//------------------------------------------------------------------------------------

void MainWindow::Status()
{
    QStringList AllCatalogs;
    AllCatalogs.clear();

    int row = tableWidget->rowCount();
    //---------------------------
    for (int n = 0; n < row; n++)
        {
        AllCatalogs << QString::fromUtf8(tableWidget->item(n,3)->text().toStdString().c_str());
        }

    AllCatalogs.sort();
    AllCatalogs.removeDuplicates();

//--------------------------------

qint64 FilesSize = 0;
qint64 ThumbsFilesSize = 0;
for(int i = 0; i < tableWidget->rowCount(); i++)
{
QFileInfo FileInfo(tableWidget->item(i,1)->text());
if (FileInfo.exists())
    {
    FilesSize = FilesSize + FileInfo.size();
    }

QDir ThumbsDir(PathToCatalogDir + "FileThumbs");
QFile ThumbsFile(ThumbsDir.path() + tableWidget->item(i,1)->text()  + ".png");

QFileInfo ThumbsFileInfo(ThumbsFile);
if (ThumbsFileInfo.exists())
    {
    ThumbsFilesSize = ThumbsFilesSize + ThumbsFileInfo.size();
    }
}

//--------------------------------
int numberoffavpresent = 0;
for (int i = 0; i < tableWidget->rowCount(); i++)
{
    if (tableWidget->item(i,4)->text() == "Favorite")
        numberoffavpresent++;
}
//--------------------------------

const quint64 kb = 1024;
const quint64 mb = 1024 * kb;
const quint64 gb = 1024 * mb;
const quint64 tb = 1024 * gb;

QString Fin;

if (FilesSize >= tb)
    Fin = QFileSystemModel::tr("%1 TB").arg(QLocale().toString(qreal(FilesSize) / tb, 'f', 2));
else if (FilesSize >= gb)
    Fin =   QFileSystemModel::tr("%1 GB").arg(QLocale().toString(qreal(FilesSize) / gb, 'f', 2));
else if (FilesSize >= mb)
    Fin =   QFileSystemModel::tr("%1 MB").arg(QLocale().toString(qreal(FilesSize) / mb, 'f', 2));
else if (FilesSize >= kb)
    Fin =   QFileSystemModel::tr("%1 KB").arg(QLocale().toString(FilesSize / kb));

//--------------------------------------

QString TFin;

if (ThumbsFilesSize >= tb)
    TFin = QFileSystemModel::tr("%1 TB").arg(QLocale().toString(qreal(ThumbsFilesSize) / tb, 'f', 2));
else if (ThumbsFilesSize >= gb)
    TFin =   QFileSystemModel::tr("%1 GB").arg(QLocale().toString(qreal(ThumbsFilesSize) / gb, 'f', 2));
else if (ThumbsFilesSize >= mb)
    TFin =   QFileSystemModel::tr("%1 MB").arg(QLocale().toString(qreal(ThumbsFilesSize) / mb, 'f', 2));
else if (ThumbsFilesSize >= kb)
    TFin =   QFileSystemModel::tr("%1 KB").arg(QLocale().toString(ThumbsFilesSize / kb));

//--------------------------------------

QString info2 = QString("<table width='100%' border='0' cellpadding='2'>")
        + QString("<tr><td><b>Number of catalogs:</b></td><td align=right>") + QString::number(AllCatalogs.count()) + QString("</td></tr>")
        + QString("<tr><td><b>Number of books:</b></td><td align=right>") + QString::number(tableWidget->rowCount()) + QString("</td></tr>")
        + QString("<tr><td><b>Number of favorites:</b></td><td align=right>") + QString::number(numberoffavpresent) + QString("</td></tr>")
        + QString("<tr><td><b>Total book file size:</b></td><td align=right>") + Fin + QString("</td></tr>")
        + QString("<tr><td><b>Total book thumbnails size:</b></td><td align=right>") + TFin + QString("</td></tr>");

    QString info1 = "Shelf properties:";

    QMessageBox m;
    m.setWindowTitle(QApplication::applicationName());
    m.setWindowIcon(windowIcon());
    m.setText("<font size=5><b>" + QString::fromUtf8(info1.toStdString().c_str()) + "</b></font>");
    m.setInformativeText(QString("<table width='400' border='0' cellpadding='6'><tr><td align='center' width='15%'><img src='" + QString(":/image/IMG/glebc_icon96.png'/>")) + QString("</td><td>" + QString(info2) + "</td></tr></table>"));
    m.exec();
}

//------------------------------------------------------------------------------------

void MainWindow::OpenBooksInDefaultAppOption()
{
    openBookInDefaultApp = true;
}

//------------------------------------------------------------------------------------

void MainWindow::OpenBooksInThisAppOption()
{
    openBookInDefaultApp = false;
}

//------------------------------------------------------------------------------------

void MainWindow::ShowContextMenu(const QPoint& pos)
{
    QModelIndex t = ui->CatalogWidget->indexAt(pos);
    if (ui->CatalogWidget->item(t.row()))
    ui->CatalogWidget->item(t.row())->setSelected(true);

    menu = new QMenu;
    menu->setObjectName("menu");

    menu->setStyleSheet(SkinStyle);

    //--------OVO je stavka koje se dodaje u kasniji meni Maintnance i u Glavni meni i zato se pravi pre svih, da bi bila dostupna svim menijima -------
    QAction *AddNewBooks = new QAction(menu);
    QString AddNewBooksText("Add new book...");

    AddNewBooks->setText(QString::fromUtf8(AddNewBooksText.toStdString().c_str()));
    AddNewBooks->setVisible(true);
    connect(AddNewBooks, SIGNAL(triggered()), this, SLOT(AddNewBooks()));



    //---
    if((DeepnessLevel == 1) && (ui->CatalogWidget->selectedItems().count() > 0))
    {
    QAction *OpenBook = new QAction(menu);
    QString OpenBookText("Open here");
    OpenBook->setText(QString::fromUtf8(OpenBookText.toStdString().c_str()));
    OpenBook->setVisible(true);
    connect(OpenBook, SIGNAL(triggered()), this, SLOT(OpenThisBook()));
    menu->addAction(OpenBook);
    menu->setDefaultAction(OpenBook);
    }

    //---
    if((DeepnessLevel == 1) && (ui->CatalogWidget->selectedItems().count() > 0))
    {
    QAction *OpenBookDefault = new QAction(menu);
    QString OpenBookDefaultText("Open [default program]");
    OpenBookDefault->setText(QString::fromUtf8(OpenBookDefaultText.toStdString().c_str()));
    OpenBookDefault->setVisible(true);
    connect(OpenBookDefault, SIGNAL(triggered()), this, SLOT(OpenThisBookWithDefaultApp()));
    menu->addAction(OpenBookDefault);
    }

    if((DeepnessLevel == 1) && (ui->CatalogWidget->selectedItems().count() > 0))
    {
    QAction *OpenBooksFolder = new QAction(menu);
    QString OpenBooksFolderText("Open book folder");
    OpenBooksFolder->setText(QString::fromUtf8(OpenBooksFolderText.toStdString().c_str()));
    OpenBooksFolder->setVisible(true);
    connect(OpenBooksFolder, SIGNAL(triggered()), this, SLOT(OpenThisBooksFolder()));
    menu->addAction(OpenBooksFolder);
    }

    menu->addSeparator();

    //-----------SELECT ALL & UNSELECT
    if(DeepnessLevel == 1)
    {
    QAction *SelectAllItems = new QAction(menu);
    QString SelectAllItemsText("Select all");
    SelectAllItems->setText(QString::fromUtf8(SelectAllItemsText.toStdString().c_str()));
    SelectAllItems->setVisible(true);
    connect(SelectAllItems, SIGNAL(triggered()), this, SLOT(SelectAllItems()));
    menu->addAction(SelectAllItems);
    }

    if (ui->CatalogWidget->selectedItems().count() > 0)
    {
    QAction *UnSelectAllItems = new QAction(menu);
    QString UnSelectAllItemsText("Deselect all");
    UnSelectAllItems->setText(QString::fromUtf8(UnSelectAllItemsText.toStdString().c_str()));
    UnSelectAllItems->setVisible(true);
    connect(UnSelectAllItems, SIGNAL(triggered()), this, SLOT(UnselectItems()));
    menu->addAction(UnSelectAllItems);
    }

    menu->addSeparator();

    //----------------------------------------------------------------------------
    if((DeepnessLevel == 1) && (ui->CatalogWidget->selectedItems().count() > 0))
    {
    QAction *RenameBook = new QAction(menu);
    QString RenameBookText("Rename...");
    RenameBook->setText(QString::fromUtf8(RenameBookText.toStdString().c_str()));
    RenameBook->setVisible(true);
    connect(RenameBook, SIGNAL(triggered()), this, SLOT(RenameBook()));
    menu->addAction(RenameBook);
    }

    if((DeepnessLevel == 1) && (ui->CatalogWidget->selectedItems().count() > 0))
    {
        QAction *FavoriteONOFFBook = new QAction(menu);
        QString FavoriteONOFFBookText("Favorite (on/off)");
        FavoriteONOFFBook->setText(QString::fromUtf8(FavoriteONOFFBookText.toStdString().c_str()));
        FavoriteONOFFBook->setVisible(true);
        connect(FavoriteONOFFBook, SIGNAL(triggered()), this, SLOT(FavoriteONOFF()));
        menu->addAction(FavoriteONOFFBook);

    }
    menu->addSeparator();

    //---
    if((DeepnessLevel == 1) && (ui->CatalogWidget->selectedItems().count() > 0))
    {
    QAction *DeleteBook = new QAction(menu);
    QString DeleteBookText("Delete");
    DeleteBook->setText(QString::fromUtf8(DeleteBookText.toStdString().c_str()));
    DeleteBook->setVisible(true);
    connect(DeleteBook, SIGNAL(triggered()), this, SLOT(DeleteBook()));
    menu->addAction(DeleteBook);
    }

    //---
    if((DeepnessLevel == 1) && (ui->CatalogWidget->selectedItems().count() > 0))
    {
    QAction *DeleteBookDisk = new QAction(menu);
    QString DeleteBookDiskText("Delete from disk");
    DeleteBookDisk->setText(QString::fromUtf8(DeleteBookDiskText.toStdString().c_str()));
    DeleteBookDisk->setVisible(true);
    connect(DeleteBookDisk, SIGNAL(triggered()), this, SLOT(DeleteBookDisk()));
    menu->addAction(DeleteBookDisk);
    }

    menu->addSeparator();

    //---
    if ((DeepnessLevel == 1) && (ui->CatalogWidget->selectedItems().count() > 0))
    {
    QMenu *move_menu = new QMenu;
    move_menu->setTitle("Move to...");
    move_menu->setStyleSheet(SkinStyle);

    //---
    QAction *MoveToNewCatalog = new QAction(move_menu);
    QString MoveToNewCatalogText("...new catalog");
    MoveToNewCatalog->setText(QString::fromUtf8(MoveToNewCatalogText.toStdString().c_str()));
    MoveToNewCatalog->setVisible(true);
    connect(MoveToNewCatalog, SIGNAL(triggered()), this, SLOT(MoveToNewCatalog()));
    move_menu->addAction(MoveToNewCatalog);
    move_menu->addSeparator();

    QStringList CatalogStringList;
    CatalogStringList.clear();

    //Прављење листе каталога
    int row = tableWidget->rowCount();
    //---------------------------
    for (int n = 0; n < row; n++)
        {
        if (tableWidget->item(n,4)->text().isEmpty())
        CatalogStringList << tableWidget->item(n,3)->text();
        }

    CatalogStringList.sort();
    CatalogStringList.removeDuplicates();

    //---
    for (int i = 0; i < CatalogStringList.count(); i++)
        {
        QAction *CatalogName = new QAction(move_menu);
        CatalogName->setObjectName(CatalogStringList.at(i));
        QString CatalogNameText(CatalogStringList.at(i));
        CatalogName->setText(QString::fromUtf8(CatalogNameText.toLocal8Bit()));
        CatalogName->setVisible(true);
        connect(CatalogName, SIGNAL(triggered()), this, SLOT(MoveToExistingCatalog()));
        move_menu->addAction(CatalogName);
        }
    menu->addMenu(move_menu);
    }

    menu->addSeparator();
    //---
    if((DeepnessLevel == 1) && (ui->CatalogWidget->selectedItems().count() > 0))
    {
    QAction *MoreInfoAboutBook = new QAction(menu);
    QString MoreInfoAboutBookText("Properties");
    MoreInfoAboutBook->setText(QString::fromUtf8(MoreInfoAboutBookText.toStdString().c_str()));
    MoreInfoAboutBook->setVisible(true);
    connect(MoreInfoAboutBook, SIGNAL(triggered()), this, SLOT(BookProperties()));
    menu->addAction(MoreInfoAboutBook);
    }

    //KATALOG
    if((DeepnessLevel == 0) && (ui->CatalogWidget->selectedItems().count() > 0))
    {
    QAction *RenameCatalog = new QAction(menu);
    QString RenameCatalogText("Rename...");
    RenameCatalog->setText(QString::fromUtf8(RenameCatalogText.toStdString().c_str()));
    RenameCatalog->setVisible(true);
    connect(RenameCatalog, SIGNAL(triggered()), this, SLOT(RenameCatalog()));
    menu->addAction(RenameCatalog);
    }

    //---
    if(ui->CatalogWidget->selectedItems().count() <= 0)
    {
        if (DeepnessLevel == 0)
        {
        menu->addAction(AddNewBooks);
        menu->addSeparator();
        }
        //-------------------------------
        settings_menu = new QMenu;
        settings_menu->setObjectName("settings_menu");
        settings_menu->setTitle("Settings");
        settings_menu->setContentsMargins(5,0,5,0);
        settings_menu->setStyleSheet(SkinStyle);
        menu->addMenu(settings_menu);
        //---------------SKINOVI----------------
        GetAvaiableSkins();
        //---------------THUMBS STYLE----------------
        if (DeepnessLevel == 0)
        {
            QMenu *CatalogStyle_menu = new QMenu;
            CatalogStyle_menu->setTitle("Thumbnails");
            CatalogStyle_menu->setStyleSheet(SkinStyle);
            settings_menu->addMenu(CatalogStyle_menu);

            CatalogStyle_menu->addAction(IconStyleOne);
            CatalogStyle_menu->addAction(IconStyleTwo);
            CatalogStyle_menu->addAction(IconStyleTwoScattered);
            CatalogStyle_menu->addAction(IconStyleThree);
            CatalogStyle_menu->addAction(IconStyleInOrder);
            CatalogStyle_menu->addAction(IconStyleScattered);
            CatalogStyle_menu->addAction(IconStyleFiveInOrder);
            CatalogStyle_menu->addAction(IconStyleExt);
        }
            //-------------------------------
            QMenu *FontSizeBooks_menu = new QMenu;
            FontSizeBooks_menu->setTitle("Font size");
            FontSizeBooks_menu->setStyleSheet(SkinStyle);
            settings_menu->addMenu(FontSizeBooks_menu);

            QActionGroup *FontSiyeBooksGroup = new QActionGroup(this);

            QAction *Font8 = new QAction(menu);
            QString Font8Text("8");
            Font8->setText(QString::fromUtf8(Font8Text.toStdString().c_str()));
            Font8->setVisible(true);
            connect(Font8, SIGNAL(triggered()), this, SLOT(FontFromMenu()));
            connect(Font8, SIGNAL(hovered()), this, SLOT(FontFromMenu()));
            FontSiyeBooksGroup->addAction(Font8);

            QAction *Font9 = new QAction(menu);
            QString Font9Text("9");
            Font9->setText(QString::fromUtf8(Font9Text.toStdString().c_str()));
            Font9->setVisible(true);
            connect(Font9, SIGNAL(triggered()), this, SLOT(FontFromMenu()));
            connect(Font9, SIGNAL(hovered()), this, SLOT(FontFromMenu()));
            FontSiyeBooksGroup->addAction(Font9);

            QAction *Font10 = new QAction(menu);
            QString Font10Text("10");
            Font10->setText(QString::fromUtf8(Font10Text.toStdString().c_str()));
            Font10->setVisible(true);
            connect(Font10, SIGNAL(triggered()), this, SLOT(FontFromMenu()));
            connect(Font10, SIGNAL(hovered()), this, SLOT(FontFromMenu()));
            FontSiyeBooksGroup->addAction(Font10);

            QAction *Font11 = new QAction(menu);
            QString Font11Text("11");
            Font11->setText(QString::fromUtf8(Font11Text.toStdString().c_str()));
            Font11->setVisible(true);
            connect(Font11, SIGNAL(triggered()), this, SLOT(FontFromMenu()));
            connect(Font11, SIGNAL(hovered()), this, SLOT(FontFromMenu()));
            FontSiyeBooksGroup->addAction(Font11);

            QAction *Font12 = new QAction(menu);
            QString Font12Text("12");
            Font12->setText(QString::fromUtf8(Font12Text.toStdString().c_str()));
            Font12->setVisible(true);
            //Font12->setCheckable(true);
            connect(Font12, SIGNAL(triggered()), this, SLOT(FontFromMenu()));
            connect(Font12, SIGNAL(hovered()), this, SLOT(FontFromMenu()));
            FontSiyeBooksGroup->addAction(Font12);

            QAction *Font13 = new QAction(menu);
            QString Font13Text("13");
            Font13->setText(QString::fromUtf8(Font13Text.toStdString().c_str()));
            Font13->setVisible(true);
            connect(Font13, SIGNAL(triggered()), this, SLOT(FontFromMenu()));
            connect(Font13, SIGNAL(hovered()), this, SLOT(FontFromMenu()));
            FontSiyeBooksGroup->addAction(Font13);

            QAction *Font14 = new QAction(menu);
            QString Font14Text("14");
            Font14->setText(QString::fromUtf8(Font14Text.toStdString().c_str()));
            Font14->setVisible(true);
            //Font14->setCheckable(true);
            connect(Font14, SIGNAL(triggered()), this, SLOT(FontFromMenu()));
            connect(Font14, SIGNAL(hovered()), this, SLOT(FontFromMenu()));
            FontSiyeBooksGroup->addAction(Font14);

            QAction *Font15 = new QAction(menu);
            QString Font15Text("15");
            Font15->setText(QString::fromUtf8(Font15Text.toStdString().c_str()));
            Font15->setVisible(true);
            connect(Font15, SIGNAL(triggered()), this, SLOT(FontFromMenu()));
            connect(Font15, SIGNAL(hovered()), this, SLOT(FontFromMenu()));
            FontSiyeBooksGroup->addAction(Font15);

            QAction *Font16 = new QAction(menu);
            QString Font16Text("16");
            Font16->setText(QString::fromUtf8(Font16Text.toStdString().c_str()));
            Font16->setVisible(true);
            connect(Font16, SIGNAL(triggered()), this, SLOT(FontFromMenu()));
            connect(Font16, SIGNAL(hovered()), this, SLOT(FontFromMenu()));
            FontSiyeBooksGroup->addAction(Font16);

            FontSizeBooks_menu->addAction(Font8);
            FontSizeBooks_menu->addAction(Font9);
            FontSizeBooks_menu->addAction(Font10);
            FontSizeBooks_menu->addAction(Font11);
            FontSizeBooks_menu->addAction(Font12);
            FontSizeBooks_menu->addAction(Font13);
            FontSizeBooks_menu->addAction(Font14);
            FontSizeBooks_menu->addAction(Font15);
            FontSizeBooks_menu->addAction(Font16);

            //----------------GRID SIZE---------------
            QMenu *SpaceSize_menu = new QMenu;
            SpaceSize_menu->setTitle("Space");
            SpaceSize_menu->setStyleSheet(SkinStyle);
            settings_menu->addMenu(SpaceSize_menu);

            QActionGroup *SpaceSize_menuGroup = new QActionGroup(this);

            QAction *Size160 = new QAction(menu);
            QString Size160Text("160");
            Size160->setText(QString::fromUtf8(Size160Text.toStdString().c_str()));
            Size160->setVisible(true);
            connect(Size160, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size160, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size160);

            QAction *Size180 = new QAction(menu);
            QString Size180Text("180");
            Size180->setText(QString::fromUtf8(Size180Text.toStdString().c_str()));
            Size180->setVisible(true);
            connect(Size180, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size180, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size180);

            QAction *Size200 = new QAction(menu);
            QString Size200Text("200");
            Size200->setText(QString::fromUtf8(Size200Text.toStdString().c_str()));
            Size200->setVisible(true);
            connect(Size200, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size200, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size200);

            QAction *Size220 = new QAction(menu);
            QString Size220Text("220");
            Size220->setText(QString::fromUtf8(Size220Text.toStdString().c_str()));
            Size220->setVisible(true);
            connect(Size220, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size220, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size220);

            QAction *Size240 = new QAction(menu);
            QString Size240Text("240");
            Size240->setText(QString::fromUtf8(Size240Text.toStdString().c_str()));
            Size240->setVisible(true);
            connect(Size240, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size240, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size240);

            QAction *Size260 = new QAction(menu);
            QString Size260Text("260");
            Size260->setText(QString::fromUtf8(Size260Text.toStdString().c_str()));
            Size260->setVisible(true);
            connect(Size260, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size260, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size260);

            QAction *Size280 = new QAction(menu);
            QString Size280Text("280");
            Size280->setText(QString::fromUtf8(Size280Text.toStdString().c_str()));
            Size280->setVisible(true);
            connect(Size280, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size280, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size280);

            QAction *Size300 = new QAction(menu);
            QString Size300Text("300");
            Size300->setText(QString::fromUtf8(Size300Text.toStdString().c_str()));
            Size300->setVisible(true);
            connect(Size300, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size300, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size300);

            QAction *Size320 = new QAction(menu);
            QString Size320Text("320");
            Size320->setText(QString::fromUtf8(Size320Text.toStdString().c_str()));
            Size320->setVisible(true);
            connect(Size320, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size320, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size320);

            QAction *Size340 = new QAction(menu);
            QString Size340Text("340");
            Size340->setText(QString::fromUtf8(Size340Text.toStdString().c_str()));
            Size340->setVisible(true);
            connect(Size340, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size340, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size340);

            QAction *Size360 = new QAction(menu);
            QString Size360Text("360");
            Size360->setText(QString::fromUtf8(Size360Text.toStdString().c_str()));
            Size360->setVisible(true);
            connect(Size360, SIGNAL(triggered()), this, SLOT(SetGridSizeByMenu()));
            connect(Size360, SIGNAL(hovered()), this, SLOT(SetGridSizeByMenu()));
            SpaceSize_menuGroup->addAction(Size360);

            SpaceSize_menu->addAction(Size160);
            SpaceSize_menu->addAction(Size180);
            SpaceSize_menu->addAction(Size200);
            SpaceSize_menu->addAction(Size220);
            SpaceSize_menu->addAction(Size240);
            SpaceSize_menu->addAction(Size260);
            SpaceSize_menu->addAction(Size280);
            SpaceSize_menu->addAction(Size300);
            SpaceSize_menu->addAction(Size320);
            SpaceSize_menu->addAction(Size340);
            SpaceSize_menu->addAction(Size360);

            //-------------------------------
            QMenu *OpenBooksInWhatApp_menu = new QMenu;
            OpenBooksInWhatApp_menu->setTitle("Open books...");
            OpenBooksInWhatApp_menu->setStyleSheet(SkinStyle);
            settings_menu->addMenu(OpenBooksInWhatApp_menu);

            QActionGroup *OpenBookinWhereGroup = new QActionGroup(this);

            QAction *OpenBookInDefaultApp = new QAction(menu);
            QString OpenBookInDefaultAppText("... in default program");
            OpenBookInDefaultApp->setText(QString::fromUtf8(OpenBookInDefaultAppText.toStdString().c_str()));
            OpenBookInDefaultApp->setVisible(true);
            OpenBookInDefaultApp->setCheckable(true);
            connect(OpenBookInDefaultApp, SIGNAL(triggered()), this, SLOT(OpenBooksInDefaultAppOption()));
            OpenBookinWhereGroup->addAction(OpenBookInDefaultApp);

            QAction *OpenBookInThisApp = new QAction(menu);
            QString OpenBookInThisAppText("... here");
            OpenBookInThisApp->setText(QString::fromUtf8(OpenBookInThisAppText.toStdString().c_str()));
            OpenBookInThisApp->setVisible(true);
            OpenBookInThisApp->setCheckable(true);
            connect(OpenBookInThisApp, SIGNAL(triggered()), this, SLOT(OpenBooksInThisAppOption()));
            OpenBookinWhereGroup->addAction(OpenBookInThisApp);

            OpenBooksInWhatApp_menu->addAction(OpenBookInDefaultApp);
            OpenBooksInWhatApp_menu->addAction(OpenBookInThisApp);

            //-------------------------------
        if (DeepnessLevel == 0)
        {
        QMenu *maintenance_menu = new QMenu;
        maintenance_menu->setTitle("Maintenance ");
        maintenance_menu->setStyleSheet(SkinStyle);

            //---
            QAction *ScanFolderForeBooks = new QAction(maintenance_menu);
            QString ScanFolderForeBooksText("Scan for books...");
            ScanFolderForeBooks->setText(QString::fromUtf8(ScanFolderForeBooksText.toStdString().c_str()));
            ScanFolderForeBooks->setVisible(true);
            connect(ScanFolderForeBooks, SIGNAL(triggered()), this, SLOT(ScanFolderForEbooks()));

            //---
            QAction *WatchFolderForeBooks = new QAction(maintenance_menu);
            QString WatchFolderForeBooksText("Folder watcher");
            WatchFolderForeBooks->setText(QString::fromUtf8(WatchFolderForeBooksText.toStdString().c_str()));
            WatchFolderForeBooks->setVisible(true);
            connect(WatchFolderForeBooks, SIGNAL(triggered()), this, SLOT(WatchFolderOpenDialog()));


            maintenance_menu->addAction(ScanFolderForeBooks);
            maintenance_menu->addAction(AddNewBooks);
            maintenance_menu->addSeparator();
            maintenance_menu->addAction(WatchFolderForeBooks);

            menu->addMenu(maintenance_menu);
        }
            //-------------------------------
        menu->addSeparator();

    QAction *StatusMenu = new QAction(menu);
    QString StatusMenuText("Properties");
    StatusMenu->setText(QString::fromUtf8(StatusMenuText.toStdString().c_str()));
    StatusMenu->setVisible(true);
    connect(StatusMenu, SIGNAL(triggered()), this, SLOT(Status()));
    menu->addAction(StatusMenu);
    //-------------------------------
    menu->addSeparator();
    //-------------------------------
    QMenu *help_menu = new QMenu;
    help_menu->setTitle("Help");
    help_menu->setStyleSheet(SkinStyle);

        QAction *HelpPDFMenu = new QAction(help_menu);
        QString HelpPDFMenuText("Help [PDF File]");
        HelpPDFMenu->setText(QString::fromUtf8(HelpPDFMenuText.toStdString().c_str()));
        HelpPDFMenu->setVisible(true);
        connect(HelpPDFMenu, SIGNAL(triggered()), this, SLOT(HelpPDFMenu()));
        help_menu->addAction(HelpPDFMenu);

        QAction *HelpYoutubeVideoMenu = new QAction(help_menu);
        QString HelpYoutubeVideoMenuText("Help [Online Videos]");
        HelpYoutubeVideoMenu->setText(QString::fromUtf8(HelpYoutubeVideoMenuText.toStdString().c_str()));
        HelpYoutubeVideoMenu->setVisible(true);
        connect(HelpYoutubeVideoMenu, SIGNAL(triggered()), this, SLOT(HelpVideo()));
        help_menu->addAction(HelpYoutubeVideoMenu);

    menu->addMenu(help_menu);
    //-------------------------------
    QAction *AboutMenu = new QAction(menu);
    QString AboutMenuText("About");
    AboutMenu->setText(QString::fromUtf8(AboutMenuText.toStdString().c_str()));
    AboutMenu->setVisible(true);
    connect(AboutMenu, SIGNAL(triggered()), this, SLOT(About()));
    menu->addAction(AboutMenu);

    menu->setDefaultAction(AddNewBooks);
    }

    if (!menu->isEmpty())
    {
    menu->exec(ui->CatalogWidget->mapToGlobal(pos));
    }
}

//------------------------------------------------------------------------------------
void MainWindow::WatchFolderOpenDialog()
{
    writeSettings();

    watchfolder *watchfolder1 = new watchfolder(this);
    watchfolder1->setWindowTitle("Folder watcher");
    if (watchfolder1->exec())
        {
        readSettings();
        if (WatchEnabled)
            {
            WatchAndAdd(PathToWatch);
            WatchFolderSetFolder();
            if (DeepnessLevel == 0)
                {
                    ShowCatalogs();
                    repaint();
                }
            }
        }
}

//------------------------------------------------------------------------------------

void MainWindow::WatchFolderSetFolder()
{
QString predhodnaputanja = FolderWatcher->directories().at(0);
FolderWatcher->removePath(predhodnaputanja);
FolderWatcher->addPath(PathToWatch);
}

//------------------------------------------------------------------------------------

void MainWindow::WatchingFolderDirectoryChanged()
{
    if (WatchEnabled)
        {
            WatchAndRemove();
            WatchAndAdd(PathToWatch);
            if (DeepnessLevel == 0)
            {
                ShowCatalogs();
                repaint();
            }
        }
}
//------------------------------------------------------------------------------------

void MainWindow::WatchAndAdd(QString DirPath)
{
QDirIterator dirIt(DirPath,QDirIterator::Subdirectories);
QFileInfo OldFiles;
while (dirIt.hasNext())
{
dirIt.next();
if (QFileInfo(dirIt.filePath()).isFile())
    {
    if ((QFileInfo(dirIt.filePath()).suffix() == "pdf")  || (QFileInfo(dirIt.filePath()).suffix() == "PDF"))
            {
            int FileFound = false;
            for (int i = 0; i < tableWidget->rowCount(); i++)
                {
                OldFiles.setFile(tableWidget->item(i,1)->text());
                if (OldFiles.absoluteFilePath() == dirIt.fileInfo().absoluteFilePath())
                    {
                    FileFound = 0;
                    break;
                    }
                    else if ((dirIt.fileInfo().baseName() == OldFiles.baseName()) && (dirIt.fileInfo().absoluteFilePath() != OldFiles.absoluteFilePath()))
                        {
                        FileFound = 1;
                        break;
                        }
                        else if ((dirIt.fileInfo().baseName() != OldFiles.baseName()) && (dirIt.fileInfo().absoluteFilePath() != OldFiles.absoluteFilePath()))
                            {
                            FileFound = 2;
                            }
                }
            //----
            switch (FileFound) {
                case 2:
                        {
                        //NOVI FAJL UBACI GA
                        int row = tableWidget->rowCount();
                        tableWidget->insertRow(row);

                        QTableWidgetItem *eBookName = new QTableWidgetItem;
                        eBookName->setText(dirIt.fileInfo().baseName());

                        QTableWidgetItem *eBookPath = new QTableWidgetItem;
                        eBookPath->setText(dirIt.fileInfo().absoluteFilePath());

                        QTableWidgetItem *eBookLastPage = new QTableWidgetItem;
                        eBookLastPage->setText("");

                        QTableWidgetItem *eBookCategory = new QTableWidgetItem;
                        eBookCategory->setText("All Books");

                        QTableWidgetItem *eBookFavorite = new QTableWidgetItem;
                        eBookFavorite->setText("");


                        tableWidget->setItem(row, 0, eBookName);
                        tableWidget->setItem(row, 1, eBookPath);
                        tableWidget->setItem(row, 2, eBookLastPage);
                        tableWidget->setItem(row, 3, eBookCategory);
                        tableWidget->setItem(row, 4, eBookFavorite);

                        SaveCatalog();
                        break;
                        }
                break;
                case 1:
                        {
                        //Isto ime, na drugom mestu
                        //ISTI ITEM RAZLICITA ADRESA, PITAJ STA DA SE RADI, DA DODA, DA ZAMENI, NISTA DA NE RADI

                        QString info1 = "Same or similar file already existing";

                        QString info2 = QString("<html><body>Now we have two same or similar pdf documents in diferent locations, here you can compare them. What you want to do now, with new file that you wanted to add?<ul><li>You can add this new file an you will have this two files in shelf</li><li>You can replace old file with new one.</li><li>You don't have to do anything.</li></ul><br>")
                                + QString("<table width='100%' border='0' cellpadding='2'>")
                                + QString("<tr><td align='left'><b>File to add</b></td><td></td><td align='right'><b>File in shelf</b></td></tr>")
                                + QString("<font size=1><tr><td align='left'>" + dirIt.fileInfo().baseName() + "</td><td align='center'><b>name</b></td><td align='right'>" + OldFiles.baseName() + "</td></tr>")
                                + QString("<tr><td align='left'>" + dirIt.fileInfo().path() + "</td><td align='center'><b>path</b></td><td align='right'>" + OldFiles.path() + "</td></tr>")
                                + QString("<tr><td align='left'>" + QString::number(dirIt.fileInfo().size()/1024) + "KB</td><td align='center'><b>size</b></td><td align='right'>" + QString::number(OldFiles.size()/1024) + "KB</td></tr>")
                                + QString("<tr><td align='left'>" + dirIt.fileInfo().created().toString() + "</td><td align='center'><b>created</b></td><td align='right'>" + OldFiles.created().toString() + "</td></tr>")
                                + QString("<tr><td align='left'>" + dirIt.fileInfo().owner() + "</td><td align='center'><b>owner</b></td><td align='right'>" + OldFiles.owner() + "</td></tr>")
                                + QString("</table><body><html>");

                            QMessageBox m;
                            m.setWindowTitle(QApplication::applicationName());
                            m.setWindowIcon(windowIcon());

                            //QDialogButtonBox buttonBox = new QDialogButtonBox;
                            QPushButton* ButtonDontDoAnything = m.addButton(tr("&Cancel"), QMessageBox::NoRole);
                            QPushButton* ButtonReplaceOldFile = m.addButton(tr("&Replace"), QMessageBox::YesRole);
                            QPushButton* ButtonAddNewFile = m.addButton(tr("&Add"), QMessageBox::AcceptRole);

                            m.setDefaultButton(ButtonDontDoAnything);
                            m.setText("<font size=5><b>" + QString::fromUtf8(info1.toStdString().c_str()) + "</b></font>");
                            m.setInformativeText(QString("<table width='600' border='0' cellpadding='6'><tr><td align='left'>") + QString(info2) + "</td></tr></table>");
                        }
                break;
                default: 0;
                        {
                        //Apsolutno isti fajl, nista ne radi
                        }
                break;
            }

            }
            }
    }
SaveCatalog();
}

//------------------------------------------------------------------------------------

void MainWindow::WatchAndRemove()
{
    for (int i = tableWidget->rowCount()-1; i > 0; i--)
    {
        QFile aFile(tableWidget->item(i,1)->text());
        if (!aFile.exists())
        {
            tableWidget->removeRow(i);
        }
    }
SaveCatalog();
}

//------------------------------------------------------------------------------------


void MainWindow::OneClick(QListWidgetItem *item)
{
   item->setSelected(!item->isSelected());
}

//------------------------------------------------------------------------------------
void MainWindow::BookClick(QListWidgetItem *item)
{
    if (QApplication::keyboardModifiers() != Qt::CTRL)// && (mouseEvent->buttons() != Qt::RightButton))
        {
            if (openBookInDefaultApp)
                {
                UnselectItems();
                SelectUnselectItem(ui->CatalogWidget->currentItem(),true);
                QFile FileToOpen(ui->CatalogWidget->currentItem()->whatsThis());

                if (FileToOpen.exists())
                    {
                    QDesktopServices Process;
                    Process.openUrl(QUrl::fromLocalFile(FileToOpen.fileName()));
                    }
                }
            else
                {
                UnselectItems();
                SelectUnselectItem(item,true);
                OpenBook(item);
                }

            ClickedItemText = ui->CatalogWidget->currentItem()->text();
            ClickedItemWhatsThis = ui->CatalogWidget->currentItem()->whatsThis();
            ClickedItemStatusTip = ui->CatalogWidget->currentItem()->statusTip();
            ClickedItemToolTip = ui->CatalogWidget->currentItem()->toolTip();
            for (int i = 0; i < tableWidget->rowCount(); i++)
            {
                if (tableWidget->item(i,1)->text() == ui->CatalogWidget->currentItem()->whatsThis())
                {
                    CurrentOpenedCatalog = tableWidget->item(i,3)->text();
                    ui->BackToThisCatalogButton->setText(CurrentOpenedCatalog);
                }
            }
        }
}

//---------------------------------------------------------

void MainWindow::OpenThisBook()
{
    if (QApplication::keyboardModifiers() != Qt::CTRL)
        {
                UnselectItems();
                SelectUnselectItem(ui->CatalogWidget->currentItem(),true);
                OpenBook(ui->CatalogWidget->currentItem());
        }
}

//---------------------------------------------------------

void MainWindow::OpenThisBookWithDefaultApp()
{
    if (QApplication::keyboardModifiers() != Qt::CTRL)
        {
                UnselectItems();
                SelectUnselectItem(ui->CatalogWidget->currentItem(),true);
                QFile FileToOpen(ui->CatalogWidget->currentItem()->whatsThis());

                if (FileToOpen.exists())
                    {
                    QDesktopServices Process;
                    Process.openUrl(QUrl::fromLocalFile(FileToOpen.fileName()));

                    ClickedItemText = ui->CatalogWidget->currentItem()->text();
                    ClickedItemWhatsThis = ui->CatalogWidget->currentItem()->whatsThis();
                    ClickedItemStatusTip = ui->CatalogWidget->currentItem()->statusTip();
                    ClickedItemToolTip = ui->CatalogWidget->currentItem()->toolTip();
                    }
        }
}

//---------------------------------------------------------

void MainWindow::OpenThisBooksFolder()
{
    if (QApplication::keyboardModifiers() != Qt::CTRL)
        {
                UnselectItems();
                SelectUnselectItem(ui->CatalogWidget->currentItem(),true);
                QFile FileToOpen(ui->CatalogWidget->currentItem()->whatsThis());
                QFileInfo FileInfo(FileToOpen);

                if (FileToOpen.exists())
                    {
                    QDesktopServices Process;
                    Process.openUrl(QUrl::fromLocalFile(FileInfo.path()));
                    }
        }
}

//---------------------------------------------------------

void MainWindow::OpenThisBookWithSomeApp()
{
    if (QApplication::keyboardModifiers() != Qt::CTRL)
        {
                UnselectItems();
                SelectUnselectItem(ui->CatalogWidget->currentItem(),true);
        }
}

//---------------------------------------------------------

void MainWindow::SelectUnselectItem(QListWidgetItem *item, bool isSelected)
{
    item->setSelected(isSelected);
}

//---------------------------------------------------------

void MainWindow::UnselectItems()
{
    for (int i = 0; i < ui->CatalogWidget->count(); i++)
    {
    ui->CatalogWidget->item(i)->setSelected(false);
    }
}

//---------------------------------------------------------

void MainWindow::SelectAllItems()
{
    for (int i = 0; i < ui->CatalogWidget->count(); i++)
    {
    ui->CatalogWidget->item(i)->setSelected(true);
    }
}

//---------------------------------------------------------

void MainWindow::OpenBook(QListWidgetItem *item)
{
    ClickedItemText = item->text();
    ClickedItemWhatsThis = item->whatsThis();
    ClickedItemStatusTip = item->statusTip();
    ClickedItemToolTip = item->toolTip();

    repaint();

    ui->CatalogWidget->hide();
    ui->scrollArea->show();
    ui->BackButton->setEnabled(true);
    ui->BackButton->setText("All Catalogs");
    ui->BackToThisCatalogButton->setEnabled(true);

    IconStyleScattered->setVisible(false);
    IconStyleTwo->setVisible(false);
    IconStyleFiveInOrder->setVisible(false);
    IconStyleInOrder->setVisible(false);
    IconStyleOne->setVisible(false);
    IconStyleTwoScattered->setVisible(false);
    IconStyleExt->setVisible(false);
    IconStyleThree->setVisible(false);
    //--------------------------
    ui->scrollArea->setWidget(myeBookDocument);

    QString S(ClickedItemWhatsThis);
    for (int i = 0; i < tableWidget->rowCount(); i++)
        {
        if (tableWidget->item(i,1)->text() == S)
            {
            QFile filename(tableWidget->item(i,1)->text());
            if (filename.exists())
                {
                myeBookDocument->setDocument(filename.fileName());
                myeBookDocument->showPage(currentPage);
                ui->aSpinBox->setMaximum(myeBookDocument->document->numPages());
                ui->aSpinBox->setSuffix(" of " + QString::number(myeBookDocument->document->numPages()));
                repaint();
                }

            //QString backtoonecatalog(tableWidget->item(i,3)->text());
            ui->BackToThisCatalogButton->setText(CurrentOpenedCatalog);
            ui->BackToThisCatalogButton->setEnabled(true);
            }
        }
    ui->centralWidget->setStyleSheet("#scrollAreaWidgetContents{background: rgba(255,255,255,100);}");

DeepnessLevel = 2;
setWindowTitle("Book: " + ClickedItemToolTip);//
ui->toolBar_pdf->show();
ui->progress_widget->hide();
}

//---------------------------------------------------------
void MainWindow::eBookGoBack()
{
    try
    {
    if (currentPage > 1 )
        {
        myeBookDocument->setPage(--currentPage);
        myeBookDocument->show();
        ui->aSpinBox->setValue(currentPage);
        repaint();
        }
    }
    catch(...){}
}
//---------------------------------------------------------

void MainWindow::eBookGoNext()
{
    try
    {
    if (currentPage < myeBookDocument->document->numPages() )
        {
        myeBookDocument->setPage(++currentPage);
        myeBookDocument->show();
        ui->aSpinBox->setValue(currentPage);
        repaint();
        }
    }
    catch(...){}
}

//---------------------------------------------------------

void MainWindow::eBookGoFirst()
{
    try
    {
        myeBookDocument->setPage(1);
        myeBookDocument->show();
        currentPage = 1;
        ui->aSpinBox->setValue(currentPage);
        repaint();
    }
    catch(...){}
}
//---------------------------------------------------------

void MainWindow::eBookGoLast()
{
    try
    {
        myeBookDocument->setPage(myeBookDocument->document->numPages());
        myeBookDocument->show();
        currentPage = myeBookDocument->document->numPages();
        ui->aSpinBox->setValue(currentPage);
        repaint();
    }
    catch(...){}
}
//---------------------------------------------------------

void MainWindow::eBookGoToPage()
{
    try
    {
        myeBookDocument->setPage(ui->aSpinBox->value());
        myeBookDocument->show();
        currentPage = ui->aSpinBox->value();
        repaint();
    }
    catch(...){}
}
//---------------------------------------------------------

void MainWindow::eBookOriginalSize()
{
    try
    {
        myeBookDocument->setScale(1.0);
        myeBookDocument->adjustSize();
        repaint();
    }
    catch(...){}
}

//---------------------------------------------------------

void MainWindow::eBookZoomIn()
{
    try
    {
        double currentScale = myeBookDocument->scale();
        myeBookDocument->setScale(currentScale + 0.1);
        repaint();
    }
    catch(...){}
}
//---------------------------------------------------------

void MainWindow::eBookZoomOut()
{
    try
    {
        double currentScale = myeBookDocument->scale();
        myeBookDocument->setScale(currentScale - 0.1);
        repaint();
    }
    catch(...){}
}
//---------------------------------------------------------

void MainWindow::readSettings()
{
    QSettings settings("glbs", QApplication::applicationName());
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(800, 450)).toSize());
    move(settings.value("pos", QApplication::desktop()->availableGeometry().center() - this->rect().center()).toPoint());
    settings.endGroup();

    settings.beginGroup("OtherSettings");
    SkinStyleName = settings.value("SkinStyleName", "Ubuntu").toString();
    ThumbStyle = settings.value("ThumbStyle", "four").toString();
    openBookInDefaultApp = settings.value("openBookInDefaultApp", false).toBool();
    fontsize = settings.value("fontsize", 12).toInt();
    Space = settings.value("Space", 260).toInt();
    settings.endGroup();

    settings.beginGroup("FolderWatch");
    PathToWatch = settings.value("PathToWatch", QDir::homePath()).toString();
    WatchEnabled = settings.value("Enabled", false).toBool();
    settings.endGroup();

    //DO
    SkinByName(SkinStyleName);
    FontByNumber(fontsize);
    ui->CatalogWidget->setGridSize(QSize(Space,300));
}

//---------------------------------------------------------

void MainWindow::writeSettings()
{
    QSettings settings("glbs", QApplication::applicationName());
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("Version", QApplication::applicationVersion());
    settings.endGroup();

    settings.beginGroup("OtherSettings");
    settings.setValue("SkinStyleName", SkinStyleName);
    settings.setValue("ThumbStyle", ThumbStyle);
    settings.setValue("openBookInDefaultApp", openBookInDefaultApp);
    settings.setValue("fontsize", fontsize);
    settings.setValue("Space", Space);

    settings.endGroup();
}

//---------------------------------------------------------

void MainWindow::HelpPDFMenu()
{
    QFile FileToOpen("/opt/extras.ubuntu.com/glbs/tutorials/tutorialEN.pdf");

    if (FileToOpen.exists())
        {
        QDesktopServices Process;
        Process.openUrl(QUrl::fromLocalFile(FileToOpen.fileName()));
        }
}

//---------------------------------------------------------

void MainWindow::HelpVideo()
{
QDesktopServices Process1;
Process1.openUrl(QUrl("https://sites.google.com/site/greatlittlebookshelf/home/How-to"));
}

//---------------------------------------------------------
void MainWindow::About()
{
    QString info2 = QString("<table width='100%' border='0' cellpadding='2'>")
    + QString("<tr><td align='justify'><b>ABOUT THIS PROGRAM:</b><br><br>") + "<b>Name: </b>Great Little Book Shelf<br><b>Version: </b>" + QApplication::applicationVersion() + "<br><b>Description: </b>Great Little Book Shelf is application that colects all informations about pdf files from a folder of your choice and categorize them in the way you like it. With this application, you can easely navigate, categorize and read all your pdf files. Our goal is that you feel like you have real book shelf in your computer.<br>Enjoy!</td></tr><tr><td align='left'><PLAINTEXT><b>Copyright: </b>2013-2014 Dejan Petrovic dekiseka.petrovic@gmail.com<br>License: GPL-3.0+</PLAINTEXT><br><br><a href='https://sites.google.com/site/greatlittlebookshelf/'>Visit application website</a></td></tr>"
    +  QString("</table>");

    QString info3 = QString("<table width='100%' border='0' cellpadding='2'>")
    + QString("<tr><td align='justify'><b>ALSO FROM US:</b><br><br>") + "<b>Name: </b>Great Little Radio Player<br>" + "<b>Description: </b>A robust internet radio station streamer. It connects to web sites offering radio streaming and lets you play radio stations directly from that locations.</td></tr><tr><td align='left'><PLAINTEXT><b>Copyright: </b>2013-2014 Dejan Petrovic dekiseka.petrovic@gmail.com<br>License: GPL-3.0+</PLAINTEXT><br><br><a href='https://sites.google.com/site/glrpgreatlittleradioplayer/'>Visit application website</a></td></tr>"
    +  QString("</table>");
        QString info1 = QApplication::applicationName();

        QMessageBox m;
        m.setWindowTitle(QApplication::applicationName() + " - ABOUT");
        m.setWindowIcon(windowIcon());
        m.setText("<font size=5><b>" + QString::fromUtf8(info1.toStdString().c_str()) + "</b></font>");
        m.setInformativeText(QString("<table width='800' border='0' cellpadding='6'><tr><td align='center' width='10%'><img src='" + QString(":/image/IMG/glebc_icon96.png'/>")) + QString("</td><td>" + QString(info2) + "</td></tr><tr><td align='center' width='10%'><img src='" + QString(":/image/IMG/glrp96.png'/>")) + QString("</td><td>" + QString(info3) + "</td></tr></table>"));
        m.exec();
}

//---------------------------------------------------------
void MainWindow::FavoriteONOFF()
{
QStringList AllSelectedItems;
for (int i = 0; i < ui->CatalogWidget->count(); i++)
    {
    if (ui->CatalogWidget->item(i)->isSelected())
        AllSelectedItems << ui->CatalogWidget->item(i)->whatsThis();
    }

//QString CurrentItemPath(ui->CatalogWidget->currentItem()->whatsThis());

for (int f = 0; f < AllSelectedItems.count(); f++)
{
for (int i = 0; i < tableWidget->rowCount(); i++)
    {
        if (AllSelectedItems.at(f) == tableWidget->item(i,1)->text())
        {
            if (tableWidget->item(i,4)->text() == "Favorite")
                tableWidget->item(i,4)->setText("");
                else
                tableWidget->item(i,4)->setText("Favorite");

            break;
        }
    }
}

if (FavoriteIsON)
    Favorites();
    else
    LoadThumbsOrGenerateOne();

SaveCatalog();
CheckIfThereIsFavoritesOrNot();
}

//---------------------------------------------------------
void MainWindow::CheckIfThereIsFavoritesOrNot()
{
    bool isfavpresent = false;
    for (int i = 0; i < tableWidget->rowCount(); i++)
    {
        if (tableWidget->item(i,4)->text() == "Favorite")
        {
            isfavpresent = true;
            break;
        }
    }
    //----
    int numberoffavpresent = 0;
    for (int i = 0; i < tableWidget->rowCount(); i++)
    {
        if (tableWidget->item(i,4)->text() == "Favorite")
            numberoffavpresent++;
    }
    //-----
    if (isfavpresent)
    {
        ui->FavoritePushButton->setEnabled(true);

        if (FavoriteIsON)
        ui->FavoritePushButton->setText("All Catalogs");
        else
            ui->FavoritePushButton->setText("Favorites (" + QString::number(numberoffavpresent) + ")");
    }
    else
    {
        ui->FavoritePushButton->setEnabled(false);
        ui->FavoritePushButton->setText("No favorites");

        //Vrati se nazad
        DeepnessLevel = 0;
        ShowCatalogs();
        repaint();
        ui->CatalogWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    }

}

//---------------------------------------------------------
void MainWindow::SetGridSizeByMenu()
{
    int number;
    QAction *A;
    QObject *Y = QObject::sender();
    A = qobject_cast<QAction *>(Y);
    if (Y)
        number = A->text().toInt();

    ui->CatalogWidget->setGridSize(QSize(number,300));
    Space = number;
}
