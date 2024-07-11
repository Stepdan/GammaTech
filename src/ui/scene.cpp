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
	GAMMA_LOG(L_TRACE, "Scene on_shape_processed: {}", shape);
}