#pragma once

#include <algorithm>
#include <cctype>
#include <cmath>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

class FleetError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ArgumentError : public FleetError {
public:
    using FleetError::FleetError;
};

class InspectionRejected : public FleetError {
public:
    InspectionRejected() : FleetError("inspection rejected") {}
};

class InventoryItem {
public:
    virtual ~InventoryItem() = default;
    virtual const std::string& inventoryId() const = 0;
    virtual const std::string& name() const = 0;
    virtual void describe(std::ostream& output) const = 0;
    virtual void inspect() const {}
    virtual double dailyEnergy() const { return 0.0; }
    virtual bool suitableForNovice() const { return false; }
};

inline std::ostream& operator<<(std::ostream& output, const InventoryItem& item) {
    item.describe(output);
    return output;
}

inline bool operator<(const InventoryItem& left, const InventoryItem& right) {
    return left.inventoryId() < right.inventoryId();
}

class EnergyConsumer {
public:
    virtual ~EnergyConsumer() = default;
    virtual double dailyEnergy() const = 0;
};

class NamedItem : public InventoryItem {
public:
    NamedItem(std::string id, std::string name)
        : id_(std::move(id)), name_(std::move(name)) {
        if (id_.empty() || name_.empty()) {
            throw ArgumentError("empty id or name");
        }
    }

    const std::string& inventoryId() const override { return id_; }
    const std::string& name() const override { return name_; }

    void describe(std::ostream& output) const override {
        output << inventoryId() << ' ' << name();
    }

private:
    std::string id_;
    std::string name_;
};

class Transport : public NamedItem {
public:
    Transport(std::string id, std::string name, int score)
        : NamedItem(std::move(id), std::move(name)), score_(score) {
        if (score_ < 1 || score_ > 10) {
            throw ArgumentError("score must be between 1 and 10");
        }
    }

    void inspect() const override {
        std::string lowercaseName = name();
        std::transform(lowercaseName.begin(), lowercaseName.end(), lowercaseName.begin(),
                       [](unsigned char symbol) {
                           return static_cast<char>(std::tolower(symbol));
                       });
        if (lowercaseName.find("broken") != std::string::npos) {
            throw InspectionRejected();
        }
    }

    bool suitableForNovice() const override { return score_ >= 6; }

private:
    int score_;
};

inline double checkedEnergy(double energy) {
    if (!std::isfinite(energy) || energy < 0.0) {
        throw ArgumentError("energy must be finite and nonnegative");
    }
    return energy == 0.0 ? 0.0 : energy;
}

class Scooter final : public Transport, public EnergyConsumer {
public:
    Scooter(std::string id, std::string name, double energy, int score)
        : Transport(std::move(id), std::move(name), score), energy_(checkedEnergy(energy)) {}

    double dailyEnergy() const override { return energy_; }

private:
    double energy_;
};

class Bike : public Transport {
public:
    using Transport::Transport;
};

class ElectricBike final : public Bike, public EnergyConsumer {
public:
    ElectricBike(std::string id, std::string name, double energy, int score)
        : Bike(std::move(id), std::move(name), score), energy_(checkedEnergy(energy)) {}

    double dailyEnergy() const override { return energy_; }

private:
    double energy_;
};

class Helmet final : public NamedItem {
public:
    using NamedItem::NamedItem;
};

class Dock final : public NamedItem, public EnergyConsumer {
public:
    Dock(std::string id, std::string name, double energy)
        : NamedItem(std::move(id), std::move(name)), energy_(checkedEnergy(energy)) {}

    double dailyEnergy() const override { return energy_; }

private:
    double energy_;
};

class Trailer final : public NamedItem {
public:
    using NamedItem::NamedItem;
};
