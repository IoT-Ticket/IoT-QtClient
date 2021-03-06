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

#include "statistics.h"

namespace iot {

Statistics::Statistics():
    m_minimum(QVariant()),
    m_maximum(QVariant()),
    m_average(QVariant()),
    m_count(QVariant()),
    m_sum(QVariant())
{

}

Statistics::~Statistics()
{

}

QVariant Statistics::minimum() const
{
    return m_minimum;
}

void Statistics::setMinimum(const QVariant &minimum)
{
    m_minimum = minimum;
}

QVariant Statistics::maximum() const
{
    return m_maximum;
}

void Statistics::setMaximum(const QVariant &maximum)
{
    m_maximum = maximum;
}

QVariant Statistics::average() const
{
    return m_average;
}

void Statistics::setAverage(const QVariant &average)
{
    m_average = average;
}

QVariant Statistics::count() const
{
    return m_count;
}

void Statistics::setCount(const QVariant &count)
{
    m_count = count;
}

QVariant Statistics::sum() const
{
    return m_sum;
}

void Statistics::setSum(const QVariant &sum)
{
    m_sum = sum;
}

}
