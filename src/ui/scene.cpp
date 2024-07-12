#include "scene.hpp"

#include "core/log/log.hpp"

#include "utils/objects_connector.hpp"

#include "declare_metatype.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QList>
#include <QWheelEvent>

using namespace gamma;
using namespace gamma::utils;

namespace {

constexpr double SCALE_UP_FACTOR = 1.1;
constexpr double SCALE_DOWN_FACTOR = 0.9;

constexpr int INVALID_ITEM_ID = -1;

}

Scene::Scene(QWidget* parent)
	: QGraphicsView(parent)
{
	m_scene.setParent((parent));
	setScene(&m_scene);

	ObjectsConnector::register_receiver(ObjectsConnectorID::SHAPE_PROCESSED, this, SLOT(on_shape_processed(std::shared_ptr<gamma::types::IShape>)));
}

Scene::~Scene()
{
	GAMMA_LOG(L_TRACE, "Scene destruction");
}

void Scene::mouseDoubleClickEvent(QMouseEvent *event)
{
	QGraphicsView::mouseDoubleClickEvent(event);
}

void Scene::mousePressEvent(QMouseEvent *event)
{
	const auto pos = mapToScene(event->pos());
	GAMMA_LOG(L_INFO, "pressed to {}, {}", static_cast<int>(pos.x()), static_cast<int>(pos.y()));
	QGraphicsItem *item = itemAt(static_cast<int>(pos.x()), static_cast<int>(pos.y()));

	auto id = item ? get_graphics_item_id(item) : INVALID_ITEM_ID;
	GAMMA_LOG(L_TRACE, "Pressed item with id {}", id);
	if(id == INVALID_ITEM_ID)
	{
		QGraphicsView::mousePressEvent(event);
	}

	const auto btn = event->button();
	switch(btn)
	{
		case Qt::MouseButton::LeftButton:
		break;

		case Qt::MouseButton::RightButton:
		break;

		case Qt::MouseButton::MiddleButton:
		break;
	}


	QGraphicsView::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QMouseEvent *event)
{
	QGraphicsView::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent(event);
}

void Scene::wheelEvent(QWheelEvent* event)
{
	static double factor;

	factor = (event->angleDelta().y() > 0) ? SCALE_UP_FACTOR : SCALE_DOWN_FACTOR;

	const QPointF viewportPos = underMouse() ? mapFromGlobal(QCursor::pos()) : QPointF();
	const QPointF scenePos = mapToScene(viewportPos.toPoint());

	scale(factor, factor);

	if(!viewportPos.isNull())
	{
		centerOn(scenePos);
		QPointF viewportDelta = viewportPos - QPointF(viewport()->width() / 2., viewport()->height() / 2.);
		QPointF viewportCenter = mapFromScene(scenePos) - viewportDelta;
		centerOn(mapToScene(viewportCenter.toPoint()));
	}

	update();
}

void Scene::on_shape_processed(std::shared_ptr<gamma::types::IShape> shape)
{
	QColor color = QColor(QString::fromStdString(shape->color_str()));
	switch(shape->type())
	{
		case types::IShape::ShapeType::Ellipse:
		{
			auto ellipse = std::dynamic_pointer_cast<gamma::types::Ellipse>(shape);
			SceneEllipseItem* item = new SceneEllipseItem(shape->id(), ellipse->x, ellipse->y, ellipse->r1, ellipse->r2);
			item->setPen(QPen(color));
			item->setBrush(QBrush(color));
			m_scene.addItem(item);
		}
		break;

		case types::IShape::ShapeType::Line:
		{
			auto line = std::dynamic_pointer_cast<gamma::types::Line>(shape);
			SceneLineItem* item = new SceneLineItem(shape->id(), line->x1, line->y1, line->x2, line->y2);
			item->setPen(QPen(color));
			m_scene.addItem(item);
		}
		break;

		case types::IShape::ShapeType::Rect:
		{
			auto rect = std::dynamic_pointer_cast<gamma::types::Rect>(shape);
			SceneRectItem* item = new SceneRectItem(shape->id(), rect->x, rect->y, rect->width, rect->height);
			item->setPen(QPen(color));
			item->setBrush(QBrush(color));
			m_scene.addItem(item);
		}
		break;

		case types::IShape::ShapeType::Triangle:
		{
			auto triangle = std::dynamic_pointer_cast<gamma::types::Triangle>(shape);
			QPolygonF polygon({ {static_cast<qreal>(triangle->x1), static_cast<qreal>(triangle->y1)}, {static_cast<qreal>(triangle->x2), static_cast<qreal>(triangle->y2)}, {static_cast<qreal>(triangle->x3), static_cast<qreal>(triangle->y3)} });
			ScenePolygonItem* item = new ScenePolygonItem(shape->id(), polygon);
			item->setPen(QPen(color));
			item->setBrush(QBrush(color));
			m_scene.addItem(item);
		}
		break;
	}

	update();
}

int Scene::get_graphics_item_id(QGraphicsItem* item)
{
	if(auto scene_item = qgraphicsitem_cast<SceneEllipseItem*>(item))
	{
		return scene_item->get_id();
	}

	if(auto scene_item = qgraphicsitem_cast<SceneRectItem*>(item))
	{
		return scene_item->get_id();
	}

	if(auto scene_item = qgraphicsitem_cast<SceneLineItem*>(item))
	{
		return scene_item->get_id();
	}

	if(auto scene_item = qgraphicsitem_cast<ScenePolygonItem*>(item))
	{
		return scene_item->get_id();
	}

	return INVALID_ITEM_ID;
}