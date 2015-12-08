/**
 * @file QtVk.cpp
 * @brief
 * @details
 * @date Created on 08.12.2015
 * @copyright 2015 DDwarf LLC, <gil_ildar@mail.ru>
 * @author Gilmanov Ildar <gil_ildar@mail.ru>
 */

#include "QtVk.h"
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

} // namespace Social
} // namespace DDwarf

