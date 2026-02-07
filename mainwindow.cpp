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

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open File",
        QDir::homePath(),              // starting directory
        "Music files (*.mp3 *.flac *.wav *.aac *.ogg *.m4a *.opus);;All files (*.*)"
    );

    ui->filePathBox->setText(fileName);
}

void MainWindow::on_getMetadataButton_clicked()
{
    QString fileName = ui->filePathBox->text();

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
        ui->filePathBox->setText(filePath);
        qDebug() << "Dropped file:" << filePath;
    }
}
