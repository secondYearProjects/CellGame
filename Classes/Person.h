//
// Created by sergei on 25.11.18.
//

#ifndef CELLGAME_PERSON_H
#define CELLGAME_PERSON_H

#include "random.h"

#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <iostream>


class Tribe;

struct SPECIAL {
    int Strength = 1;
    int Perception = 1;
    int Endurance = 1;
    int Charisma = 1;
    int Intelligence = 1;
    int Agility = 1;
    int Luck = 1;

    static const int maxAttribute = 10;
};


void increaseAttribute(SPECIAL &special, int attribute, int val);

class Person {
    friend class Tribe;

public:
    Person();

    Person(int specialPoints);

    ~Person() = default;

    int attack();

    void eat(int foodAmount);

    int getHunger() const;

    void stats() {
        std::cout << "hp: " << health << " hunger: " << hunger << std::endl;
    }

    std::string getInfoString(bool getSPECIAL = false);

    static int getMaxHunger() { return maxHunger; }


    static void
    setPersonParams(int _basicAttack, int _basicRegen, int _pregnancyPerion, int _hungerPerAttack, int _hungerPerStep,
                    int _stepsBeforBreed) {

        Person::basicAttack = _basicAttack;
        Person::basicRegen = _basicRegen;
        Person::pregnancyPerion = _pregnancyPerion;
        Person::hungerPerAttack = _hungerPerAttack;
        Person::hungerPerStep = _hungerPerStep;
        Person::stepsBeforBreed = _stepsBeforBreed;
    }

private:
    bool success(float p);

    int calculateAttack();

    void step();

    void heal(int ammount);

    void recieveDamage(int val);

    int ID;

    std::string name;

    int health = 100;
    int maxHealth = 100;

    int hunger = 10;

    SPECIAL attributes;

    int age = 180;

    bool male = false;
    bool isPregnant = false;
    int PregnantSteps = 0;

    static int basicAttack;
    static int basicRegen;
    static int pregnancyPerion;
    static int hungerPerAttack;
    static int hungerPerStep;
    static int stepsBeforBreed;
    static int maxHunger;


    static int MAX_ID;
};


#endif //CELLGAME_PERSON_H
