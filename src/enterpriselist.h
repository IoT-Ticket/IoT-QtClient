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

#ifndef ENTERPRISELIST_H
#define ENTERPRISELIST_H

#include "iotlib_global.h"

#include <QObject>

namespace iot
{

// forward declarations
class EnterpriseListPrivate;
class Enterprise;
class Error;

//! \brief The EnterpriseList class can be used to fetch a list of Enterprises from the IoT-Ticket
//! server
//!
//! readEnterprises() method starts the async operation to fetch the list of enterprises.
//! readEnterprisesFinished() is emitted once the operaton completes and after that enterprises()
//! method can be used to access the fetched enterprises.
//!
class IOTLIBSHARED_EXPORT EnterpriseList : public QObject
{
    Q_OBJECT
public:

    //! \brief Constructor
    //! \param parent Parent QObject, passed to QObject constructor.
    explicit EnterpriseList(QObject *parent = nullptr);

    //! \brief Destructor.
    virtual ~EnterpriseList();

    //! \brief Return list of enterprises.
    //!
    //! Before enterpriselist is valid, enterprises needs to be fetched from the server using readEnterprises()
    //! method.
    //!
    //! \return list of Enterprises.
    QList<Enterprise*> enterprises();

    //! \brief Removes enterprise from the list. Enterprise is not destructed. Ownership of the object is removed from the enterpriseList.
    //! \param enterprise Removed enterprise.
    void removeEnterprise(Enterprise* enterprise);

    //! \brief Removes list of enterprises from the list. Enterprises are not destructed. Ownership of the object is removed from the enterpriseList.
    //! \param enterprises Removed enterprises.
    void removeEnterprises(QList<Enterprise*> enterprises);

    //! \brief Return details from error.
    //! \return Error details
    Error* readEnterprisesError();

signals:

    //! \brief Signal is emitted when async get opration completes.
    //! \param success True if operation finished successfully.
    void readEnterprisesFinished(bool success);

public slots:

    //! \brief Read enterprises.
    //! This is async operation. Signal readEnterprisesFinished() is emitted when operation completes.
    //! enterprises() function can be used to access the enterprises.
    //! \param parentEnterpriseId EnterpriseId of the enterprise whose sub enterprises are read.
    //! If argument is empty, user's root enterprises are read.
    void readEnterprises(QString parentEnterpriseId = QString());

protected:

    QScopedPointer<EnterpriseListPrivate> const d_ptr;
    EnterpriseList(EnterpriseListPrivate& dd, QObject* parent = nullptr);

private:

    Q_DECLARE_PRIVATE(EnterpriseList)
    friend IOTLIBSHARED_EXPORT QDebug operator<<(QDebug, const EnterpriseList&);

};

} // namespace iot

#endif // ENTERPRISELIST_H
