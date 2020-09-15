// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "MobileRobots.hpp"
#include "EnvironmentDescriptor.hpp"
#include "MapLoader.hpp"
#include "AI.hpp"
#include "Sensor.hpp"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QScrollBar>
#include <QScreen>

inline constexpr auto CONFIG_NAME{ ":/cfg/map.json" };

namespace MobileRobots
{
    void MobileRobots::initWidgets()
    {
        m_ui->setupUi(this);

        m_scene = std::make_unique<QGraphicsScene>(m_ui->canvas);

        const int SCALED_CANVAS_WIDTH = m_envDescr->getWidth() * m_scaleFactor.x;
        const int SCALED_HEIGHT = m_envDescr->getHeight() * m_scaleFactor.y;

        auto canvas = m_ui->canvas;
        canvas->resize(SCALED_CANVAS_WIDTH, SCALED_HEIGHT);
        canvas->horizontalScrollBar()->blockSignals(true);
        canvas->verticalScrollBar()->blockSignals(true);
        canvas->setScene(m_scene.get());

        m_scene->setSceneRect(0, 0, canvas->width(), canvas->height());

        m_ui->infoWidget->resize(MobileRobots::INFO_WIDTH, SCALED_HEIGHT);
        m_ui->infoWidget->move(SCALED_CANVAS_WIDTH, 0);


        // const auto size = qApp->screens()[0]->size().height();
        const auto width = m_envDescr->getWidth() * MobileRobots::IMAGE_SIZE;
        const auto height = m_envDescr->getHeight() * MobileRobots::IMAGE_SIZE;
        QMainWindow::setWindowIcon(QIcon(":/assets/icon.ico"));      
        QMainWindow::setFixedSize(width + 200, height);
        // QMainWindow::showFullScreen();
        QMainWindow::move(qApp->screens()[0]->size().width() / 4, 0);

        updateInfo({ 0, 0 });
    }

