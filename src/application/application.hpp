#pragma once

#include <QApplication>

#include <memory>

class QMainWindow;

namespace gamma {

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);

private:
};

#define gamma_app (static_cast<gamma::Application*>(QApplication::instance()))

}  // namespace step::gui