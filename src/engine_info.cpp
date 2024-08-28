#include "engine_info.hpp"

namespace dust
{

const EngineInfo &engineInfo()
{
	static auto engInfo = EngineInfo {};

	return engInfo;
}

} // namespace dust