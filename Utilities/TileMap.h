//
// Created by Leustean Stefan on 24.03.2025.
//

#ifndef TILEMAP_H
#define TILEMAP_H

// ca idee facem ca un tile sa fie 1/15 din screen height
class TileMap {
  private:
    float tileWidth, tileHeight;
    std::vector<Texture2D> tileTextures;
    // momentan codam tile-urile, tile-ul 1 este dirttile
  public:
    TileMap();
    void Draw();
};

#endif //TILEMAP_H
