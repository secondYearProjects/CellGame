//
// Created by sju on 17.11.18.
//

#ifndef TEMPLATECPP_TERRAINGENERATOR_H
#define TEMPLATECPP_TERRAINGENERATOR_H

#include "CImg.h"
#include "zlib.h"
#include "png.h"
#include "CCImage.h"
#include "random.h"
#include "FastNoise.h"

#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>


#define cimg_use_png

double clamp(double x, double upper, double lower);

double sigmoid(double x);

double linearNormalize(double x, double a, double b);

class Tribe;
namespace terrainGenerator {


    namespace cl = cimg_library;

    enum TileType {
        water = 0, sand, dirt, grass
    };

    class Tile {
    public:
        Tile() = default;

        Tile(size_t _x, size_t _y, TileType _type, double _height = 0.0f) :
                x(_x), y(_y), type(_type), height(_height) {}

        void assign(size_t _x, size_t _y, TileType _type);

        void addCreature(Tribe *tribe);

        void removeCreature(Tribe *tribe);

        std::size_t x = 0;
        std::size_t y = 0;
        double height = 0.0f;

        TileType type = TileType::dirt;

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

        static const int chunkSize = 10;

        //std::vector<double> probability = {0.3, 0.5, 0.1, 0.1};

        std::vector<double> levels = {0.2, 0.4, 0.6, 1.0};
        double minHeight = 100.0;
        double maxHeight = -100.0;

        uint32_t seed = 1337;
        std::size_t tileSize = 30;
        std::size_t n = 10;
        const char *const texturePath = "Resources/fieldTexture.png";
        const char *const heightMap = "Resources/heightMap.png";


        std::vector<std::vector<Tile> > terrainMap;

        std::vector<std::vector<double> > bufferHeights;

        void createTexture();

        void greenscreen(cl::CImg<unsigned char> &img,
                         const unsigned char changeFrom[3],
                         const unsigned char changeTo[3]);

        void generateHeightMap();

        void smoothHeights(double a);

        cl::CImg<unsigned char> loadTile(const char *const tileTexture, int width, int heigt, bool replaceAlpha = true);

        const char *const getTexturePath(TileType tileType);

        TileType tileByHeight(double height);
    };
}


#endif //TEMPLATECPP_TERRAINGENERATOR_H
