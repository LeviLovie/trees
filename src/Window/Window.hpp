#pragma once
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
using namespace std;

class Window {
    public:
        Window();
        void StartUpdateLoop();

        static const int MaxFPS = 30;
        int FPS;
    private:
        void Update(bool* debug);

        static const int WindowSizeX = 1280;
        static const int WindowSizeY = 720;
        static const int uiSizeY = 100;

        static const int tileSize = 20;
        static const int worldSizeX = WindowSizeX / tileSize;
        static const int worldSizeY = (WindowSizeY - uiSizeY) / tileSize;
        int** worldData;
        
        sf::RenderWindow* renderWindow;
        chrono::high_resolution_clock::time_point startTime, endTime;
        double workTime, sleepTime;

        bool debug;

        sf::RectangleShape background;
        sf::RectangleShape uibackground;
        sf::RectangleShape uibackgroundborder;

        sf::Font font;
        sf::Text debug_text;
};
