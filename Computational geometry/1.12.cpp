#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <random>
#include <set>

using namespace std;
using namespace sf;


struct point{
    int x;
    int y;

    point(int x, int y): x(x), y(y){};
};

struct st{
    point p1;
    point p2;
    int t;
    int index;
};

const int width = 1280;
const int height = 720;
const int len = 50;
const int n = 1000;


void generate(VertexArray &lines, vector<st> &points) {
    std::default_random_engine generator;
    generator.seed(time(nullptr));
    std::uniform_int_distribution<int> dist1(50, width - 50);
    std::uniform_int_distribution<int> dist2(50, height - 50);
    std::uniform_real_distribution<double> dist3(0, 1);
    int x, y;
    for (int i = 0; i < lines.getVertexCount(); i += 2) {
        if (dist3(generator) < 0.5) {
            x = dist1(generator);
            y = dist2(generator);
            lines[i].position.x = x;
            lines[i].position.y = y;
            lines[i + 1].position.x = x + len;
            lines[i + 1].position.y = y;
            points.push_back({{x, y}, {x + len, y}, 0, i});
            points.push_back({{x + len, y}, {x, y}, 2, i + 1});
        } else {
            x = dist1(generator);
            y = dist2(generator);
            lines[i].position.x = x;
            lines[i].position.y = y;
            lines[i + 1].position.x = x;
            lines[i + 1].position.y = y + len;
            points.push_back({{x, y}, {x, y + len}, 1, i});
        }
        lines[i].color = Color::White;
        lines[i + 1].color = Color::White;
    }
}

struct sort_by_y{
    bool operator ()(const st &a, const st &b) const{
        if (a.p1.y == b.p1.y){
            if (a.p1.x == b.p1.x){
                return a.p2.x < b.p2.x;
            }
            return a.p1.x < b.p1.x;
        }
        return a.p1.y < b.p1.y;
    }
};

void get_intersections(VertexArray &lines, vector<st> &points) {
    set<st,sort_by_y> s;
    int y1, y2;
    for (auto &p : points) {
        if (p.t == 0) {
            s.insert(p);
        } else if (p.t == 2) {
            s.erase({p.p2, p.p1, 0, p.index - 1});
        } else if (p.t == 1) {
            y1 = p.p1.y;
            y2 = p.p2.y;

            for (auto it = s.lower_bound({{0, y1 - 1}, {0, 0}, 0, 0}); it != s.end() and it->p1.y <= y2; ++it) {
                if (it->p1.x <= p.p1.x and it->p2.x >= p.p1.x){
                    lines[it->index].color = Color::Blue;
                    lines[it->index + 1].color = Color::Blue;
                    lines[p.index].color = Color::Blue;
                    lines[p.index + 1].color = Color::Blue;
                }
            }
        }
    }
}

auto compare = [](const st &v1, const st &v2) {
    if (v1.p1.x == v2.p1.x) {
        if (v1.t < v2.t){
            return true;
        } else if (v1.t > v2.t){
            return false;
        }
        return v1.p1.y < v2.p2.y;
    }
    return v1.p1.x < v2.p1.x;
};

int main() {
    RenderWindow window(VideoMode(width, height), "SFML");
    vector<st> points;
    VertexArray lines(Lines, 2 * n);
    generate(lines, points);
    sort(points.begin(), points.end(), compare);
    bool was_found = false;
    while (window.isOpen()) {
        window.clear();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left and !was_found) {
                    get_intersections(lines, points);
                    was_found = true;
                } else {
                    was_found = false;
                    window.clear();
                    points.clear();
                    generate(lines, points);
                    sort(points.begin(), points.end(), compare);
                }
            }
        }
        window.draw(lines);
        window.display();
    }
    return 0;
}