    void MobileRobots::loadImages()
    {
        const QString path = ":/assets/images/scaled/";

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

    void MobileRobots::drawGrid(const int width, const int height)
    {
        for (unsigned x{}; x <= width; x += m_scaleFactor.x)
            m_grid[0].emplace_back(m_scene->addLine(x, 0, x, height, QPen(Qt::black)));

        for (unsigned y{}; y <= height; y += m_scaleFactor.y)
            m_grid[1].emplace_back(m_scene->addLine(0, y, width, y, QPen(Qt::black)));
    }

    void MobileRobots::drawModules()
    {
        static auto createEllipse = [&](const Coord& coord, const unsigned r, Qt::GlobalColor color, Qt::PenStyle penStyle, Qt::BrushStyle brushStyle)
        {
            auto&& item = new QGraphicsEllipseItem(
                coord.x,
                coord.y,
                r * 2. * m_scaleFactor.x,
                r * 2. * m_scaleFactor.y);

            QPen pen(color);
            pen.setStyle(penStyle);
            item->setPen(pen);
            item->setBrush(QBrush(color, brushStyle));

            return item;
        };

        for (const auto& object : m_envDescr->getObjects())
            if (typeid(*object) != typeid(Barrier) && typeid(*object) != typeid(InterestingObject))
            {
                const auto& obj = std::dynamic_pointer_cast<ObservationCenter>(object);
                for (const auto& module : obj->getModules())
                {
                    if (typeid(*module) == typeid(Sensor))
                    {
                        const auto& sensor = std::dynamic_pointer_cast<Sensor>(module);
                        if (sensor->getGraphicsItem())
                            m_scene->removeItem(sensor->getGraphicsItem());

                        const auto& r = sensor->getRadius();

                        auto&& item = createEllipse(obj->getPos(), r, Qt::blue, Qt::PenStyle::DashLine, Qt::BrushStyle::BDiagPattern);                                           
                        if (typeid(*object) == typeid(ObservationCenter) || typeid(*object) == typeid(CommandCenter))
                        {
                            item->setStartAngle(sensor->getDirection() * 90 * 16);
                            item->setSpanAngle(sensor->getAngle() * 16);
                        }

                        sensor->setGraphicsItem(item);
                        m_scene->addItem(item);
                        item->setPos((obj->getX() + .5 - r) * m_scaleFactor.x, (obj->getY() + .5 - r) * m_scaleFactor.y);
                    }
                    else if (typeid(*module) == typeid(ManagerModule))
                    {
                        const auto& manager = std::dynamic_pointer_cast<ManagerModule>(module);
                        if (manager->getGraphicsItem())
                            m_scene->removeItem(manager->getGraphicsItem());

                        const auto& r = manager->getRadius();

                        auto&& item = createEllipse(obj->getPos(), r, Qt::red, Qt::PenStyle::DotLine, Qt::BrushStyle::Dense7Pattern);
                        manager->setGraphicsItem(item);
                        m_scene->addItem(item);
                        item->setPos((obj->getX() + .5 - r) * m_scaleFactor.x, (obj->getY() + .5 - r) * m_scaleFactor.y);
                    }
                }
            }
    }

    void MobileRobots::initMap()
    {
        loadImages();

        m_map = map_t(m_envDescr->getWidth());
        for (uint32_t i{}; static_cast<size_t>(i) < m_map.size(); ++i)
        {
            const auto size = m_envDescr->getHeight();

            m_map[static_cast<size_t>(i)].resize(static_cast<size_t>(size));
            for (uint32_t j{}; j < size; ++j)
            {
                m_map[static_cast<size_t>(i)][static_cast<size_t>(j)] = m_scene->addPixmap(m_images.at("DarkGrass"));
                m_map[static_cast<size_t>(i)][static_cast<size_t>(j)]->setPos(i * static_cast<qreal>(m_scaleFactor.x), j * static_cast<qreal>(m_scaleFactor.y));
            }
        }  

        for (const auto& object : m_envDescr->getObjects())
        {
            std::string name;
            const auto& id = typeid(*object);
            if (id == typeid(Barrier))                name = "DarkBarrier";
            else if (id == typeid(InterestingObject)) name = "DarkInterestingObject";
            else if (id == typeid(RobotScout))        name = "RobotScout";
            else if (id == typeid(RobotCommander))    name = "RobotCommander";
            else if (id == typeid(ObservationCenter)) name = "ObservationCenter";
            else if (id == typeid(CommandCenter))     name = "CommandCenter";

            if (id != typeid(RobotScout) && id != typeid(RobotCommander))
                m_map[object->getX()][object->getY()]->setPixmap(m_images.at(name));
            else
            {
                m_scouts.emplace_back(m_scene->addPixmap(m_images.at(id == typeid(RobotScout) ? "RobotScout" : "RobotCommander")), std::dynamic_pointer_cast<RobotScout>(object));

                auto&& [pixmap, scout] = m_scouts.back();
                pixmap->setPos(static_cast<qreal>(scout->getX()) * m_scaleFactor.x, static_cast<qreal>(scout->getY()) * m_scaleFactor.y);
            }
        }

        auto canvas = m_ui->canvas;
        drawGrid(canvas->width(), canvas->height());

        drawModules();
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
        // TODO: reimpl!!!

        auto canvas = m_ui->canvas;
        canvas->resize(QMainWindow::width() - MobileRobots::INFO_WIDTH, QMainWindow::height());

        m_ui->infoWidget->resize(MobileRobots::INFO_WIDTH, QMainWindow::height());
        m_ui->infoWidget->move(canvas->width(), 0);

        m_scene->setSceneRect(0, 0, canvas->width(), canvas->height());

        m_scaleFactor.x = canvas->width() / m_envDescr->getWidth();
        m_scaleFactor.y = canvas->height() / m_envDescr->getHeight();

        for (uint32_t i{}; i < m_map.size(); ++i)
            for (uint32_t j{}; j < m_map[0].size(); ++j)
            {
                m_map[i][j]->setPixmap(m_map[i][j]->pixmap().scaled(m_scaleFactor.x, m_scaleFactor.y));
                m_map[i][j]->setPos(i * static_cast<qreal>(m_scaleFactor.x), j * static_cast<qreal>(m_scaleFactor.y));
            }

        for (auto& line : m_grid[0])
            m_scene->removeItem(line);
        
        m_grid[0].clear();
        for (unsigned x{}; x <= canvas->width(); x += m_scaleFactor.x)
            m_grid[0].emplace_back(m_scene->addLine(x, 0, x, canvas->height(), QPen(Qt::black)));

        for (auto& line : m_grid[1])
            m_scene->removeItem(line);

        m_grid[1].clear();
        for (unsigned y{}; y <= canvas->height(); y += m_scaleFactor.y)
            m_grid[1].emplace_back(m_scene->addLine(0, y, canvas->width(), y, QPen(Qt::black)));

        for (auto& [pixmap, scout] : m_scouts)
        {
            pixmap->setPixmap(pixmap->pixmap().scaled(m_scaleFactor.x, m_scaleFactor.y));
            pixmap->setPos(static_cast<qreal>(scout->getX()) * m_scaleFactor.x, static_cast<qreal>(scout->getY()) * m_scaleFactor.y);
        }

        drawModules();

        QMainWindow::resizeEvent(event);
    }

    void MobileRobots::mousePressEvent(QMouseEvent* event)
    {
        static QGraphicsRectItem* sCurrentTile = m_scene->addRect({ 0., 0., static_cast<qreal>(m_scaleFactor.x), static_cast<qreal>(m_scaleFactor.y) }, QPen(Qt::cyan));

        if (event->x() <= m_ui->canvas->width() && event->y() <= m_ui->canvas->height())
        {
            auto x{ event->x() / m_scaleFactor.x };
            auto y{ event->y() / m_scaleFactor.y };
            updateInfo({ x,  y});

            sCurrentTile->setX(static_cast<qreal>(x) * m_scaleFactor.x);
            sCurrentTile->setY(static_cast<qreal>(y) * m_scaleFactor.y);
        }
        
        QMainWindow::mousePressEvent(event);
    }

    void MobileRobots::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_Escape)
            QMainWindow::close();

