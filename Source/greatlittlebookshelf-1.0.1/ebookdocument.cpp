#include "ebookdocument.h"
#include <QtGui>
#include <poppler-qt4.h>

eBookDocument::eBookDocument(QWidget *parent) :
    QLabel(parent)
{
    trenutnaStrana = -1;
    document = 0;
    SelectionBoundary = 0;
    FactorSkaliranja = 1.0;
    setAlignment(Qt::AlignCenter);
}

//----------------------------------------------------

eBookDocument::~eBookDocument()
{
    delete document;
}

//----------------------------------------------------

Poppler::Document *eBookDocument::aDocument()
{
    return document;
}

//----------------------------------------------------

QMatrix eBookDocument::matrix() const
{
    return QMatrix(FactorSkaliranja * physicalDpiX() / 72.0, 0,
                   0, FactorSkaliranja * physicalDpiY() / 72.0,
                   0, 0);
}

//----------------------------------------------------

/*void eBookDocument::wheelEvent(QWheelEvent* event)
{
    if (QApplication::keyboardModifiers() == Qt::CTRL)
    {
        if (event->delta() > 0)
            setScale(FactorSkaliranja + 0.075);
        else if (event->delta() < 0)
                setScale(FactorSkaliranja - 0.075);
    }
}*/
//----------------------------------------------------

void eBookDocument::mousePressEvent(QMouseEvent *event)
{
    if (!document)
        return;

    dragPosition = event->pos();
    if (!SelectionBoundary)
        SelectionBoundary = new QRubberBand(QRubberBand::Rectangle, this);
    SelectionBoundary->setGeometry(QRect(dragPosition, QSize()));
    SelectionBoundary->show();
}

//----------------------------------------------------

void eBookDocument::mouseMoveEvent(QMouseEvent *event)
{
    if (!document)
        return;

    SelectionBoundary->setGeometry(QRect(dragPosition, event->pos()).normalized());
}

//----------------------------------------------------

void eBookDocument::mouseReleaseEvent(QMouseEvent *)
{
    if (!document)
        return;

    if (!SelectionBoundary->size().isEmpty()) {
        // Correct for the margin around the image in the label.
        QRectF rect = QRectF(SelectionBoundary->pos(), SelectionBoundary->size());
        rect.moveLeft(rect.left() - (width() - pixmap()->width()) / 2.0);
        rect.moveTop(rect.top() - (height() - pixmap()->height()) / 2.0);
        selectedText(rect);
    }

    SelectionBoundary->hide();
}

//----------------------------------------------------

qreal eBookDocument::scale() const
{
    return FactorSkaliranja;
}

//----------------------------------------------------

void eBookDocument::showPage(int page)
{
    if (page != -1 && page != trenutnaStrana + 1)
    {
        trenutnaStrana = page - 1;
        emit pageChanged(page);
    }

    QImage image = document->page(trenutnaStrana)->renderToImage(FactorSkaliranja * physicalDpiX(), FactorSkaliranja * physicalDpiY());

    if (!searchLocation.isEmpty())
    {
        QRect highlightRect = matrix().mapRect(searchLocation).toRect();
        highlightRect.adjust(-2, -2, 2, 2);
        QImage highlight = image.copy(highlightRect);
        QPainter painter;
        painter.begin(&image);
        painter.fillRect(image.rect(), QColor(0, 0, 0, 32));
        painter.drawImage(highlightRect, highlight);
        painter.end();
    }

    setPixmap(QPixmap::fromImage(image));
}

//----------------------------------------------------

void eBookDocument::showPageFit(int page)
{
    if (page != -1 && page != trenutnaStrana + 1)
    {
        trenutnaStrana = page - 1;
        emit pageChanged(page);
    }

    QImage image = document->page(trenutnaStrana)->renderToImage(physicalDpiX()/FactorSkaliranja, physicalDpiY()/FactorSkaliranja);

    if (!searchLocation.isEmpty())
    {
        QRect highlightRect = matrix().mapRect(searchLocation).toRect();
        highlightRect.adjust(-2, -2, 2, 2);
        QImage highlight = image.copy(highlightRect);
        QPainter painter;
        painter.begin(&image);
        painter.fillRect(image.rect(), QColor(0, 0, 0, 32));
        painter.drawImage(highlightRect, highlight);
        painter.end();
    }

    setPixmap(QPixmap::fromImage(image));
}
//----------------------------------------------------

