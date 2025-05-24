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
Player::Player() : ImageLabel("Player", "rsc/Pinky.png") {
    AnimInfo info;
    info.duration = 0.7;
    info.loop = true;
    info.LoadAnimation("rsc/Pinky_Idle_Anim.png");
    anim_handler.AddAnimation("idle", info);
    info.LoadAnimation("rsc/Pinky_WalkLeft_Anim.png");
    anim_handler.AddAnimation("walkleft", info);
    info.LoadAnimation("rsc/Pinky_WalkRight_Anim.png");
    anim_handler.AddAnimation("walkright", info);
    info.LoadAnimation("rsc/Pinky_WalkFront_Anim.png");
    anim_handler.AddAnimation("walkup", info);
    info.LoadAnimation("rsc/Pinky_WalkBack_Anim.png");
    anim_handler.AddAnimation("walkdown", info);

    for (int i = 0; i < 5; i++)
        state_handler[i] = 0;
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

}