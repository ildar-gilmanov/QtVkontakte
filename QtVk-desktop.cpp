/**
 * @file QtVk-desktop.cpp
 * @brief
 * @details
 * @date Created on 11.12.2015
 * @copyright 2015 DDwarf LLC, <gil_ildar@mail.ru>
 * @author Gilmanov Ildar <gil_ildar@mail.ru>
 */

#include "QtVk.h"
#include <QPixmap>
#include <QDebug>

namespace DDwarf {
namespace Social {

void QtVk::openShareDialog(const QString &textToPost,
                           const QStringList &photoLinks,
                           const QList<QPixmap> &photos,
                           const QString &linkTitle,
                           const QString &linkUrl)
{
    QString message = QString("openShareDialog: textToPost: '%1' photoLinks: ").arg(textToPost);

    for(QStringList::const_iterator it = photoLinks.cbegin(); it != photoLinks.cend(); ++it)
    {
        message.push_back(*it);
        message.push_back(", ");
    }

    message.push_back("photos: ");

    for(QList<QPixmap>::const_iterator it = photos.cbegin(); it != photos.cend(); ++it)
    {
        const QPixmap &pixmap = *it;

        message.push_back(QString("pixmap(%1, %2)")
                          .arg(pixmap.width())
                          .arg(pixmap.height()));

        message.push_back(", ");
    }

    message.push_back(QString("linkTitle: '%1'' linkUrl: '%2'")
                      .arg(linkTitle)
                      .arg(linkUrl));

    qInfo() << message;

    qWarning() << QString("Method openShareDialog() is not implemented yet");
}

} // namespace Social
} // namespace DDwarf

