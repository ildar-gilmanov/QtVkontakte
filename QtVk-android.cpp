/**
 * @file QtVk-android.cpp
 * @brief
 * @details
 * @date Created on 08.12.2015
 * @copyright 2015 DDwarf LLC, <gil_ildar@mail.ru>
 * @author Gilmanov Ildar <gil_ildar@mail.ru>
 */

#include "QtVk.h"
#include <QPixmap>
#include <QtAndroidExtras>
#include <QDebug>

namespace DDwarf {
namespace Social {

static const QString jClassName = "org/ddwarf/vk/QtVkBinding";

static jobjectArray createStringList(QAndroidJniEnvironment &env, const QStringList &stringList)
{
    // Get the string class
    jclass stringClass = env->FindClass("java/lang/String");

    // Check if we properly got the string class
    if(!stringClass)
    {
        qWarning() << "Can not get string class";
        return nullptr;
    }

    jobjectArray stringArray = env->NewObjectArray(stringList.size(), stringClass, nullptr);

    for(int i = 0; i < stringList.size(); ++i)
    {
        env->SetObjectArrayElement(stringArray,
                                   i,
                                   QAndroidJniObject::fromString(stringList.at(i))
                                   .object<jstring>());
    }

    return stringArray;
}

static jobjectArray createPixmapList(QAndroidJniEnvironment &env, const QList<QPixmap> &photos)
{
    // Get the byte array class
    jclass byteArrayClass = env->FindClass("[B");

    // Check if we properly got the byte array class
    if(!byteArrayClass)
    {
        qWarning() << "Can not get byte array class";
        return nullptr;
    }

    // Create the 2D array
    jobjectArray photosArray = env->NewObjectArray(photos.size(), byteArrayClass, nullptr);

    // Go through the firs dimension and add the second dimension arrays
    for(int i = 0; i < photos.size(); ++i)
    {
        QByteArray imgData;
        QBuffer buffer(&imgData);
        buffer.open(QIODevice::WriteOnly);
        photos.at(i).save(&buffer, "PNG");

        jbyteArray imgBytes = env->NewByteArray(imgData.size());
        env->SetByteArrayRegion(imgBytes, 0, imgData.size(), (jbyte*)imgData.constData());
        env->SetObjectArrayElement(photosArray, i, imgBytes);
        env->DeleteLocalRef(imgBytes);
    }

    return photosArray;
}

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

    QAndroidJniEnvironment env;

    jobjectArray photoLinksArray = nullptr;

    if(!photoLinks.isEmpty())
    {
        photoLinksArray = createStringList(env, photoLinks);

        if(!photoLinksArray)
        {
            qWarning() << "Can not create photo links array";
            return;
        }
    }

    jobjectArray photosArray = nullptr;

    if(!photos.isEmpty())
    {
        photosArray = createPixmapList(env, photos);

        if(!photosArray)
        {
            qWarning() << "Can not create photos array";
            return;
        }
    }

    // call the java implementation
    QAndroidJniObject::callStaticMethod<void>(
                jClassName.toLatin1().data(),
                "openShareDialog",
                "(Ljava/lang/String;[Ljava/lang/String;[[BLjava/lang/String;Ljava/lang/String;)V",
                QAndroidJniObject::fromString(textToPost).object<jstring>(),
                photoLinksArray,
                photosArray,
                QAndroidJniObject::fromString(linkTitle).object<jstring>(),
                QAndroidJniObject::fromString(linkUrl).object<jstring>());
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

        if(data)
        {
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
    {"operationComplete", "(Ljava/lang/String;[Ljava/lang/String;)V",(void*)(fromJavaOnOperationCompleted)},
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

