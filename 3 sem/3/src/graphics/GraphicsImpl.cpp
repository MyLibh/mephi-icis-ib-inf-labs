#include "GraphicsImpl.hpp"
#include "../dynamic/DiceGroup.hpp"

#include "ui_GraphicsImpl.h"

#include <QPushButton>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QGraphicsPixmapItem>
#include <QDir>

void GraphicsImpl::loadImages()
{
    QString path = ":/assets/";
    for (const auto& file : QDir(path).entryList())
        m_images.emplace_back(QPixmap(path + file));
}

void GraphicsImpl::increaseChance()
{
    if (m_dices->getSize() == 1)
    {
        m_ui->increaseChanceButton->setEnabled(false);
    }

    *m_dices -= uchar_t();

    m_scene->removeItem(m_items.back());
    m_items.pop_back();
}

void GraphicsImpl::decreaseChance()
{
    *m_dices += uchar_t();

    const auto size = m_dices->getSize();
    m_items.emplace_back(m_scene->addPixmap(m_images.at(0/*(*m_dices)[size - 1]*/)));

    m_items.back()->setPos(
        GraphicsImpl::GRID_SPACING + (size % 6) * (GraphicsImpl::GRID_SPACING + GraphicsImpl::IMAGE_SIZE),
        GraphicsImpl::GRID_SPACING + (size / 6) * (GraphicsImpl::GRID_SPACING + GraphicsImpl::IMAGE_SIZE));
}

void GraphicsImpl::animate()
{
    
}

void GraphicsImpl::roll()
{
    m_ui->statusLabel->setText("Rolling");

    m_dices->roll();

    animate();

    showResult();
}

void GraphicsImpl::showResult()
{
    m_ui->statusLabel->setText(m_dices->areAllTheSame() ? "WIN" : "LOSE");
}

GraphicsImpl::GraphicsImpl(QWidget* parent /* = nullptr */) :
    QMainWindow(parent),
    m_ui(std::make_unique<Ui::GraphicsImplClass>()),
    m_scene(std::make_unique<QGraphicsScene>()),
    m_dices(std::make_unique<DiceGroup>())
{
    m_ui->setupUi(this);

    auto canvas = m_ui->canvas;

    canvas->setScene(m_scene.get());
    canvas->horizontalScrollBar()->blockSignals(true);
    canvas->verticalScrollBar()->blockSignals(true);

    m_scene->setParent(canvas);

    QObject::connect(m_ui->increaseChanceButton, &QPushButton::clicked, [&]() { increaseChance(); });
    QObject::connect(m_ui->decreaseChanceButton, &QPushButton::clicked, [&]() { decreaseChance(); });
    QObject::connect(m_ui->rollButton, &QPushButton::clicked, [&]() { roll(); });

    loadImages();
}

GraphicsImpl::~GraphicsImpl() noexcept
{ }