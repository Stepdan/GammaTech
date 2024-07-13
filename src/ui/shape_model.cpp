#include "shape_model.hpp"

#include "core/log/log.hpp"

#include "utils/objects_connector.hpp"

#include "declare_metatype.hpp"
#include "shape_model_roles.hpp"

using namespace gamma::utils;

ShapeModel::ShapeModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    ObjectsConnector::register_receiver(ObjectsConnectorID::SHAPE_PROCESSED, this, SLOT(on_shape_processed(std::shared_ptr<gamma::types::IShape>)));
    ObjectsConnector::register_receiver(ObjectsConnectorID::SCENE_ITEM_ACTION, this, SLOT(on_scene_item_action(SceneItemAction)));

    //headerDataChanged(Qt::Horizontal, 0, 5);
}

int ShapeModel::rowCount(const QModelIndex & /*parent*/) const
{
   return static_cast<int>(m_items.size());
}

int ShapeModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 6;
}

QVariant ShapeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    const auto r = index.row();
    const auto c = index.column();

    if (role == Qt::DisplayRole)
    {
        if(m_items.size() < r + 1)
            return QString();

        if(c >= 0 && c <= 3)
            return m_items[r].get_data_str(c);
    }

    return QVariant();
}

bool ShapeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid())
    {
        switch (static_cast<ShapeModelRoles>(role))
        {
            case ShapeModelRoles::Item:
            {
                ShapeModelItem item;
                item.shape = value.value<std::shared_ptr<gamma::types::IShape>>();

                this->beginResetModel();
                m_items.push_back(item);
                this->endResetModel();
                return true;
            }

            case ShapeModelRoles::Remove:
            {
                auto id = value.value<int>();
                this->beginResetModel();
                m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [&id](const auto& item) { return id == item.shape->id(); }));
                this->endResetModel();
                return true;
            }
        }
    }

    return true;
}

void ShapeModel::on_shape_processed(std::shared_ptr<gamma::types::IShape> shape)
{
    setData(QModelIndex(), QVariant::fromValue<std::shared_ptr<gamma::types::IShape>>(shape), static_cast<int>(ShapeModelRoles::Item));
}

QVariant ShapeModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            if(section >= 0 && section < columnCount(QModelIndex()))
            {
                switch(section)
                {
                    case 0: return QString("ID");
                    case 1: return QString("Type");
                    case 2: return QString("Position (X, Y)");
                    case 3: return QString("Region (Width, Height)");
                    case 4: return QString("Show/Hide");
                    case 5: return QString("Delete");
                    default:
                        return QString();
                }
            }
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

void ShapeModel::on_scene_item_action(SceneItemAction action)
{
    if(action.id == -1)
        return;

    switch(action.type)
    {
        case SceneItemActionType::Removing:
            setData(QModelIndex(), action.id, static_cast<int>(ShapeModelRoles::Remove));
            return;
    }
}