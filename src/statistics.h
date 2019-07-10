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

#ifndef STATISTICS_H
#define STATISTICS_H

#include "iotlib_global.h"

#include <QVariant>

namespace iot {

// TODO: Should we extend QObject?
class IOTLIBSHARED_EXPORT Statistics
{

public:

    Statistics();
    virtual ~Statistics();

    QVariant minimum() const;
    void setMinimum(const QVariant& minimum);
    QVariant maximum() const;
    void setMaximum(const QVariant& maximum);
    QVariant average() const;
    void setAverage(const QVariant& average);
    QVariant count() const;
    void setCount(const QVariant& count);
    QVariant sum() const;
    void setSum(const QVariant& sum);

private:

    QVariant m_minimum;
    QVariant m_maximum;
    QVariant m_average;
    QVariant m_count;
    QVariant m_sum;

};
}

#endif // STATISTICS_H
