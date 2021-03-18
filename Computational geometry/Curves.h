#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Predicates.h"
#include <iostream>
using namespace std;

sf::Vector2f lerp(float t, const sf::Vector2f a, const sf::Vector2f b) {
    return a + t * (b - a);
}

bool MouseInPoint(const float x, const float y, const sf::Vector2i &mpos, const int &pointsize) {
    sf::Vector2f tl(x - pointsize / 2, y - pointsize / 2);
    sf::Vector2f br(x + pointsize / 2, y + pointsize / 2);
    return mpos.x > tl.x && mpos.x < br.x && mpos.y > tl.y && mpos.y < br.y;
}

float length(const sf::Vector2f &p0, const sf::Vector2f &p1) {
    return abs(sqrtf(((p1.x - p0.x) * (p1.x - p0.x))
                     + ((p1.y - p0.y) * (p1.y - p0.y))));
}

exact::position orient2d(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2i p3) {
    double pa[2] = {p1.x, p1.y};
    double pb[2] = {p2.x, p2.y};
    double pc[2] = {double(p3.x), double(p3.y)};
    return exact::orient2d(pa, pb, pc);
}

class Line {
public:
    sf::Vector2f p0, p1;
    sf::Vertex *line{};

    Line() {
        p0.x = 0;
        p0.y = 0;
        p1.x = 0;
        p1.y = 0;
    }

    Line(float x0, float y0, float x1, float y1) {
        p0.x = x0;
        p0.y = y0;
        p1.x = x1;
        p1.y = y1;
        line = new sf::Vertex[2]{
                sf::Vertex(p0), sf::Vertex(p1)
        };
    }

    void Update(float x0, float y0, float x1, float y1) {
        p0.x = x0;
        p0.y = y0;
        p1.x = x1;
        p1.y = y1;
        line[0] = p0;
        line[1] = p1;
    }
};

class DiscreteCurve {
protected:
    std::vector<sf::RectangleShape *> points;
    std::vector<Line *> lines;
    int _npoints, nlines, pointsize;

    void MovePoint(sf::RectangleShape *currentPoint, int currentPointID, const sf::Vector2f &pos) {
        currentPoint->setPosition(pos);
        if (currentPointID == 0) {
            lines[0]->Update(currentPoint->getPosition().x, currentPoint->getPosition().y,
                             lines[0]->p1.x, lines[0]->p1.y);
        } else if (currentPointID == _npoints - 1) {
            lines[currentPointID - 1]->Update(lines[currentPointID - 1]->p0.x, lines[currentPointID - 1]->p0.y,
                                              currentPoint->getPosition().x, currentPoint->getPosition().y);
        } else {
            lines[currentPointID - 1]->Update(lines[currentPointID - 1]->p0.x, lines[currentPointID - 1]->p0.y,
                                              currentPoint->getPosition().x, currentPoint->getPosition().y);
            lines[currentPointID]->Update(currentPoint->getPosition().x, currentPoint->getPosition().y,
                                          lines[currentPointID]->p1.x, lines[currentPointID]->p1.y);
        }
    }

public:
    DiscreteCurve(int npoints, int pointsize, sf::Vector2f p0, sf::Vector2f p1)
            : _npoints(npoints), nlines(npoints - 1) {
        this->pointsize = pointsize;
        for (int i = 0; i < npoints; i++) {
            sf::RectangleShape *p = new sf::RectangleShape(sf::Vector2f(pointsize, pointsize));
            p->setFillColor(sf::Color::Red);
            p->setOrigin(pointsize / 2, pointsize / 2);
            p->setPosition(lerp((float) i / (npoints - 1), p0, p1));
            points.push_back(p);
        }
        for (int i = 0; i < nlines; i++) {
            Line *l = new Line(points[i]->getPosition().x, points[i]->getPosition().y,
                               points[i + 1]->getPosition().x, points[i + 1]->getPosition().y);
            lines.push_back(l);
        }
    }

