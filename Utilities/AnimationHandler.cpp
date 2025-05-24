#include <iostream>
#include <vector>
#include <string>
#include "Vector2D.h"
#include <raylib.h>
#include "Defines.h"
#include "object.h"
#include "Game.h"
#include "TextureHandler.h"
#include "AnimationHandler.h"

#include "Game.h"

using namespace std;

void AnimInfo::LoadAnimation(const std::string &filepath) {
    TextureHandler::ReleaseTexture(animation);
    animation = TextureHandler::AddTexture(filepath);
}

AnimInfo::~AnimInfo() {
    TextureHandler::ReleaseTexture(animation);
}

void AnimInfo::ClearCallbacks() {
    animation_end_callbacks.clear();
}

AnimInfo::AnimInfo(const AnimInfo& info) {
    TextureHandler::ReleaseTexture(animation);
    TextureHandler::CopyTexture(animation, info.animation);

    currentTime = 0;
    isPlaying = false;
    duration = info.duration;
    duration = info.loop;

    animation_end_callbacks.clear();
    for (int i = 0; i < info.animation_end_callbacks.size(); i++) {
        animation_end_callbacks[i] = info.animation_end_callbacks[i];
    }
}

AnimationHandler::AnimationHandler(Object* owner, Texture2D* base_texture) {
    this->owner = owner;
    TextureHandler::CopyTexture(this->base_texture, base_texture);
}
AnimationHandler::AnimationHandler(Object* owner, const string &filepath) {
    this->owner = owner;
    base_texture = TextureHandler::AddTexture(filepath);
}
AnimationHandler::AnimationHandler(const AnimationHandler &other) {
    TextureHandler::ReleaseTexture(this->base_texture);
    RemoveAllAnimations();
    TextureHandler::CopyTexture(this->base_texture, other.base_texture);

    for (auto &i : other.animations) {
        AddAnimation(i.first, i.second);
    }
}

AnimationHandler::~AnimationHandler() {
    TextureHandler::ReleaseTexture(this->base_texture);
    RemoveAllAnimations();
}

void AnimationHandler::AddAnimation(const string &key, const AnimInfo &info) {
    animations[key] = info;
    animations[key].currentTime = 0;
}

void AnimationHandler::PlayAnimation(const string &name) {
    if (!animations.contains(name)) {
        throw AnimationHandlerException();
    }

    animations[name].isPlaying = true;
    animations[name].currentTime = 0;
}

void AnimationHandler::StopAnimation(const string &name, bool callback) {
    if (!animations.contains(name)) {
        cout << "WARNING: Animation " << name << " has not been added to handler" << endl;
        return;
    }

    if (animations[name].isPlaying)
        if (callback)
            for (int i = 0; i < animations[name].animation_end_callbacks.size(); i++)
                animations[name].animation_end_callbacks[i](owner);
    animations[name].isPlaying = false;
}

void AnimationHandler::StopAllAnimations(bool callback) {
    for (auto &i : animations) {
        if (i.second.isPlaying && callback)
            for (int j = 0; j < i.second.animation_end_callbacks.size(); j++)
                i.second.animation_end_callbacks[j](owner);
        i.second.isPlaying = false;
    }
}

void AnimationHandler::AddAnimationEndCallback(const std::string &name, void (*callback)(Object *)) {
    if (!animations.contains(name)) {
        cout << "WARNING: Animation " << name << " has not been added to handler" << endl;
        return;
    }

    animations[name].animation_end_callbacks.push_back(callback);
}

void AnimationHandler::RemoveAnimation(const std::string &name) {
    if (!animations.contains(name)) {
        cout << "WARNING: removing animation from handler that doesn't exist" << endl;
        return;
    }
    animations.erase(name);
}

void AnimationHandler::RemoveAllAnimations() {
    animations.clear();
}

Texture2D* AnimationHandler::GetPlayingAnimationTexture() {
    for (auto &i : animations) {
        if (i.second.isPlaying) {
            return i.second.animation;
        }
    }
    return nullptr;
}

AnimInfo&  AnimationHandler::GetAnimationInfo(const std::string &name) {
    if (!animations.contains(name)) {
        throw AnimationHandlerException();
    }
    return animations[name];
}

Object* AnimationHandler::GetOwner() { return owner; }
void AnimationHandler::SetOwner(Object *owner) { this->owner = owner; }

void AnimationHandler::Update() {
    for (auto &i : animations) {
        if (!i.second.isPlaying) continue;

        i.second.currentTime+= GetFrameTime();
        if (i.second.currentTime >= i.second.duration) {
            i.second.currentTime = 0;
            if (!i.second.loop) {
                i.second.isPlaying = false;
                for (int j = 0; j < i.second.animation_end_callbacks.size(); j++)
                    i.second.animation_end_callbacks[j](owner);
            }
        }
    }
}

void AnimationHandler::Draw(Vector2D position, Vector2D size, Color colorTint) {
    float scale = Game::Main->GetScaleFactor();
    for (auto &i : animations) {
        if (!i.second.isPlaying) continue;

        Rectangle dest = {position[0], position[1], size[0], size[1]};
        dest.x *= scale; dest.y *= scale; dest.width *= scale; dest.height *= scale;
        Rectangle source;
        source.y = 0;
        try {
            source.height = base_texture->height;
            source.width = base_texture->width;
            int anim_frame = i.second.currentTime / (i.second.duration / (i.second.animation->width / base_texture->width));
            source.x = anim_frame * source.width;
        } catch (...) {
            cout << "ERROR: base_texture not initialized";
            throw AnimationHandlerException();
        }
        DrawTexturePro(*i.second.animation, source, dest, {0,0}, 0, colorTint);

        break;
    }
}

void AnimationHandler::SetBaseTexture(Texture2D* texture) {
    RemoveAllAnimations();
    TextureHandler::CopyTexture(this->base_texture, texture);
}
void AnimationHandler::SetBaseTexture(const std::string& filePath) {
    RemoveAllAnimations();
    TextureHandler::ReleaseTexture(this->base_texture);
    this->base_texture = TextureHandler::AddTexture(filePath);
}






