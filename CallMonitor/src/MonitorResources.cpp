#include "MonitorResources.hpp"
#include <pera_software/aidkit/qt/gui/Resources.hpp>
#include <pera_software/aidkit/qt/core/Strings.hpp>
#include <QIcon>

using namespace pera_software::aidkit::qt;

QIcon MonitorResources::icon()
{
	return loadIcon(":/telephone-icon-256x256.png"_qs);
}
