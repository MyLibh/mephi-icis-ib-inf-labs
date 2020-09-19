#ifndef __GRAPHICS_IMPL_HPP_INCLUDED__
#define __GRAPHICS_IMPL_HPP_INCLUDED__

#include <QtWidgets/QMainWindow>

namespace Ui
{
    class GraphicsImplClass;
}

class QGraphicsScene;
class QGraphicsPixmapItem;
class DiceGroup;

class GraphicsImpl : public QMainWindow
{
    Q_OBJECT

private:
    inline static constexpr auto IMAGE_SIZE{ 32U };
    inline static constexpr auto GRID_SPACING{ 5U };

private:
    void loadImages();

    void increaseChance();

    void decreaseChance();

    void animate();

    void roll();

    void showResult();

public:
    GraphicsImpl(QWidget* parent = nullptr);

    ~GraphicsImpl() noexcept;

private:
    std::unique_ptr<Ui::GraphicsImplClass> m_ui;
    std::unique_ptr<QGraphicsScene>        m_scene;
    std::unique_ptr<DiceGroup>             m_dices;
    std::vector<QPixmap>                   m_images;
    std::vector<QGraphicsPixmapItem*>      m_items;
};

#endif /* !__GRAPHICS_IMP_HPP_INCLUDED__ */