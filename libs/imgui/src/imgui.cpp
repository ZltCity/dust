#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl2.h>

#include <dust/imgui/imgui.hpp>

namespace dust::imgui
{

Lib::Lib() : m_context {}
{
	IMGUI_CHECKVERSION();

	m_context = ImGui::CreateContext();

	if (not m_context)
	{
		throw std::runtime_error("Could not create Dear ImGui context.");
	}
}

Lib::~Lib() noexcept
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::DestroyContext(m_context);
}

void Lib::initContext(std::shared_ptr<sdl::Window> window)
{

	if (not ImGui_ImplSDL2_InitForOpenGL(window->handle(), nullptr))
	{
		throw std::runtime_error("Could not init SDL backend for Dear ImGui lib.");
	}

	if (not ImGui_ImplOpenGL3_Init(GLSLVersion))
	{
		throw std::runtime_error("Could not init GLES3 backend for Dear ImGui lib.");
	}

	m_window = std::move(window);
}

void Lib::config(ImGuiConfigFlags flags)
{
	auto &io = ImGui::GetIO();

	io.ConfigFlags = flags;
}

ImFont *Lib::loadFont(const std::filesystem::path &path, float sizePixels)
{
	auto &io = ImGui::GetIO();

	return io.Fonts->AddFontFromFileTTF(path.generic_string().c_str(), sizePixels);
}

void Lib::beginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Lib::endFrame()
{
	ImGui::Render();
}

void Lib::present()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Lib::processEvent(const SDL_Event &event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

float Lib::framerate() const
{
	auto &io = ImGui::GetIO();

	return io.Framerate;
}

Lib &Lib::instance()
{
	static auto lib = Lib();

	return lib;
}

} // namespace dust::imgui