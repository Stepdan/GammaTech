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

	ObjectsConnector::register_emitter(ObjectsConnectorID::SCENE_ITEM_ACTION, this, SIGNAL(send_scene_item_action(SceneItemAction)));
	
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
	QGraphicsItem *item = itemAt(static_cast<int>(event->pos().x()), static_cast<int>(event->pos().y()));

	auto id = item ? get_graphics_item_id(item) : INVALID_ITEM_ID;
	if(id == INVALID_ITEM_ID)
	{
		QGraphicsView::mousePressEvent(event);
		return;
	}

	const auto btn = event->button();
	switch(btn)
	{
		case Qt::MouseButton::LeftButton:
			{
				reset_action();
				m_action.type = SceneItemActionType::Movement;
				const auto mapped_pos = mapToScene(event->pos());
				m_action.initial_pos = { static_cast<int>(mapped_pos.x()), static_cast<int>(mapped_pos.y()) };
				m_action.id = id;
				emit send_scene_item_action(m_action);
			}
			break;

		case Qt::MouseButton::RightButton:
			{
				reset_action();
				m_action.type = SceneItemActionType::Visibility;
				m_action.id = id;
				emit send_scene_item_action(m_action);
			}
			break;

		case Qt::MouseButton::MiddleButton:
			{
				reset_action();
				m_action.type = SceneItemActionType::Removing;
				m_action.id = id;
				emit send_scene_item_action(m_action);
				on_item_removing(id);
			}
			break;
	}


	QGraphicsView::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QMouseEvent *event)
{
	if(m_action.type == SceneItemActionType::Movement)
	{
		const auto mapped_pos = mapToScene(event->pos());
		const std::pair<int, int> cur_pos = { static_cast<int>(mapped_pos.x()), static_cast<int>(mapped_pos.y()) };
		m_action.delta_coord = { cur_pos.first - m_action.initial_pos.first, cur_pos.second - m_action.initial_pos.second };
		emit send_scene_item_action(m_action);
		
		auto* item = get_item(m_action.id);
		if(item)
			get_item(m_action.id)->moveBy(m_action.delta_coord.first, m_action.delta_coord.second);

		m_action.initial_pos = cur_pos;
	}

	QGraphicsView::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QMouseEvent *event)
{
	if(m_action.type == SceneItemActionType::Movement)
	{
		const auto mapped_pos = mapToScene(event->pos());
		const std::pair<int, int> cur_pos = { static_cast<int>(mapped_pos.x()), static_cast<int>(mapped_pos.y()) };
		m_action.delta_coord = { cur_pos.first - m_action.initial_pos.first, cur_pos.second - m_action.initial_pos.second };
		emit send_scene_item_action(m_action);
		
		auto* item = get_item(m_action.id);
		if(item)
			get_item(m_action.id)->moveBy(m_action.delta_coord.first, m_action.delta_coord.second);

		reset_action();
	}

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
	if(auto* scene_item = dynamic_cast<SceneItem*>(item); scene_item)
	{
		return scene_item->get_id();
	}

	return INVALID_ITEM_ID;
}

void Scene::reset_action()
{
	m_action.type = SceneItemActionType::Undefined;
	m_action.initial_pos = {0, 0};
	m_action.delta_coord = {0, 0};
	m_action.id = INVALID_ITEM_ID;
}

void Scene::on_item_removing(int id)
{
	if(auto* item = get_item(id); item)
	{
		m_scene.removeItem(item);
	}
}

QGraphicsItem* Scene::get_item(int id)
{
	for(auto* item : items())
	{
		if(get_graphics_item_id(item) == id)
			return item;
	}

	return nullptr;
}