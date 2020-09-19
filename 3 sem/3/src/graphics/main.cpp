#include "GraphicsImpl.hpp"

#include <QtWidgets/QApplication>

#include <iostream>

signed main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int rc{};
    try
    {
        GraphicsImpl w;
        w.show();

        rc = a.exec();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "[ERROR]" << ex.what() << std::endl;

        return -1;
    }
    catch (...)
    {
        std::cerr << "[FATAL]" << "Unknown exception" << std::endl;

        return -1;
    }

    return rc;
}