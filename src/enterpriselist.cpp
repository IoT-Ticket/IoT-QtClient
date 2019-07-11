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

#include "enterpriselist.h"
#include "enterpriselist_p.h"

namespace iot {

EnterpriseList::EnterpriseList(QObject *parent) : QObject(parent), d_ptr(new EnterpriseListPrivate(this))
{
}

EnterpriseList::EnterpriseList(EnterpriseListPrivate &dd, QObject *parent) : QObject(parent), d_ptr(&dd)
{
}

EnterpriseList::~EnterpriseList()
{

}

QList<Enterprise *> EnterpriseList::enterprises()
{
    Q_D(EnterpriseList);
    return d->m_enterprises;
}

void EnterpriseList::removeEnterprise(Enterprise *enterprise)
{
    Q_D(EnterpriseList);
    d->m_enterprises.removeOne(enterprise);

}

void EnterpriseList::removeEnterprises(QList<Enterprise *> enterprises)
{
    foreach(auto enterprise, enterprises) {
        removeEnterprise(enterprise);
    }
}

Error *EnterpriseList::readEnterprisesError()
{
    Q_D(EnterpriseList);
    return &d->m_getError;
}

void EnterpriseList::readEnterprises(QString parentEnterpriseId)
{
    Q_D(EnterpriseList);
    d->readEnterprises(parentEnterpriseId);
}


}
