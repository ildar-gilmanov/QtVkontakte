/**
 * @file QtVk.cpp
 * @brief
 * @details
 * @date Created on 08.12.2015
 * @copyright 2015 DDwarf LLC, <gil_ildar@mail.ru>
 * @author Gilmanov Ildar <gil_ildar@mail.ru>
 */

#include "QtVk.h"
#include <QPixmap>
#include <QtQuick/QQuickItemGrabResult>
#include <QDebug>

namespace DDwarf {
namespace Social {

QtVk *QtVk::m_instance = nullptr;

void QtVk::initStatic()
{
    if(m_instance)
    {
        qWarning() << QString("QtVk is already initialized");
    }

    m_instance = new QtVk();

    qmlRegisterSingletonType<QtVk>("org.ddwarf.social", 1, 0, "QtVk", &QtVk::qtVkProvider);
}

QtVk *QtVk::instance()
{
    if(!m_instance)
    {
        qWarning() << "QtVk had not initialized. You must call QtVk::initStatic() method after Application created";
    }

    return m_instance;
}

QtVk::QtVk(QObject *parent) :
    QObject(parent)
{
}

QtVk::~QtVk()
{
}

QObject *QtVk::qtVkProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return m_instance;
}

void QtVk::openShareDialog(const QString &textToPost,
                           const QStringList *photoLinks,
                           const QVariantList *photos,
                           const QString &linkTitle,
                           const QString &linkUrl)
{
    QList<QPixmap> pixmapList;

    for(QVariantList::const_iterator it = photos->cbegin(); it != photos->cend(); ++it)
    {
        const QVariant &var = *it;

        if(var.canConvert<QPixmap>())
        {
            pixmapList.push_back(var.value<QPixmap>());
        }
        else if(var.canConvert<QQuickItemGrabResult*>())
        {
            QQuickItemGrabResult *grabResult = var.value<QQuickItemGrabResult*>();
            pixmapList.push_back(QPixmap::fromImage(grabResult->image()));
        }
        else
        {
            qWarning() << "Incorrect value for photos";
        }
    }

    openShareDialog(textToPost,
                    photoLinks,
                    pixmapList,
                    linkTitle,
                    linkUrl);
}

} // namespace Social
} // namespace DDwarf

