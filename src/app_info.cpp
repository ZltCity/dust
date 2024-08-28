#include "app_info.hpp"

namespace dust
{

const ApplicationInfo &applicationInfo()
{
	static auto appInfo = ApplicationInfo {};

	return appInfo;
}

} // namespace dust