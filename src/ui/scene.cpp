#include "scene.hpp"

#include "core/log/log.hpp"

#include "utils/objects_connector.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QList>
#include <QWheelEvent>

using namespace gamma;
using namespace gamma::utils;

namespace {

constexpr double SCALE_UP_FACTOR = 1.1;
constexpr double SCALE_DOWN_FACTOR = 0.9;

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
			m_scene.addEllipse(ellipse->x, ellipse->y, ellipse->r1, ellipse->r2, QPen(color), QBrush(color));
		}
		break;

		case types::IShape::ShapeType::Line:
		{
			auto line = std::dynamic_pointer_cast<gamma::types::Line>(shape);
			m_scene.addLine(line->x1, line->y1, line->x2, line->y2, QPen(color));
		}
		break;

		case types::IShape::ShapeType::Rect:
		{
			auto rect = std::dynamic_pointer_cast<gamma::types::Rect>(shape);
			m_scene.addRect(rect->x, rect->y, rect->width, rect->height, QPen(color), QBrush(color));
		}
		break;

		case types::IShape::ShapeType::Triangle:
		{
			auto triangle = std::dynamic_pointer_cast<gamma::types::Triangle>(shape);
			QPolygonF polygon({ {static_cast<qreal>(triangle->x1), static_cast<qreal>(triangle->y1)}, {static_cast<qreal>(triangle->x2), static_cast<qreal>(triangle->y2)}, {static_cast<qreal>(triangle->x3), static_cast<qreal>(triangle->y3)} });
			m_scene.addPolygon(polygon, QPen(color), QBrush(color));
		}
		break;
	}

	update();
}