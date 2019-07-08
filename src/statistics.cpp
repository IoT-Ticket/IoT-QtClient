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
