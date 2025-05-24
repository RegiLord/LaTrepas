//
// Created by Leustean Stefan on 05.05.2025.
//
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include <Utilities.h>
#include <GUI.h>
#include "QuickTime.h"

using namespace std;

QuickTime::~QuickTime() {
    if (text != nullptr)
        text->Destroy();
}

QuickTime::QuickTime(const std::string& name, const std::string& filePath, double time, Vector2D position, Vector2D size, Color colorTint)
    : ImageLabel(name, filePath, position, size)
{
    this->colorTint = colorTint;
    this->time = time;
    this->letter = 'A';
    this->text = new ::TextLabel("", string() + letter);
    this->text->setFontColor(BLACK);
}

QuickTime::QuickTime(const QuickTime &orig)
    : ImageLabel(orig)
{
    this->current_time = 0;
    this->time = orig.time;
    this->letter = orig.letter;
    this->colorTint = orig.colorTint;
    if (text != nullptr)
        text->Destroy();
    text = new ::TextLabel(*orig.text);
}

double QuickTime::getTime() const { return time; }
double QuickTime::getCurrentTime() const { return current_time; }
char QuickTime::getLetter() const { return letter; }
TextLabel& QuickTime::TextLabel() { return *text; }
Alignment QuickTime::getAlignment() const { return alignment; }

void QuickTime::setAlignment(Alignment alignment) { this->alignment = alignment; }
void QuickTime::setTime(double time) { this->time = time; }
void QuickTime::setCurrentTime(double time) { this->current_time = time; }
void QuickTime::setLetter(char letter) {
    this->letter = letter;
    this->text->setText(string() + letter);
}

void QuickTime::Update() {
    if (!isActive || stop_update) return;
    UpdateSecluded();
    Object::Update();
}

void QuickTime::Draw() {
    if (!isActive) return;
    DrawSecluded();
    Object::Draw();
}

void QuickTime::UpdateSecluded() {
    ImageLabel::UpdateSecluded();

    if (text == nullptr) return;
    if (!text->getActive()) return;
    text->Update();
    text->setFontSize(min(size[0], size[1]) / 2);
    Rectangle quickRec(position[0], position[1], size[0], size[1]);
    Vector2D pos = text->getPosition();
    if (text != nullptr)
        switch (alignment) {
            case Centered: {
                AlignCenterText(quickRec, pos, text->getSize());
                break;
            }
            case Left: {
                AlignLeftText(quickRec, pos, text->getSize());
                break;
            }
            case Right: {
                AlignRightText(quickRec, pos, text->getSize());
                break;
            }
            default: break;
        }
    text->setPosition(pos);

    int keycode = int(letter);
    if (letter >= 'a' && letter <= 'z')
        keycode -= 'a' - 'A';

    if (IsKeyPressed(keycode)) {
        try {
            if (text != nullptr)
                text->setActive(false);
            if (active_tween != nullptr)
                this->_set_active_tween(nullptr);
            anim_handler.PlayAnimation("Pop");
            anim_handler.AddAnimationEndCallback("Pop", [](Object* obj) {
                obj->Destroy();
            });
        } catch (AnimationHandlerException e) {
            this->Destroy();
        }
        return;
    }

    current_time+= GetFrameTime();
    if (current_time >= time) {
        try {
            if (text != nullptr)
                text->setActive(false);
            anim_handler.PlayAnimation("Attack");
            anim_handler.AddAnimationEndCallback("Attack", [](Object* obj) {
                obj->Destroy();
            });
        } catch (AnimationHandlerException e) {
            this->Destroy();
        }
    }

}

void QuickTime::DrawSecluded() {
    if (imageTexture != nullptr) {
        backgroundColor = BLANK;
        ImageLabel::DrawSecluded();
    }
    else
        GUIObject::DrawSecluded();



    if (text != nullptr && text->getActive()) {
        Vector2D center = position + size / 2;
        float radius = min(size[0], size[1]) / 4;
        float thickness = min(size[0], size[1]) / 20;
        float endangle = (current_time == 0) ? 360 : 360 - (360 / time) * current_time;

        DrawRing(center, radius, radius + thickness, 0, endangle, 0,
            ColorAlpha(text->getFontColor(), QUICKTIME_AlPHA_VALUE));

        Color save = text->getFontColor();
        text->setFontColor(ColorAlpha(save, QUICKTIME_AlPHA_VALUE));
        text->Draw();
        text->setFontColor(save);
    }
}


