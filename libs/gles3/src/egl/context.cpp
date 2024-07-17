#include <dust/egl/context.hpp>

#include "error.hpp"

namespace dust::egl
{

Context::Context() : m_config {}, m_context(EGL_NO_CONTEXT), m_debugContext(false)
{}

Context::Context(
	std::shared_ptr<Display> display, std::shared_ptr<Surface> surface, EGLConfig config, EGLContext shared,
	bool debugContext)
	: m_display(std::move(display)),
	  m_surface(std::move(surface)),
	  m_config(config),
	  m_context(eglCreateContext(m_display->handle(), m_config, shared, attributes(debugContext).data())),
	  m_debugContext(debugContext)
{
	if (m_context == EGL_NO_CONTEXT)
	{
		throw Error("Could not create EGL context.");
	}
}

Context::Context(Context &&other) noexcept : m_config {}, m_context(EGL_NO_CONTEXT), m_debugContext(false)
{
	swap(other);
}

Context::~Context() noexcept
{
	reset();
}

Context &Context::operator=(Context &&other) noexcept
{
	reset();
	swap(other);

	return *this;
}

Context::operator bool() const
{
	return m_context != EGL_NO_CONTEXT;
}

EGLContext Context::handle() const
{
	return m_context;
}

Context Context::share() const
{
	if (not operator bool())
	{
		throw std::logic_error("Invalid EGL context.");
	}

	return {m_display, m_surface, m_config, m_context, m_debugContext};
}

bool Context::debugContext() const
{
	return m_debugContext;
}

void Context::makeCurrent() const
{
	if (not operator bool())
	{
		throw std::logic_error("Invalid EGL context.");
	}

	if (not eglMakeCurrent(m_display->handle(), m_surface->handle(), m_surface->handle(), m_context))
	{
		throw Error("Could not set EGL context.");
	}
}

void Context::swap(Context &other)
{
	std::swap(m_display, other.m_display);
	std::swap(m_surface, other.m_surface);
	std::swap(m_config, other.m_config);
	std::swap(m_context, other.m_context);
	std::swap(m_debugContext, other.m_debugContext);
}

void Context::reset() noexcept
{
	if (m_context != EGL_NO_CONTEXT)
	{
		eglDestroyContext(m_display->handle(), m_context);

		m_context = EGL_NO_CONTEXT;
		m_config = {};
		m_surface = {};
		m_display = {};
		m_debugContext = false;
	}
}

std::vector<EGLint> Context::attributes(bool debugContext)
{
	auto attributes_ = std::vector<EGLint>(baseAttributes.begin(), baseAttributes.end());

	if (debugContext)
	{
		attributes_.insert(attributes_.end(), {EGL_CONTEXT_OPENGL_DEBUG, EGL_TRUE});
	}

	attributes_.push_back(EGL_NONE);

	return attributes_;
}

} // namespace dust::egl
