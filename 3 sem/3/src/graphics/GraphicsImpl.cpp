// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "GraphicsImpl.hpp"
#include "../dynamic/DiceGroup.hpp"

#include "ui_GraphicsImpl.h"

#include <QPushButton>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QKeyEvent>

void GraphicsImpl::loadImages()
{
    QString path = ":/assets/";
    for (const auto& file : QDir(path).entryList())
        m_images.emplace(file.section(".", 0, 0).toStdString(), QPixmap(path + file));

    path += "status/";
    for (const auto& file : QDir(path).entryList())
        m_images.emplace(file.section(".", 0, 0).toStdString(), QPixmap(path + file));
}

void GraphicsImpl::repos()
{
    for (size_t i{}; i < m_items.size(); ++i)
        m_items[i]->setPos(
            GraphicsImpl::GRID_SPACING + (i % 6) * (GraphicsImpl::GRID_SPACING + GraphicsImpl::IMAGE_SIZE),
            GraphicsImpl::GRID_SPACING + (i / 6) * (GraphicsImpl::GRID_SPACING + GraphicsImpl::IMAGE_SIZE));
}

void GraphicsImpl::removeDice()
{
    if (m_dices->getSize() < 2)
    {
        m_ui->removeDiceButton->setEnabled(false);

        return;
    }

    m_dices->popBack();

    m_scene->removeItem(m_items.back());
    m_items.pop_back();

    if (m_dices->getSize() < 18)
        m_ui->addDiceButton->setEnabled(true);
    if (m_dices->getSize() < 2)
        m_ui->removeDiceButton->setEnabled(false);

    repos();
}

void GraphicsImpl::addDice()
{
    *m_dices += uchar_t();

    const auto size = m_dices->getSize();
    m_items.emplace_back(m_scene->addPixmap(m_images.at("base")));

    if (size > 1)
        m_ui->removeDiceButton->setEnabled(true);
    if (size > 17)
        m_ui->addDiceButton->setEnabled(false);

    repos();
}

void GraphicsImpl::update()
{
    const auto size = m_dices->getSize();
    for (size_t i{}; i < size; ++i)
        m_items[i]->setPixmap(m_images.at(std::to_string((*m_dices)[i])));

    showResult();
}

void GraphicsImpl::roll()
{
    m_ui->statusLabel->setPixmap(m_images.at("roll"));

    m_dices->roll();

    update();
}

void GraphicsImpl::showResult()
{
    m_ui->statusLabel->setPixmap(m_images.at( m_dices->areAllTheSame() ? "win" : "lose"));
}

void GraphicsImpl::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
        QMainWindow::close();

    QMainWindow::keyPressEvent(event);
}

GraphicsImpl::GraphicsImpl(QWidget* parent /* = nullptr */) :
    QMainWindow(parent),
    m_ui(std::make_unique<Ui::GraphicsImplClass>()),
    m_scene(std::make_unique<QGraphicsScene>()),
    m_dices(std::make_unique<DiceGroup>())
{
    m_ui->setupUi(this);

    QMainWindow::setStyleSheet("background:transparent;");
    QMainWindow::setAttribute(Qt::WA_TranslucentBackground);
    QMainWindow::setWindowFlags(Qt::FramelessWindowHint);

    auto canvas = m_ui->canvas;
    canvas->setScene(m_scene.get());
    canvas->horizontalScrollBar()->blockSignals(true);
    canvas->verticalScrollBar()->blockSignals(true);

    m_scene->setParent(canvas);

    QObject::connect(m_ui->removeDiceButton, &QPushButton::clicked, [&]() { removeDice(); });
    QObject::connect(m_ui->addDiceButton, &QPushButton::clicked, [&]() { addDice(); });
    QObject::connect(m_ui->rollButton, &QPushButton::clicked, [&]() { roll(); });

    loadImages();

    addDice();
}

GraphicsImpl::~GraphicsImpl() noexcept
{ }