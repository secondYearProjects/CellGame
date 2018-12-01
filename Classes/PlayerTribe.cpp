//
// Created by sergei on 27.11.18.
//

#include "PlayerTribe.h"

#include <iostream>

void PlayerTribe::manage() {
    Tribe::manage();
    std::cout << "I am amnaging myself!\n";
}
