//
// Created by sju on 20.11.18.
//
#include "terrainGenerator.h"


void terrainGenerator::Terrain::createTexture() {

    cl::CImg<unsigned char> canvas(static_cast<const unsigned int>(n * tileSize),
                                   static_cast<const unsigned int>(n * tileSize));

    cl::CImg<unsigned char> grass("Resources/grass.png", tileSize, tileSize);
    //grass = cl::CImg<unsigned char>::load_rgba("Resources/grass.png");
    //grass.load_png("Resources/grass.png");
    // grass = cl::CImg<unsigned char>("Resources/grass.jpg");
    //grass.resize(tileSize, tileSize);
    // grass.display();
    // TODO: here
    grass.display();
    const unsigned char white[] = {255, 255, 255};
    canvas.draw_fill(0, 0, white);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            canvas.draw_image(i * tileSize, j * tileSize, grass);
        }
    }
    canvas.display();
    canvas.save(texturePath);
    //canvas.display();
}

terrainGenerator::Terrain::Terrain(std::size_t _n, int _seed, size_t _tileSize) :
        n(_n), seed(_seed), tileSize(_tileSize) {
    createTexture();
}

const terrainGenerator::Tile &terrainGenerator::Terrain::getTile(std::size_t x, std::size_t y) const {
    return map[x][y];
}

std::string terrainGenerator::Terrain::getTexturePath() const {
    return texturePath;
}

std::string terrainGenerator::Terrain::getTextureName() const {
    return "fieldTexture.png";
}
