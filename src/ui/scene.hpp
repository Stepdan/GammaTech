#pragma once

#include "core/types/shape.hpp"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>

#include <memory>

class SceneItem 
{
public:
	SceneItem(int id) : m_id(id) {}

	int get_id() const noexcept { return m_id; }

private:
	int m_id;
};

class SceneEllipseItem : public QGraphicsEllipseItem, public SceneItem
{
public:
	SceneEllipseItem(int id, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr)
		: QGraphicsEllipseItem(x, y, w, h, parent)
		, SceneItem(id)
	{}
};

class SceneLineItem : public QGraphicsLineItem, public SceneItem
{
public:
	SceneLineItem(int id, qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr)
		: QGraphicsLineItem(x1, y1, x2, y2, parent)
		, SceneItem(id)
	{}
};

class SceneRectItem : public QGraphicsRectItem, public SceneItem
{
public:
	SceneRectItem(int id, qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = nullptr)
		: QGraphicsRectItem(x, y, w, h, parent)
		, SceneItem(id)
	{}
};

class ScenePolygonItem : public QGraphicsPolygonItem, public SceneItem
{
public:
	ScenePolygonItem(int id, const QPolygonF &polygon, QGraphicsItem *parent = nullptr)
		: QGraphicsPolygonItem(polygon, parent)
		, SceneItem(id)
	{}
};

class Scene	: public QGraphicsView
{
	Q_OBJECT

public:
	explicit Scene(QWidget *parent = nullptr);
	~Scene();

protected:
	void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent* event) override;

private slots:
	void on_shape_processed(std::shared_ptr<gamma::types::IShape>);

private:
	int get_graphics_item_id(QGraphicsItem* item);

private:
	QGraphicsScene m_scene;
};