#ifndef GAME_H
#define GAME_H
#include <unordered_map>

class Game;

class InvalidExternal : public std::exception {
public:
  void what() {
    std::cout << "File format is incorrect" << std::endl;
  }
};

enum GameState {
  MainMenu_State, SettingsMenu_State, GameScreen_State, FightScreen_State, DialogScreen_State, GameMenuScreen_State
};

void ReadFile(Game* game, const std::string &filename);

class Game {
  private:
    const int fps = DEFAULT_FPS;

    std::pair<int, int> aspect_ratio = DEFAULT_ASPECT_RATIO;
    std::pair<float, float> resolution;
    float volume;
    bool background_music;
    bool sound_effects;
    bool voice;

    std::unordered_map<std::string, Font> fonts;
    Font* default_font;

    float scale_factor;
    Game() {}

  public:
    ~Game();

    static Game* Main;
    static Game* StartGame();

    static GameState CurrentState;
    static GameState LastState;

    float GetVolume() const;
    bool GetBackgroundMusic() const;
    bool GetSoundEffects() const;
    bool GetVoice() const;

    void SetVolume(float);
    void SetBackgroundMusic(bool);
    void SetSoundEffects(bool);
    void SetVoice(bool);
    void SetResolution(std::pair<float,float>);
    void SetResolution(float width, float height);



    Font* AddFont(const std::string& fontname, const std::string& filepath);
    Font* GetFont(const std::string& fontname);
    Font* GetDefaultFont();

    std::pair<float, float> GetResolution();
    float GetScaleFactor();
    void ResizeWindow(float width, float height);
    void ResizeWindow(std::pair<float,float> resolution);

    friend void ReadFile(Game* game, const std::string &filename);
};

bool operator==(Color a, Color b);
bool operator==(const Texture2D& A, const Texture2D& B);
bool IsTextureValid(Texture2D* texture);


#endif //GAME_H