QRectF eBookDocument::searchBackwards(const QString &text)
{
    QRectF oldLocation = searchLocation;

    int page = trenutnaStrana;
    if (oldLocation.isNull())
        page -= 1;

    while (page > -1)
    {
        QList<QRectF> locations;
        searchLocation = QRectF();

        while (document->page(page)->search(text, searchLocation,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {

            if (searchLocation != oldLocation)
                locations.append(searchLocation);
            else
                break;
        }

        int index = locations.indexOf(oldLocation);
        if (index == -1 && !locations.isEmpty()) {
            searchLocation = locations.last();
            showPage(page + 1);
            return searchLocation;
        } else if (index > 0) {
            searchLocation = locations[index - 1];
            showPage(page + 1);
            return searchLocation;
        }

        oldLocation = QRectF();
        page -= 1;
    }

    if (trenutnaStrana == document->numPages() - 1)
        return QRectF();

    oldLocation = QRectF();
    page = document->numPages() - 1;

    while (page > trenutnaStrana)
    {
        QList<QRectF> locations;
        searchLocation = QRectF();

        while (document->page(page)->search(text, searchLocation,Poppler::Page::NextResult, Poppler::Page::CaseInsensitive))
        {
            locations.append(searchLocation);
        }

        if (!locations.isEmpty())
        {
            searchLocation = locations.last();
            showPage(page + 1);
            return searchLocation;
        }
        page -= 1;
    }

    return QRectF();
}

//----------------------------------------------------

QRectF eBookDocument::searchForwards(const QString &text)
{
    int page = trenutnaStrana;
    while (page < document->numPages())
    {
        if (document->page(page)->search(text, searchLocation,Poppler::Page::NextResult, Poppler::Page::CaseInsensitive))
        {
            if (!searchLocation.isNull())
                {
                    showPage(page + 1);
                    return searchLocation;
                }
        }
        page += 1;
        searchLocation = QRectF();
    }

    page = 0;

    while (page < trenutnaStrana)
    {
        searchLocation = QRectF();

        if (document->page(page)->search(text, searchLocation,Poppler::Page::NextResult, Poppler::Page::CaseInsensitive))
        {
            if (!searchLocation.isNull())
                {
                    showPage(page + 1);
                    return searchLocation;
                }
        }
        page += 1;
    }

    return QRectF();
}

//----------------------------------------------------

void eBookDocument::selectedText(const QRectF &rect)
{
    QRectF selectedRect = matrix().inverted().mapRect(rect);
    // QString text = doc->page(trenutnaStrana)->text(selectedRect);

    QString text;
    bool hadSpace = false;
    QPointF center;
    foreach (Poppler::TextBox *box, document->page(trenutnaStrana)->textList()) {
        if (selectedRect.intersects(box->boundingBox())) {
            if (hadSpace)
                text += " ";
            if (!text.isEmpty() && box->boundingBox().top() > center.y())
                text += "\n";
            text += box->text();
            hadSpace = box->hasSpaceAfter();
            center = box->boundingBox().center();
        }
    }

    if (!text.isEmpty())
        emit textSelected(text);
}

//----------------------------------------------------

bool eBookDocument::setDocument(const QString &filePath)
{
    Poppler::Document *oldDocument = document;

    document = Poppler::Document::load(filePath);
    if (document) {
        delete oldDocument;
        document->setRenderHint(Poppler::Document::Antialiasing);
        document->setRenderHint(Poppler::Document::TextAntialiasing);
        searchLocation = QRectF();
        trenutnaStrana = -1;
        setPage(1);
    }
    return document != 0;
}

//----------------------------------------------------

void eBookDocument::setPage(int page)
{
    if (page != trenutnaStrana + 1) {
        searchLocation = QRectF();
        showPage(page);
    }
}

//----------------------------------------------------

void eBookDocument::setScale(qreal scale)
{
    if (FactorSkaliranja != scale)
    {
        FactorSkaliranja = scale;
        showPage();
    }
}


