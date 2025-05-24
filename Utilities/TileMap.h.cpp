#include <iostream>
#include <vector>
#include <string>
#include <raylib.h>
#include "TileMap.h"
using namespace std;

TileMap::TileMap() {
    tileTextures = vector<Texture2D> ();
    tileTextures.push_back(LoadTexture("rsc/tiles/DirtTile.png"));
    tileHeight = 1.0f * GetScreenHeight() / 15;
    tileWidth = tileHeight;
}

void TileMap::Draw() {
    Rectangle SourceRect = { 0,0,64,64 };

    for (int i = 0; i < 50; i++)
      for (int j = 0; j < 15; j++) {
        Rectangle DestRect;
        DestRect.width = tileWidth;
        DestRect.height = tileHeight;
        DestRect.x = (i * tileWidth);
        DestRect.y = (j * tileHeight);

        DrawTexturePro(tileTextures[0], SourceRect, DestRect, Vector2(0,0), 0.0f, WHITE);
      }
}