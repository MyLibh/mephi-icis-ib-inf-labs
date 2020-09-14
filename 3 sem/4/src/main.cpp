// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "MobileRobots.hpp"
#include "Utility.hpp"

#include <QtWidgets/QApplication>

signed main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    int rc{};
    try
    {
        MobileRobots::MobileRobots w;
        w.show();

        rc = app.exec();
    }
    catch (const std::exception& e)
    {
        MobileRobots::Utility::showError(e.what());

        return -1;
    }
    catch (...)
    {
        MobileRobots::Utility::showError("Unknown exception");

        return -1;
    }

    return rc;
}