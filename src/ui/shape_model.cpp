#include "shape_model.hpp"

#include "core/log/log.hpp"

#include "declare_metatype.hpp"
#include "shape_model_roles.hpp"

ShapeModel::ShapeModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    setHeaderData(0, Qt::Orientation::Vertical, QString("ID"));
    setHeaderData(1, Qt::Orientation::Vertical, QString("Type"));
    setHeaderData(2, Qt::Orientation::Vertical, QString("Position (X, Y)"));
    setHeaderData(3, Qt::Orientation::Vertical, QString("Region (Width, Height)"));
    setHeaderData(4, Qt::Orientation::Vertical, QString("Show/Hide"));
    setHeaderData(5, Qt::Orientation::Vertical, QString("Delete")); 
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
    if (index.isValid())
    {

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
                item.id = m_counter++;
                item.shape = value.value<std::shared_ptr<gamma::types::IShape>>();

                this->beginResetModel();
                m_items.push_back(item);
                this->endResetModel();
                return true;
            }
        }
    }
}