/**
 * @file QtVk.h
 * @brief
 * @details
 * @date Created on 08.12.2015
 * @copyright 2015 DDwarf LLC, <dev@ddwarf.org>
 * @author Gilmanov Ildar <dev@ddwarf.org>
 */

#ifndef DDWARF_SOCIAL_QTVK_H
#define DDWARF_SOCIAL_QTVK_H

#include <QObject>
#include <QVariantMap>
#include <QtQml>

namespace DDwarf {
namespace Social {

class QtVk : public QObject
{
    Q_OBJECT

public:
    static void initStatic();

    /**
     * Singleton object provider for C++
     */
    static QtVk *instance();

public slots:

    /**
     * Show dialog for sharing
     * @param textToPost Post text. User can change that text
     * @param photoLinks Array of already uploaded photos from VK, that will be attached to post
     * @param photos Images that will be uploaded with post. Supported types: QPixmap, QQuickItemGrabResult
     * @param linkTitle A small description for your link
     * @param linkUrl Url that link follows
     */
    void openShareDialog(const QString &textToPost,
                         const QVariantList &photoLinks,
                         const QVariantList &photos,
                         const QString &linkTitle,
                         const QString &linkUrl);

signals:
    /**
     * Emitted when an operation is completed
     * @param operation The name of the method called (i.e. openShareDialog)
     * @param data Eventually data returned by the operation completed
     */
    void operationCompleted(QString operation, QVariantMap data = QVariantMap());

    /**
     * Emitted when an VKontakte operation is canceled
     * @param operation The name of the method called (i.e. openShareDialog)
     */
    void operationCancel(QString operation);

    /**
     * Emitted when an error occur during a VKontakte operation
     * @param operation The name of the method called (i.e. openShareDialog)
     * @param error The error returned by VKontakte
     */
    void operationError(QString operation, QString error);

private:
    static QtVk *m_instance;

    explicit QtVk(QObject *parent = 0);
    ~QtVk();

    Q_DISABLE_COPY(QtVk)

    /**
     * Singleton type provider function for Qt Quick
     */
    static QObject *qtVkProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

    void openShareDialog(const QString &textToPost,
                         const QStringList &photoLinks,
                         const QList<QPixmap> &photos,
                         const QString &linkTitle,
                         const QString &linkUrl);
};

} // namespace Social
} // namespace DDwarf

#endif // DDWARF_SOCIAL_QTVK_H
