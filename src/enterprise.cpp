/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Wapice Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */


#include "enterprise.h"

#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include "enterprise_p.h"

namespace iot
{

Enterprise::Enterprise(QObject *parent): QObject(parent), d_ptr(new EnterprisePrivate(this))
{

}

Enterprise::~Enterprise()
{

}

Enterprise::Enterprise(EnterprisePrivate &dd, QObject *parent): QObject(parent), d_ptr(&dd)
{

}

Enterprise *Enterprise::fromJson(const QJsonObject &object)
{
    Enterprise* enterprise = new Enterprise();
    if (!enterprise->d_ptr->initialize(object)) {
        delete enterprise;
        enterprise = nullptr;
    }
    return enterprise;
}

QString Enterprise::name() const
{
    Q_D(const Enterprise);
    return d->m_name;
}

QString Enterprise::resourceId() const
{
    Q_D(const Enterprise);
    return d->m_resourceId;
}

QString Enterprise::href() const
{
    Q_D(const Enterprise);
    return d->m_href;
}

bool Enterprise::hasSubEnterprises() const
{
    Q_D(const Enterprise);
    return d->m_hasSubEnterprises;
}

void Enterprise::setName(const QString &name)
{
    Q_D(Enterprise);
    if (d->m_name != name) {
        d->m_name = name;
        emit nameChanged(d->m_name);
    }
}
void Enterprise::setResourceId(const QString &resourceId)
{
    Q_D(Enterprise);
    if (d->m_resourceId != resourceId) {
        d->m_resourceId = resourceId;
        emit resourceIdChanged(d->m_resourceId);
    }
}

void Enterprise::setHref(const QString &href)
{
    Q_D(Enterprise);
    if (d->m_href != href) {
        d->m_href = href;
        emit hrefChanged(d->m_href);
    }
}

void Enterprise::setHasSubEnterprises(const bool &hasSubEnterprises)
{
    Q_D(Enterprise);
    if (d->m_hasSubEnterprises != hasSubEnterprises) {
        d->m_hasSubEnterprises = hasSubEnterprises;
        emit hasSubEnterprisesChanged(d->m_hasSubEnterprises);
    }
}

} // namespace iot
