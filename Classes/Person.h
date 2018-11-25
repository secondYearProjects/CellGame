//
// Created by sergei on 25.11.18.
//

#ifndef CELLGAME_PERSON_H
#define CELLGAME_PERSON_H

#include <string>
#include <random>
#include <vector>
#include <ctime>
#include <cmath>

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

bool success(float p);

void increaseAttribute(SPECIAL &special, int attribute, int val);

class Person {
    friend class Tribe;

public:
    Person();

    Person(int specialPoints);

    ~Person() = default;

    int attack();

    void eat(int foodAmount);


private:
    int calculateAttack();

    void step();

    void recieveDamage(int val);

    int ID;

    std::string name;

    int health = 100;
    int maxHealth = 100;

    int hunger = 0;

    SPECIAL attributes;

    bool male = false;
    bool isPregnant = false;

    static const int basicAttack = 1;
    static const int pregnancyPerion = 10;
    static const int hungerPerAttack = 5;
    static const int hungerPerStep = 2;

    static int MAX_ID;
};


#endif //CELLGAME_PERSON_H
