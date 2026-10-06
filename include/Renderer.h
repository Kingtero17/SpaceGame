#pragma once
#include <vector>
#include <string>

struct Pixel {
    char c;
    std::string color;
};

class Renderer {
public:
    Renderer(int width = 120, int height = 30);
    ~Renderer() = default;

    void clearBuffer();
    void drawString(int x, int y, const std::string& str, const std::string& color = "\x1b[0m");
    void drawChar(int x, int y, char c, const std::string& color = "\x1b[0m");
    void render();
    void restore();

private:
    int width_;
    int height_;
    std::vector<std::vector<Pixel>> buffer_;
};
