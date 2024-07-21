#pragma once

namespace dust::gles3
{

template<class T>
struct Binding
{};

template<class T>
concept Bindable = requires(Binding<T> binding, GLuint handle)
{
	// clang-format off
	{ std::is_constructible_v<Binding<T>, T> };
	{ std::is_same_v<decltype(binding.handle), GLuint> };
	{ binding.bind() };
	{ binding.bind(handle) };
	{ binding.current() } -> std::same_as<GLuint>;
	// clang-format on
};

template<Bindable T>
class BindGuard
{
public:
	explicit BindGuard(const T &object) : m_binding(object), m_oldHandle(m_binding.current())
	{
		if (m_binding.handle != m_oldHandle)
		{
			m_binding.bind();
		}
	}
	~BindGuard()
	{
		if (m_binding.handle != m_oldHandle)
		{
			m_binding.bind(m_oldHandle);
		}
	}

private:
	Binding<T> m_binding;
	GLuint m_oldHandle;
};

} // namespace dust::gles3