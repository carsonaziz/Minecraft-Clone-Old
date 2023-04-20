#include "engine/application.h"

#include "engine/log.h"
#include "engine/engine.h"

namespace Minecraft
{
    std::unique_ptr<Application> Application::create()
    {
        return std::make_unique<Application>();
    }

    Application::Application()
    {
    }

    Application::~Application() 
    {
    }

    void Application::run()
    {
        Engine::instance().run();
    }
}