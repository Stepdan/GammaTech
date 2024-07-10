#include "ui/MainWindow.hpp"

#include "application/application.hpp"
#include "application/mediator.hpp"

#include "core/log/log.hpp"

int main(int argc, char *argv[])
{
    GAMMA_LOG(L_INFO, "main()");
    gamma::Application application(argc, argv);

    std::shared_ptr<MainWindow> main_window(new MainWindow());
    gamma::Mediator mediator(main_window);

    // stepWoodApp->installEventFilter(&mediator);
    gamma_app->installEventFilter(main_window.get());
    //stepWoodApp->SetStyle();
    GAMMA_LOG(L_INFO, "Show MainWindow");
    main_window->show();

    return application.exec();
}