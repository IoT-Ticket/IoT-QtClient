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

    // TODO: does there exist better alternatives to QVariant?
    QVariant m_minimum;
    QVariant m_maximum;
    QVariant m_average;
    QVariant m_count;
    QVariant m_sum;

};
}

#endif // STATISTICS_H
