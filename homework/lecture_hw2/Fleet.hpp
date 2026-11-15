#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "Inventory.hpp"

template <typename Value>
class ReportTotal {
public:
    ReportTotal& operator+=(Value contribution) {
        value_ += contribution;
        return *this;
    }

    Value value() const { return value_; }

private:
    Value value_{};
};

template <typename Value, typename Container, typename GetContribution>
Value sumReport(const Container& items, GetContribution contribution) {
    ReportTotal<Value> total;
    for (const auto& item : items) {
        total += contribution(*item);
    }
    return total.value();
}

class Fleet {
public:
    void admit(std::unique_ptr<InventoryItem> candidate) {
        candidate->inspect();
        if (find(candidate->inventoryId()) != items_.end()) {
            throw ArgumentError("duplicate id");
        }

        std::shared_ptr<InventoryItem> accepted(std::move(candidate));
        items_.push_back(std::move(accepted));
    }

    std::size_t count() const { return items_.size(); }

    double energy() const {
        return sumReport<double>(items_, [](const InventoryItem& item) {
            return item.dailyEnergy();
        });
    }

    std::size_t countNovice() const {
        return sumReport<std::size_t>(items_, [](const InventoryItem& item) {
            return item.suitableForNovice();
        });
    }

    void list(std::ostream& output, bool onlyNovice) const {
        std::vector<const InventoryItem*> sorted;
        for (const auto& item : items_) {
            if (!onlyNovice || item->suitableForNovice()) {
                sorted.push_back(item.get());
            }
        }

        std::sort(sorted.begin(), sorted.end(),
                  [](const InventoryItem* left, const InventoryItem* right) {
                      return *left < *right;
                  });
        for (const InventoryItem* item : sorted) {
            output << *item << '\n';
        }
    }

    void focus(const std::string& id) {
        focus_ = *findExisting(id);
    }

    void showFocus(std::ostream& output) const {
        if (const auto item = focus_.lock()) {
            output << *item << '\n';
        } else {
            output << "expired\n";
        }
    }

    void remove(const std::string& id) {
        items_.erase(findExisting(id));
    }

private:
    using Items = std::vector<std::shared_ptr<InventoryItem>>;

    Items::const_iterator find(const std::string& id) const {
        return std::find_if(items_.begin(), items_.end(), [&id](const auto& item) {
            return item->inventoryId() == id;
        });
    }

    Items::const_iterator findExisting(const std::string& id) const {
        const auto item = find(id);
        if (item == items_.end()) {
            throw ArgumentError("unknown id");
        }
        return item;
    }

    Items items_;
    std::weak_ptr<InventoryItem> focus_;
};
