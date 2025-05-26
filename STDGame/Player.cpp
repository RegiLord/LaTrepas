//
// Created by Leustean Stefan on 15.05.2025.
//
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include <Utilities.h>
#include <GUI.h>
#include "Player.h"

using namespace std;

Player* Player::player;
Player::Player() : ImageLabel("Player", "rsc/Pinky.png") {
}

Player* Player::CreatePlayer() {
    Player* player = new Player();
    AnimInfo info;
    info.duration = 0.7;
    info.loop = true;
    info.LoadAnimation("rsc/Pinky_Idle_Anim.png");
    player->anim_handler.AddAnimation("idle", info);
    info.LoadAnimation("rsc/Pinky_WalkLeft_Anim.png");
    player->anim_handler.AddAnimation("walkleft", info);
    info.LoadAnimation("rsc/Pinky_WalkRight_Anim.png");
    player->anim_handler.AddAnimation("walkright", info);
    info.LoadAnimation("rsc/Pinky_WalkFront_Anim.png");
    player->anim_handler.AddAnimation("walkup", info);
    info.LoadAnimation("rsc/Pinky_WalkBack_Anim.png");
    player->anim_handler.AddAnimation("walkdown", info);

    for (int i = 0; i < 5; i++)
        player->state_handler[i] = 0;
    player->setAttribute("Health", 5);
    player->setAttribute("Speed", 60);
    return player;
}


Player::~Player() {
    if (state_handler != nullptr)
        delete [] state_handler;
}


void Player::Draw() {
    if (!isActive) return;
    DrawSecluded();
    Object::Draw();
}

void Player::Update() {
    if (!isActive || stop_update) return;
    UpdateSecluded();
    Object::Update();
}

void Player::DrawSecluded() {
    if (imageTexture != nullptr) {
        backgroundColor = BLANK;
        ImageLabel::DrawSecluded();
    }
    else
        GUIObject::DrawSecluded();
}

void Player::UpdateSecluded() {
    ImageLabel::UpdateSecluded();
    float speed = this->getAttribute("Speed");

    switch (state) {
        case Idle: {
            if (entering) {
                entering = false;
                anim_handler.StopAllAnimations();
                anim_handler.PlayAnimation("idle");
            }
            break;
        }
        case Walk_Down: {
            if (entering) {
                entering = false;
                anim_handler.StopAllAnimations();
                anim_handler.PlayAnimation("walkdown");
            }
            this->setPosition(this->X(), this->Y() + speed / DEFAULT_FPS);
            break;
        }
        case Walk_Left: {
            if (entering) {
                entering = false;
                anim_handler.StopAllAnimations();
                anim_handler.PlayAnimation("walkleft");
            }
            this->setPosition(this->X() - speed / DEFAULT_FPS, this->Y());
            break;
        }
        case Walk_Right: {
            if (entering) {
                entering = false;
                anim_handler.StopAllAnimations();
                anim_handler.PlayAnimation("walkright");
            }
            this->setPosition(this->X() + speed / DEFAULT_FPS, this->Y());
            break;
        }
        case Walk_Up: {
            if (entering) {
                entering = false;
                anim_handler.StopAllAnimations();
                anim_handler.PlayAnimation("walkup");
            }
            this->setPosition(this->X(), this->Y() - speed / DEFAULT_FPS);
            break;
        }
    }

    // aici schimbam starile
    int key[5] = {0, KEY_A, KEY_D, KEY_W, KEY_S};
    for (int i = 1; i < 5; i++) {
        if (IsKeyPressed(key[i])) {
            if (state_handler[i] == 0) {
                state = PlayerState(i);
                entering = true;
                state_handler[i] = GetTime();
            }
        }
        else if (IsKeyReleased(key[i])) {
            state_handler[i] = 0;
            if (state == PlayerState(i)) {
                entering = true;

                int min = 0;
                for (int i = 1; i < 5; i++) {
                    if (state_handler[i] > state_handler[min])
                        min = i;
                }
                state = PlayerState(min);
            }
        }
    }

    if (this->X() < 0)
        this->setPosition(0, this->Y());
    if (this->X() + this->Width() > DEFAULT_RESOLUTION.first)
        this->setPosition(DEFAULT_RESOLUTION.first - this->Width(), this->Y());
    if (this->Y() < 0)
        this->setPosition(this->X(), 0);
    if (this->Y() + this->Height() > DEFAULT_RESOLUTION.second)
        this->setPosition(this->X(), DEFAULT_RESOLUTION.second - this->Height());

}