#include "ui/MainWindow.hpp"

#include "application/application.hpp"
#include "application/mediator.hpp"

#include "core/log/log.hpp"

int main(int argc, char *argv[])
{
    gamma::log::Logger::instance().set_log_level(L_TRACE);

    gamma::Application application(argc, argv);
    GAMMA_LOG(L_DEBUG, "Application has been created");

    auto main_window = std::make_shared<MainWindow>();
    GAMMA_LOG(L_DEBUG, "MainWindow has been created");

    gamma::Mediator mediator(main_window);
    GAMMA_LOG(L_DEBUG, "Mediator has been created");

    gamma_app->installEventFilter(&mediator);
    gamma_app->installEventFilter(main_window.get());
    //gamma_app->SetStyle();

    main_window->show();

    return application.exec();
}