//
// Created by sju on 20.11.18.
//
#include "terrainGenerator.h"

using Random = effolkronium::random_static;

void terrainGenerator::Terrain::createTexture() {
    //cl::CImg<unsigned char> canvas((n * tileSize) * 2, (n * tileSize) * 2);
    //canvas.channels(0, 3);

    // load tile textures.
    std::vector<cl::CImg<unsigned char> > tiles = {
            loadTile(getTexturePath(TileType::water), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::sand), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::dirt), tileSize * 2, tileSize * 2),
            loadTile(getTexturePath(TileType::grass), tileSize * 2, tileSize * 2)
    };

    //canvas.draw_fill(0, 0, white);

    generateHeightMap();


    // Only maps with n=chunkSize*a
    int lineChunks = n / chunkSize;

    cl::CImg<double> chunkCanvas((chunkSize * tileSize) * 2, (chunkSize * tileSize) * 2);
    chunkCanvas.channels(0, 3);

    //smoothHeights(4);

    // generate height map chunks
    for (int chunkRow = 0; chunkRow < lineChunks; chunkRow++) {
        for (int chunkCol = 0; chunkCol < lineChunks; chunkCol++) {

            int iDraw = 0;
            int chunkID = chunkRow + chunkCol * lineChunks;

            for (int i = chunkRow * chunkSize; i < chunkRow * chunkSize + chunkSize; i++, iDraw++) {
                int jDraw = 0;
                for (int j = chunkCol * chunkSize; j < chunkCol * chunkSize + chunkSize; j++, jDraw++) {

                    terrainMap[i][n - j - 1].height = linearNormalize(terrainMap[i][n - j - 1].height,
                                                                      minHeight,
                                                                      maxHeight);
                    double height = terrainMap[i][n - j - 1].height;

                    double heightColor[3] = {height * 255, height * 255, height * 255};
                    chunkCanvas.draw_rectangle(iDraw * tileSize * 2, jDraw * tileSize * 2,
                                               (iDraw + 1) * tileSize * 2, (jDraw + 1) * tileSize * 2,
                                               heightColor);
                }
            }
            chunkCanvas.save(("Resources/Chunks/HeightChunks/heightMap" + std::to_string(chunkID) + ".png").c_str());

        }
    }

    // generate texture map chunks and apply height texture
    for (int chunkRow = 0; chunkRow < lineChunks; chunkRow++) {
        for (int chunkCol = 0; chunkCol < lineChunks; chunkCol++) {

            int iDraw = 0;
            int chunkID = chunkRow + chunkCol * lineChunks;

            for (int i = chunkRow * chunkSize; i < chunkRow * chunkSize + chunkSize; i++, iDraw++) {
                int jDraw = 0;
                for (int j = chunkCol * chunkSize; j < chunkCol * chunkSize + chunkSize; j++, jDraw++) {

                    double height = terrainMap[i][n - j - 1].height;

                    TileType tileType = tileByHeight(height);
                    terrainMap[i][n - j - 1].assign(i, j, tileType);

                    chunkCanvas.draw_image(iDraw * tileSize * 2, jDraw * tileSize * 2, tiles[tileType]);

//                    std::string heightPath =
//                            "Resources/Chunks/HeightChunks/heightMap" + std::to_string(chunkID) + ".png";
//
//                    cl::CImg<unsigned char> heightFilter = loadTile(heightPath.c_str(),
//                                                                    2 * tileSize * chunkSize,
//                                                                    2 * tileSize * chunkSize);
//
//                    chunkCanvas.draw_image(0, 0, heightFilter, 0.1);
                }
            }
            cl::CImg<double> heightFilter =
                    loadTile(("Resources/Chunks/HeightChunks/heightMap" + std::to_string(chunkID) + ".png").c_str(),
                             (chunkSize * tileSize) * 2, (chunkSize * tileSize) * 2);

            heightFilter.channels(0, 3);
            //heightFilter.blur_box(10.0f,10.0f,1.0f);

            //chunkCanvas.draw_image(0, 0, heightFilter, 0.1);
            //multiptyImages(chunkCanvas, heightFilter, 1.0f);

            chunkCanvas.save(
                    ("Resources/Chunks/TextureChunks/fieldTexture" + std::to_string(chunkID) + ".png").c_str());

        }
    }


//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < n; j++) {
//
//
//            //terrainMap[i][n - j - 1].height = linearNormalize(terrainMap[i][n - j - 1].height, minHeight, maxHeight);
//            TileType tileType = tileByHeight(terrainMap[i][n - j - 1].height);
//            //terrainMap[i][n - j - 1].assign(i, j, tileType);
//
//            canvas.draw_image(i * tileSize * 2, j * tileSize * 2, tiles[tileType]);
//        }
//    }
//    // height color filter
//    cl::CImg<unsigned char> heightFilter = loadTile(heightMap, 2 * tileSize * n, 2 * tileSize * n);
//
//    canvas.draw_image(0, 0, heightFilter, 0.1);

    //canvas.save(texturePath);
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
    FastNoise noise1(seed);
    noise1.SetNoiseType(FastNoise::Cellular);
    //noise1.SetFractalOctaves(5);
    noise1.SetFrequency(0.25);

    FastNoise noise2(seed);
    noise2.SetNoiseType(FastNoise::Cellular);
    //noise2.SetFractalOctaves(5);
    noise2.SetFrequency(0.15);

    FastNoise noise3(seed);
    noise3.SetNoiseType(FastNoise::PerlinFractal);
    noise3.SetFractalOctaves(5);
    noise3.SetFrequency(0.1);


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double height = 1 * noise1.GetNoise(i + 2, j + 2) +
                            0.1 * noise2.GetNoise(i + 2, j + 2) +
                            0.25 * noise3.GetNoise(i + 2, j + 2);

            if (height > maxHeight)
                maxHeight = height;
            if (height < minHeight)
                minHeight = height;

            double heightColor[3] = {height * 255, height * 255, height * 255};

            terrainMap[i][n - j - 1].height = height;
        }
    }
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

void terrainGenerator::Terrain::multiptyImages(cimg_library::CImg<double> &A, cimg_library::CImg<double> &mul,
                                               float coef) {
    cimg_forXY(A, x, y) {
            if (A(x, y, 0) != white[0] &&
                A(x, y, 1) != white[1] &&
                A(x, y, 2) != white[2]) {
                for (int i = 0; i < 3; i++) {
                    A(x, y, i) += mul(x, y, i) * coef;
                }
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