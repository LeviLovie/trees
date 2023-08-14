#pragma once
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
using namespace std;

class Window {
    public:
        Window();
        void StartUpdateLoop();

        int MaxFPS = 30;
        int FPS;
    private:
        void Update(bool* debug);

        int WindowSizeX = 1280;
        int WindowSizeY = 720;
        int uiSizeY = 100;

        int tileSize = 20;
        int worldSizeX = WindowSizeX / tileSize;
        int worldSizeY = (WindowSizeY - uiSizeY) / tileSize;
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
