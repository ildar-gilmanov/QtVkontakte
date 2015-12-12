/**
 * @file QtVk-android.cpp
 * @brief
 * @details
 * @date Created on 08.12.2015
 * @copyright 2015 DDwarf LLC, <gil_ildar@mail.ru>
 * @author Gilmanov Ildar <gil_ildar@mail.ru>
 */

#include "QtVk.h"
#include <QDebug>
#include <QtAndroidExtras>

namespace DDwarf {
namespace Social {

void QtVk::openShareDialog(const QString &textToPost,
                           const QStringList *photoLinks,
                           const QList<QPixmap> &photos,
                           const QString &linkTitle,
                           const QString &linkUrl)
{
    QString message = QString("openShareDialog: textToPost: '%1' photoLinks: ").arg(textToPost);

    for(QStringList::const_iterator it = photoLinks->cbegin(); it != photoLinks->cend(); ++it)
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
}

static void fromJavaOnOperationCompleted(JNIEnv *env,
                                         jobject thiz,
                                         jstring operation,
                                         jobjectArray data) {
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    if(QtVk::instance())
    {
        QString operationString = QAndroidJniObject(operation).toString();
        QVariantMap dataMap;
        int count = env->GetArrayLength(data);

        for(int i = 0; i < count; i = i + 2)
        {
            QAndroidJniObject key(env->GetObjectArrayElement(data, i));
            QAndroidJniObject value;

            if((i + 1) < count)
            {
                value = env->GetObjectArrayElement(data, i + 1);
            }

            QString qkey = key.toString();

            if(qkey.endsWith(":list"))
            {
                qkey.chop(5);
                dataMap[qkey] = value.toString().split(',', QString::SkipEmptyParts);
            }
            else
            {
                dataMap[qkey] = value.toString();
            }
        }

        QMetaObject::invokeMethod(QtVk::instance(),
                                  "operationCompleted",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, operationString),
                                  Q_ARG(QVariantMap, dataMap));
    }
}

static void fromJavaOnOperationCancel(JNIEnv *env, jobject thiz, jstring operation)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    if(QtVk::instance())
    {
        QString operationString = QAndroidJniObject(operation).toString();

        QMetaObject::invokeMethod(QtVk::instance(),
                                  "operationCancel",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, operationString));
    }
}

static void fromJavaOnOperationError(JNIEnv *env, jobject thiz, jstring operation, jstring error)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)

    if(QtVk::instance())
    {
        QString operationString = QAndroidJniObject(operation).toString();
        QString errorString = QAndroidJniObject(error).toString();

        QMetaObject::invokeMethod(QtVk::instance(),
                                  "operationError",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, operationString),
                                  Q_ARG(QString, errorString));
    }
}

static JNINativeMethod methods[]
{
    {"operationCompleted", "(Ljava/lang/String;[Ljava/lang/String;)V",(void*)(fromJavaOnOperationCompleted)},
    {"operationCancel", "(Ljava/lang/String;)V", (void*)(fromJavaOnOperationCancel)},
    {"operationError", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)(fromJavaOnOperationError)}
};

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *)
{
    JNIEnv *env;
    if(vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4) != JNI_OK)
    {
        return JNI_FALSE;
    }

    jclass clazz = env->FindClass("org/ddwarf/vk/QtVkBinding");

    if(env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0)
    {
        return JNI_FALSE;
    }

    return JNI_VERSION_1_4;
}

} // namespace Social
} // namespace DDwarf

