// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Utility.hpp"

#include <QMessageBox>

void MobileRobots::Utility::showError(const std::string_view msg)
{
	QMessageBox _(QMessageBox::Icon::Critical,
		"ERROR",
		msg.data(),
		QMessageBox::StandardButton::Ok);

	_.exec();
}
