#pragma once

#include <memory>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Minecraft
{
    class Log
    {
    private:
        static std::shared_ptr<spdlog::logger> s_consolelog;

    public:
        static void init();

        static std::shared_ptr<spdlog::logger>& get_consolelog() { return s_consolelog; }
    };
}

// Console log macros
#define MC_LOG_TRACE(...) Minecraft::Log::get_consolelog()->trace(__VA_ARGS__)
#define MC_LOG_INFO(...) Minecraft::Log::get_consolelog()->info(__VA_ARGS__)
#define MC_LOG_WARN(...) Minecraft::Log::get_consolelog()->warn(__VA_ARGS__)
#define MC_LOG_ERROR(...) Minecraft::Log::get_consolelog()->error(__VA_ARGS__)
#define MC_LOG_FATAL(...) Minecraft::Log::get_consolelog()->critical(__VA_ARGS__)