#include <dust/logging/log.hpp>

#include "config.hpp"
#include "util.hpp"

namespace dust::editor
{

Config loadConfig(const std::filesystem::path &path)
try
{
	return nlohmann::json::parse(openStream(path, std::ios_base::binary | std::ios_base::in)).get<Config>();
}
catch (const std::exception &ex)
{
	logging::Log::warning(ex.what());
	logging::Log::warning("Could not load config, use default.");

	return Config {};
}

void saveConfig(const Config &config, const std::filesystem::path &path)
try
{
	const auto dump = nlohmann::json(config).dump(2);

	openStream(path, std::ios_base::binary | std::ios_base::out)
		.write(dump.c_str(), static_cast<std::streamsize>(dump.size()));
}
catch (const std::exception &ex)
{
	logging::Log::warning(ex.what());
	logging::Log::warning("Could not save config file.");
}

} // namespace dust::editor