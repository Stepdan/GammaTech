#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "core/log/log.hpp"

#include "ui/shape_model.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(std::make_unique<Ui::MainWindow>())
{
    m_ui->setupUi(this);
    
    m_shape_model = new ShapeModel(this);
    m_ui->tableView->setModel(m_shape_model);

    for(size_t i = 0; i < m_shape_model->columnCount(QModelIndex()); ++i)
        m_ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
}

MainWindow::~MainWindow()
{
    GAMMA_LOG(L_DEBUG, "MainWindow destruction");
}
