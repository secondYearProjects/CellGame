//
// Created by sju on 20.11.18.
//
#include "terrainGenerator.h"


void terrainGenerator::Terrain::createTexture() {

    cl::CImg<unsigned char> canvas((n * tileSize), (n * tileSize), 1, 3, 0);

    canvas.channels(0, 3);
    cl::CImg<unsigned char> grass("Resources/grass.png");
  

    grass.channels(0, 3);
    // grass.display();
    // TODO: here
    grass.resize(tileSize, tileSize);
    //grass.display();
    const unsigned char white[] = {255, 255, 255, 255};
    //grass.draw_gaussian(0.2, 0.5, 0.6, 0.9, {100,100,100,100});
    canvas.draw_fill(0, 0, white);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            canvas.draw_image(i * tileSize, j * tileSize, grass);
            //canvas.draw_image(grass);
        }
    }
    //canvas.display();
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
