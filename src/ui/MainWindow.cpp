#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "core/log/log.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(std::make_unique<Ui::MainWindow>())
{
    m_ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    GAMMA_LOG(L_DEBUG, "MainWindow destruction");
}
