#ifndef __MAP_LOADER_HPP_INCLUDED__
#define __MAP_LOADER_HPP_INCLUDED__

#include <QString>

#include <memory>

namespace MobileRobots
{
	class EnvironmentDescriptor;

	class MapLoader
	{
	public:
		static std::shared_ptr<EnvironmentDescriptor> load(const QString& path);
	};
} // namespace MobileRobots

#endif /* !__MAP_LOADER_HPP_INCLUDED__ */