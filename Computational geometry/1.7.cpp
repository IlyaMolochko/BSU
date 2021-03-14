#include "Curves.h"

using namespace std;
using namespace sf;
//using namespace exact;

void HandleEvents(sf::RenderWindow &);

void HandleInput(sf::RenderWindow &);

bool lmbd = false;
bool lmbp = false;
bool lmbr = false;
sf::Vector2i mpos;
sf::Vector2i pos;
Bezier bezier(2, 1000, 15, sf::Vector2f(100, 100), sf::Vector2f(1200, 100));

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Bezier curves.", sf::Style::Close | sf::Style::Titlebar);

    while (window.isOpen()) {
        HandleEvents(window);
        HandleInput(window);


        bezier.update(lmbp, lmbr, mpos);
        bezier.DrawLoop(true);
        window.clear();
        {
            pos = sf::Mouse::getPosition(window);
            bezier.draw(window, pos);
        }
        window.display();
    }

    return 0;
}

void HandleEvents(sf::RenderWindow &window) {
    sf::Event ev;
    while (window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) window.close();
    }
}

void HandleInput(sf::RenderWindow &window) {
    if (lmbr) lmbr = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        mpos = sf::Mouse::getPosition(window);
        if (lmbd) lmbp = false;
        else lmbp = true;
        lmbd = true;
    } else {
        if (lmbd) lmbr = true;
        lmbd = false;
    }
}