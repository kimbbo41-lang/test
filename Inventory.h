#pragma once

#include "Item.h"

#include <memory>
#include <vector>

class Inventory
{
public:
    bool addItem(std::unique_ptr<Item> item);
    std::unique_ptr<Item> removeItem(size_t index);

    Item* getItem(size_t index);
    size_t size() const;
    bool isFull() const;
    bool isEmpty() const;

    void print() const;

private:
    std::vector<std::unique_ptr<Item>> items;
};
