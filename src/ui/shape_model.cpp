#include "shape_model.hpp"

#include "core/log/log.hpp"

#include "utils/objects_connector.hpp"

#include "declare_metatype.hpp"
#include "shape_model_roles.hpp"

using namespace gamma::utils;

ShapeModel::ShapeModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    ObjectsConnector::register_emitter(ObjectsConnectorID::ITEM_VISIBILITY, this, SIGNAL(send_item_visibility_changed(int, bool)));
    ObjectsConnector::register_emitter(ObjectsConnectorID::SCENE_ITEM_REMOVING, this, SIGNAL(send_item_removing(int)));

    ObjectsConnector::register_receiver(ObjectsConnectorID::SHAPE_PROCESSED, this, SLOT(on_shape_processed(std::shared_ptr<gamma::types::IShape>)));
    ObjectsConnector::register_receiver(ObjectsConnectorID::SCENE_ITEM_ACTION, this, SLOT(on_scene_item_action(SceneItemAction)));
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

    if(role == static_cast<int>(ShapeModelRoles::ID))
    {
        return m_items[r].get_data_str(0).toInt();
    }

    // if(role == Qt::DecorationRole)
    // {
    //     if(c == 5)
    //     {
    //         QImage image(":/remove_btn.png");
    //         GAMMA_LOG(L_DEBUG, "Image: {}, {}", image.width(), image.height());
    //         image.save("C:/work/temp/image.png");
    //         return QPixmap::fromImage(image);
    //     }
    // }

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
                const auto id = value.value<int>();
                this->beginResetModel();
                m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [&id](const auto& item) { return id == item.shape->id(); }));
                this->endResetModel();
                emit send_item_removing(id);
                return true;
            }

            case ShapeModelRoles::Movement:
            {
                const auto action = value.value<SceneItemAction>();
                if(auto it = std::find_if(m_items.begin(), m_items.end(), [&action](const auto& item) { return action.id == item.shape->id(); }); it != m_items.end())
                {
                    this->beginResetModel();
                    it->shape->move(action.delta_coord.first, action.delta_coord.second);
                    this->endResetModel();
                }
                return true;
            }

            case ShapeModelRoles::Visibility:
            {
                const auto action = value.value<SceneItemAction>();
                if(auto it = std::find_if(m_items.begin(), m_items.end(), [&action](const auto& item) { return action.id == item.shape->id(); }); it != m_items.end())
                {
                    this->beginResetModel();
                    it->is_visible = action.is_visible;
                    this->endResetModel();

                    emit send_item_visibility_changed(action.id, it->is_visible);
                }
                return true;
            }

            case ShapeModelRoles::VisibilityByID:
            {
                const auto id = value.value<int>();
                if(auto it = std::find_if(m_items.begin(), m_items.end(), [&id](const auto& item) { return id == item.shape->id(); }); it != m_items.end())
                {
                    this->beginResetModel();
                    it->is_visible = !it->is_visible;
                    this->endResetModel();

                    emit send_item_visibility_changed(id, it->is_visible);
                }
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
        
        case SceneItemActionType::Visibility:
            setData(QModelIndex(), QVariant::fromValue<SceneItemAction>(action), static_cast<int>(ShapeModelRoles::Visibility));
            return;

        case SceneItemActionType::Movement:
            setData(QModelIndex(), QVariant::fromValue<SceneItemAction>(action), static_cast<int>(ShapeModelRoles::Movement));
            return;
    }
}

void ShapeModel::on_item_pressed(const QModelIndex& index)
{
    if(index.column() == 4) // Visibility
    {
        const auto value = data(index, static_cast<int>(ShapeModelRoles::ID));
        setData(QModelIndex(), value, static_cast<int>(ShapeModelRoles::VisibilityByID));
    }

    if(index.column() == 5) // Removing
    {
        const auto value = data(index, static_cast<int>(ShapeModelRoles::ID));
        setData(QModelIndex(), value, static_cast<int>(ShapeModelRoles::Remove));
    }
}