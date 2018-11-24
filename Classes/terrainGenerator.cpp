//
// Created by sju on 20.11.18.
//
#include "terrainGenerator.h"


void terrainGenerator::Terrain::createTexture() {

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> tileRange(0, 2);


    cl::CImg<unsigned char> canvas((n * tileSize) * 2, (n * tileSize) * 2);
    canvas.channels(0, 3);

    // load tile textures.
    std::vector<cl::CImg<unsigned char> > tiles = {
            loadTile(getTexturePath(TileType::grass), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::dirt), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::water), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::sand), tileSize * 2, tileSize * 2)
    };



    //cl::CImg<unsigned char> grass = loadTile("Resources/Tiles/grass.png", tileSize * 2, tileSize * 2);

    canvas.draw_fill(0, 0, white);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int typeID = tileRange(rng);
            if (typeID == TileType::water)
                cocos2d::log("water");

            terrainMap[i][n - j - 1] = Tile(i, j, 0, TileType(typeID));
            canvas.draw_image(i * tileSize * 2, j * tileSize * 2, tiles[TileType(typeID)]);
        }
    }


    //canvas.draw_image(0, 100, tiles[TileType(3)]);


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

void terrainGenerator::Terrain::addCreature(int _x, int _y, Creature *creature) {
    terrainMap[_x][_y].addCreature(creature);
}

void terrainGenerator::Terrain::removeCreature(int _x, int _y, Creature *creature) {
    terrainMap[_x][_y].removeCreature(creature);
}

void terrainGenerator::Tile::addCreature(Creature *creature) {
    creatures.push_back(creature);
}

void terrainGenerator::Tile::removeCreature(Creature *creature) {
    creatures.erase(std::find(creatures.begin(), creatures.end(), creature));
}
