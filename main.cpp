#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>
#include "Globals.h"
#include "ComplexNum.h"

sf::Color HSVtoRGB(int h, float s, float v) {
//    s /= 100;
//    v /= 100;
//    h %= 360;
    float c = v * s;
    float x = c * (1 - fabs((h / 60) % 2 - 1));
    float m = v - c;
    float r, g, b;
    if (h >= 0 && h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (h >= 60 && h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (h >= 180 && h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (h >= 240 && h < 300) {
        r = x;
        g = 0;
        b = c;
    } else if (h >= 300 && h < 360) {
        r = c;
        g = 0;
        b = x;
    }
    sf::Color res = sf::Color((r + m) * 255, (g + m) * 255, (b + m) * 255);
//    std::cout << res.r << ' ' << res.g << ' ' << res.b << '\n';
    return res;
}

ComplexNum f(ComplexNum z, ComplexNum c) {
    return (z * z + c);
}

sf::Texture MakeFractal(sf::RenderWindow &window,
                        sf::Vector2f cords_center,
                        float cell_size) {
    auto size = window.getView().getSize();

    sf::Image image;
    image.create(size.x, size.y);
    for (int i = 0; i < size.x; ++i) {
        for (int j = 0; j < size.y; ++j) {
            float x = (i - cords_center.x) / cell_size;
            float y = (cords_center.y - j) / cell_size;
            ComplexNum z = f(ComplexNum(x, y), kC);
            int it = 0;
            bool got_to_inf = true;
            while (z.GetAbs() <= 2) {
                z = f(z, kC);
                ++it;
                if (it >= kMaxIterations) {
                    got_to_inf = false;
                    break;
                }
            }
            if (got_to_inf) {
                sf::Color color;
//                color = sf::Color(16 * (it % 2), 64 * (it % 4) + 128, 32 * (it % 2) + 128);
//                color = HSVtoRGB(it, 0.75, 0.75);
//                color = sf::Color(16 * (it % 2), 64 * (it % 4) + 128, 32 * (it % 2) + 128);
                color = sf::Color(32 * (it) + 16, 64 * (it % 4) + 53, 8 * (it % 8) + 138);
//                color = sf::Color(32 * (it) + 16, 64 * (it % 4) + 53, 8 * (it % 4) + 138);
                image.setPixel(i, j, color);
            } else {
                image.setPixel(i, j, kPointColor);
            }
        }
    }
    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}

int main() {
    sf::RenderWindow window({kResolution.x, kResolution.y}, "Fractal", sf::Style::Close);

    sf::Text tutorial_text;
    sf::Font font;
    font.loadFromFile("Assets/JetBrainsMono-Regular.ttf");
    tutorial_text.setOutlineThickness(1);
    tutorial_text.setOutlineColor(sf::Color::Black);
    tutorial_text.setFont(font);
    tutorial_text.setString("LMB + drag: zoom in area\nRMB: zoom back");

    auto size = window.getView().getSize();
    sf::Vector2f cords_center = {size.x / 2, size.y / 2};
    float cell_size = kStartSize;

    sf::RectangleShape zoom_rect;
    zoom_rect.setFillColor(sf::Color::Transparent);
    zoom_rect.setOutlineColor(kSelectedColor);
    zoom_rect.setOutlineThickness(2);
    bool is_dragging = false;

    sf::Texture fractal_texture = MakeFractal(window, cords_center, cell_size);
    sf::Sprite fractal;
    fractal.setTexture(fractal_texture);

    std::vector<std::pair<sf::Vector2f, float>> states;
    std::vector<sf::Texture> textures;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    is_dragging = true;
                    sf::Vector2f mouse_pos;
                    mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(window);
                    zoom_rect.setPosition(mouse_pos);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    states.push_back(std::make_pair(cords_center, cell_size));
                    textures.push_back(fractal_texture);

                    is_dragging = false;
                    sf::Vector2f new_view_size = zoom_rect.getSize();
                    sf::Vector2f top_left, bottom_right;
                    top_left = {std::min(zoom_rect.getPosition().x, zoom_rect.getPosition().x + zoom_rect.getSize().x),
                                std::min(zoom_rect.getPosition().y, zoom_rect.getPosition().y + zoom_rect.getSize().y)};
                    bottom_right = {
                            std::max(zoom_rect.getPosition().x, zoom_rect.getPosition().x + zoom_rect.getSize().x),
                            std::max(zoom_rect.getPosition().y, zoom_rect.getPosition().y + zoom_rect.getSize().y)};
                    zoom_rect.setPosition(top_left);
                    zoom_rect.setSize(bottom_right - top_left);
                    if (zoom_rect.getSize().x > zoom_rect.getSize().y * (float) kResolution.x / (float) kResolution.y) {
                        new_view_size.y = zoom_rect.getSize().x * (float) kResolution.y / (float) kResolution.x;
                    } else {
                        new_view_size.x = zoom_rect.getSize().y * (float) kResolution.x / (float) kResolution.y;
                    }
                    float relation = kResolution.x / (new_view_size.x);

                    if (relation * cell_size > 0 && relation * cell_size < kInf) {
                        sf::Vector2f zoom_center = {zoom_rect.getPosition().x + zoom_rect.getSize().x / 2,
                                                    zoom_rect.getPosition().y + zoom_rect.getSize().y / 2};

                        float x = (zoom_center.x - cords_center.x) / cell_size;
                        float y = (cords_center.y - zoom_center.y) / cell_size;

                        cell_size = relation * cell_size;

                        cords_center = {size.x / 2 - x * cell_size, size.y / 2 + y * cell_size};

                        fractal_texture = MakeFractal(window, cords_center, cell_size);
                        fractal.setTexture(fractal_texture);
                    } else {
                        states.pop_back();
                        textures.pop_back();
                    }
                } else {
                    if (!states.empty()) {
                        cell_size = states[states.size() - 1].second;
                        cords_center = states[states.size() - 1].first;
                        states.pop_back();
                        fractal_texture = textures[textures.size() - 1];
                        textures.pop_back();
                        fractal.setTexture(fractal_texture);
                    }
                }
            }
        }

        window.clear(kBackgroundColor);

        window.draw(fractal);
        if (is_dragging) {

            sf::Vector2f mouse_pos;
            mouse_pos = (sf::Vector2f) sf::Mouse::getPosition(window);
            sf::Vector2f new_size = zoom_rect.getSize(), delta = mouse_pos - zoom_rect.getPosition();
            if (mouse_pos.x > 0 && mouse_pos.x < size.x)
                new_size.x = delta.x;
            if (mouse_pos.y > 0 && mouse_pos.y < size.y)
                new_size.y = delta.y;
            zoom_rect.setSize(new_size);

            sf::Vector2f top_left, bottom_right;
            top_left = {std::min(zoom_rect.getPosition().x, zoom_rect.getPosition().x + zoom_rect.getSize().x),
                        std::min(zoom_rect.getPosition().y, zoom_rect.getPosition().y + zoom_rect.getSize().y)};
            bottom_right = {
                    std::max(zoom_rect.getPosition().x, zoom_rect.getPosition().x + zoom_rect.getSize().x),
                    std::max(zoom_rect.getPosition().y, zoom_rect.getPosition().y + zoom_rect.getSize().y)};
            sf::RectangleShape new_zoom_rect;
            new_zoom_rect.setPosition(top_left);
            new_zoom_rect.setSize(bottom_right - top_left);
            sf::Vector2f new_view_size = new_zoom_rect.getSize();
//            std::cout << top_left.x << ' ' << top_left.y << '\n' << bottom_right.x << ' ' << bottom_right.y << '\n';
//                    std::cout << zoom_rect.getSize().x << ' ' << zoom_rect.getSize().y << '\n';
            if (new_zoom_rect.getSize().x > new_zoom_rect.getSize().y * (float) kResolution.x / (float) kResolution.y) {
                new_view_size.y = new_zoom_rect.getSize().x * (float) kResolution.y / (float) kResolution.x;
            } else {
                new_view_size.x = new_zoom_rect.getSize().y * (float) kResolution.x / (float) kResolution.y;
            }
//                    std::cout << new_view_size.x << ' ' << new_view_size.y << "\n";
            float relation = kResolution.x / (new_view_size.x);

            if (!(relation * cell_size > 0 && relation * cell_size < kInf)) {
//                std::cout << relation << ' ' << cell_size << ' ' << relation * cell_size << "\n\n";
                zoom_rect.setOutlineColor(kErrorColor);
            } else {
                zoom_rect.setOutlineColor(kSelectedColor);
            }
            window.draw(zoom_rect);
        }

        window.draw(tutorial_text);

        window.display();
    }

    return 0;
}
