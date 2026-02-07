#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
        QDir::homePath(),
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

        qDebug() << "Title:" << title;
        qDebug() << "Artist:" << artist;
        qDebug() << "Album:" << album;
        qDebug() << "Genre:" << genre;
        qDebug() << "Year:" << year;
        qDebug() << "Track:" << track;
    }

    if (f.audioProperties()) {
        TagLib::AudioProperties *props = f.audioProperties();
        qDebug() << "Length (s):" << props->length();
        qDebug() << "Bitrate:" << props->bitrate();
        qDebug() << "Sample rate:" << props->sampleRate();
    }
}

