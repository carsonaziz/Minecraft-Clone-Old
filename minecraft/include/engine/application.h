#pragma once

#include <memory>
// maybe remove and forward declare window
#include "window.h"

namespace Minecraft
{
    class Application
    {

    public:
        Application();
        ~Application();

        void run();

        static std::unique_ptr<Application> create();
    };
}