#include <array>
#include <vector>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/log.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>

#include <dust/game/game_loop.hpp>

#include "android/event_poller.hpp"
#include "android/window.hpp"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "dustandroid", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "dustandroid", __VA_ARGS__))

class Application
{
public:
	explicit Application(android_app *androidApp);
	Application(const Application &) = delete;

	~Application();

	Application &operator=(const Application &) = delete;

	void run();

private:
	void initDisplay();
	void termDisplay();
	void drawFrame();

	static int32_t handleInput(android_app *androidApp, AInputEvent *inputEvent);
	static void handleCMD(android_app *androidApp, int32_t cmd);

	android_app *m_androidApp;

	ASensorManager *m_sensorManager;
	ASensorEventQueue *m_sensorEventQueue;

	EGLDisplay m_EGLDisplay;
	EGLSurface m_EGLSurface;
	EGLContext m_EGLContext;
};

Application::Application(android_app *androidApp) : m_androidApp {androidApp}
{
	m_androidApp->userData = this;
	m_androidApp->onAppCmd = Application::handleCMD;
	m_androidApp->onInputEvent = Application::handleInput;
}

Application::~Application()
{
	termDisplay();

	m_androidApp->userData = nullptr;
	m_androidApp->onAppCmd = nullptr;
	m_androidApp->onInputEvent = nullptr;
}

void Application::run()
{
	while (true)
	{
		int events;
		struct android_poll_source *source;

		while (ALooper_pollOnce(0, nullptr, &events, (void **)&source) >= 0)
		{
			if (source != nullptr)
			{
				source->process(m_androidApp, source);
			}

			if (m_androidApp->destroyRequested != 0)
			{
				return;
			}
		}
	}
}

void Application::initDisplay()
{
	static const auto attribs = std::array {
		EGL_SURFACE_TYPE,
		EGL_WINDOW_BIT,
		EGL_COLOR_BUFFER_TYPE,
		EGL_RGB_BUFFER,
		EGL_BLUE_SIZE,
		8,
		EGL_GREEN_SIZE,
		8,
		EGL_RED_SIZE,
		8,
		EGL_ALPHA_SIZE,
		8,
		EGL_DEPTH_SIZE,
		24,
		EGL_CONFORMANT,
		EGL_OPENGL_ES3_BIT,
		EGL_RENDERABLE_TYPE,
		EGL_OPENGL_ES3_BIT,
		EGL_NONE};

	m_EGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	if (!eglInitialize(m_EGLDisplay, nullptr, nullptr))
	{
		throw std::runtime_error {"Could not initialize EGL display connection."};
	}

	auto numConfigs = EGLint {};

	if (!eglChooseConfig(m_EGLDisplay, attribs.data(), nullptr, 0, &numConfigs))
	{
		throw std::runtime_error {"Could not retrieve number of the supported EGL configurations."};
	}

	auto configs = std::vector<EGLConfig>(numConfigs, {});

	if (!eglChooseConfig(m_EGLDisplay, attribs.data(), configs.data(), numConfigs, &numConfigs))
	{
		throw std::runtime_error {"Could not retrieve supported EGL configurations."};
	}

	m_EGLSurface = eglCreateWindowSurface(m_EGLDisplay, configs.front(), m_androidApp->window, nullptr);

	if (m_EGLSurface == EGL_NO_SURFACE)
	{
		throw std::runtime_error {"Could not create EGL surface."};
	}

	m_EGLContext = eglCreateContext(m_EGLDisplay, configs.front(), nullptr, nullptr);

	if (m_EGLContext == EGL_NO_CONTEXT)
	{
		throw std::runtime_error {"Could not create EGL rendering context."};
	}

	if (!eglMakeCurrent(m_EGLDisplay, m_EGLSurface, m_EGLSurface, m_EGLContext))
	{
		throw std::runtime_error {"Could not set EGL rendering context."};
	}
}

void Application::termDisplay()
{
	if (m_EGLDisplay != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(m_EGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (m_EGLContext != EGL_NO_CONTEXT)
		{
			eglDestroyContext(m_EGLDisplay, m_EGLContext);
		}

		if (m_EGLSurface != EGL_NO_SURFACE)
		{
			eglDestroySurface(m_EGLDisplay, m_EGLSurface);
		}

		eglTerminate(m_EGLDisplay);
	}

	m_EGLDisplay = EGL_NO_DISPLAY;
	m_EGLContext = EGL_NO_CONTEXT;
	m_EGLSurface = EGL_NO_SURFACE;
}

void Application::drawFrame()
{
	if (m_EGLDisplay == EGL_NO_DISPLAY)
	{
		return;
	}

	glClearColor(0.5f, 0.6f, 0.45f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	eglSwapBuffers(m_EGLDisplay, m_EGLSurface);
}

int32_t Application::handleInput(android_app *androidApp, AInputEvent *inputEvent)
{
	auto *app = reinterpret_cast<Application *>(androidApp->userData);

	if (AInputEvent_getType(inputEvent) == AINPUT_EVENT_TYPE_MOTION)
	{
		return 1;
	}

	return 0;
}

void Application::handleCMD(android_app *androidApp, int32_t cmd)
{
	auto *app = reinterpret_cast<Application *>(androidApp->userData);

	switch (cmd)
	{
		case APP_CMD_SAVE_STATE: break;
		case APP_CMD_INIT_WINDOW:
			if (androidApp->window != nullptr)
			{
				app->initDisplay();
				app->drawFrame();
			}
			break;
		case APP_CMD_TERM_WINDOW: app->termDisplay(); break;
		case APP_CMD_GAINED_FOCUS: break;
		case APP_CMD_LOST_FOCUS: break;
		default: break;
	}
}

void android_main(android_app *androidApp)
{
	using namespace dust::android;

	dust::game::GameLoop().start(std::make_shared<EventPoller>(androidApp), std::make_shared<Window>());
}
