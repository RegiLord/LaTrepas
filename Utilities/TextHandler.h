//
// Created by Leustean Stefan on 06.04.2025.
//

#ifndef TEXTHANDLER_H
#define TEXTHANDLER_H

enum Alignment {
    Centered, Left, Right, NoAlignment
};

inline namespace TextHandler {
    Vector2D GetTextBoxSize(const std::string &Text, Font* font, float fontsize = 20, float spacing = 2);
    void DefFontFormatString(std::string &Text, Vector2D Box, Font* font, int fontsize = 20, int spacing = 2);
    void AlignCenterText(Rectangle Box, Vector2D& TextBoxPosition, Vector2D TextBoxSize);
    void AlignRightText(Rectangle Box, Vector2D& TextBoxPosition, Vector2D TextBoxSize);
    void AlignLeftText(Rectangle Box, Vector2D& TextBoxPosition, Vector2D TextBoxSize);
}
#endif //TEXTHANDLER_H
