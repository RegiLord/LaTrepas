//
// Created by Leustean Stefan on 19.05.2025.
//

#ifndef ANIMATIONHANDLER_H
#define ANIMATIONHANDLER_H

class AnimationHandlerException : public std::exception {
    public:
        void what() {
            std::cout << "AnimationHandler doesn't have animation" << std::endl;
        }
};

class AnimationHandler;

struct AnimInfo {
  private:
    double currentTime = 0;
    bool isPlaying = false;
    Texture2D* animation = nullptr;

    friend AnimationHandler;
  public:
    double duration = 1;
    bool loop = false;
    std::vector<void(*)(Object*)> animation_end_callbacks;

    void LoadAnimation(const std::string &filepath);
    void ClearCallbacks();

    AnimInfo() = default;
    AnimInfo(const AnimInfo& info);
    ~AnimInfo();

    AnimInfo& operator=(const AnimInfo& info) {
        TextureHandler::ReleaseTexture(this->animation);
        TextureHandler::CopyTexture(this->animation, info.animation);
        this->isPlaying = false;
        this->currentTime = 0;
        this->loop = info.loop;
        this->duration = info.duration;
        this->animation_end_callbacks.clear();
        for (int i = 0; i < info.animation_end_callbacks.size(); i++)
            animation_end_callbacks.push_back(info.animation_end_callbacks[i]);

        return *this;
    }
};



class AnimationHandler {
    private:
        Object* owner;
        Texture2D* base_texture;
        std::unordered_map<std::string, AnimInfo> animations;
    public:
        AnimationHandler(Object* owner, Texture2D* base_texture);
        AnimationHandler(Object* owner, const std::string &filepath);
        AnimationHandler(const AnimationHandler &other);
        ~AnimationHandler();

        void AddAnimation(const std::string &name, const AnimInfo &anim);
        void PlayAnimation(const std::string &name);
        void StopAnimation(const std::string &name, bool callback = false);
        void RemoveAnimation(const std::string &name);
        void AddAnimationEndCallback(const std::string &name, void (*callback)(Object*));
        void StopAllAnimations(bool callback = false);
        void RemoveAllAnimations();

        Object* GetOwner();
        void SetOwner(Object* owner);
        void SetBaseTexture(const std::string& filepath);
        void SetBaseTexture(Texture2D* texture);

        Texture2D* GetPlayingAnimationTexture();
        AnimInfo& GetAnimationInfo(const std::string &name);

        void Update();
        void Draw(Vector2D position, Vector2D size, Color colorTint);
};



#endif //ANIMATIONHANDLER_H
