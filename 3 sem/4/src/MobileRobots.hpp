#ifndef __MOBILE_ROBOTS_HPP_INCLUDED__
#define __MOBILE_ROBOTS_HPP_INCLUDED__

#include <QtWidgets/QMainWindow>
#include <QTimer>

#include <vector>

#include "ui_MobileRobots.h"

#include "Coord.hpp"
#include "RobotScout.hpp"

namespace MobileRobots
{
    class EnvironmentDescriptor;
    class AI;

    class MobileRobots : public QMainWindow
    {
        Q_OBJECT

    private:
        using row_t = std::vector<QGraphicsPixmapItem*>;
        using map_t = std::vector<row_t>;
        using scouts_map_t = std::vector<std::pair<QGraphicsPixmapItem*, std::shared_ptr<RobotScout>>>;

    private:
        inline static constexpr auto IMAGE_SIZE{ 64U }; //-V112
        inline static constexpr auto INFO_WIDTH{ 200U }; //-V112

    private:
        void initWidgets();

        void loadImages();

        void drawGrid(const int width, const int height);

        void drawModules();

        void initMap();

        void updateInfo(const Coord& coord);

        void resizeEvent(QResizeEvent* event) override;

        void mousePressEvent(QMouseEvent* event) override;

        void keyPressEvent(QKeyEvent* event) override;

    public:
        MobileRobots(QWidget *parent = nullptr);

        inline ~MobileRobots() noexcept = default;

        void draw() const;

        void update();

    private:
        std::unique_ptr<Ui::MobileRobotsClass> m_ui;
        std::shared_ptr<EnvironmentDescriptor> m_envDescr;
        std::shared_ptr<AI>                    m_ai;
        std::unique_ptr<QTimer>                m_timer;
        std::unique_ptr<QGraphicsScene>        m_scene;
        map_t                                  m_map;
        scouts_map_t                           m_scouts;
        std::map<std::string, QPixmap>         m_images;
        Coord                                  m_scaleFactor;
        std::vector<QGraphicsLineItem*>        m_grid[2];
    };
} // namespace MobileRobots

#endif /* !__MOBILE_ROBOTS_HPP_INCLUDED__ */