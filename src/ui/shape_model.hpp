#pragma once

#include "model_item.hpp"

#include "scene_item_action.hpp"

#include <QAbstractTableModel>

#include <memory>
#include <unordered_map>

class ShapeModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ShapeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    

signals:
    void send_item_visibility_changed(int, bool);

private:
    void reset();

private slots:
    void on_shape_processed(std::shared_ptr<gamma::types::IShape>);
    void on_scene_item_action(SceneItemAction);

private:
    std::vector<ShapeModelItem> m_items;
    int m_counter{0};
};