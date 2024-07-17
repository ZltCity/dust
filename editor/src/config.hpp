#pragma once

#include <nlohmann/json.hpp>

#include <dust/storage/file.hpp>

namespace dust::editor
{

struct Config
{
	struct GLES3
	{
		bool debugContext = false;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(GLES3, debugContext)
	} gles3;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Config, gles3)
};

[[nodiscard]] Config loadConfig(const storage::File &file);
void saveConfig(const Config &config, storage::File &file);

} // namespace dust::editor
