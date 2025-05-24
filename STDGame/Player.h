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
    float speed = 600; //pixels per second
protected:
    PlayerState state = Idle;

    public:
        Player();
        ~Player();

        void UpdateSecluded() override;
        void DrawSecluded() override;
        void Update() override;
        void Draw() override;
};



#endif //PLAYER_H
