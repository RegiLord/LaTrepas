//
// Created by Leustean Stefan on 15.05.2025.
//

#ifndef PLAYER_H
#define PLAYER_H

enum PlayerState {
    Idle, Walk_Left, Walk_Right, Walk_Up, Walk_Down
};

class Player : public ImageLabel {
private:
    float *state_handler = new float[5];
    bool entering = true;
    PlayerState state = Idle;
    //speed is pixels per second

    Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
public:
    static Player* player;
    static Player* CreatePlayer();
    ~Player();
    void UpdateSecluded() override;
    void DrawSecluded() override;
    void Update() override;
    void Draw() override;
};



#endif //PLAYER_H
