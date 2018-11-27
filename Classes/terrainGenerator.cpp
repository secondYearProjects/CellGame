//
// Created by sju on 20.11.18.
//
#include "terrainGenerator.h"

using Random = effolkronium::random_static;

void terrainGenerator::Terrain::createTexture() {

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> tileRange(0, 3);
    std::discrete_distribution<> d(probability.begin(), probability.end());


    cl::CImg<unsigned char> canvas((n * tileSize) * 2, (n * tileSize) * 2);
    canvas.channels(0, 3);

    // load tile textures.
    std::vector<cl::CImg<unsigned char> > tiles = {
            loadTile(getTexturePath(TileType::grass), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::dirt), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::water), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::sand), tileSize * 2, tileSize * 2)
    };

    canvas.draw_fill(0, 0, white);

    generateHeightMap();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int typeID = d(rng);

            terrainMap[i][n - j - 1].assign(i, j, TileType(typeID));
            canvas.draw_image(i * tileSize * 2, j * tileSize * 2, tiles[TileType(typeID)]);
        }
    }

    canvas.save(texturePath);
}

terrainGenerator::Terrain::Terrain(std::size_t _n, int _seed, size_t _tileSize) :
        n(_n), seed(_seed), tileSize(_tileSize) {
    terrainMap = std::vector<std::vector<Tile> >(_n, std::vector<Tile>(_n));

    createTexture();
}

const terrainGenerator::Tile &terrainGenerator::Terrain::getTile(std::size_t x, std::size_t y) const {
    return terrainMap[x][y];
}

std::string terrainGenerator::Terrain::getTexturePath() const {
    return texturePath;
}

std::string terrainGenerator::Terrain::getTextureName() const {
    return "fieldTexture.png";
}

void terrainGenerator::Terrain::greenscreen(cimg_library::CImg<unsigned char> &img, const unsigned char *changeFrom,
                                            const unsigned char *changeTo) {
    cimg_forXY(img, x, y) {
            if (img(x, y, 0) == changeFrom[0] &&
                img(x, y, 1) == changeFrom[1] &&
                img(x, y, 2) == changeFrom[2]) {
                for (int i = 0; i < 3; i++)
                    img(x, y, i) = changeTo[i];
            }
        }
}

cimg_library::CImg<unsigned char> terrainGenerator::Terrain::loadTile(const char *const tileTexture,
                                                                      int width, int heigt, bool replaceAlpha) {
    cl::CImg<unsigned char> texture(tileTexture);
    texture.channels(0, 3);
    texture.resize(width, heigt);
    if (replaceAlpha)
        greenscreen(texture, black, white);

    return texture;
}

const char *const terrainGenerator::Terrain::getTexturePath(terrainGenerator::TileType tileType) {
    switch (tileType) {
        case grass:
            return "Resources/Tiles/grass.png";
        case dirt:
            return "Resources/Tiles/dirt.png";
        case water:
            return "Resources/Tiles/water.png";
        case sand:
            return "Resources/Tiles/sand.png";
    }
    return "Resourсes/Tiles/imageError.png";
}

void terrainGenerator::Terrain::addCreature(int _x, int _y, Tribe *creature) {
    terrainMap[_x][_y].addCreature(creature);
}

void terrainGenerator::Terrain::removeCreature(int _x, int _y, Tribe *creature) {
    terrainMap[_x][_y].removeCreature(creature);
}

void terrainGenerator::Terrain::generateHeightMap() {
    cl::CImg<double> canvas((n * tileSize) * 2, (n * tileSize) * 2);
    canvas.channels(0, 3);

    FastNoise perlin(seed);
    perlin.SetNoiseType(FastNoise::Perlin);


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double height = perlin.GetNoise(i,j);
            double heightColor[3] = {height*255, height*255, height*255};

            terrainMap[i][n - j - 1].height = height;
            canvas.draw_rectangle(i * tileSize * 2, j * tileSize * 2, i * tileSize * 4, j * tileSize * 4, heightColor);

        }
    }
    //canvas.draw_rectangle(0, 0, 10, 10, white);
    canvas.save(heightMap);
}


void terrainGenerator::Tile::addCreature(Tribe *tribe) {
    tribes.push_back(tribe);
}

void terrainGenerator::Tile::removeCreature(Tribe *tribe) {
    tribes.erase(std::find(tribes.begin(), tribes.end(), tribe));
}

void terrainGenerator::Tile::assign(size_t _x, size_t _y, terrainGenerator::TileType _type) {
    this->x = _x;
    this->y = _y;
    this->type = _type;
}

double clamp(double x, double upper, double lower) {
    return std::min(upper, std::max(x, lower));
}