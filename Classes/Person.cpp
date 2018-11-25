//
// Created by sergei on 25.11.18.
//

#include "Person.h"

bool success(float p) {
    std::mt19937 rng(time(0));
    std::uniform_real_distribution<float> d(0.0f, 1.0f);
    return (p >= d(rng));
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
    Person::MAX_ID++;
}


Person::Person(int specialPoints) : Person() {
    if (specialPoints > (SPECIAL::maxAttribute - 1) * 7)
        specialPoints = (SPECIAL::maxAttribute - 1) * 7;

    std::mt19937 rng(time(0));
    std::uniform_int_distribution<int> d(0, 6);
    while (specialPoints > 0) {
        increaseAttribute(attributes, d(rng), 1);
        specialPoints--;
    }
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

void Person::eat(int foodAmount) { hunger += foodAmount; }

void Person::recieveDamage(int val) { health -= val; }

void Person::step() {
    if (hunger < 0) {
        hunger -= static_cast<int>(hungerPerStep * (-hunger) *
                                   (1.1f - static_cast<float >(attributes.Endurance) / SPECIAL::maxAttribute));
    }
}
