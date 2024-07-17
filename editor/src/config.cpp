#include <dust/logging/log.hpp>

#include "config.hpp"

namespace dust::editor
{

Config loadConfig(const storage::File &file)
try
{
	return nlohmann::json::parse(*file.stream()).get<Config>();
}
catch (const std::exception &ex)
{
	logging::Log::warning(ex.what());
	logging::Log::warning("Could not load config, use default.");

	return Config {};
}

void saveConfig(const Config &config, storage::File &file)
try
{
	const auto dump = nlohmann::json(config).dump(2);

	file.stream(storage::StreamFlags::Truncate)->write(dump.c_str(), static_cast<std::streamsize>(dump.size()));
}
catch (const std::exception &ex)
{
	logging::Log::warning(ex.what());
	logging::Log::warning("Could not save config file.");
}

} // namespace dust::editor