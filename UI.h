#pragma once

#include <string>

namespace UI
{
    enum class Color
    {
        White = 15,
        Yellow = 14,
        Cyan = 11,
        Green = 10,
        Red = 12,
        Magenta = 13,
        Gray = 8
    };

    void setColor(Color color);
    void resetColor();
    void clear();
    void pause();

    void printTitle(const std::string& text);
    void printDivider();
    void printColored(const std::string& text, Color color);
    void printLine(const std::string& text, Color color = Color::White);

    int askChoice(const std::string& prompt, int minVal, int maxVal);
    std::string askString(const std::string& prompt);
    bool askYesNo(const std::string& prompt);

    void printHpMpBar(int current, int max, const std::string& label, Color color);
}
