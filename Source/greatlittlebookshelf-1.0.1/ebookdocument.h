#ifndef EBOOKDOCUMENT_H
#define EBOOKDOCUMENT_H

#include <QLabel>
#include <QRectF>
#include <poppler-qt4.h>

class QRubberBand;

class eBookDocument : public QLabel
{
    Q_OBJECT

public:
    eBookDocument(QWidget *parent = 0);
    ~eBookDocument();
    Poppler::Document *aDocument();
    QMatrix matrix() const;
    qreal scale() const;
    Poppler::Document *document;
    qreal FactorSkaliranja;

signals:
    void pageChanged(int trenutnaStrana);
    void textSelected(const QString &text);
    
public slots:
    QRectF searchBackwards(const QString &text);
    QRectF searchForwards(const QString &text);
    bool setDocument(const QString &filePath);
    void setPage(int page = -1);
    void setScale(qreal scale);
    void showPage(int page = -1);
    void showPageFit(int page = -1);
    //void wheelEvent(QWheelEvent* event);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private:
    void selectedText(const QRectF &rect);



    int trenutnaStrana;
    QPoint dragPosition;
    QRubberBand *SelectionBoundary;
    QRectF searchLocation;

    
};

#endif // EBOOKDOCUMENT_H
