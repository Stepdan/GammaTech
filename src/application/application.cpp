#include <QFile>
#include <QFontDatabase>
#include <QString>

#include "application.hpp"

namespace gamma {

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv)
{
}

}