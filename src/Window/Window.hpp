#pragma once
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
using namespace std;

class Window {
    public:
        Window();
        void StartUpdateLoop();

        int WindowSizeX = 1280;
        int WindowSizeY = 720;
    private:
        void Update(bool* debug);
        
        sf::RenderWindow* renderWindow;
        chrono::high_resolution_clock::time_point startTime, endTime;
        double elapsedTime, sleepTime;
        int fps;
        bool debug;

        sf::RectangleShape background;

        sf::Font font;
        sf::Text debug_text;
};
