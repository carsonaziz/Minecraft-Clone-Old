#pragma once

namespace Minecraft
{
    enum XboxOneButtons
    {
        A = 0,
        B = 1,
        X = 3,
        Y = 4,
        LB = 6,
        RB = 7,
        L3 = 13,
        R3 = 14
    };

    class Joystick
    {
    private:
        // make an array of maximum number of joysticks, in callback method, when joystick is connected pick first available slot
        // (if there is a gap in second slot between first and third it means that one was disconnected and now reconnected)
        // ^^ all of that also might be completely wrong
    public:
        static void joystick_callback(int jid, int event);
    };
}