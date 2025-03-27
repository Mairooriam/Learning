
// file: AdvancedEventSystem.cpp
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include <memory>
#include <set>  // Add this include for std::set
// Simple event system with variadic templates
template<typename... Args>
class Event {
public:
    // Define the callback type
    using Callback = std::function<void(Args...)>;
    using CallbackId = unsigned int;
    
    // Add a listener and return its ID
    CallbackId addListener(Callback callback) {
        CallbackId id = nextId++;
        callbacks[id] = callback;
        return id;
    }
    
    // Remove a listener by ID
    bool removeListener(CallbackId id) {
        return callbacks.erase(id) > 0;
    }
    
    // Invoke the event with arguments
    void invoke(Args... args) {
        for (auto& [id, callback] : callbacks) {
            callback(args...);
        }
    }
    
private:
    std::map<CallbackId, Callback> callbacks;
    CallbackId nextId = 0;
};

// Game entity class
class Entity {
public:
    Entity(const std::string& name, int health) : name(name), health(health) {}
    
    std::string getName() const { return name; }
    int getHealth() const { return health; }
    
    void takeDamage(int amount) {
        health = std::max(0, health - amount);
        std::cout << name << " takes " << amount << " damage! Health: " << health << std::endl;
        
        if (health <= 0) {
            std::cout << name << " has been defeated!" << std::endl;
            onDeath.invoke(this);
        }
    }
    
    void heal(int amount) {
        int oldHealth = health;
        health += amount;
        std::cout << name << " heals for " << amount << "! Health: " << health << std::endl;
        
        if (oldHealth <= 0 && health > 0) {
            std::cout << name << " has been revived!" << std::endl;
            onRevive.invoke(this);
        }
    }
    
    // Events
    Event<Entity*> onDeath;
    Event<Entity*> onRevive;
    
private:
    std::string name;
    int health;
};

// Game system that manages multiple entities
class BattleSystem {
public:
    void addEntity(std::shared_ptr<Entity> entity) {
        entities.push_back(entity);
        
        // Subscribe to entity's death event
        entity->onDeath.addListener([this](Entity* e) {
            this->handleEntityDeath(e);
        });
        
        // Subscribe to entity's revive event
        entity->onRevive.addListener([this](Entity* e) {
            this->handleEntityRevive(e);
        });
        
        // Notify others that entity joined
        onEntityJoined.invoke(entity.get());
    }
    
    void simulateAttack() {
        if (entities.empty()) return;
        
        // Random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Select random attacker and target
        std::uniform_int_distribution<> attackerDist(0, entities.size() - 1);
        int attackerIndex = attackerDist(gen);
        
        std::uniform_int_distribution<> targetDist(0, entities.size() - 1);
        int targetIndex = targetDist(gen);
        
        // Make sure attacker and target are different
        while (entities.size() > 1 && targetIndex == attackerIndex) {
            targetIndex = targetDist(gen);
        }
        
        // Generate random damage
        std::uniform_int_distribution<> damageDist(5, 25);
        int damage = damageDist(gen);
        
        auto attacker = entities[attackerIndex];
        auto target = entities[targetIndex];
        
        std::cout << "\n" << attacker->getName() << " attacks " << target->getName() << "!" << std::endl;
        
        // Trigger attack event before damage is dealt
        onEntityAttack.invoke(attacker.get(), target.get(), damage);
        
        // Apply damage
        target->takeDamage(damage);
    }
    
    void simulateHeal() {
        if (entities.empty()) return;
        
        // Random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Select random healer and target
        std::uniform_int_distribution<> healerDist(0, entities.size() - 1);
        int healerIndex = healerDist(gen);
        
        std::uniform_int_distribution<> targetDist(0, entities.size() - 1);
        int targetIndex = targetDist(gen);
        
        // Generate random heal amount
        std::uniform_int_distribution<> healDist(10, 30);
        int heal = healDist(gen);
        
        auto healer = entities[healerIndex];
        auto target = entities[targetIndex];
        
        std::cout << "\n" << healer->getName() << " heals " << target->getName() << "!" << std::endl;
        
        // Apply heal
        target->heal(heal);
    }
    
    void handleEntityDeath(Entity* entity) {
        std::cout << "Battle system: " << entity->getName() << " has fallen in battle!" << std::endl;
        
        // 50% chance to attempt revival after 2 seconds
        std::thread([this, entity]() {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, 1);
            
            if (dist(gen) == 1) {
                std::cout << "\nAttempting to revive " << entity->getName() << "..." << std::endl;
                entity->heal(15);
            }
        }).detach();
    }
    
    void handleEntityRevive(Entity* entity) {
        std::cout << "Battle system: " << entity->getName() << " has returned to battle!" << std::endl;
    }
    
    // Events
    Event<Entity*> onEntityJoined;
    Event<Entity*, Entity*, int> onEntityAttack;
    
private:
    std::vector<std::shared_ptr<Entity>> entities;
};

// Audio system that reacts to game events
class AudioSystem {
public:
    AudioSystem(BattleSystem& battleSystem) {
        // Subscribe to battle events
        battleSystem.onEntityJoined.addListener([this](Entity* entity) {
            this->playSound("join", entity->getName());
        });
        
        battleSystem.onEntityAttack.addListener([this](Entity* attacker, Entity* target, int damage) {
            if (damage > 20) {
                this->playSound("critical_hit", attacker->getName());
            } else {
                this->playSound("attack", attacker->getName());
            }
        });
    }
    
    void playSound(const std::string& sound, const std::string& source) {
        std::cout << " Playing " << sound << " sound for " << source << std::endl;
    }
};

// Achievement system
class AchievementSystem {
public:
    AchievementSystem(BattleSystem& battleSystem) {
        battleSystem.onEntityAttack.addListener([this](Entity* attacker, Entity* target, int damage) {
            if (damage > 20) {
                unlockAchievement(attacker->getName(), "PowerStrike", "Deal over 20 damage in a single hit");
            }
        });
    }
    
    void unlockAchievement(const std::string& player, const std::string& name, const std::string& description) {
        // Check if player already has this achievement
        auto key = player + "_" + name;
        if (unlockedAchievements.find(key) != unlockedAchievements.end()) {
            return;
        }
        
        unlockedAchievements.insert(key);
        std::cout << "🏆 Achievement unlocked for " << player << ": " << name 
                  << " - " << description << std::endl;
    }
    
private:
    std::set<std::string> unlockedAchievements;
};

int main() {
    BattleSystem battleSystem;
    AudioSystem audioSystem(battleSystem);
    AchievementSystem achievementSystem(battleSystem);
    
    // Create entities
    auto warrior = std::make_shared<Entity>("Warrior", 100);
    auto mage = std::make_shared<Entity>("Mage", 70);
    auto archer = std::make_shared<Entity>("Archer", 85);
    
    // Add entities to battle system
    battleSystem.addEntity(warrior);
    battleSystem.addEntity(mage);
    battleSystem.addEntity(archer);
    
    // Simulate a few rounds of combat
    for (int round = 1; round <= 5; ++round) {
        std::cout << "\n===== ROUND " << round << " =====\n";
        
        // 70% chance of attack, 30% chance of heal
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 9);
        
        if (dist(gen) < 7) {
            battleSystem.simulateAttack();
        } else {
            battleSystem.simulateHeal();
        }
        
        // Short delay between rounds
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    std::cout << "\nBattle simulation complete!\n";
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}