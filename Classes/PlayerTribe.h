//
// Created by sergei on 27.11.18.
//

#ifndef CELLGAME_PLAYERTRIBE_H
#define CELLGAME_PLAYERTRIBE_H

#include "Tribe.h"

class PlayerTribe : public Tribe {
public:
    virtual void manage();

private:
    PlayerTribe();

    ~PlayerTribe() override;
};


#endif //CELLGAME_PLAYERTRIBE_H
