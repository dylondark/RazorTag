#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMimeData>
#include <QUrl>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileInBrowseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open File",
        QDir::homePath(),              // starting directory
        "Music files (*.mp3 *.flac *.wav *.aac *.ogg *.m4a *.opus);;All files (*.*)"
    );

    ui->fileInPathBox->setText(fileName);
    ui->fileOutPathBox->setText(fileName);
}

void MainWindow::on_getMetadataButton_clicked()
{
    QString fileName = ui->fileInPathBox->text();

    if (fileName.isEmpty())
        return;

    TagLib::FileRef f(fileName.toUtf8().constData());

    if (!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();

        QString title  = QString::fromStdString(tag->title().to8Bit(true));
        QString artist = QString::fromStdString(tag->artist().to8Bit(true));
        QString album  = QString::fromStdString(tag->album().to8Bit(true));
        QString genre  = QString::fromStdString(tag->genre().to8Bit(true));
        int year       = tag->year();
        int track      = tag->track();

        ui->titleBox->setText(title);
        ui->artistBox->setText(artist);
        ui->albumBox->setText(album);
        ui->genreBox->setText(genre);
        ui->yearBox->setText(year > 0 ? QString::number(year) : "");
        ui->trackBox->setText(track > 0 ? QString::number(track) : "");
    }

    if (f.audioProperties()) {
        TagLib::AudioProperties *props = f.audioProperties();
        qDebug() << "Length (s):" << props->length();
        qDebug() << "Bitrate:" << props->bitrate();
        qDebug() << "Sample rate:" << props->sampleRate();
    }

    qDebug() << "\n";
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // check if dropped folder has a url (is a file)
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    // Take the first dropped file
    QString filePath = urls.first().toLocalFile();

    if (!filePath.isEmpty()) {
        ui->fileInPathBox->setText(filePath);
        qDebug() << "Dropped file:" << filePath;
    }
}

void MainWindow::on_tagButton_clicked()
{
    QString fileName  = ui->fileOutPathBox->text();
    QString newTitle  = ui->titleBox->text();
    QString newArtist = ui->artistBox->text();
    QString newAlbum  = ui->albumBox->text();
    QString newGenre  = ui->genreBox->text();
    QString newTrack  = ui->trackBox->text();
    QString newYear   = ui->yearBox->text();

    if (fileName.isEmpty())
        return;

    TagLib::FileRef f(fileName.toUtf8().constData());

    if (f.isNull() || !f.tag())
        return;

    TagLib::Tag* tag = f.tag();
    bool modified = false;

    // ---- Write only if user provided input ----
    if (!newTitle.isEmpty()) {
        tag->setTitle(TagLib::String(newTitle.toUtf8().constData(),
                                     TagLib::String::UTF8));
        modified = true;
    }

    if (!newArtist.isEmpty()) {
        tag->setArtist(TagLib::String(newArtist.toUtf8().constData(),
                                      TagLib::String::UTF8));
        modified = true;
    }

    if (!newAlbum.isEmpty()) {
        tag->setAlbum(TagLib::String(newAlbum.toUtf8().constData(),
                                     TagLib::String::UTF8));
        modified = true;
    }

    if (!newGenre.isEmpty()) {
        tag->setGenre(TagLib::String(newGenre.toUtf8().constData(),
                                     TagLib::String::UTF8));
        modified = true;
    }

    if (!newYear.isEmpty()) {
        bool ok;
        int year = newYear.toInt(&ok);
        if (ok) {
            tag->setYear(year);
            modified = true;
        }
    }

    if (!newTrack.isEmpty()) {
        bool ok;
        int track = newTrack.toInt(&ok);
        if (ok) {
            tag->setTrack(track);
            modified = true;
        }
    }

    if (modified) {
        f.file()->save();
        qDebug() << "Metadata updated successfully";
    }

    qDebug() << "Title:"  << QString::fromStdString(tag->title().to8Bit(true));
    qDebug() << "Artist:" << QString::fromStdString(tag->artist().to8Bit(true));
    qDebug() << "Album:"  << QString::fromStdString(tag->album().to8Bit(true));
    qDebug() << "Genre:"  << QString::fromStdString(tag->genre().to8Bit(true));
    qDebug() << "Year:"   << tag->year();
    qDebug() << "Track:"  << tag->track();
    qDebug() << "\n";
}

