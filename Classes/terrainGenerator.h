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
#include <random>
#include "random.h"

#define cimg_use_png

class Tribe;
namespace terrainGenerator {


    namespace cl = cimg_library;

    enum TileType {
        grass = 0, dirt, water, sand
    };

    class Tile {
    public:
        Tile() = default;

        Tile(size_t _x, size_t _y, int _height, TileType _type) :
                x(_x), y(_y), height(_height), type(_type) {}

        void addCreature(Tribe *tribe);

        void removeCreature(Tribe *tribe);

        std::size_t x, y;
        int height;

        TileType type;

        std::vector<Tribe *> tribes;
    };


    class Terrain {
    public:
        Terrain(std::size_t _n, int _seed, std::size_t _tileSize = 30);

        Terrain() = default;

        ~Terrain() = default;

        const Tile &getTile(std::size_t x, std::size_t y) const;

        Tile &changeTile(std::size_t x, std::size_t y) { return terrainMap[x][y]; }

        void addCreature(int _x, int _y, Tribe *creature);


        void removeCreature(int _x, int _y, Tribe *creature);

        std::string getTexturePath() const;

        std::string getTextureName() const;


    private:
        // Constant colors
        const unsigned char white[3] = {255, 255, 255};
        const unsigned char black[3] = {0, 0, 0};
        const unsigned char red[3] = {255, 0, 0};
        const unsigned char green[3] = {0, 255, 0};
        const unsigned char blue[3] = {0, 0, 255};

        std::vector<double> probability = {0.5, 0.3, 0.1, 0.1};

        int seed;
        std::size_t tileSize;
        std::size_t n;
        const char *const texturePath = "Resources/fieldTexture.png";

        std::vector<std::vector<Tile> > terrainMap;

        void createTexture();

        void greenscreen(cl::CImg<unsigned char> &img,
                         const unsigned char changeFrom[3],
                         const unsigned char changeTo[3]);

        cl::CImg<unsigned char> loadTile(const char *const tileTexture, int width, int heigt, bool replaceAlpha = true);

        const char *const getTexturePath(TileType tileType);
    };
}


#endif //TEMPLATECPP_TERRAINGENERATOR_H