    bool drawPoints = true;

    virtual void draw(sf::RenderWindow &window, const sf::Vector2i &mpos) {
        if (pointsize > 0 && drawPoints)
            for (int i = 0; i < _npoints; i++) window.draw(*points[i]);
//            Draw curve
//        for (int i = 0; i < nlines; i++) window.draw(lines[i]->line, 2, sf::Lines);
    }
};


class Controls {
private:
    int size, nodesize, nlines;
    std::vector<sf::RectangleShape *> nodes;
    std::vector<Line *> lines;
    sf::RectangleShape *currentNode = nullptr;
public:
    Controls(int size, int nodesize, sf::Vector2f *positions)
            : size(size), nodesize(nodesize) {
        for (int i = 0; i < size; i++) {
            sf::RectangleShape *p = new sf::RectangleShape(sf::Vector2f(nodesize, nodesize));
            p->setFillColor(sf::Color::Blue);
            p->setOrigin(nodesize / 2, nodesize / 2);
            p->setPosition(positions[i]);
            nodes.push_back(p);
        }
        nlines = size - 1;
        for (int i = 0; i < nlines; i++) {
            lines.push_back(new Line(positions[i].x, positions[i].y, positions[i + 1].x, positions[i + 1].y));
        }
    }

    bool update(bool lmbp, bool lmbr, const sf::Vector2i &mpos) {
        if (lmbr) currentNode = nullptr;
        int ind = 0;
        if (lmbp) {
            for (int i = 0; i < size; i++) {
                if (MouseInPoint(nodes[i]->getPosition().x, nodes[i]->getPosition().y, mpos, nodesize)) {
                    currentNode = nodes[i];
                    ind = i;
                    break;
                }
            }
        }
        if (lmbp and currentNode == nullptr) {
            Line *new_lines;
            ++size;
            sf::RectangleShape *p = new sf::RectangleShape(sf::Vector2f(nodesize, nodesize));
            p->setPosition(mpos.x, mpos.y);
            p->setOrigin(sf::Vector2f(nodesize / 2, nodesize / 2));
            p->setFillColor(sf::Color::Blue);
            nodes.push_back(p);
            nlines = size - 1;
            Line *l = new Line(nodes[size - 2]->getPosition().x, nodes[size - 2]->getPosition().y,
                               nodes[size - 1]->getPosition().x, nodes[size - 1]->getPosition().y);
            lines.push_back(l);

            return true;
        }
        if (currentNode != nullptr) {
            currentNode->setPosition(mpos.x, mpos.y);
            for (int i = std::max(0, ind - 1); i < nlines; i++) {
                lines[i]->Update(nodes[i]->getPosition().x, nodes[i]->getPosition().y,
                                 nodes[i + 1]->getPosition().x, nodes[i + 1]->getPosition().y);
            }
        }
        return false;
    }

    bool drawLoop = true;

    void draw(sf::RenderWindow &window, const sf::Vector2i &mpos) {
        Line *l = new Line(nodes[0]->getPosition().x, nodes[0]->getPosition().y,
                           nodes.back()->getPosition().x, nodes.back()->getPosition().y);
        lines.push_back(l);
        int num_intersections = 0;
        float x = mpos.x, y = mpos.y;
        int orient;
        sf::Vector2f min_point, max_point;
        bool inside = false;
        for (int i = 0; i <= nlines and !inside; ++i){
            min_point = lines[i]->p0;
            max_point = lines[i]->p1;
            if (min_point.y > max_point.y){
                std::swap(min_point, max_point);
            }
            orient = orient2d(min_point, max_point, mpos);
            if (orient == 0 and max_point.y >= y and min_point.y <= y
            and x <= fmax(max_point.x, min_point.x) and x <= fmin(max_point.x, min_point.x)){
//                cerr << "line" << '\n';
                inside = true;
            } else if (max_point.y == min_point.y){
                continue;
            } else if (y == max_point.y and x < fmax(max_point.x, min_point.x)){
                ++num_intersections;
            } else if (fmin(max_point.y, min_point.y) == y){
                continue;
            } else if (max_point.y < y or min_point.y > y){
                continue;
            } else if (orient == 1){
                ++num_intersections;
            }
        }
//        if (num_intersections % 2){
//            cerr << num_intersections << '\n';
//        }
        inside |= (num_intersections % 2);
        sf::Color c = sf::Color::White;
        if (inside){
//            cerr << num_intersections << '\n';
            c = sf::Color::Red;
        }

        for (int i = 0; i < size; i++){
            window.draw(*nodes[i]);
        }
        if (drawLoop) {
            for (int i = 0; i <= nlines; i++) {
                lines[i]->line[0].color = c;
                lines[i]->line[1].color = c;
                window.draw(lines[i]->line, 2, sf::Lines);
            }
            lines.pop_back();

        }
    }

