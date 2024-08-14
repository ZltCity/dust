#pragma once

#include <filesystem>

#include <nlohmann/json.hpp>

namespace dust::game
{

struct Config
{
	struct GLES3
	{
		bool debugContext = false;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(GLES3, debugContext)
	} gles3;

	struct Window
	{
		int32_t width = 1280, height = 720;
		bool fullscreen = false;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Window, width, height, fullscreen)
	} window;

	struct Camera
	{
		struct FlyMode
		{
			float velocity = 1.f;
			bool enabled = true;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(FlyMode, velocity, enabled)
		} flyMode;

		float sensitivity = 1.f, fov = 95.f;
		bool inverseX = false, inverseY = false;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Camera, flyMode, sensitivity, inverseX, inverseY)
	} camera;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Config, gles3, window, camera)
};

[[nodiscard]] Config loadConfig(const std::filesystem::path &path);
void saveConfig(const Config &config, const std::filesystem::path &path);

} // namespace dust::game