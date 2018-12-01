//
// Created by sju on 20.11.18.
//
#include "terrainGenerator.h"

using Random = effolkronium::random_static;

void terrainGenerator::Terrain::createTexture() {
    cl::CImg<unsigned char> canvas((n * tileSize) * 2, (n * tileSize) * 2);
    canvas.channels(0, 3);

    // load tile textures.
    std::vector<cl::CImg<unsigned char> > tiles = {
            loadTile(getTexturePath(TileType::water), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::sand), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::dirt), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::grass), tileSize * 2, tileSize * 2)
    };

    canvas.draw_fill(0, 0, white);

    generateHeightMap();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            smoothHeights(4);

            terrainMap[i][n - j - 1].height = linearNormalize(terrainMap[i][n - j - 1].height, minHeight, maxHeight);
            TileType tileType = tileByHeight(terrainMap[i][n - j - 1].height);
            terrainMap[i][n - j - 1].assign(i, j, tileType);

            canvas.draw_image(i * tileSize * 2, j * tileSize * 2, tiles[tileType]);
        }
    }
    // height color filter
    cl::CImg<unsigned char> heightFilter = loadTile(heightMap, 2 * tileSize * n, 2 * tileSize * n);

    canvas.draw_image(0, 0, heightFilter, 0.1);


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

    FastNoise perlin1(seed);
    perlin1.SetNoiseType(FastNoise::Perlin);
    perlin1.SetFractalOctaves(6);
    perlin1.SetFrequency(0.3);

    FastNoise perlin2(seed);
    perlin2.SetNoiseType(FastNoise::Perlin);
    perlin2.SetFractalOctaves(6);
    perlin2.SetFrequency(0.4);

    FastNoise perlin3(seed);
    perlin3.SetNoiseType(FastNoise::Perlin);
    perlin3.SetFractalOctaves(6);
    perlin3.SetFrequency(0.6);


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double height = 1 * perlin1.GetNoise(i + 2, j + 2) +
                            0.5 * perlin2.GetNoise(i + 2, j + 2) +
                            0.25 * perlin3.GetNoise(i + 2, j + 2);

            if (height > maxHeight)
                maxHeight = height;
            if (height < minHeight)
                minHeight = height;

            double heightColor[3] = {height * 255, height * 255, height * 255};

            terrainMap[i][n - j - 1].height = height;
            canvas.draw_rectangle(i * tileSize * 2, j * tileSize * 2, i * tileSize * 4, j * tileSize * 4, heightColor);

        }
    }

    canvas.save(heightMap);
}

terrainGenerator::TileType terrainGenerator::Terrain::tileByHeight(double height) {
    for (int i = 0; i < levels.size(); i++) {
        if (height <= levels[i]) {
            return TileType(i);
        }
    }
    return TileType::grass;
}

void terrainGenerator::Terrain::smoothHeights(double a) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            terrainMap[i][j].height = std::round(terrainMap[i][j].height * a) / a;
        }
    }
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

double sigmoid(double x) {
    return 1 / (1 + std::exp(-x));
}

double linearNormalize(double x, double a, double b) {

    return (x - a) / (b - a);
}