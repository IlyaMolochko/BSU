#include <SFML\Graphics.hpp>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <random>
#include <iostream>
#include <set>

using namespace std;

//struct label {
//    int index{};
//    sf::Vertex vertex;
//};
//
//const int vertexCount = 1000000;
//const int height = 720;
//const int width = 1280;
//
//sf::VertexArray points(sf::Points, vertexCount);
//vector<int> tree(2 * vertexCount + 1, 0);
//unordered_set<int> prevBluePoints;
//unordered_set<int> currentBluePoints;

struct st {
    int x;
    int y;
    int i;
    int mn_x = -1;
    int mx_x = -1;
    int mn_y = -1;
    int mx_y = -1;
};

int mn_y;
int mx_y;
int mn_x;
int mx_x;

auto sort_by_x = [](st s1, st s2) {
    if (s1.x == s2.x) {
        return s1.y < s2.y;
    }
    return s1.x < s2.x;
};

auto sort_by_y = [](st s1, st s2) {
    if (s1.y == s2.y) {
        return s1.x < s2.x;
    }
    return s1.y < s2.y;
};

void generatePoints(vector<st> &v, sf::VertexArray &points, int n,
                    int width, int height) {
    std::default_random_engine generator;
    generator.seed(time(nullptr));
    std::uniform_int_distribution<int> x(0, width);
    std::uniform_int_distribution<int> y(0, height);
    set<pair<int, int>> s;
    st p;
    for (int i = 0; i < n; i++) {
        p.x = x(generator);
        p.y = y(generator);
        if (s.count({p.x, p.y}) == 0){
            points.append({sf::Vertex({float(p.x), float(p.y)}, sf::Color::White)});
            v.push_back({p.x, p.y, int(s.size())});
            s.insert({p.x, p.y});
        }
    }
//    cout << v.size() << endl;
}

void build(vector<st>::iterator b, vector<st>::iterator e, vector<st> &v, bool c) {
    if (e - b > 1) {
        auto m = b + (e - b) / 2 - ((e - b) % 2 == 0);
        nth_element(b, m, e, (c ? sort_by_x : sort_by_y));
        build(b, m, v, !c);
        build(m + 1, e, v, !c);
        m = b + (e - b) / 2 - ((e - b) % 2 == 0);
        auto l = b + (m - b) / 2 - ((m - b) % 2 == 0);
        auto r = m + 1 + (e - m - 1) / 2 - ((e - m - 1) % 2 == 0);
        m->mx_x = max(l->mx_x, max(r->mx_x, m->x));
        m->mn_x = min(l->mn_x, min(r->mn_x, m->x));
        m->mx_y = max(l->mx_y, max(r->mx_y, m->y));
        m->mn_y = min(l->mn_y, min(r->mn_y, m->y));
    } else if (e - b == 1) {
        b->mn_x = b->x;
        b->mx_x = b->x;
        b->mn_y = b->y;
        b->mx_y = b->y;
    }
}

void get_inside(vector<st>::iterator b, vector<st>::iterator e,
                sf::VertexArray &points, vector<st> &v,
                unordered_set<int> &cur, unordered_set<int> &prev,
                st p1, st p2) {
    if (e - b > 1) {
        auto m = b + (e - b) / 2 - ((e - b) % 2 == 0);
        if (m->x >= p1.x and m->x <= p2.x and
            m->y >= p1.y and m->y <= p2.y) {
            points[m->i].color = sf::Color::Blue;
            cur.insert(m->i);
            prev.erase(m->i);
        }
        auto l = b + (m - b) / 2 - ((m - b) % 2 == 0);
        if (l->mx_y <= p2.y and l->mn_y >= p1.y and l->mx_x <= p2.x and l->mn_x >= p1.x) {
            for (auto it = b; it != m; ++it) {
                points[it->i].color = sf::Color::Blue;
                cur.insert(it->i);
                prev.erase(it->i);
            }
        } else if (l->mx_y < p1.y or l->mn_y > p2.y or l->mx_x < p1.x or l->mn_x > p2.x) {
            {}
        } else {
            get_inside(b, m, points, v, cur, prev, p1, p2);
        }
        auto r = m + 1 + (e - m - 1) / 2 - ((e - m - 1) % 2 == 0);
        if (r != v.end() and r->mx_y <= p2.y and r->mn_y >= p1.y and r->mx_x <= p2.x and r->mn_x >= p1.x) {
            for (auto it = m + 1; it != e; ++it) {
                points[it->i].color = sf::Color::Blue;
                cur.insert(it->i);
                prev.erase(it->i);
            }
        } else if (r != v.end() and (r->mx_y < p1.y or r->mn_y > p2.y or r->mx_x < p1.x or r->mn_x > p2.x)) {
            {}
        } else if (r != v.end()) {
            get_inside(m + 1, e, points, v, cur, prev, p1, p2);
        }
    } else if (e - b == 1) {
        if (b->x >= p1.x and b->x <= p2.x and
            b->y >= p1.y and b->y <= p2.y) {
            points[b->i].color = sf::Color::Blue;
            cur.insert(b->i);
            prev.erase(b->i);
        }
    }
}

