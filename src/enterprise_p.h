#ifndef ENTERPRISE_P_H
#define ENTERPRISE_P_H

#include "enterprise.h"
#include "response.h"
#include "error.h"
#include "item_p.h"

#include <QScopedPointer>

namespace iot {

class EnterprisePrivate: public ItemPrivate
{
    Q_OBJECT
public:
    EnterprisePrivate(Enterprise* parent);
    virtual ~EnterprisePrivate();

    QString m_name;
    QString m_resourceId;
    QString m_enterpriseId;
    QString m_href;
    bool m_hasSubEnterprises;

    // bool getSubEnterprises(const QString& enterpriseId=QString());
    bool initialize(const QJsonObject& object);
private:

    Q_DECLARE_PUBLIC(Enterprise)
    Enterprise* const q_ptr;

};

}

#endif // ENTERPRISE_P_H
