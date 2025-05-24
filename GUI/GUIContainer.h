//
// Created by Leustean Stefan on 26.03.2025.
//

#ifndef GUICONTAINER_H
#define GUICONTAINER_H

#define DEFAULT_CONTAINER_BACKGROUND_COLOR BLANK

class GUIContainer : public Object {
  protected:
    Texture2D* backgroundTexture;
    Color backgroundColor = DEFAULT_CONTAINER_BACKGROUND_COLOR;
    Color colorTint = WHITE;
    // va contine toate GUI thingies, cum ar fi butoane si altele in container

  public:
    // trebuie sa creez si un container cu parametrii
    // probabil va avea detalii legate de animatiix
    GUIContainer(std::string name = "Container");
    GUIContainer(std::string name, Vector2D position, Vector2D size, Color backgroundColor = DEFAULT_CONTAINER_BACKGROUND_COLOR);
    GUIContainer(const GUIContainer& guiContainer);
    ~GUIContainer() override;

    Texture2D* getBackgroundTexture() const;
    Color getColorTint() const;
    Color getBackgroundColor() const;

    void setBackgroundTexture(Texture2D *backgroundTexture);
    void setBackgroundTexture(const std::string& filepath);
    void setColorTint(Color colorTint);
    void setBackgroundColor(Color backgroundColor);
    void setBackgroundColor(int a, int b, int c, int alph = 255);

    //void Destroy() override;
    // update-ul si draw-ul se da la toate GUIObjects;
    void UpdateSecluded() override;
    void Update() override;

    /* foarte important la Draw
      Prima oara dam draw la background dupa
      apelam draw ptr fiecare obiect in parte
    */
    void DrawSecluded() override;
    void Draw() override;
};

#endif //GUICONTAINER_H
