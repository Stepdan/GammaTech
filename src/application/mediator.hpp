#pragma once

#include <memory>

#include <QObject>

#include "ui/MainWindow.hpp"

namespace gamma {

class Mediator : public QObject
{
    Q_OBJECT

public:
    Mediator(const std::shared_ptr<MainWindow>& main_window);
    ~Mediator();

private slots:

private:
    std::shared_ptr<MainWindow> m_main_window;
};

}