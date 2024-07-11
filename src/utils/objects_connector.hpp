#pragma once

#include "objects_connector_id.hpp"

#include <map>
#include <list>
#include <string>
#include <utility>

#include <QObject>

namespace gamma::utils {

class ObjectsConnector : public QObject
{
	Q_OBJECT

public:
	explicit ObjectsConnector(QObject *parent = 0);

private:
	ObjectsConnector(const ObjectsConnector &);
	ObjectsConnector& operator=(const ObjectsConnector &);

public:
	Q_INVOKABLE static void register_emitter(const QString &ID, QObject *sender, const QString &signal, bool queued = false);
	Q_INVOKABLE static void register_receiver(const QString &ID, QObject *receiver, const QString &slot, bool queued = false);
	Q_INVOKABLE static void unregister_receiver(const QString &ID, QObject *receiver, const QString &slot);
	Q_INVOKABLE static void unregister_emitter(const QString &ID, QObject *sender, const QString &signal);

private:
	Q_INVOKABLE static void register_meta_type(const QString &ID, const std::string &metaType, const std::pair<const QObject*, const std::string> &metaPair);

private:
	typedef std::map<QString, std::map<std::string, std::list<std::pair<const QObject*, const std::string>>>> MetaObjectsMap;
	static MetaObjectsMap m_metaCollector;
};

}