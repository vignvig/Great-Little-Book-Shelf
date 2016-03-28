#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ebookdocument.h"
#include <QListWidgetItem>
#include <poppler-qt4.h>
#include "QMessageBox"
#include "QTableWidgetItem"
#include "QSpinBox"
#include "QMouseEvent"
#include <QFileSystemWatcher>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SaveCatalog();
    void SloveIfCatalogFileNotExists();
    void LoadCatalog(QString PathToCatalogFile);
    int currentPage;
    qreal scaleFactor;
    qreal scaleFactorForPreView;
    int DeepnessLevel;
    int restoreX;
    int restoreY;
    int restoreW;
    int restoreH;
    QString CurrentOpenedCatalog;
    bool FavoriteIsON;
    QString ClickedItemText;
    QString ClickedItemWhatsThis;
    QString ClickedItemStatusTip;
    QString ClickedItemToolTip;
    QString ThumbStyle;

    QString PathToCatalogFile;
    QTableWidget *tableWidget;
    QString PathToCatalogDir;

    QWidget *ProgressWidget;
    //Watching
    QFileSystemWatcher *FolderWatcher;
    QString PathToWatch;
    bool WatchEnabled;
    //---
    QLabel *Label;
    QAction *pdfOriginalSize;
    QAction *pdfZoomIn;
    QAction *pdfZoomOut;
    QAction *IconStyleOne;
    QAction *IconStyleScattered;
    QAction *IconStyleExt;
    QAction *IconStyleFiveInOrder;
    QAction *IconStyleInOrder;
    QAction *IconStyleTwo;
    QAction *IconStyleTwoScattered;
    QAction *IconStyleThree;
    QAction *pdfBack;
    QAction *pdfFirst;
    QAction *pdfLast;
    QAction *pdfNext;
    QWidget *emptyWidget1;
    QWidget *emptyWidget2;
    QWidget *emptyWidget3;
    QAction *OptionButton;
    QSpinBox *aSpinBox;
    int eBookPage;
    QMessageBox testmsg;
    QString SkinStyle;
    QString SkinStyleName;
    QMenu *settings_menu;
    bool openBookInDefaultApp;
    int fontsize;
    int Space;


    
private slots:
    void ShowCatalogs();
    void SetAppStyle();
    void ScanFolderForEbooks();
    void AddNewBooks();
    void ShowContextMenu(const QPoint& pos);
    void RenameBook();
    void MoveToNewCatalog();
    void MoveToExistingCatalog();
    void OpenThisBooksFolder();
    void RenameCatalog();
    void LoadThumbsOrGenerateOne();
    void OpenBook(QListWidgetItem *item);
    void OpenThisBook();
    void OpenThisBookWithDefaultApp();
    void OpenThisBookWithSomeApp();
    void OpenBooksInDefaultAppOption();
    void OpenBooksInThisAppOption();
    void DeleteBook();
    void DeleteBookDisk();
    void BookProperties();
    void CatalogClick(QListWidgetItem *item);
    void Favorites();
    void FavoritesButtonClick();
    void OneClick(QListWidgetItem *item);
    void BookClick(QListWidgetItem *item);
    void UnselectItems();
    void SelectAllItems();
    void SelectUnselectItem(QListWidgetItem *item, bool isSelected);
    void SetToolBar1();
    void Connecting();
    void SetProgressWidget();
    void Status();
    void HelpPDFMenu();
    void HelpVideo();
    void About();
    void WatchFolderOpenDialog();
    void WatchFolderSetFolder();
    void WatchingFolderDirectoryChanged();
    void WatchAndRemove();
    void WatchAndAdd(QString DirPath);


public slots:
    void BackToAllCatalogsButtonClick();
    void BackToThisCatalogsButtonClick();
    void CatalogBookClick(QListWidgetItem *item);
    void eBookGoBack();
    void eBookGoNext();
    void eBookGoFirst();
    void eBookGoLast();
    void eBookGoToPage();
    void eBookOriginalSize();
    void eBookZoomIn();
    void eBookZoomOut();
    void SetOneStyle();
    void SetScatteredStyle();
    void SetFiveInOrderStyle();
    void SetThreeStyle();
    void SetInOrderStyle();
    void SetExtStyle();
    void SetTwoStyle();
    void SetTwoScatteredStyle();
    void CheckIfCatalogFilesStillExists();
    void SettingsDialog();
    void GetAvaiableSkins();
    void FontFromMenu();
    void FontByNumber(int number);
    void SkinFromMenu();
    void SkinByName(QString skin);
    void writeSettings();
    void readSettings();
    void hit();
    void FavoriteONOFF();
    void CheckIfThereIsFavoritesOrNot();
    void SetGridSizeByMenu();

private:
    Ui::MainWindow *ui;
    eBookDocument *myeBookDocument;
    QPoint dragPosition;
    QMenu *menu;
    void wheelEvent(QWheelEvent* event);

protected:

};

#endif // MAINWINDOW_H
