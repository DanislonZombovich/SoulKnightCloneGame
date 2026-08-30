#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Quiz {
public:
    Quiz();
    void draw(sf::RenderWindow& win);
    void handleInput(sf::Keyboard::Key key);
    bool isFinished() const;
    int getRemaining() const { return questionsLeft; }
    void reset();
private:
    struct Question {
        std::string text;
        std::vector<std::string> options;
        int correct;
    };
    std::vector<Question> questions;
    int currentQuestion = 0;
    int questionsLeft = 5;
    int selectedOption = 0;
    bool finished = false;
    sf::Font font;
    sf::Text questionText;
    std::vector<sf::Text> optionTexts;
    void loadQuestions();
    void nextQuestion();
};