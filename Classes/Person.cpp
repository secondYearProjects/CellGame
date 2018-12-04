//
// Created by sergei on 25.11.18.
//

#include "Person.h"

using Random = effolkronium::random_static;

bool Person::success(float p) {
//    std::random_device rd;
//    std::mt19937 rng(rd());
//    std::uniform_real_distribution<float> d(0.0f, 1.0f);
    return (p >= Random::get(0.0f, 1.0f));
}

void increaseAttribute(SPECIAL &special, int attribute, int val) {
    switch (attribute) {
        case 0:
            special.Strength += val;
            break;
        case 1:
            special.Perception += val;
            break;
        case 2:
            special.Endurance += val;
            break;
        case 3:
            special.Charisma += val;
            break;
        case 4:
            special.Intelligence += val;
            break;
        case 5:
            special.Agility += val;
            break;
        case 6:
            special.Luck += val;
            break;

        default:
            break;
    }
}


Person::Person() {
    ID = Person::MAX_ID;

//    std::random_device rd;
//    std::mt19937 rng(rd());
//    std::uniform_int_distribution<int> gender(0, 1);
//    std::uniform_int_distribution<int> maxHP(80, 200);
//    std::uniform_int_distribution<int> nameD(0, 5);

    const std::vector<std::string> names = {"Obuka", "Kiba", "Abba", "Uka", "Totto", "Dudu", "Fefe", "Doro", "Abekwa",
                                            "Onu", "Abbiba", "Ubutu", "Kali", "Shindo", "Debia", "Biba", "Boba",
                                            "Sasuke", "Uzumaki", "Dodo", "Uma", "Korol", "Abekwa", "Mita", "Mito"};

    male = Random::get<bool>();
    maxHealth = 100;
    health = maxHealth;
    name = names[Random::get<std::size_t>(0, names.size() - 1)];


    Person::MAX_ID++;
}


Person::Person(int specialPoints) : Person() {
    // SPECIAL generation
    if (specialPoints > (SPECIAL::maxAttribute - 1) * 7)
        specialPoints = (SPECIAL::maxAttribute - 1) * 7;

//    std::random_device rd;
//    std::mt19937 rng(rd());
//    std::uniform_int_distribution<int> d(0, 6);

    while (specialPoints > 0) {
        increaseAttribute(attributes, Random::get(0, 6), 1);
        specialPoints--;
    }

    maxHealth += 10 * attributes.Strength;
    health = maxHealth;
}

int Person::attack() {
    int hungerChange = static_cast<int>(hungerPerAttack * (1.0f - static_cast<float >(attributes.Endurance) /
                                                                  static_cast<float >(SPECIAL::maxAttribute)));
    hunger -= hungerChange;

    return calculateAttack();
}


int Person::MAX_ID = 0;

int Person::calculateAttack() {
    float criticalMult = 1.0f + static_cast<float >(attributes.Perception) /
                                static_cast<float>(SPECIAL::maxAttribute);

    float criticalSuccess = static_cast<float >(attributes.Luck) /
                            static_cast<float >(SPECIAL::maxAttribute);

    float attackSuccess = 0.1f + static_cast<float >(attributes.Agility) /
                                 static_cast<float >(SPECIAL::maxAttribute);

    if (!success(attackSuccess))
        return 0;

    float damage = static_cast<float >(basicAttack);
    damage *= static_cast<float >(attributes.Strength);

    if (success(criticalSuccess))
        damage *= criticalMult;

    return static_cast<int>(damage);
}

void Person::eat(int foodAmount) {
    if (hunger + foodAmount > maxHunger)
        hunger = maxHunger;
    else
        hunger += foodAmount;
    //std::cout << "hunger + " << hunger << std::endl;
}

void Person::recieveDamage(int val) { health -= val; }

void Person::step() {
    this->hunger -= static_cast<int>(hungerPerStep *
                                     (1.1f - static_cast<float >(attributes.Endurance) / SPECIAL::maxAttribute));
    //double hch = -static_cast<int>(hungerPerStep *
    //                              (1.1f - static_cast<float >(attributes.Endurance) / SPECIAL::maxAttribute));
    //std::cout << hch << "\n";
    if (this->hunger < 0) {
        //health -= 3;
        health -= static_cast<int>(10 * static_cast<float >(attributes.Endurance) / SPECIAL::maxAttribute);
        if (hunger < -maxHunger)
            hunger = -maxHunger;
        //std::cout << "--hp " << health << " hunger " << hunger << std::endl;
    } else {

        //health += 4;
        heal(basicRegen * attributes.Endurance);
        //std::cout << "heal\n";
    }

    if (isPregnant) {
        PregnantSteps++;
    }
}

int Person::getHunger() const { return hunger; }

void Person::heal(int ammount) {
    if (health + ammount > maxHealth)
        health = maxHealth;
    else
        health += ammount;
}

int Person::basicAttack = 1;
int Person::basicRegen = 1;
int Person::pregnancyPerion = 10;
int Person::hungerPerAttack = 2;
int Person::hungerPerStep = 4;
int Person::stepsBeforBreed = 20;
int Person::maxHunger = 100;

std::string Person::getInfoString(bool getSPECIAL) {
    std::string res = name + "\n   (" + std::to_string(health) + "/" + std::to_string(maxHealth) + ")\n   food: " +
                      std::to_string(hunger);
    if (getSPECIAL) {
        res += "\n S:" + std::to_string(attributes.Strength) +
               " P:" + std::to_string(attributes.Perception) +
               " E:" + std::to_string(attributes.Endurance) +
               " C:" + std::to_string(attributes.Charisma) +
               " I:" + std::to_string(attributes.Intelligence) +
               " A:" + std::to_string(attributes.Agility) +
               " L:" + std::to_string(attributes.Luck);
    }
    return res;
}







