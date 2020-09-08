// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "MobileRobots.hpp"
#include "EnvironmentDescriptor.hpp"
#include "MapLoader.hpp"
#include "AI.hpp"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QScrollBar>

namespace MobileRobots
{
    void MobileRobots::initWidgets()
    {
        m_ui->setupUi(this);

        m_scene = std::make_unique<QGraphicsScene>(m_ui->canvas);

        const int SCALED_CANVAS_WIDTH = m_envDescr->getWidth() * m_scaleFactor;
        const int SCALED_HEIGHT = m_envDescr->getHeight() * m_scaleFactor;

        auto canvas = m_ui->canvas;
        canvas->resize(SCALED_CANVAS_WIDTH, SCALED_HEIGHT);
        canvas->horizontalScrollBar()->blockSignals(true);
        canvas->verticalScrollBar()->blockSignals(true);

        m_ui->infoWidget->resize(MobileRobots::INFO_WIDTH, SCALED_HEIGHT);
        m_ui->infoWidget->move(SCALED_CANVAS_WIDTH, 0);

        QMainWindow::setFixedSize(SCALED_CANVAS_WIDTH + MobileRobots::INFO_WIDTH, SCALED_HEIGHT);
        QMainWindow::setWindowIcon(QIcon(":/assets/icon.ico"));

        updateInfo({ 0, 0 });
    }

    void MobileRobots::initMap()
    {
        loadImages();

        m_map = map_t(m_envDescr->getWidth());
        for (size_t i{}; i < m_map.size(); ++i)
        {
            const auto size = m_envDescr->getHeight();

            m_map[i].resize(size);
            for (size_t j{}; j < size; ++j)
            {
                m_map[i][j] = new QGraphicsPixmapItem(m_images["DarkGrass"]);
                m_map[i][j]->setPos(i * static_cast<qreal>(m_scaleFactor), j * static_cast<qreal>(m_scaleFactor));
            }
        }

        auto canvas = m_ui->canvas;
        const auto WIDTH = canvas->width();
        const auto HEIGHT = canvas->height();

        m_scene->setSceneRect(0, 0, WIDTH, HEIGHT);
        canvas->setScene(m_scene.get());
        for (auto&& row : m_map)
            for (auto&& obj : row)
                m_scene->addItem(obj);

        for (unsigned x{}; x <= WIDTH; x += m_scaleFactor)
            m_scene->addLine(x, 0, x, HEIGHT, QPen(Qt::black));

        for (unsigned y{}; y <= HEIGHT; y += m_scaleFactor)
            m_scene->addLine(0, y, WIDTH, y, QPen(Qt::black));
    }

    void MobileRobots::loadImages()
    {
        const QString path = ":/assets/images/";

        m_images.emplace("DarkGrass", QPixmap(path + "dark_grass.png"));
        m_images.emplace("LightGrass", QPixmap(path + "light_grass.png"));
        m_images.emplace("DarkBarrier", QPixmap(path + "dark_barrier.png"));
        m_images.emplace("LightBarrier", QPixmap(path + "light_barrier.png"));
        m_images.emplace("DarkInterestingObject", QPixmap(path + "dark_interesting_object.png"));
        m_images.emplace("LightInterestingObject", QPixmap(path + "light_interesting_object.png"));
        m_images.emplace("RobotScout", QPixmap(path + "robot_scout.png"));
        m_images.emplace("RobotCommander", QPixmap(path + "robot_commander.png"));
        m_images.emplace("ObservationCenter", QPixmap(path + "observation_center.png"));
        m_images.emplace("CommandCenter", QPixmap(path + "command_center.png"));
    }

    void MobileRobots::updateInfo(const Coord& coord)
    {
        QString text;
        text.append(QString("<b>width:</b> %1<br/><b>height:</b> %2<br/><br/>").arg(m_envDescr->getWidth()).arg(m_envDescr->getHeight()));
    
        if (const auto object = m_envDescr->getObject(coord))
            text.append(object->toString());
        else
            text.append(QString("<b>x:</b> %1<br/><b>y:</b> %2<br/><b>type:</b> Grass<br/>").arg(coord.x).arg(coord.y));

        auto infoWidget = m_ui->infoWidget;
        infoWidget->setPlainText("");
        infoWidget->appendHtml(text);
        infoWidget->moveCursor(QTextCursor::Start);
    }

    void MobileRobots::resizeEvent(QResizeEvent* event)
    {
        m_ui->canvas->resize(QMainWindow::width() - 200, QMainWindow::height());

        QMainWindow::resizeEvent(event);
    }

    void MobileRobots::mousePressEvent(QMouseEvent* event)
    {
        static QGraphicsRectItem* sCurrentTile = m_scene->addRect({ 0., 0., static_cast<qreal>(m_scaleFactor), static_cast<qreal>(m_scaleFactor) }, QPen(Qt::red));

        if (event->x() <= m_ui->canvas->width() && event->y() <= m_ui->canvas->height())
        {
            auto x{ event->x() / m_scaleFactor };
            auto y{ event->y() / m_scaleFactor };
            updateInfo({ x,  y});

            sCurrentTile->setX(static_cast<qreal>(x) * m_scaleFactor);
            sCurrentTile->setY(static_cast<qreal>(y) * m_scaleFactor);
        }
        
        QMainWindow::mousePressEvent(event);
    }

    MobileRobots::MobileRobots(QWidget* parent /* = nullptr */) :
        QMainWindow(parent),
        m_ui(std::make_unique<Ui::MobileRobotsClass>()),
        m_envDescr(MapLoader::load(QString(":/cfg/config.json"))),
        m_ai(std::make_shared<AI>(m_envDescr)),
        m_timer(std::make_unique<QTimer>(this)),
        m_scene(),
        m_map(),
        m_images(),
        m_scaleFactor(MobileRobots::IMAGE_SIZE)
    {
        initWidgets();
        initMap();

        connect(m_timer.get(), &QTimer::timeout, this, &MobileRobots::update);
        m_timer->start(1000);

        ManagerModule::setAI(m_ai->shared_from_this());

        draw();
    }

    void MobileRobots::draw() const
    {
        for (auto&& row : m_map)
            for (auto tile : row)
                tile->setPixmap(m_images.at("DarkGrass"));

        auto objects = m_envDescr->getObjects();
        for (const auto& object : objects)
        {
            std::string name;
            if (const auto& id = typeid(*object); id == typeid(Barrier))
                name = "DarkBarrier";
            else if (id == typeid(InterestingObject))
                name = "DarkInterestingObject";
            else if (id == typeid(RobotScout))
                name = "RobotScout";
            else if (id == typeid(RobotCommander))
                name = "RobotCommander";
            else if (id == typeid(ObservationCenter))
                name = "ObservationCenter";
            else if (id == typeid(CommandCenter))
                name = "CommandCenter";

            if (!name.empty())
                m_map[object->getX()][object->getY()]->setPixmap(m_images.at(name));
        }
            
        // TODO: draw explored
    }

    void MobileRobots::update()
    {
        // TODO: Implement steps

        draw();
    }
} // namespace MobileRobots