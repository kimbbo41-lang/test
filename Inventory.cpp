#include "Inventory.h"
#include "GameConfig.h"
#include "UI.h"

#include <iostream>

bool Inventory::addItem(std::unique_ptr<Item> item)
{
    if (isFull()) return false;
    items.push_back(std::move(item));
    return true;
}

std::unique_ptr<Item> Inventory::removeItem(size_t index)
{
    if (index >= items.size()) return nullptr;
    auto item = std::move(items[index]);
    items.erase(items.begin() + index);
    return item;
}

Item* Inventory::getItem(size_t index)
{
    if (index >= items.size()) return nullptr;
    return items[index].get();
}

size_t Inventory::size() const { return items.size(); }
bool Inventory::isFull() const { return items.size() >= static_cast<size_t>(Config::MAX_INVENTORY_SIZE); }
bool Inventory::isEmpty() const { return items.empty(); }

void Inventory::print() const
{
    if (items.empty())
    {
        UI::printLine("(인벤토리가 비어있습니다)", UI::Color::Gray);
        return;
    }

    for (size_t i = 0; i < items.size(); ++i)
    {
        std::cout << "  " << (i + 1) << ". " << items[i]->getDescription() << "\n";
    }
}
