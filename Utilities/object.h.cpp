#include <iostream>
#include <algorithm>
#include <exception>
#include <vector>
#include <string>
#include <queue>
#include <raylib.h>
#include "Vector2D.h"
#include "object.h"
using namespace std;

int Object::ObjectCounter = 0;

Object::Object() : ID(ObjectCounter++){
    name = "object" + to_string(ObjectCounter);
    position = Vector2D(0, 0); size = Vector2D(0, 0);
    parent = nullptr;
    children = vector<Object*>();
    attributes = unordered_map<std::string, float>();
}

Object::Object(const char *name, Vector2D position, Vector2D size) : ID(ObjectCounter++) {
    this->name = string(name);
    this->position = position; this->size = size;
    parent = nullptr;
    children = vector<Object*>();
    attributes = unordered_map<std::string, float>();
}

Object::Object(const std::string& name, Vector2D position, Vector2D size) : ID(ObjectCounter++) {
    this->name = name;
    this->position = position; this->size = size;
    parent = nullptr;
    children = vector<Object*>();
    attributes = unordered_map<std::string, float>();
}

Object::Object(const Object& other) : ID(ObjectCounter++) {
    this->name = other.name; this->isActive = other.isActive;
    this->position = other.position; this->size = other.size;
    this->zIndex = other.zIndex; this->setParent(other.parent);
    children = vector<Object*>();
    attributes = unordered_map<std::string, float>();
}

/// PENTRU MOTIVE DE ORGANIZAREA HEADERELOR
/// Destructorul object se afla in TweenService.cpp

Object::~Object() {
    if (active_tween != nullptr)
        active_tween->Stop();
    cout << "INFO: " << name << " Object destroyed" << endl;
}
void Object::_set_active_tween(Tween* tween) {
    if (active_tween != nullptr)
        active_tween->Stop();
    active_tween = tween;
}

Object Object::workspace("workspace", Vector2D(0, 0), Vector2D(GetScreenWidth(), GetScreenHeight()));
Object Object::nullObject("null");

bool Object::getActive() const { return isActive; }
int Object::getZIndex() const { return zIndex; }

Vector2D& Object::getPosition() { return position; }
Vector2D& Object::getSize() { return size; }
string Object::getName() const { return name; }
int Object::getID() const { return ID; }
Object* Object::getChild(int index) {
    if (index >= children.size()) {
        cout << "WARNING: " << name << " child index out of bounds returning nullObject." << endl;
        return nullptr;
    }
    return children[index];
}

inline void Object::setPosition(Vector2D position) {
    Vector2D temp = position - this->position;
    this->position = position;
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == nullptr) continue;
        children[i]->setPosition(temp + children[i]->getPosition());
    }
}
Object* Object::Parent() const { return parent; }
vector<Object*>& Object::Children() { return children; }

float &Object::Height() { return size[1]; }
float &Object::Width() { return size[0]; }
float &Object::X() { return position[0]; }
float &Object::Y() { return position[1]; }

inline void Object::setPosition(float x, float y) { this->setPosition(Vector2D(x, y)); }
inline void Object::setSize(Vector2D size) { this->size = {size[0], size[1]}; }
inline void Object::setSize(float width, float height) { this->setSize({width, height}); }
void Object::setName(string name) { this->name = name; }
void Object::setName(const char *name) { this->name = name; }
void Object::addChild(Object* child) { children.push_back(child); }
void Object::addChild(Object& child) { children.push_back(&child); }
void Object::setZIndex(int zIndex) { this->zIndex = zIndex; }
void Object::setActive(bool isActive) { this->isActive = isActive; }

void Object::setParent(Object &parent) { this->setParent(&parent); }
void Object::setParent(Object *parent) {
    if (this->parent != nullptr)
        this->parent->RemoveChild(this);

    this->parent = parent;

    if (this->parent != nullptr)
        this->parent->addChild(this);
}

bool Object::operator==(const Object& other) const { return &(*this) == &other; }
bool Object::operator!=(const Object& other) const { return &(*this) != &other; }
bool Object::operator==(int other) const {
    if (other == 0.0f) return *this == nullObject;
    return *this != nullObject;
}
bool Object::operator!=(int other) const { return !(*this == other); }
bool Object::operator==(nullptr_t nr) const { return *this == nullObject; }
bool Object::operator!=(nullptr_t nr) const { return *this != nullObject; }

