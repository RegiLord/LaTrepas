//
// Created by Leustean Stefan on 21.03.2025.
//

#ifndef OBJECT_H
#define OBJECT_H

/* PLANUL ESTE ca fiecare lucru din jocul acesta sa fie un obiect
 *  Ca sa avem o organizare de stil ierarhic astfel incat sa putem accesa usor elementele dorite
 *  o sa existe un workspace care va fi capul la arborele nostru
 *  in fiecare runda vom da update la workspace si draw la workspace
 *  daca ceva este in workspace va fi loaduita daca nu, atunci nu va fi
 *
 *  Facem workspace-ul static pentru a intelege importanta ei
 */
#include <unordered_map>

class Tween;
class TweenService;
class Object;

class Object {
protected:
    std::string name;
    Vector2D position;
    Vector2D size;
    bool isActive = true;
    bool stop_update = false;

    bool to_destroy = false;
    Tween* active_tween = nullptr;

    int zIndex = 0;
    const int ID; // unique id for objects
    Object* parent;
    std::vector<Object*> children;
    std::unordered_map<std::string, float> attributes;
public:
    static Object workspace;
    static Object nullObject;
    static int ObjectCounter;


    Object();
    Object(const std::string& name, Vector2D position = Vector2D(0, 0), Vector2D size = Vector2D(0, 0));
    Object(const char* name, Vector2D position = Vector2D(0, 0), Vector2D size = Vector2D(0, 0));
    Object(const Object& other);
    virtual ~Object();

    //Attribute functions on unordered_map
    std::unordered_map<std::string, float>& getAttributes();
    void setAttribute(const char* name, float value);
    void setAttribute(std::string name, float value);
    // Throws error and returns 0 on no attribute found
    float getAttribute(const char* name);
    float getAttribute(std::string name);

    void _destroy();
    void _set_to_destroy_(bool to_destroy);
    void _set_active_tween(Tween* tween);


    //Quick Access functions
    float& Width();
    float& Height();
    float& X();
    float& Y();

    bool getStopUpdate() const;
    Vector2D& getPosition();
    Vector2D& getSize();
    std::string getName() const;
    int getID() const;
    Object* getChild(int index);
    Object* Parent() const;
    std::vector<Object*>& Children();
    bool getActive() const;
    int getZIndex() const;

    void setActive(bool isActive);
    virtual void setPosition(Vector2D position);
    virtual void setPosition(float x, float y);
    virtual void setSize(Vector2D size);
    virtual void setSize(float x, float y);
    void setName(std::string name);
    void setName(const char* name);
    void setParent(Object& parent);
    void setParent(Object* parent);
    void addChild(Object& child);
    void addChild(Object* child);
    void setZIndex(int zIndex);
    void setStopUpdate(bool stop_update);

    // Cauta fii directi
    Object* FindFirstChild(std::string name);
    Object* FindFirstChild(const char* name);
    Object* FindFirstChild(int id);
    // Caut in tot subarborele obiectului
    // TIP DE CAUTARE !!BFS (vrem sa gasim la un nivel cat mai mic)
    Object* FindFirstDescendant(std::string name);
    Object* FindFirstDescendant(const char* name);
    Object* FindFirstDescendant(int id);
    void RemoveChild(int index);
    void RemoveChild(Object* child);
    void RemoveChild(const Object& child);
    void ClearChildren(); // this destroys the children

    bool operator==(const Object &) const;
    bool operator==(int nr) const;
    bool operator==(std::nullptr_t nr) const;
    friend bool operator==(int nr, const Object &obj);
    friend bool operator==(std::nullptr_t nr, const Object &obj);
    bool operator!=(const Object &) const;
    bool operator!=(int nr) const;
    bool operator!=(std::nullptr_t nr) const;
    friend bool operator!=(int nr, const Object &obj);
    friend bool operator!=(std::nullptr_t nr, const Object &obj);

    Object& operator=(Object& other);
    Object*& operator[](int index);
    friend std::ostream& operator<<(std::ostream& os, const Object& obj);

    void Destroy();
    virtual void UpdateSecluded();
    virtual void Update();
    virtual void DrawSecluded(); // desenam doar acel obiect fara intrgul arbore
    virtual void Draw();
};

class No_Points : public std::exception {
public:
    void what() {
        std::cout << "Creating Tween without points" << std::endl;
    }
};

struct TweenInfo {
    Object* object;
    double duration;
    std::vector<Vector2D> points;
};
class Tween {
private:
    Object *object;
    double current_time, duration;
    float distanta_totala, distanta_parcursa;
    bool paused, playing;
    std::vector<Vector2D> points;

    Vector2D GetPositionFromDistance() const;
    float CalculateTotalDistance();
    void Update();
public:
    Tween() = default;
    Tween(const TweenInfo& info);
    Tween(Object* obj, const std::vector<Vector2D> &points, double duration);
    friend TweenService;

    void Play();
    void Pause();
    void Resume();
    void Stop();
};

class TweenService : public Object {
private:
    std::vector<Tween*> tweens;

public:
    TweenService();

    static std::vector<TweenService*> Services;
    static void CreateTweenService(const std::string &name, Object* parent);
    static TweenService* GetTweenService(const std::string &name);

    Tween* CreateTween(const TweenInfo& info);
    Tween* CreateTween(Object* object, Vector2D point, double duration);
    Tween* CreateTween(Object* object, const std::vector<Vector2D> &points, double duration);
    void ClearTweens();

    void Update() override;
    void UpdateSecluded() override;
    void Draw() override;
    void DrawSecluded() override;
};

#endif //OBJECT_H
