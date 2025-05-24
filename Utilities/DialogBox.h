//
// Created by Leustean Stefan on 21.03.2025.
//

#ifndef DIALOGBOX_H
#define DIALOGBOX_H

class DialogBox {
  private:
    bool finished = false;
    int line;
    int column;
    int animFrames;
    int animFrame;
    Rectangle dest;
    Texture2D profilePic;
    std::string name;
    std::vector<std::string> dialogLines;


  public:
    static DialogBox* ActiveDialog;

    DialogBox();
    //cream un dialog box al, npc-ului, la linia
    DialogBox(std::string name, Texture2D profilePic, std::vector<std::string> dialogLines);
    DialogBox(std::string name, const char* profilePic, std::vector<std::string> dialogLines);
    DialogBox(const DialogBox& dialogBox);
    ~DialogBox();

    int getColumn() const;
    int getLine() const;
    void setLine(int line);
    void setColumn(int column);

    std::string getName() const;
    std::vector<std::string> getDialogLines() const;

    void setName(std::string name);
    void setProfilePic(Texture2D profilePic);
    void setProfilePic(const char* filename);

    void Draw();
    //un fel de copiere default
    DialogBox& operator=(const DialogBox& dialogBox);

    //verifica daca sunt pentru acelais pers
    bool operator==(const DialogBox& dialog);
    bool operator!=(const DialogBox& dialog);

    // compara nr de caractere de pe fiecare linie
    bool operator<(const DialogBox& dialog);
    bool operator>(const DialogBox& dialog);

    // adauga text-ul ca o ultima linie
    DialogBox& operator+(const char* text);
    DialogBox& operator+(std::string text);
    // concateneaza primele scalar linii si le trimite inapoi;
    std::string operator*(int scalar) const;

    // trece la urmt linie
    DialogBox& operator++(int dummy);
    // reda linia i
    std::string& operator[](int index);

    friend std::ostream& operator<<(std::ostream& os, const DialogBox& dialog);
    friend std::istream& operator>>(std::istream& is, DialogBox& dialog);
};

#endif //DIALOGBOX_H
