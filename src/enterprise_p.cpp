#include "enterprise_p.h"
#include "requesthandlerprovider.h"
#include "requesthandler.h"

namespace iot {

EnterprisePrivate::EnterprisePrivate(Enterprise *parent): ItemPrivate(), q_ptr(parent)
{

}

EnterprisePrivate::~EnterprisePrivate()
{

}

bool EnterprisePrivate::initialize(const QJsonObject &object)
{
    Q_Q(Enterprise);

    try {
        q->setName( getValue(object, "name", QJsonValue::String).toString() );
        q->setResourceId( getValue(object, "resourceId", QJsonValue::String).toString() );
        q->setHref( getValue(object, "href", QJsonValue::String).toString());
        q->setHasSubEnterprises( getValue(object, "hasSubEnterprises", QJsonValue::Bool).toBool() );
    } catch (JsonError& e) {
        qDebug() << "Could not create Enterprise: " << e.what();
        return false;
    }

    return true;

}

}
