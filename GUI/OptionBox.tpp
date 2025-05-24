
template<class T>
OptionBox<T>::OptionBox(std::string name, std::vector<std::pair<std::string, T> > initvals, Vector2D position, Vector2D size, Color backgroundColor)
    : Button(name, position, size, backgroundColor) {
    roundness = 0;
    alignment = Right;
    this->setNormalColor(BLACK);
    this->TextLabel()->setFontColor(WHITE);

    for (int i = 0; i < initvals.size(); i++) {
        this->addOption(initvals[i].first, initvals[i].second);
    }
}



template<class T>
OptionBox<T>::OptionBox(const OptionBox &object)
    : Button(object) {
    this->currentValue = object.currentValue;
    this->values = object.values;
    this->toReconfigure = true;
    this->showOptions = false;

    this->options = std::vector<Button*>();
    for (int i = 0; i < object.options.size(); i++) {
        this->options.push_back(new Button(object.options[i]));
    }
}

template<class T>
OptionBox<T>::~OptionBox() {
    for (int i = 0; i < options.size(); i++)
        if (options[i] != nullptr) {
            delete options[i]; options[i] = nullptr;
        }
}

template<class T>
void OptionBox<T>::addOption(std::string name, T value) {
    if (options.size() == 0) {
        this->currentValue = {name, value};
        this->TextLabel()->setText(name);
    }
    Button* newButton = new Button(*this);
    newButton->setParent(nullptr);
    newButton->TextLabel()->setText(name);
    options.push_back(newButton);
    values.push_back(value);

    newButton->X() = this->X();
    newButton->Y() = this->Y() + this->Height() * options.size();

    toReconfigure = true;
}

template<class T>
bool OptionBox<T>::getShowOptions() const { return this->showOptions; }
template<class T>
void OptionBox<T>::setShowOptions(bool showOptions) { this->showOptions = showOptions; }

template<class T>
T OptionBox<T>::getValue() const { return this->currentValue.second; }

template<class T>
void OptionBox<T>::setCurrentValue(const std::string &name) {
    for (int i = 0; i < options.size(); i++) {
        if (options[i]->getName() == name) {
            this->currentValue = {name, values[i]};
            this->TextLabel()->setText(name);
            return;
        }
    }
    std::cout << "WARNING: Trying to set invalid value " << name << " not changing value" << std::endl;
}

template<class T>
void OptionBox<T>::setCurrentValue(T value) {
    try {
        for (int i = 0; i < values.size(); i++) {
            if (values[i] == value) {
                this->currentValue = {options[i]->getName(), values[i]};
                this->TextLabel()->setText(options[i]->TextLabel()->getText());
                return;
            }
        }
    }
    catch (...) {
        std::cout << "WARNING: template has no == operator";
    }
    std::cout << "WARNING: Trying to set invalid value" << std::endl;
}


template<class T>
OptionBox<T> &OptionBox<T>::operator=(OptionBox &optionbox) { return optionbox; }

template<class T>
void OptionBox<T>::Update() {
    if (!isActive || stop_update) return;
    UpdateSecluded();
    Object::Update();
}

template<class T>
void OptionBox<T>::Draw() {
    if (!isActive) return;
    DrawSecluded();
    Object::Draw();
}

template<class T>
void OptionBox<T>::DrawSecluded() {
    Button::DrawSecluded();

    if (showOptions)
        for (int i = 0; i < options.size(); i++) {
            options[i]->DrawSecluded();
        }
}

template<class T>
void OptionBox<T>::UpdateSecluded() {
    if (!showOptions) {
        Button::UpdateSecluded();
        framewait = -1;
    }
    if (this->buttonState == Pressed)
        showOptions = true;

    if (showOptions) {

        if (this->getSize() != options[0]->getSize())
            toReconfigure = true;
        if (this->TextLabel()->getFontSize() != options[0]->TextLabel()->getFontSize())
            toReconfigure = true;
        if (this->TextLabel()->getFontColor() != options[0]->TextLabel()->getFontColor())
            toReconfigure = true;
        if (options[0]->getPosition() != Vector2D(this->X(), this->Y() + this->Height()))
            toReconfigure = true;


        if (toReconfigure) {
            reconfigure();
            toReconfigure = false;
        }

        for (int i = 0; i < options.size(); i++) {
            options[i]->UpdateSecluded();
            if (options[i]->getButtonState() == Pressed) {
                this->setCurrentValue(values[i]);
                showOptions = false;
            }
        }

        if (framewait == FRAME_WAIT) {
            showOptions = false;
            framewait = -1;
        }
        if (framewait != -1)
            framewait++;
        if (framewait == -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            framewait = 0;
    }

    if (!showOptions)
        Button::UpdateSecluded();
}

template<class T>
void OptionBox<T>::reconfigure() {

    for (int i = 0; i < options.size(); i++) {
        options[i]->setPosition(Vector2D(this->X(), this->Y() + this->Height() * (i + 1)));
        options[i]->setSize(this->size);
        options[i]->TextLabel()->setFontSize(this->TextLabel()->getFontSize());
        options[i]->TextLabel()->setFontColor(this->TextLabel()->getFontColor());
        options[i]->setNormalColor(this->normalColor);
        options[i]->setHoverColor(this->onHoverColor);
        options[i]->setPressColor(this->onPressColor);
    }
}

template<class T>
std::vector<Button *> &OptionBox<T>::getOptions() {
    return options;
}
template<class T>
std::vector<T> &OptionBox<T>::getValues() {
    return values;
}









