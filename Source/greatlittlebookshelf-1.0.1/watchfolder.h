#ifndef WATCHFOLDER_H
#define WATCHFOLDER_H

#include <QDialog>

namespace Ui {
class watchfolder;
}

class watchfolder : public QDialog
{
    Q_OBJECT

public:
    QString PathToWatch;
    explicit watchfolder(QWidget *parent = 0);
    ~watchfolder();

public slots:
    void writeSettings();
    void readSettings();
    void browseFolder();
    void EnableDisable();

private:
    Ui::watchfolder *ui;
};

#endif // WATCHFOLDER_H
