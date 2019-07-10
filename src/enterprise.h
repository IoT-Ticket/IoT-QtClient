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


#ifndef ENTERPRISE_H
#define ENTERPRISE_H

#include "iotlib_global.h"

#include <QObject>

namespace iot
{

// forward declaration
class EnterprisePrivate;
class Error;

//! \brief The Enterprise class presents an enterprise on Iot-Ticket server.
//!
//! Enterprises can be read by using EnterpriseList class.
class IOTLIBSHARED_EXPORT Enterprise: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString resourceId READ resourceId WRITE setResourceId NOTIFY resourceIdChanged)
    Q_PROPERTY(bool hasSubEnterprises READ hasSubEnterprises WRITE setHasSubEnterprises NOTIFY hasSubEnterprisesChanged)

public:

    //! \brief Constructor.
    //! \param parent Parent qobject, passed to QObject constructor.
    explicit Enterprise(QObject *parent = nullptr);

    //! \brief Destructor.
    virtual ~Enterprise();

    //! \brief Return Enterprise name.
    //! \return Name.
    QString name() const;

    //! \brief Return Enterprise resourceId.
    //! \return Resource id.
    QString resourceId() const;

    //! \brief Return Enterprise href.
    //! \return Href.
    QString href() const;

    //! \brief Does enterprise have sub enterprises.
    //! \return Return true if enterprise has sub enterprises, otherwise return false.
    bool hasSubEnterprises() const;

public slots:

    //! \brief Set enterprise name.
    //! \param name Name
    void setName(const QString& name);

    //! \brief Set enterprise resource id.
    //! \param resourceId Resource id
    void setResourceId(const QString& resourceId);

    //! \brief Set enterprise Href.
    //! \param href Href
    void setHref(const QString& href);

    //! \brief Set value indicating if enterprise has sub enterprises.
    //! \param hasSubEnterprises A bool that indicates if enterprise has sub enterprises.
    void setHasSubEnterprises(const bool& hasSubEnterprises);

signals:
    void nameChanged(const QString &name);
    void resourceIdChanged(const QString &resourceId);
    void hrefChanged(const QString &href);
    void hasSubEnterprisesChanged(const bool &hasSubEnterprises);

protected:
    QScopedPointer<EnterprisePrivate> const d_ptr;

    Enterprise(EnterprisePrivate& dd, QObject* parent = 0);

private:

    friend class EnterpriseListPrivate;

    static Enterprise* fromJson(const QJsonObject& object);

    Q_DECLARE_PRIVATE(Enterprise)
    friend IOTLIBSHARED_EXPORT QDebug operator<<(QDebug, const Enterprise&);

};

} // namespace iot

#endif // ENTERPRISE_H
