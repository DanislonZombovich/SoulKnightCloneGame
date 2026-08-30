#include "Quiz.h"

Quiz::Quiz() : questionText(font, "", 30) {
    if (!font.openFromFile("assets/fonts/arial.ttf")) {}
    questionText.setFont(font);
    questionText.setCharacterSize(30);
    questionText.setFillColor(sf::Color::White);
    questionText.setPosition({400, 200});
    for (int i=0; i<4; ++i) {
        optionTexts.emplace_back(font, "", 24);
        optionTexts[i].setFont(font);
        optionTexts[i].setCharacterSize(24);
        optionTexts[i].setFillColor(sf::Color::White);
        optionTexts[i].setPosition({450, 300 + i*50.f});
    }
    loadQuestions();
}

void Quiz::loadQuestions() {
    questions = {
        {"How many bullets in a pistol magazine?", {"10","20","30","40"}, 1},
        {"How many locations are there?", {"1","2","3","4"}, 1},
        {"What damage does the assault rifle deal?", {"20","30","34","40"}, 2},
        {"How to increase max HP?", {"Find potion","Touch the tree","Kill boss","Never"}, 1},
        {"What does the yellow beam do?", {"50 damage","Instant death","Stun","Nothing"}, 1}
    };
    currentQuestion = 0; selectedOption = 0; finished = false; questionsLeft = 5;
}

void Quiz::draw(sf::RenderWindow& win) {
    if (finished || questionsLeft <= 0) return;
    Question& q = questions[currentQuestion];
    questionText.setString(q.text);
    win.draw(questionText);
    for (int i=0; i<4; ++i) {
        optionTexts[i].setString(q.options[i]);
        optionTexts[i].setFillColor(i == selectedOption ? sf::Color::Yellow : sf::Color::White);
        win.draw(optionTexts[i]);
    }
}

void Quiz::handleInput(sf::Keyboard::Key key) {
    if (finished) return;
    if (key == sf::Keyboard::Key::W || key == sf::Keyboard::Key::Up)
        selectedOption = (selectedOption - 1 + 4) % 4;
    else if (key == sf::Keyboard::Key::S || key == sf::Keyboard::Key::Down)
        selectedOption = (selectedOption + 1) % 4;
    else if (key == sf::Keyboard::Key::Enter) {
        if (selectedOption == questions[currentQuestion].correct)
            nextQuestion();
        else
            loadQuestions();
    }
}

void Quiz::nextQuestion() {
    questionsLeft--;
    if (questionsLeft > 0) currentQuestion = (currentQuestion + 1) % questions.size();
    else finished = true;
}

bool Quiz::isFinished() const { return finished; }
void Quiz::reset() { loadQuestions(); }