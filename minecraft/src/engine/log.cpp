#include "engine/log.h"

namespace Minecraft
{
    std::shared_ptr<spdlog::logger> Log::s_consolelog;
    void Log::init()
    {
        spdlog::set_pattern("%^[%T][%n][%l]: %v%$");
        s_consolelog = spdlog::stdout_color_mt("CONSOLE");
        s_consolelog->set_level(spdlog::level::debug);
    }
}