void get_outside(sf::VertexArray &points,
                 unordered_set<int> &cur,
                 unordered_set<int> &prev) {
    for (auto i : prev) {
        if (cur.count(i) == 0) {
            points[i].color = sf::Color::White;
        }
    }
    prev.clear();
    prev = move(cur);
}



int main() {
    int width = 1280;
    int height = 720;
    int n = 1000000;
    vector<st> v;
    v.reserve(n);
    sf::VertexArray points(sf::Points);
    bool isRectangleDrawed = false;
    bool isMove = false;
    float dx = 0;
    float dy = 0;
    generatePoints(v, points, n, width, height);
//    auto t = clock();
    build(v.begin(), v.end(), v, true);
    st p1, p2;
    unordered_set<int> cur;
    unordered_set<int> prev;
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML");
    sf::Vector2f mouseDownPosition;
    sf::RectangleShape rectangle;
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(1);
    rectangle.setFillColor(sf::Color::Transparent);
    sf::VertexArray p;
    while (window.isOpen()) {
        window.clear();
        window.draw(points);
        window.draw(rectangle);
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!isRectangleDrawed) {
                        mouseDownPosition.x = event.mouseButton.x;
                        mouseDownPosition.y = event.mouseButton.y;
                    }
                    if (rectangle.getGlobalBounds().contains(pixelPos.x, pixelPos.y) and isRectangleDrawed) {
                        isMove = true;
                        dx = pixelPos.x - rectangle.getPosition().x;
                        dy = pixelPos.y - rectangle.getPosition().y;
                        get_inside(v.begin(), v.end(), points, v, cur, prev, p1, p2);
                        get_outside(points, cur, prev);
                        window.draw(points);
                        window.draw(rectangle);
                    }
                }
            }
            if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) and isRectangleDrawed and isMove) {
                rectangle.setPosition(pixelPos.x - dx, pixelPos.y - dy);
                p1 = {int(rectangle.getGlobalBounds().left),
                      int(rectangle.getGlobalBounds().top)};
                p2 = {int(rectangle.getGlobalBounds().left + rectangle.getGlobalBounds().width),
                      int(rectangle.getGlobalBounds().top + rectangle.getGlobalBounds().height)};
//                t = clock();
                get_inside(v.begin(), v.end(), points, v, cur, prev, p1, p2);
//                cout << clock() - t << " inside" << '\n';
//                t = clock();
                get_outside(points, cur, prev);
//                cout << clock() - t << " outside" << '\n';
                isMove = false;
                window.draw(points);
                window.draw(rectangle);
            }
        }
        if (isMove) {
            rectangle.setPosition(pixelPos.x - dx, pixelPos.y - dy);
            p1 = {int(rectangle.getGlobalBounds().left),
                  int(rectangle.getGlobalBounds().top)};
            p2 = {int(rectangle.getGlobalBounds().left + rectangle.getGlobalBounds().width),
                  int(rectangle.getGlobalBounds().top + rectangle.getGlobalBounds().height)};
//            t = clock();
            get_inside(v.begin(), v.end(), points, v, cur, prev, p1, p2);
//            cout << clock() - t << " inside" << '\n';
//            t = clock();
            get_outside(points, cur, prev);
//            cout << clock() - t << " outside" << '\n';
            window.draw(points);
            window.draw(rectangle);
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!isRectangleDrawed) {
                sf::Vector2i globalPosition = sf::Mouse::getPosition(window);
                rectangle.setPosition(mouseDownPosition.x, mouseDownPosition.y);
                sf::Vector2f newSize(globalPosition.x - mouseDownPosition.x, globalPosition.y - mouseDownPosition.y);
                rectangle.setSize(newSize);

                p1 = {int(rectangle.getGlobalBounds().left),
                      int(rectangle.getGlobalBounds().top)};
                p2 = {int(rectangle.getGlobalBounds().left + rectangle.getGlobalBounds().width),
                      int(rectangle.getGlobalBounds().top + rectangle.getGlobalBounds().height)};
//                t = clock();
                get_inside(v.begin(), v.end(), points, v, cur, prev, p1, p2);
//                cout << clock() - t << " inside" << '\n';
//                t = clock();
                get_outside(points, cur, prev);
//                cout << clock() - t << " outside" << '\n';
                window.draw(points);
                window.draw(rectangle);
            }
        } else if (sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (!isRectangleDrawed) {
                    isRectangleDrawed = true;
                    get_inside(v.begin(), v.end(), points, v, cur, prev, p1, p2);
                    window.draw(points);
                    window.draw(rectangle);
                }
            }
        }
        window.display();
    }
    return 0;
}