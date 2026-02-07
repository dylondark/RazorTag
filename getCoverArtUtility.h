#ifndef GETCOVERARTUTILITY_H
#define GETCOVERARTUTILITY_H

#include <QPixmap>
#include <taglib/audioproperties.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>

QPixmap getCoverArtUtility(const QString& filePath)
{
    TagLib::MPEG::File file(filePath.toUtf8().constData());
    TagLib::ID3v2::Tag* tag = file.ID3v2Tag();

    if (!tag)
        return {};

    auto frames = tag->frameList("APIC");
    if (frames.isEmpty())
        return {};

    auto* frame =
        static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());

    const auto& data = frame->picture();

    QPixmap pixmap;
    pixmap.loadFromData(
        reinterpret_cast<const uchar*>(data.data()),
        data.size()
        );

    return pixmap;
}

#endif // GETCOVERARTUTILITY_H
