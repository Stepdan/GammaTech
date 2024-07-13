#include <QFile>
#include <QFontDatabase>
#include <QString>

#include "application.hpp"

const QString APP_STYLE_FILE = ":/style.qss";

namespace gamma {

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv)
{
}

void Application::SetStyle()
{
    QFile styleFile(APP_STYLE_FILE);
    styleFile.open(QIODevice::ReadOnly);
    QString styleSheet = styleFile.readAll();
    setStyleSheet(styleSheet);
}

}