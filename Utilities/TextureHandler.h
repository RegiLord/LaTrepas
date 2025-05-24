//
// Created by Leustean Stefan on 06.05.2025.
//

#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H
#include <list>

struct TextureHandlerCell {
    Texture2D texture;
    std::string filepath;
    int count;
};

namespace TextureHandler {

    extern std::list<TextureHandlerCell> Handler;
    void CleanTextureCache();

    Texture2D* CloneTexture(Texture2D* texture);
    Texture2D* AddTexture(const std::string &filepath);

    void ReleaseTexture(const std::string &filepath);
    void ReleaseTexture(Texture2D*& texture);

    void DeleteTexture(Texture2D *texture);
    void DeleteTexture(const std::string &filepath);

    void CopyTexture(Texture2D* &texture, Texture2D* texturetocopy);
}



#endif //TEXTUREHANDLER_H
