#include "mediator.hpp"

namespace gamma {

Mediator::Mediator(const std::shared_ptr<MainWindow>& mw)
    : m_main_window(mw)
{

}


Mediator::~Mediator() {}

}