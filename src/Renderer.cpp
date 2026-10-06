#include "Renderer.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

Renderer::Renderer(int width, int height) : width_(width), height_(height) {
    buffer_.resize(height_, std::vector<Pixel>(width_));
    clearBuffer();

#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

void Renderer::clearBuffer() {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            buffer_[y][x].c = ' ';
            buffer_[y][x].color = "\x1b[0m"; // COLOR_RESET
        }
    }
}

void Renderer::drawString(int x, int y, const std::string& str, const std::string& color) {
    if (y >= 0 && y < height_) {
        for (size_t i = 0; i < str.length(); ++i) {
            int cx = x + static_cast<int>(i);
            if (cx >= 0 && cx < width_) {
                buffer_[y][cx].c = str[i];
                buffer_[y][cx].color = color;
            }
        }
    }
}

void Renderer::drawChar(int x, int y, char c, const std::string& color) {
    if (y >= 0 && y < height_ && x >= 0 && x < width_) {
        buffer_[y][x].c = c;
        buffer_[y][x].color = color;
    }
}

void Renderer::render() {
    std::string output = "\033[?25l\033[H";
    output.reserve(height_ * (width_ + 15) + 50);

    std::string currentColor = "";

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            const Pixel& p = buffer_[y][x];
            
            if (p.color != currentColor) {
                currentColor = p.color;
                output += currentColor;
            }
            output += p.c;
        }
        if (y < height_ - 1) {
            output += "\n";
        }
    }
    
    std::cout << output << std::flush;
}

void Renderer::restore() {
    std::cout << "\033[?25h\033[0m\033[2J\033[H" << std::flush;
}
