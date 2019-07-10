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

#include "enterpriselist_p.h"
#include "enterprise.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace iot
{

EnterpriseListPrivate::EnterpriseListPrivate(EnterpriseList *parent): ItemPrivate(), q_ptr(parent)
{

}

EnterpriseListPrivate::~EnterpriseListPrivate()
{
    qDeleteAll(m_enterprises);
}

void EnterpriseListPrivate::readEnterprises(QString parentEnterpriseId)
{
    QString path = "/enterprises";
    if (!parentEnterpriseId.isEmpty()) {
        path += "/" + parentEnterpriseId;
    }

    // TODO: Should we be able to get more than 100 enterprises?
    // 100 is the maximum for the limit parameter.
    path += "?limit=100";

    m_getResponse.reset( RequestHandlerProvider::instance()->getRequest(path) );
    QObject::connect( m_getResponse.data(), &Response::finished, this, &EnterpriseListPrivate::onReadEnterprisesFinished);
}

void EnterpriseListPrivate::onReadEnterprisesFinished()
{
    Q_Q(EnterpriseList);
    bool error = true;

    try {
        if (isValidResponse(*m_getResponse, m_getError, 200)) {
            error = false;
            m_getError.reset();

            const QJsonObject rootObject = getObject(m_getResponse->document());

            int fullSize = getValue(rootObject, "fullSize", QJsonValue::Double).toInt();
            bool isMandatory = fullSize != 0;
            const QJsonArray items = getValue(rootObject, "items", QJsonValue::Array, isMandatory).toArray();
            foreach(auto item, items) {
                Enterprise* enterprise = Enterprise::fromJson(getObject(item));
                if (enterprise) {
                    m_enterprises << enterprise;
                } else {
                    error = true;
                    break;
                }
            }
        }
    } catch (std::exception& exception) {
        m_getError.setErrorType(Error::ErrorType::GenericError);
        m_getError.setDescription(exception.what());
        error = true;
    }

    emit q->readEnterprisesFinished(!error);

}

} // namespace iot
