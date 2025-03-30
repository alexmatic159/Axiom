#include "Logger.h"

namespace AXIOM {

	std::shared_ptr<spdlog::logger> Logger::s_Logger;

    void Logger::Init() {
        // Crea un logger con console + file
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/axiom.log"));

        s_Logger = std::make_shared<spdlog::logger>("AXIOM", begin(sinks), end(sinks));
        spdlog::register_logger(s_Logger);

        // Configurazione
        s_Logger->set_level(spdlog::level::trace);
        s_Logger->flush_on(spdlog::level::warn);

        // Pattern personalizzato
        s_Logger->set_pattern("[%Y-%m-%d %T.%e] [%^%l%$] [%n] %v");

        spdlog::set_default_logger(s_Logger);
    }
}