        QMainWindow::keyPressEvent(event);
    }

    MobileRobots::MobileRobots(QWidget* parent /* = nullptr */) :
        QMainWindow(parent),
        m_ui(std::make_unique<Ui::MobileRobotsClass>()),
        m_envDescr(MapLoader::load(QString(CONFIG_NAME))),
        m_ai(std::make_shared<AI>(m_envDescr)),
        m_timer(std::make_unique<QTimer>(this)),
        m_scene(),
        m_map(),
        m_scouts(),
        m_images(),
        m_scaleFactor({ MobileRobots::IMAGE_SIZE, MobileRobots::IMAGE_SIZE })
    {
        initWidgets();
        initMap();

        connect(m_timer.get(), &QTimer::timeout, this, &MobileRobots::update);
        m_timer->start(50);

        ManagerModule::setAI(m_ai->shared_from_this());

        draw();
    }

    void MobileRobots::draw() const
    {
        for (auto&& coord : m_ai->getMapUpdates())
            if (auto&& object = m_envDescr->getObject(coord); !object || typeid(*object) == typeid(RobotScout) || typeid(*object) == typeid(RobotCommander))
                m_map[coord.x][coord.y]->setPixmap(m_images.at("LightGrass").scaled(m_scaleFactor.x, m_scaleFactor.y));
            else if (typeid(*object) == typeid(Barrier))
                m_map[coord.x][coord.y]->setPixmap(m_images.at("LightBarrier").scaled(m_scaleFactor.x, m_scaleFactor.y));
            else if (typeid(*object) == typeid(InterestingObject))
                m_map[coord.x][coord.y]->setPixmap(m_images.at("LightInterestingObject").scaled(m_scaleFactor.x, m_scaleFactor.y));

        for (const auto& [pixmap, scout] : m_scouts)
        {
            pixmap->setPos(static_cast<qreal>(scout->getX()) * m_scaleFactor.x, static_cast<qreal>(scout->getY()) * m_scaleFactor.y);

            scout->redrawModules(m_scaleFactor);
        }
    }

    void MobileRobots::update()
    {
        m_ai->work();

        draw();

        if (m_ai->finished())
            ; // TODO: analyze results
    }
} // namespace MobileRobots