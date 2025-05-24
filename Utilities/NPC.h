//
// Created by Leustean Stefan on 21.03.2025.
//

#ifndef NPC_H
#define NPC_H

class NPC {
  private:
    std::string name; // Name-ul e unic
    std::vector<std::string> dialogLines;
    // profile pic e folosit la dialogbox-uri, iar sprite in realmap;
    Vector2D position;
    Texture2D profilePic;
    Texture2D sprite;
    int animFrames;
    int animFrame;
    int* permList;
    /*
      1->interactable
      2->fightable
      3->obligatory (ptr story progression)
     */
    Sound voiceSound;

  public:
    NPC();
    NPC(std::string name, int* permList);
    NPC(const NPC& npc);
    ~NPC();

    std::string getName();
    Vector2D& Position();
    std::vector<std::string>& getDialogLines();

    static std::vector<NPC*> npcs;
    static NPC* getNPC(std::string name);

    void LoadSpriteTexture(const char* filepath);
    void LoadSpriteTexture(Texture2D texture);
    void LoadProfilePic(const char* filepath);
    void LoadProfilePic(Texture2D texture);
    void LoadVoiceSound(const char* filepath);
    void LoadVoiceSound(Sound voiceSound);
    void AddDialogLine(std::string line);
    void AddDialogLine(const char* line);

    // trebuie sa tin cont de animatii
    void Draw();

    //atribuirea e prin adresa nu copiere
    NPC& operator=(NPC& npc);

    // verifica numele intre ele
    bool operator==(const NPC& npc);
    bool operator!=(const NPC& npc);

    //<, > in functie de importanta -> dedusa din perm list
    bool operator<(const NPC& npc);
    bool operator>(const NPC& npc);

    //change position of npc
    NPC& operator+(const Vector2D& vector);
    NPC& operator-(const Vector2D& vector);

    //go to next animatinon frame
    NPC& operator++(int dummy);

    // returneaza dialog line la anumitul index
    std::string& operator[](int index);

    friend std::ostream& operator<<(std::ostream& os, const NPC& npc);
    friend std::istream& operator>>(std::istream& is, NPC& npc);
};
#endif //NPC_H