    sf::Vector2f getNode(int i) {
        return nodes[i]->getPosition();
    }
};


class Bezier : public DiscreteCurve {
private:
    Controls *controls;
    int degree;

    sf::Vector2f *bezierLerp(float t, sf::Vector2f *old, int oldsize) {
        sf::Vector2f *newvals = new sf::Vector2f[oldsize - 1];
        for (int i = 0; i < oldsize - 1; i++) {
            newvals[i] = lerp(t, old[i], old[i + 1]);
        }
        return newvals;
    }

    sf::Vector2f bezierPoint(float t) {
        sf::Vector2f *oldv, *newv;
        oldv = new sf::Vector2f[degree + 1];
        for (int i = 0; i < degree + 1; i++) oldv[i] = controls->getNode(i);
        for (int i = degree + 1; i > 1; i--) {
            newv = bezierLerp(t, oldv, i);
            delete oldv;
            oldv = newv;
        }
        sf::Vector2f res = oldv[0];
        delete oldv;
        return res;
    }

public:
    Bezier(int degree, int npoints, int pointsize, sf::Vector2f p0, sf::Vector2f p1)
            : DiscreteCurve(npoints, pointsize, p0, p1), degree(degree) {
        float len = length(p0, p1) / npoints;
        sf::Vector2f *vertices = new sf::Vector2f[degree + 1];
        for (int i = 0; i < degree + 1; i++) {
            vertices[i] = lerp(i * len / (len * (degree + 1)), p0, p1);
        }
        controls = new Controls(degree + 1, pointsize, vertices);
        drawPoints = false;
    }

    void DrawLoop(bool b) {
        controls->drawLoop = b;
    }

    void update(bool lmbp, bool lmbr, const sf::Vector2i &mpos) {
        bool is_add = controls->update(lmbp, lmbr, mpos);
        if (!is_add) {
            MovePoint(points[0], 0, controls->getNode(0));
            MovePoint(points[_npoints - 1], _npoints - 1, controls->getNode(degree));
            for (int i = 1; i < _npoints - 1; i++) {
                sf::Vector2f p = bezierPoint((float) i / _npoints);
                MovePoint(points[i], i, p);
            }
        } else {
            ++degree;
            ++nlines;
            ++_npoints;
            sf::RectangleShape *p = new sf::RectangleShape(sf::Vector2f(pointsize, pointsize));
            p->setPosition(mpos.x, mpos.y);
            p->setOrigin(sf::Vector2f(pointsize / 2, pointsize / 2));
            p->setFillColor(sf::Color::Blue);
            points.push_back(p);
            Line *l = new Line(points[points.size() - 2]->getPosition().x, points[points.size() - 2]->getPosition().y,
                               points[points.size() - 1]->getPosition().x, points[points.size() - 1]->getPosition().y);
            lines.push_back(l);
        }

    }

    void draw(sf::RenderWindow &window, const sf::Vector2i &mpos) override {
//        DiscreteCurve::draw(window, mpos);
        if (pointsize > 0)
            controls->draw(window, mpos);
    }
};