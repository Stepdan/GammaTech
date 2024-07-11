#include "objects_connector.hpp"

#include <cassert>

namespace gamma::utils {

static const std::string TYPE_SIGNAL = "TYPE_SIGNAL";
static const std::string TYPE_SLOT = "TYPE_SLOT";

ObjectsConnector::MetaObjectsMap ObjectsConnector::m_metaCollector = ObjectsConnector::MetaObjectsMap();

ObjectsConnector::ObjectsConnector(QObject *parent/* = 0*/) :
    QObject(parent)
{

}

void ObjectsConnector::register_emitter(const QString &ID, QObject *sender, const QString &signal, bool queued/* = false*/)
{
	const std::string signal_str = signal.startsWith('2') ? signal.toStdString() : QString("2"+signal).toStdString();

	for (const auto & record : m_metaCollector[ID][TYPE_SLOT])
	{
		if (sender != record.first)
		{
			const auto result = connect(sender, signal_str.c_str(), record.first, record.second.c_str(), queued ? static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection) : Qt::UniqueConnection);
			assert(result && "bad emitter");
		}
	}

	register_meta_type(ID, TYPE_SIGNAL, std::make_pair(sender, signal_str));
}

void ObjectsConnector::register_receiver(const QString &ID, QObject *receiver, const QString &slot, bool queued/* = false*/)
{
	const std::string slot_str = slot.startsWith('1') ? slot.toStdString() : QString("1"+slot).toStdString();

	for (const auto & record : m_metaCollector[ID][TYPE_SIGNAL])
	{
		if (receiver != record.first)
		{
			const auto result = connect(record.first, record.second.c_str(), receiver, slot_str.c_str(), queued ? static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection) : Qt::UniqueConnection);
			assert(result && "bad receiver");
		}
	}

	register_meta_type(ID, TYPE_SLOT, std::make_pair(receiver, slot_str));
}

void ObjectsConnector::unregister_receiver(const QString &ID, QObject *receiver, const QString &slot)
{
	const std::string slot_str = slot.startsWith('1') ? slot.toStdString() : QString("1"+slot).toStdString();

	for (const auto & record : m_metaCollector[ID][TYPE_SIGNAL])
	{
		if (receiver != record.first)
			disconnect(record.first, record.second.c_str(), receiver, slot_str.c_str());
	}

	m_metaCollector[ID][TYPE_SLOT].remove(std::make_pair(receiver, slot_str));
}


void ObjectsConnector::unregister_emitter(const QString &ID, QObject *sender, const QString &signal)
{
	const std::string signal_str = signal.startsWith('2') ? signal.toStdString() : QString("2"+signal).toStdString();

	for (const auto & record : m_metaCollector[ID][TYPE_SLOT])
	{
		if (sender != record.first)
			disconnect(sender, signal_str.c_str(), record.first, record.second.c_str());
	}
	m_metaCollector[ID][TYPE_SIGNAL].remove(std::make_pair(sender, signal_str));
}

void ObjectsConnector::register_meta_type(const QString &ID, const std::string &metaType, const std::pair<const QObject*, const std::string> &metaPair)
{
	m_metaCollector[ID][metaType].insert(m_metaCollector[ID][metaType].end(), metaPair );

	connect(metaPair.first, &QObject::destroyed, [=]()
	{
		m_metaCollector[ID][metaType].remove(metaPair);
	});
}

}