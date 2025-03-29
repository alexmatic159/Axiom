#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

namespace AXIOM::CORE {

	class Logger
	{
	public:
        static void Init();

        template<typename... Args>
        static void Trace(Args&&... args) {
            s_Logger->trace(std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Debug(Args&&... args) {
            s_Logger->debug(std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Info(Args&&... args) {
            s_Logger->info(std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Warn(Args&&... args) {
            s_Logger->warn(std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Error(Args&&... args) {
            s_Logger->error(std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Critical(Args&&... args) {
            s_Logger->critical(std::forward<Args>(args)...);
        }

    private:
        static std::shared_ptr<spdlog::logger> s_Logger;

	};

}

// Macro per uso semplificato
#define AX_TRACE(...)    ::AXIOM::CORE::Logger::Trace(__VA_ARGS__)
#define AX_DEBUG(...)    ::AXIOM::CORE::Logger::Debug(__VA_ARGS__)
#define AX_INFO(...)     ::AXIOM::CORE::Logger::Info(__VA_ARGS__)
#define AX_WARN(...)     ::AXIOM::CORE::Logger::Warn(__VA_ARGS__)
#define AX_ERROR(...)    ::AXIOM::CORE::Logger::Error(__VA_ARGS__)
#define AX_CRITICAL(...) ::AXIOM::CORE::Logger::Critical(__VA_ARGS__)