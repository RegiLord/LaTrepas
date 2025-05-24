//
// Created by Leustean Stefan on 05.05.2025.
//

#ifndef QUICKTIME_H
#define QUICKTIME_H

#define QUICKTIME_AlPHA_VALUE 0.6

class QuickTime : public ImageLabel {
    protected:
        double current_time = 0;
        double time;
        char letter;
        TextLabel* text = nullptr;
        Alignment alignment = Centered;
    public:
        QuickTime(const std::string& name, const std::string& filePath, double time = 3,
            Vector2D position = {0, 0}, Vector2D size = {64, 64}, Color colorTint = WHITE);
        QuickTime(const QuickTime& orig);
        ~QuickTime();

        QuickTime& operator=(const QuickTime& orig);

        double getTime() const;
        double getCurrentTime() const;
        char getLetter() const;
        TextLabel& TextLabel();
        Alignment getAlignment() const;

        void setTime(double time);
        void setCurrentTime(double time);
        void setLetter(char letter);
        void setAlignment(Alignment alignment);

        void UpdateSecluded() override;
        void DrawSecluded() override;
        void Update() override;
        void Draw() override;
};



#endif //QUICKTIME_H
