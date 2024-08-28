#include "config.hpp"
#include "../logging/log.hpp"
#include "../util/stream.hpp"

namespace dust::game
{

using Log = logging::Log;

Config loadConfig(const std::filesystem::path &path)
try
{
	return nlohmann::json::parse(util::openStream(path, std::ios_base::binary | std::ios_base::in)).get<Config>();
}
catch (const std::exception &ex)
{
	Log::warning(ex.what());
	Log::info("Could not load config, use default.");

	return Config {};
}

void saveConfig(const Config &config, const std::filesystem::path &path)
try
{
	const auto dump = nlohmann::json(config).dump(2);

	util::openStream(path, std::ios_base::binary | std::ios_base::out)
		.write(dump.c_str(), static_cast<std::streamsize>(dump.size()));
}
catch (const std::exception &ex)
{
	logging::Log::warning(ex.what());
	logging::Log::info("Could not save config file.");
}

} // namespace dust::game