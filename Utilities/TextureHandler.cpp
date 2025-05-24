//
// Created by Leustean Stefan on 06.05.2025.
//
#include <iostream>
#include <raylib.h>
#include "Defines.h"
#include "Game.h"
#include "TextureHandler.h"


using namespace std;

list<TextureHandlerCell> TextureHandler::Handler;

void TextureHandler::CleanTextureCache() {
    for (auto &i : Handler) {
        if (IsTextureValid(i.texture)) {
            UnloadTexture(i.texture);
        }
    }
    Handler.clear();
}

Texture2D* TextureHandler::CloneTexture(Texture2D* texture) {
    if (texture == nullptr)
        return nullptr;
    for (auto &i : Handler)
        if (i.texture == *texture) {
            i.count++;
            break;
        }

    // de adaugat exceptie aicea
    if (!IsTextureValid(*texture))
        cout << "Texture cloning failed" << endl;
    return texture;
}

Texture2D* TextureHandler::AddTexture(const string &filepath) {
    for (auto &i : Handler)
        if (filepath == i.filepath) {
            i.count++;
            return &i.texture;
        }

    Handler.emplace_back();
    Handler.back().filepath = filepath;
    Handler.back().count = 1;
    Handler.back().texture = LoadTexture(filepath.c_str());
    return &Handler.back().texture;
}

void TextureHandler::ReleaseTexture(Texture2D* &texture) {
    if (texture == nullptr)
        return;
    for (auto i = Handler.begin(); i != Handler.end(); i++)
        if (i->texture == *texture) {
            i->count--;
            if (i->count == 0) {
                if (IsTextureValid(i->texture))
                    UnloadTexture(i->texture);
                Handler.erase(i);
            }
            break;
        }
    texture = nullptr;
}

void TextureHandler::ReleaseTexture(const std::string &filepath) {
    for (auto i = Handler.begin(); i != Handler.end(); i++)
        if (i->filepath == filepath) {
            i->count--;
            if (i->count == 0) {
                UnloadTexture(i->texture);
                Handler.erase(i);
            }
            break;
        }
}

void TextureHandler::DeleteTexture(Texture2D* texture) {
    if (texture == nullptr)
        return;
    for (auto i = Handler.begin(); i != Handler.end(); i++)
        if (i->texture == *texture) {
            UnloadTexture(i->texture);
            Handler.erase(i);
            return;
        }
}

void TextureHandler::DeleteTexture(const std::string &filepath) {
    for (int i = 0; i < Handler.size(); i++)
        for (auto it = Handler.begin(); it != Handler.end(); it++)
            if (it->filepath == filepath) {
                UnloadTexture(it->texture);
                Handler.erase(it);
                return;
            }
}


void TextureHandler::CopyTexture(Texture2D* &texture, Texture2D* texturetocopy) {
    ReleaseTexture(texture);
    texture = CloneTexture(texturetocopy);
}
