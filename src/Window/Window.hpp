#pragma once
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
using namespace std;

class Window {
    public:
        Window();
        void StartUpdateLoop();
    private:
        void Update(bool* debug);

        int WindowSizeX = 1280;
        int WindowSizeY = 720;
        int uiSizeY = 100;

        int worldSizeX = WindowSizeX / 10;
        int worldSizeY = (WindowSizeY - uiSizeY) / 10;
        uint32_t** worldData;
        
        sf::RenderWindow* renderWindow;
        chrono::high_resolution_clock::time_point startTime, endTime;
        double elapsedTime, sleepTime;
        int fps;
        bool debug;

        sf::RectangleShape background;
        sf::RectangleShape uibackground;
        sf::RectangleShape uibackgroundborder;

        sf::Font font;
        sf::Text debug_text;
};
