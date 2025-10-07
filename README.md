# C++ Game Engine
  This started as a game for a faculty project in an Object Oriented Course in c++, but ended up becoming much larger. Although the game was never finished I managed to implement my own mini game engine. I'll talk about some features and things I would've done differently or things I didn't have time to do.

## Specifications
  - This project was made on a Mac with a M1 processor
  - It was made in CLion
  - It uitlizez the raylib library for the graphic interface

## The Base of the Project
  Each texture, image or thing you will see on the screen will be part of the Object class.
  <details>
    <summary> class Object; </summary>
    ```
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

    <details> 
      <summary> Object Method declarations.
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
    </details>
};
    ```
  </details>
