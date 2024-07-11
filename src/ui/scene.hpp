#pragma once

#include "core/types/shape.hpp"

#include <QGraphicsView>
#include <QGraphicsScene>

#include <memory>

class Scene	: public QGraphicsView
{
	Q_OBJECT

public:
	explicit Scene(QWidget *parent = nullptr);
	~Scene();

protected:
	void wheelEvent(QWheelEvent* event) override;

private slots:
	void on_shape_processed(std::shared_ptr<gamma::types::IShape>);

private:
	QGraphicsScene m_scene;
};