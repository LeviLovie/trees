#include "Window.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <SFML/Graphics.hpp>
#include "../logger.hpp"
#include "font.hpp"
using namespace std;

#define TARGET_FPS 60

void Window::Update(bool *debug) {
    renderWindow->draw(background);
    renderWindow->draw(uibackground);
    renderWindow->draw(uibackgroundborder);
    for (int y = 0; y < worldSizeY; y++) {
        for (int x = 0; x < worldSizeX; x++) {
            sf::RectangleShape rect(sf::Vector2f(10.0f - 2.0f, 10.0f - 2.0f));
            rect.setPosition(sf::Vector2f(x * 10.0f + 1.0f, y * 10.0f + 1.0f));
            rect.setFillColor(sf::Color{
                static_cast<uint8_t>(((worldData[x][y] >> 16) & 0xFF)),
                static_cast<uint8_t>(((worldData[x][y] >> 8) & 0xFF)),
                static_cast<uint8_t>((worldData[x][y] & 0xFF)),
                255
            });
            renderWindow->draw(rect);
        }
    }
    if (*debug) {renderWindow->draw(debug_text);}
}

void Window::StartUpdateLoop() {
    logger::Log(logger::LEVEL_DEBUG, "Starting main loop");

    const double targetFrameTime = 1000.0 / TARGET_FPS;
    int iteration = 0;
    bool debug = false;
    while (renderWindow->isOpen()) {
        chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
        renderWindow->clear();

        // Polling events
        sf::Event event;
        while (renderWindow->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {renderWindow->close();}
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F4) {debug = !debug; logger::Log(logger::LEVEL_DEBUG, "Debug mode: " + to_string(debug));}
            }
        }

        Update(&debug);

        // Sleep and display
        renderWindow->display();
        iteration++;
        chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
        double elapsedTime = chrono::duration<double, milli>(endTime - startTime).count();
        double sleepTime = targetFrameTime - elapsedTime;
        int max_possible_fps = static_cast<int>(1000.0 / elapsedTime);
        if (sleepTime > 0) {this_thread::sleep_for(chrono::milliseconds(static_cast<int>(sleepTime)));}
        debug_text.setString(
            "Debug information:\nTarget FPS: " + to_string(TARGET_FPS) + "\n" +
            "Max update time: " + to_string(targetFrameTime) + "ms\n" +
            "Max possible fps: " + to_string(max_possible_fps) + "\n" +
            "Work time: " + to_string(elapsedTime) + "ms\n" +
            "Sleep time: " + to_string(sleepTime) + "ms\n"
        );
    }
}

Window::Window() : debug_text(font) {
    logger::Log(logger::LEVEL_DEBUG, "Constructing Window");

    logger::Log(logger::LEVEL_DEBUG, "Initializing SFML Window");
    renderWindow = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(WindowSizeX, WindowSizeY)), "Trees simulation", sf::Style::Titlebar | sf::Style::Close);
    logger::Log(logger::LEVEL_DEBUG, "SFML Window initialized successfully");

    logger::Log(logger::LEVEL_DEBUG, "Initializing GUI");
    font = sf::Font();
    if (!font.loadFromMemory(ubuntu_font, ubuntu_font_len)) {
        logger::Log(logger::LEVEL_ERROR, "Failed to load font");
        exit(1);
    }

    // GUI
    background = sf::RectangleShape(sf::Vector2f(WindowSizeX, WindowSizeY));
    background.setFillColor(sf::Color{8, 8, 8, 255});

    uibackground = sf::RectangleShape(sf::Vector2f(WindowSizeX, uiSizeY));
    uibackground.setFillColor(sf::Color{18, 18, 18, 255});
    uibackground.setPosition(sf::Vector2f(0.0f, WindowSizeY - uiSizeY));

    uibackgroundborder = sf::RectangleShape(sf::Vector2f(WindowSizeX, 5.0f));
    uibackgroundborder.setFillColor(sf::Color{5, 5, 5, 255});
    uibackgroundborder.setPosition(sf::Vector2f(0.0f, WindowSizeY - uiSizeY));

    int uiZeroY = WindowSizeY - uiSizeY - 5; 

    debug_text.setCharacterSize(14);
    debug_text.setFillColor(sf::Color::Black);
    debug_text.setPosition(sf::Vector2f(0.0f, 0.0f));
    logger::Log(logger::LEVEL_DEBUG, "GUI initialized successfully");

    // World
    logger::Log(logger::LEVEL_DEBUG, "Initializing world");
    worldData = new uint32_t*[worldSizeX];
    for (int x = 0; x < worldSizeX; x++) {
        worldData[x] = new uint32_t[worldSizeY];
    }

    for (int y = 0; y < worldSizeY; y++) {
        for (int x = 0; x < worldSizeX; x++) {
            worldData[x][y] = 0x000000;
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis_color(0.0, 16777215);
    std::uniform_real_distribution<float> dis_float(0.0, 1.0);
    for (int y = 0; y < worldSizeY; y++) {
        for (int x = 0; x < worldSizeX; x++) {
            uint32_t color = static_cast<uint32_t>(dis_color(gen));
            if (dis_float(gen) > 0.5) {
                worldData[x][y] = color;
            }
        }
    }
    logger::Log(logger::LEVEL_DEBUG, "World initialized successfully");
}