bool operator==(int nr, const Object& other) { return other == nr; }
bool operator!=(int nr, const Object& other) { return other != nr; }
bool operator==(nullptr_t nr, const Object& other) { return other == nr; }
bool operator!=(nullptr_t nr, const Object& other) { return other != nr; }

Object *Object::FindFirstChild(const char *name) {
    string to_search = string(name);
    for (int i = 0; i < children.size(); i++) {
        if (to_search != children[i]->getName()) continue;
        return children[i];
    }
    return nullptr;
}

Object* Object::FindFirstChild(string name) {
    for (int i = 0; i < children.size(); i++) {
        if (name != children[i]->getName()) continue;
        return children[i];
    }
    return nullptr;
}

Object* Object::FindFirstChild(int id) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->getID() != id) continue;
        return children[i];
    }
    return nullptr;
}

Object* Object::FindFirstDescendant(string name) {
    queue<Object*> q;
    q.push(this);

    while (!q.empty()) {
        Object* const node = q.front();
        q.pop();

        for (int i = 0; i < node->children.size(); i++) {
            if (node->children[i] == nullptr) continue;
            if (node->children[i]->getName() == name) return node->children[i];

            q.push(node->children[i]);
        }
    }
    return nullptr;
}

Object* Object::FindFirstDescendant(const char* name) {
    string to_search = string(name);
    return FindFirstDescendant(to_search);
}

Object* Object::FindFirstDescendant(int id) {
    queue<Object*> q;
    q.push(this);

    while (!q.empty()) {
        Object *node = q.front();
        q.pop();

        for (int i = 0; i < node->children.size(); i++) {
            if (node->children[i] == nullptr) continue;
            if (node->children[i]->getID() == id) return node->children[i];
            q.push(node->children[i]);
        }
    }
    return nullptr;
}

void Object::RemoveChild(int index) {
    if (index >= children.size()) return;
    children.erase(children.begin() + index);
}

void Object::RemoveChild(Object* child) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            children.erase(children.begin() + i);
            i--;
        }
    }
}

void Object::RemoveChild(const Object& child) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == &child) {
            children.erase(children.begin() + i);
            i--;
        }
    }
}

void Object::ClearChildren() {
    for (int i = 0; i < children.size(); i++)
        if (children[i] != nullptr)
            children[i]->Destroy();
}


Object*& Object::operator[](int index) {
    if (index >= children.size()) {
        cout<< "WARNING: " << name << " child index out of bounds" << endl;
        throw 1;
    }
    return children[index];
}

ostream& operator<<(ostream& os, const Object& obj) {
    os << "Object: " << obj.name << endl;
    os << "Position: " << obj.position << " " << obj.size << endl;
    return os;
}

void Object::Destroy() {
    ClearChildren();
    if (parent != nullptr)
        parent->RemoveChild(this);
    delete this;
}

void Object::UpdateSecluded() {}

void Object::Update() {
    if (!isActive || stop_update) return;
    bool notOrdered = false;
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == nullptr) {
            this->RemoveChild(children[i]);
            i--;
            continue;
        }
        children[i]->Update();

        if (i != 0 && children[i - 1]->zIndex > children[i]->zIndex)
            notOrdered = true;
    }

    if (!notOrdered) return;

    sort(children.begin(), children.end(), [](Object* a, Object* b) {return a->zIndex < b->zIndex;});
}

void Object::DrawSecluded() {}
void Object::Draw() {
    if (!isActive) return;
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == nullptr) continue;
        children[i]->Draw();
    }
}

Object& Object::operator=(Object& other) { return other; }

unordered_map<string, float>& Object::getAttributes() { return attributes; }
void Object::setAttribute(const char *name, float value) { attributes[name] = value; }
void Object::setAttribute(std::string name, float value) { attributes[name] = value; }
float Object::getAttribute(const char *name) {
    if (attributes.find(name) == attributes.end()) {
        cout << "WARNING: " << name << " attribute not found [" << name << "]"<< endl;
        return 0;
    }
    return attributes[name];
}
float Object::getAttribute(std::string name) { return getAttribute(name.c_str()); }

bool Object::getStopUpdate() const { return stop_update; }
void Object::setStopUpdate(bool stop_update) {this->stop_update = stop_update;}

