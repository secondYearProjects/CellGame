//
// Created by sju on 17.11.18.
//

#ifndef TEMPLATECPP_TERRAINGENERATOR_H
#define TEMPLATECPP_TERRAINGENERATOR_H

#include "CImg.h"
#include "zlib.h"
#include "png.h"
#include "CCImage.h"


#include <string>
#include <vector>

#define cimg_use_png

namespace terrainGenerator {
    namespace cl = cimg_library;

    enum TileType {
        grass = 0, dirt, water
    };

    struct Tile {
        std::size_t x, y;
        int height;

        TileType type;
    };

    class Terrain {
    public:
        Terrain(std::size_t _n, int _seed, std::size_t _tileSize = 30);
        Terrain()= default;

        ~Terrain() = default;

        const Tile &getTile(std::size_t x, std::size_t y) const;

        std::string getTexturePath() const;

        std::string getTextureName() const;

    private:
        int seed;
        std::size_t tileSize;
        std::size_t n;
        const char *const texturePath = "Resources/fieldTexture.png";
        std::vector<std::vector<Tile> > map;

        void createTexture();

    };
}


#endif //TEMPLATECPP_TERRAINGENERATOR_H
