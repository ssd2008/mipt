#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "Fleet.hpp"

std::string readToken(std::istream& input, const std::string& field) {
    std::string token;
    if (!(input >> token)) {
        throw ArgumentError("missing " + field);
    }
    return token;
}

template <typename Number>
Number readNumber(std::istream& input, const std::string& field) {
    std::istringstream token(readToken(input, field));
    Number number{};
    if (!(token >> number) || !token.eof()) {
        throw ArgumentError("invalid " + field);
    }
    return number;
}

void requireEnd(std::istream& input) {
    std::string extra;
    if (input >> extra) {
        throw ArgumentError("too many arguments");
    }
}

std::unique_ptr<InventoryItem> readItem(std::istream& input) {
    const std::string type = readToken(input, "item type");
    const std::string id = readToken(input, "id");
    const std::string name = readToken(input, "name");

    if (type == "scooter" || type == "ebike") {
        const double energy = readNumber<double>(input, "energy");
        const int score = readNumber<int>(input, "score");
        if (type == "scooter") {
            return std::make_unique<Scooter>(id, name, energy, score);
        }
        return std::make_unique<ElectricBike>(id, name, energy, score);
    }
    if (type == "bike") {
        return std::make_unique<Bike>(id, name, readNumber<int>(input, "score"));
    }
    if (type == "dock") {
        return std::make_unique<Dock>(id, name, readNumber<double>(input, "energy"));
    }
    if (type == "helmet") {
        return std::make_unique<Helmet>(id, name);
    }
    if (type == "trailer") {
        return std::make_unique<Trailer>(id, name);
    }
    throw ArgumentError("unknown item type");
}

bool execute(std::istream& input, Fleet& fleet) {
    std::string command;
    if (!(input >> command)) {
        return true;
    }

    if (command == "add") {
        auto candidate = readItem(input);
        requireEnd(input);
        fleet.admit(std::move(candidate));
        return true;
    }
    if (command == "focus" || command == "remove") {
        const std::string id = readToken(input, "id");
        requireEnd(input);
        if (command == "focus") {
            fleet.focus(id);
        } else {
            fleet.remove(id);
        }
        return true;
    }

    requireEnd(input);
    if (command == "list" || command == "novice") {
        fleet.list(std::cout, command == "novice");
    } else if (command == "energy") {
        std::cout << std::fixed << std::setprecision(1) << fleet.energy() << '\n';
    } else if (command == "count") {
        std::cout << fleet.count() << '\n';
    } else if (command == "count_novice") {
        std::cout << fleet.countNovice() << '\n';
    } else if (command == "show_focus") {
        fleet.showFocus(std::cout);
    } else if (command == "quit") {
        return false;
    } else {
        throw ArgumentError("unknown command");
    }
    return true;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    Fleet fleet;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream input(line);
        try {
            if (!execute(input, fleet)) {
                break;
            }
        } catch (const std::exception& error) {
            std::cout << "error: " << error.what() << '\n';
        }
    }
}
