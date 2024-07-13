#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "core/log/log.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(std::make_unique<Ui::MainWindow>())
{
    m_ui->setupUi(this);
    
    m_shape_model = new ShapeModel(this);
    m_ui->tableView->setModel(m_shape_model);

    for(size_t i = 0; i < m_shape_model->columnCount(QModelIndex()); ++i)
        m_ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);

    connect(m_ui->tableView, &QTableView::clicked, m_shape_model, &ShapeModel::on_item_pressed);

    connect(m_ui->actionShapes, &QAction::triggered, this, &MainWindow::on_menu_shapes);
    connect(m_ui->actionTable, &QAction::triggered, this, &MainWindow::on_menu_table);
    connect(m_ui->actionBoth, &QAction::triggered, this, &MainWindow::on_menu_both);
}

MainWindow::~MainWindow()
{
    GAMMA_LOG(L_DEBUG, "MainWindow destruction");
}

void MainWindow::on_menu_shapes()
{
    m_ui->tableView->setVisible(false);
    m_ui->graphicsView->setVisible(true);
}

void MainWindow::on_menu_table()
{
    m_ui->tableView->setVisible(true);
    m_ui->graphicsView->setVisible(false);
}

void MainWindow::on_menu_both()
{
    m_ui->tableView->setVisible(true);
    m_ui->graphicsView->setVisible(true);
}