//
// Created by Leustean Stefan on 22.05.2025.
//
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <raylib.h>
#include <raymath.h>
#include "Defines.h"
#include "Vector2D.h"
#include "object.h"

using namespace std;

/*
 * DECLARATIILE TWEEN SI TWEENSERVICE SE AFLA IN OBJECT.H
 *
 */

Tween::Tween(const TweenInfo& info) : Tween(info.object, info.points, info.duration) {}

Tween::Tween(Object *obj, const std::vector<Vector2D> &points, double duration) {
    this->object = obj;
    this->object->_set_active_tween(this);
    this->points.push_back(obj->getPosition());
    this->points.append_range(points);
    if (this->points.size() == 0)
        throw No_Points();
    this->duration = duration;
    current_time = 0; distanta_parcursa = 0;
    paused = playing = false;
    distanta_totala = CalculateTotalDistance();
}

float Tween::CalculateTotalDistance() {
    float dist = 0;
    for (int i = 1; i < points.size(); i++) {
        dist += Vector2Distance(points[i - 1], points[i]);
    }
    return dist;
}

void Tween::Update() {
    if (!playing || paused) { return; };
    if (object == nullptr) { this->Stop(); return; };

    current_time += GetFrameTime();
    distanta_parcursa = (distanta_totala / duration) * current_time;
    object->setPosition(GetPositionFromDistance());
}
void Tween::Play() { this->playing = true; }
void Tween::Pause() { this->paused = true; }
void Tween::Resume() { this->paused = false; }
void Tween::Stop() {
    this->playing = false;
    this->distanta_parcursa = distanta_totala;
}


Vector2D Tween::GetPositionFromDistance() const {
    if (distanta_parcursa >= distanta_totala) {
        return points.back();
    }

    float copy_distance = distanta_parcursa;
    int i;
    for (i = 1; i < points.size(); i++) {
        float pt_dist = Vector2Distance(points[i - 1], points[i]);
        if (copy_distance < pt_dist)
            break;
        copy_distance -= pt_dist;
    }

    if (i == points.size())
        return points.back();

    Vector2 ans = Vector2MoveTowards(points[i - 1], points[i], copy_distance);
    return {ans.x, ans.y};
}

TweenService::TweenService() {
    zIndex = -1000;
}


vector<TweenService*> TweenService::Services;

void TweenService::CreateTweenService(const std::string &name, Object *parent) {
    TweenService* newService = new TweenService();
    newService->setName(name);
    newService->setParent(parent);
    Services.push_back(newService);
}

TweenService* TweenService::GetTweenService(const std::string &name) {
    for (auto &i : Services)
        if (i->name == name)
            return i;
    return nullptr;
}

Tween *TweenService::CreateTween(const TweenInfo &info) {
    return CreateTween(info.object, info.points, info.duration);
}

Tween* TweenService::CreateTween(Object* object, Vector2D point, double duration) {
    return CreateTween(object, std::vector<Vector2D>({point}), duration);
}

Tween* TweenService::CreateTween(Object *object, const std::vector<Vector2D> &points, double duration) {
    Tween *temp = new Tween(object, points, duration);
    tweens.push_back(temp);
    return temp;
}


void TweenService::Draw() {}
void TweenService::DrawSecluded() {}

void TweenService::UpdateSecluded() {
    for (int i = 0; i < tweens.size(); i++) {
        tweens[i]->Update();
        if (tweens[i]->distanta_parcursa >= tweens[i]->distanta_totala) {
            tweens.erase(tweens.begin() + i);
            i--;
        }
    }
}

void TweenService::Update() {
    if (!isActive || stop_update) return;
    UpdateSecluded();
}

void TweenService::ClearTweens() {
    for (int i = 0; i < tweens.size(); i++) {
        if (tweens[i] != nullptr)
            delete tweens[i];
    }
    tweens.clear();
}