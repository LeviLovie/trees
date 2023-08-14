#include "Window.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>
#include "../logger.hpp"
#include "font.hpp"
using namespace std;

#define TARGET_FPS 60

void Window::Update(bool *debug) {
    renderWindow->draw(background);
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

    background = sf::RectangleShape(sf::Vector2f(WindowSizeX, WindowSizeY));
    background.setFillColor(sf::Color::White);

    debug_text.setCharacterSize(14);
    debug_text.setFillColor(sf::Color::Black);
    debug_text.setPosition(sf::Vector2f(0.0f, 0.0f));
    logger::Log(logger::LEVEL_DEBUG, "GUI initialized successfully");
}
