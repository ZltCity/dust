#include <algorithm>
#include <iostream>
#include <ranges>

#include <cstdlib>

#include <fmt/chrono.h>
#include <fmt/format.h>

#include "log.hpp"

namespace dust::logging
{

[[nodiscard]] std::string toString(LogLevel level);
[[nodiscard]] LogLevel severityFromEnv(LogLevel default_);

Log::Log() : m_context(std::make_shared<Context>()), m_writeThread(writeRoutine, m_context)
{
	m_context->writers["default"] = [](const std::string &message) {
		std::cout << message;
	};
	m_context->severity = severityFromEnv(LogLevel::Info);
}

Log::~Log()
{
	m_context->messages.stop();
}

void Log::push(LogLevel level, const std::string &message)
{
	m_context->messages.push(Message {.level = level, .time = Clock::now(), .message = message});
}

void Log::severity(LogLevel level)
{
	auto lock = std::lock_guard(m_context->lock);

	m_context->severity = level;
}

void Log::writer(const std::string &id, WriterCallback callback)
{
	auto lock = std::lock_guard(m_context->lock);

	m_context->writers[id] = std::move(callback);
}

void Log::debug(const std::string &message)
{
	instance().push(LogLevel::Debug, message);
}

void Log::info(const std::string &message)
{
	instance().push(LogLevel::Info, message);
}

void Log::warning(const std::string &message)
{
	instance().push(LogLevel::Warning, message);
}

void Log::critical(const std::string &message)
{
	instance().push(LogLevel::Critical, message);
}

Log &Log::instance()
{
	static auto log = Log();

	return log;
}

void Log::writeRoutine(std::stop_token stop, std::shared_ptr<Context> context)
{
	while (not stop.stop_requested())
		try
		{
			const auto message = context->messages.pop();
			auto lock = std::lock_guard(context->lock);

			if (message.level < context->severity)
			{
				continue;
			}

			const auto line =
				fmt::format("{:%F %X%z} [{:^10}] {}\n", message.time, toString(message.level), message.message);

			for (auto &[id, writerCallback] : context->writers)
				try
				{
					writerCallback(line);
				}
				catch (...)
				{}
		}
		catch (const threading::QueueStopped &)
		{
			return;
		}
}

std::string toString(LogLevel level)
{
	switch (level)
	{
		case LogLevel::Debug: return "Debug";
		case LogLevel::Info: return "Info";
		case LogLevel::Warning: return "Warning";
		case LogLevel::Critical: return "Critical";
		default: return "Debug";
	}
}

LogLevel severityFromEnv(LogLevel default_)
{
	auto envValue = std::getenv("LOG_LEVEL");

	if (not envValue)
	{
		return default_;
	}

	const auto levelMap = std::map<std::string, LogLevel> {
		{"debug", LogLevel::Debug},
		{"info", LogLevel::Info},
		{"warning", LogLevel::Warning},
		{"critical", LogLevel::Critical},
	};

	try
	{
		auto severityString = std::string(std::strlen(envValue), 0);

		std::ranges::transform(envValue, envValue + severityString.size(), severityString.data(), [](char ch) {
			return static_cast<char>(std::tolower(ch));
		});

		return levelMap.at(severityString);
	}
	catch (...)
	{
		return default_;
	}
}

} // namespace dust::logging
