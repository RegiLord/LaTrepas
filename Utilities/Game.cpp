#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <raylib.h>
#include "Defines.h"
#include "Vector2D.h"
#include "Game.h"
#include "Vector2D.h"
#include "object.h"
#include "TextureHandler.h"

using namespace std;

GameState Game::CurrentState = MainMenu_State;
GameState Game::LastState = MainMenu_State;

void ReadFile(Game *game, const std::string &filename) {
    ifstream settings("externals/settings");

    try {
        char symbol;
        while (settings >> symbol)
            switch (symbol) {
                case 'R': {
                    settings >> game->resolution.first >> game->resolution.second;
                    break;
                }
                case 'V': {
                    settings >> game->volume;
                    break;
                }
                case 'b': {
                    settings >> game->background_music;
                    break;
                }
                case 's': {
                    settings >> game->sound_effects;
                    break;
                }
                case 'v': {
                    settings >> game->voice;
                    break;
                }
                default:
                    throw InvalidExternal();
            }
    }
    catch (InvalidExternal& e) {
        e.what();
        cout<< "ERROR: " << filename << " settings format" << endl;
        throw InvalidExternal();
    }
    settings.close();
}


Game *Game::StartGame() {
    Game* game = new Game();
    /*
     * Citim setarile salvate intr-un fisier extern
     */
    ReadFile(game, "externals/settings");

    /*
     * Trebuie sa configuram scale factorii
     *  La instantierea jocului nu e necesar sa modificam
     * pozitia si size-ul tuturor obiectelor pentru ca nu au fost
     * create inca, dar la resize window va fi necesar
     */

    //scale_factor
    game->scale_factor = game->resolution.first / DEFAULT_RESOLUTION.first;

    InitWindow(game->resolution.first, game->resolution.second, "La Trèpas");
    SetExitKey(0);
    SetTargetFPS(game->fps);
    game->default_font = game->AddFont("Alagard", "rsc/fonts/Alagard.png");
    return game;
}



Font* Game::AddFont(const std::string &fontname, const std::string &filepath) {
    if (fonts.contains(fontname))
        cout << "WARNING: trying to add font again :: " << fontname << endl;
    else
        fonts[fontname] = LoadFont(filepath.c_str());
    return &fonts[fontname];
}

Font *Game::GetFont(const std::string &fontname) {
    if (fonts.contains(fontname)) {
        return &fonts[fontname];
    }
    else {
        cout << "WARNING: trying to get unexistent font " << fontname << endl;
        cout << "WARNING: using default font" << endl;
        return default_font;
    }
}


Font* Game::GetDefaultFont() {
    return default_font;
}

float Game::GetScaleFactor() {
    return scale_factor;
}

pair<float,float> Game::GetResolution() {
    return resolution;
}
void Game::ResizeWindow(std::pair<float, float> resolution) {
    this->ResizeWindow(resolution.first, resolution.second);
}

void Game::ResizeWindow(float width, float height) {
    // modificam pur si simplu scale factor
    scale_factor = width / DEFAULT_RESOLUTION.first;
    resolution = {width, height};

    SetWindowSize(resolution.first, resolution.second);
    SetWindowPosition(
        GetMonitorWidth(GetCurrentMonitor()) / 2 - resolution.first / 2,
        GetMonitorHeight(GetCurrentMonitor()) / 2 - resolution.second / 2
    );
}


bool operator==(Color a, Color b) {
    return (a.a == b.a) && (a.b == b.b) && (a.g == b.g) && (a.r == b.r);
}
bool operator==(const Texture2D& A, const Texture2D& B) {
    return A.id == B.id;
}

bool IsTextureValid(Texture2D* texture) {
    if (texture == nullptr) return false;
    return IsTextureValid(*texture);
}

Game::~Game() {
    for (auto it = fonts.begin(); it != fonts.end(); it++) {
        if (IsFontValid(it->second)) {
            UnloadFont(it->second);
        }
    }
    TextureHandler::CleanTextureCache();

    for (int i = 0; i < Object::workspace.Children().size(); i++) {
        if (Object::workspace.Children()[i] != nullptr)
            Object::workspace.Children()[i]->Destroy();
    }
    ofstream settings("externals/settings");
    settings << "R " << resolution.first << " " << resolution.second << "\n";
    settings << "V " << volume << "\n";
    settings << "b " << background_music << "\n";
    settings << "s " << sound_effects << "\n";
    settings << "v " << voice << "\n";
    settings.close();
}

float Game::GetVolume() const { return volume; }
bool Game::GetSoundEffects() const { return sound_effects; }
bool Game::GetVoice() const { return voice; }
bool Game::GetBackgroundMusic() const { return background_music; }

void Game::SetVolume(float volume) { this->volume = volume; }
void Game::SetSoundEffects(bool sound_effects) { this->sound_effects = sound_effects; }
void Game::SetVoice(bool voice) { this->voice = voice; }
void Game::SetBackgroundMusic(bool background_music) { this->background_music = background_music; }
void Game::SetResolution(float width, float height) { this->SetResolution({width, height}); }
void Game::SetResolution(std::pair<float,float> resolution) {
    if (this->resolution == resolution) return;
    this->resolution = resolution;
    ResizeWindow(resolution.first, resolution.second);
}
