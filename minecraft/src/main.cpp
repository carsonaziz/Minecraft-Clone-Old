#include <iostream>

#include "engine/application.h"

#include "engine/log.h"

int main()
{
    auto app = Minecraft::Application::create();
    app->run();

    
    return 0;
}