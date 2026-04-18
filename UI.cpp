#include "UI.h"
#include "Common.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <iostream>
#include <string>

namespace UI
{
    void setColor(Color color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
            static_cast<WORD>(color));
    }

    void resetColor()
    {
        setColor(Color::White);
    }

    void clear()
    {
        system("cls");
    }

    void pause()
    {
        std::cout << "\n계속하려면 Enter 키를 누르세요...";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void printTitle(const std::string& text)
    {
        setColor(Color::Cyan);
        std::cout << "\n========================================\n";
        std::cout << "  " << text << "\n";
        std::cout << "========================================\n";
        resetColor();
    }

    void printDivider()
    {
        setColor(Color::Gray);
        std::cout << "----------------------------------------\n";
        resetColor();
    }

    void printColored(const std::string& text, Color color)
    {
        setColor(color);
        std::cout << text;
        resetColor();
    }

    void printLine(const std::string& text, Color color)
    {
        setColor(color);
        std::cout << text << "\n";
        resetColor();
    }

    int askChoice(const std::string& prompt, int minVal, int maxVal)
    {
        int choice = 0;
        while (true)
        {
            std::cout << prompt << " (" << minVal << "~" << maxVal << "): ";
            std::cin >> choice;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                printLine("잘못된 입력입니다. 숫자를 입력하세요.", Color::Red);
                continue;
            }

            if (choice < minVal || choice > maxVal)
            {
                printLine("범위를 벗어난 입력입니다.", Color::Red);
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }

    std::string askString(const std::string& prompt)
    {
        std::string input;
        std::cout << prompt << ": ";
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return input;
    }

    bool askYesNo(const std::string& prompt)
    {
        while (true)
        {
            std::cout << prompt << " (y/n): ";
            std::string input;
            std::cin >> input;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (input == "y" || input == "Y") return true;
            if (input == "n" || input == "N") return false;
            printLine("y 또는 n 으로 답하세요.", Color::Red);
        }
    }

    void printHpMpBar(int current, int max, const std::string& label, Color color)
    {
        constexpr int BAR_WIDTH = 20;
        if (max <= 0) max = 1;
        int filled = (current * BAR_WIDTH) / max;
        if (filled < 0) filled = 0;
        if (filled > BAR_WIDTH) filled = BAR_WIDTH;

        std::cout << label << " [";
        setColor(color);
        for (int i = 0; i < filled; ++i) std::cout << "#";
        resetColor();
        for (int i = filled; i < BAR_WIDTH; ++i) std::cout << "-";
        std::cout << "] " << current << "/" << max;
    }
}
