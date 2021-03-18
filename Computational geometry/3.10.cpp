#include <SFML/Graphics.hpp>
#include "Predicates.h"
#include <list>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <set>

using namespace std;
using namespace sf;
using namespace exact;

/*
 Заносим в стэк точку pl и первые две отсортированные точки
 Далее для каждой оставшейся точки выполняем следующие действия:
1.	Убираем точки из стэка, пока следующие три точки образуют
 не поворот против часовой стрелки (для проверки ориентации точек также используем предикаты Шевчука):
a)	Точка, идущая в стэке после вершины.
b)	Вершина стэка.
c)	Текущая точка из отсортированного массива точек.
2.	Если таких точек нет или мы их убрали, то заносим текущую точку из отсортированного массива в стэк.
 */

exact::position orient2d(Vertex p1, Vertex p2, Vertex p3) {
    double pa[2] = {p1.position.x, p1.position.y};
    double pb[2] = {p2.position.x, p2.position.y};
    double pc[2] = {p3.position.x, p3.position.y};
    return orient2d(pa, pb, pc);
}


void graham_hull(list<Vertex> points, VertexArray &edges) {
    vector<Vertex> st;
    st.push_back(points.front());
    edges.append(points.front());
    points.pop_front();
    Vertex v = points.front();
    v.color = sf::Color::Blue;
    points.pop_front();
    edges.append(v);
    Vertex w = points.front();
    w.color = sf::Color::Blue;
    points.pop_front();
    int n = points.size();
    for (int i = 0; i <= n; ++i){
        while (st.size() > 1 and orient2d(st.back(), v, w) == -1){
            edges.append(v);
            edges.append(w);
            v = st.back();
            st.pop_back();
            v.color = sf::Color::Blue;
        }
        if (st.size() == 1 and orient2d(st.back(), v, w) == -1){
            edges.append(v);
            edges.append(w);
            edges.append(st.back());
            edges.append(w);
            v = w;
            if (!points.empty()){
                w = points.front();
                points.pop_front();
            }
        } else {
            st.push_back(v);
            edges.append(v);
            edges.append(w);
            v = w;
            if (!points.empty()){
                w = points.front();
                points.pop_front();
            }
        }
    }
}

/*
     генерация точек и выбор среди них точки с минимальной абсцисой pl и максимальной ординатой
*/
void generatePoints(VertexArray &points,
                    list<Vertex> &ps,
                    Vertex &pl,
                    Vertex &pr,
                    int n,
                    double width = 1280,
                    double height = 720) {
    ps.clear();
    std::default_random_engine generator;
    generator.seed(time(nullptr));
    uniform_int_distribution<int> x(50, width - 50);
    uniform_int_distribution<int> y(50, height - 50);
//    normal_distribution<double> x(width / 2, 100);
//    normal_distribution<double> y(height / 2, 100);
    Vertex p;
    p.color = sf::Color::White;
    pl.position.x = width;
    pl.position.y = 0;
    pr.position.x = 0;
    pr.position.y = height;
    freopen("vertex.txt", "w", stdout);
//    freopen("vertex.txt", "r", stdin);
    set<pair<double, double>> s;
    for (int i = 0; i < n; i++) {
        p.position.x = int(x(generator));
        p.position.y = int(y(generator));
        cout << p.position.x << ' ' << p.position.y << '\n';
//        cin >> p.position.x >> p.position.y;
        if (s.count({p.position.x, p.position.y}) == 0){
            s.insert({p.position.x, p.position.y});
            if (p.position.x < pl.position.x or
                p.position.x == pl.position.x and p.position.y > pl.position.y) {
                if (pl.position.x != width and
                    pl.position.y != 0) {
                    if (pl.position.x > pr.position.x or pl.position.x == pr.position.x and pl.position.y < pr.position.y) {
                        if (pr.position.x != 0 and
                            pr.position.y != height) {
                            ps.push_back(pr);
                        }
                        pr = pl;
                    } else {
                        ps.push_back(pl);
                        ps.back().color = sf::Color::Blue;
                    }
                }
                pl = p;
            } else if (p.position.x > pr.position.x or p.position.x == pr.position.x and p.position.y < pr.position.y) {
                if (pr.position.x != 0 and
                    pr.position.y != height) {
                    ps.push_back(pr);
                    ps.back().color = sf::Color::Blue;
                }
                pr = p;
            } else {
                ps.push_back(p);
                ps.back().color = sf::Color::Blue;
            }
            p.color = sf::Color::White;
            points.append(p);
        }


    }
}


int main() {
    int width = 1280;
    int height = 720;
    sf::RenderWindow window(VideoMode(width, height), "SFML");
    int n = 500;
    VertexArray points(Points);
    VertexArray edges(LinesStrip);
    list<Vertex> ps;
    list<Vertex> left;
    Vertex pl;
    Vertex pr;
    generatePoints(points, ps, pl, pr, n);
    Init();
    bool was_created = false;
    while (window.isOpen()) {
        sf::Event event;
        window.draw(points);
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left and !was_created) {
                    left = ps;
                    pr.color = sf::Color::Blue;
                    pl.color = sf::Color::Blue;
                    left.push_back(pr);
                    left.sort([](Vertex a, Vertex b) {
                        if (a.position.x == b.position.x){
                            return a.position.y > b.position.y;
                        }
                        return a.position.x < b.position.x;
                    });
                    left.push_front(pl);
                    graham_hull(left, edges);
                    ps.push_back(pl);
                    ps.sort([](Vertex a, Vertex b) {
                        if (a.position.x == b.position.x){
                            return a.position.y < b.position.y;
                        }
                        return a.position.x > b.position.x;
                    });
                    ps.push_front(pr);
                    graham_hull(ps, edges);
                    was_created = true;
                } else if (event.mouseButton.button == Mouse::Left and was_created) {
                    edges.clear();
                    points.clear();
                    generatePoints(points, ps, pl, pr, n);
                    was_created = false;
                }
            }
        }
        window.clear();
        if (edges.getVertexCount() != 0) {
            window.draw(edges);
        }
        window.draw(points);
        window.display();
    }

    return 0;
}