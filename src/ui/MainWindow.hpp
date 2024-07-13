#pragma once

#include "ui/shape_model.hpp"

#include <QAbstractTableModel>
#include <QMainWindow>

#include <memory>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    std::unique_ptr<Ui::MainWindow> m_ui;

    ShapeModel* m_shape_model;
};
