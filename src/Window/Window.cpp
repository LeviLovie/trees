#include "Window.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <SFML/Graphics.hpp>
#include "../logger.hpp"
#include "font.hpp"
using namespace std;

void Window::Update(bool *debug) {
    renderWindow->draw(uibackground);
    renderWindow->draw(uibackgroundborder);
    for (int y = 0; y < worldSizeY; y++) {
        for (int x = 0; x < worldSizeX; x++) {
            sf::RectangleShape rect(sf::Vector2f(tileSize - 2.0f, tileSize - 2.0f));
            rect.setPosition(sf::Vector2f(x * tileSize + 1.0f, y * tileSize + 1.0f));
            if (worldData[x][y] == 0) {rect.setFillColor(sf::Color(18, 18, 18, 255));}
            if (worldData[x][y] == 1) {
                rect.setFillColor(sf::Color(25, 100, 25, 255));
            }
            renderWindow->draw(rect);
        }
    }
    if (*debug) {renderWindow->draw(debug_text);}
}

void Window::StartUpdateLoop() {
    logger::Log(logger::LEVEL_DEBUG, "Starting main loop");

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
        endTime = chrono::high_resolution_clock::now();
        workTime = chrono::duration<double, milli>(endTime - startTime).count();
        sleepTime = 1000.0 / FPS - workTime;
        if (sleepTime > 1) {this_thread::sleep_for(chrono::milliseconds(static_cast<int>(sleepTime)));}
        FPS = 1000.0 / workTime;
        if (FPS > MaxFPS) {FPS = MaxFPS;}
        debug_text.setString(
            "Debug information:\nFPS: " + to_string(FPS) +
            "\nWork time: " + to_string(workTime) +
            "ms\nSleep time: " + to_string(sleepTime) +
            "ms\n"
        );
    }
}

Window::Window() {
    logger::Log(logger::LEVEL_DEBUG, "Constructing Window");

    logger::Log(logger::LEVEL_DEBUG, "Initializing SFML Window");
    renderWindow = new sf::RenderWindow(sf::VideoMode(), "Trees simulation", sf::Style::Titlebar | sf::Style::Close);
    renderWindow->setSize(sf::Vector2u(WindowSizeX, WindowSizeY));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    renderWindow->setPosition(sf::Vector2i(desktop.width/2 - renderWindow->getSize().x/2, desktop.height/2 - renderWindow->getSize().y/2));
    logger::Log(logger::LEVEL_DEBUG, "SFML Window initialized successfully");

    logger::Log(logger::LEVEL_DEBUG, "Initializing GUI");
    font = sf::Font();
    if (!font.loadFromMemory(ubuntu_font, ubuntu_font_len)) {
        logger::Log(logger::LEVEL_ERROR, "Failed to load font");
        exit(1);
    }

    // GUI
    background = sf::RectangleShape(sf::Vector2f(WindowSizeX, WindowSizeY));
    background.setFillColor(sf::Color(8, 8, 8, 255));
    renderWindow->draw(background);

    uibackground = sf::RectangleShape(sf::Vector2f(WindowSizeX, uiSizeY));
    uibackground.setFillColor(sf::Color(18, 18, 18, 255));
    uibackground.setPosition(sf::Vector2f(0.0f, WindowSizeY - uiSizeY));

    uibackgroundborder = sf::RectangleShape(sf::Vector2f(WindowSizeX, 5.0f));
    uibackgroundborder.setFillColor(sf::Color(5, 5, 5, 255));
    uibackgroundborder.setPosition(sf::Vector2f(0.0f, WindowSizeY - uiSizeY));

    int uiZeroY = WindowSizeY - uiSizeY - 5; 

    debug_text = sf::Text();
    debug_text.setFont(font);
    debug_text.setCharacterSize(14);
    debug_text.setFillColor(sf::Color::White);
    debug_text.setPosition(sf::Vector2f(0.0f, 0.0f));
    logger::Log(logger::LEVEL_DEBUG, "GUI initialized successfully");

    // World
    logger::Log(logger::LEVEL_DEBUG, "Initializing world");
    worldData = new int*[worldSizeX];
    for (int x = 0; x < worldSizeX; x++) {
        worldData[x] = new int[worldSizeY];
    }

    for (int y = 0; y < worldSizeY; y++) {
        for (int x = 0; x < worldSizeX; x++) {
            worldData[x][y] = 0;
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);
    for (int y = 0; y < worldSizeY; y++) {
        for (int x = 0; x < worldSizeX; x++) {
            if (dis(gen) > 0.5) {
                worldData[x][y] = 1;
            }
        }
    }
    logger::Log(logger::LEVEL_DEBUG, "World initialized successfully");
}
