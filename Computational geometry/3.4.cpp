#include <SFML/Graphics.hpp>
#include "Predicates.h"
#include <list>
#include <vector>
#include <cmath>
#include <random>

using namespace std;
using namespace sf;
using namespace exact;

/*
 Заносим в стэк точку pl и первые две отсортированные точки
 Далее для каждой оставшейся точки выполняем следующие действия:
1.  Убираем точки из стэка, пока следующие три точки образуют
 не поворот против часовой стрелки (для проверки ориентации точек также используем предикаты Шевчука):
a)  Точка, идущая в стэке после вершины.
b)  Вершина стэка.
c)  Текущая точка из отсортированного массива точек.
2.  Если таких точек нет или мы их убрали, то заносим текущую точку из отсортированного массива в стэк.
 */

exact::position orient2d(Vertex p1, Vertex p2, Vertex p3) {
    double pa[2] = {p1.position.x, p1.position.y};
    double pb[2] = {p2.position.x, p2.position.y};
    double pc[2] = {p3.position.x, p3.position.y};
    return orient2d(pa, pb, pc);
}

double dist(Vertex p1, Vertex p2) {
    return (p2.position.x - p1.position.x) *
           (p2.position.x - p1.position.x) +
           (p2.position.y - p1.position.y) *
           (p2.position.y - p1.position.y);
}

void graham_hull(list<Vertex> &points, Vertex pl, VertexArray &convexHull) {
    vector<Vertex> st;
    st.push_back(pl);
    Vertex v = points.front();
    points.pop_front();
    Vertex w = points.front();
    points.pop_front();
    while (w.position.x != pl.position.x or w.position.y != pl.position.y) {
        if (orient2d(st.back(), v, w) == -1) {
            v = st.back();
            st.pop_back();
        } else {
            st.push_back(v);
            v = w;
            w = points.front();
            points.pop_front();
        }
    }
    st.push_back(pl);
    st.push_back(v);

    Vertex vertex;
    while (!st.empty()) {
        vertex = st.back();
        vertex.color = Color::Blue;
        convexHull.append(vertex);
        st.pop_back();
    }
}

/*
     генерация точек и выбор среди них точки с минимальной абсцисой pl и максимальной ординатой
*/
void generatePoints(VertexArray &points,
                    list<Vertex> &ps,
                    Vertex &pl,
                    int n,
                    double width = 1280,
                    double height = 720) {
    ps.clear();
    std::default_random_engine generator;
    generator.seed(time(nullptr));
    normal_distribution<double> x(width / 2, 50);
    normal_distribution<double> y(height / 2, 50);

    Vertex p;
    pl.position.x = numeric_limits<double>::max();
    pl.position.y = numeric_limits<double>::min();
    for (int i = 0; i < n; i++) {
        p.position.x = x(generator);
        p.position.y = y(generator);
        if (p.position.x < pl.position.x or
            p.position.x == pl.position.x and p.position.y > pl.position.y) {
            if (pl.position.x != numeric_limits<double>::max() and
                pl.position.y != numeric_limits<double>::min()) {
                ps.push_back(pl);
            }
            pl = p;
        } else {
            ps.push_back(p);
        }
        points[i] = p;
        points[i].color = Color::White;
    }
}


int main() {
    double width = 1280;
    double height = 720;
    sf::RenderWindow window(VideoMode(width, height), "SFML");
    int n = 100000;
    VertexArray points(Points, n);
    VertexArray convexHull(LinesStrip, 0);
    list<Vertex> ps;
    Vertex pl;
    generatePoints(points, ps, pl, n);
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
                    /*
                     сортировка по полярному углу с использованием предикатов Шевчука
                     */
                    ps.sort([&pl](Vertex a, Vertex b) {
                        if (orient2d(pl, a, b) == 0) {
                            return a.position.x < b.position.x;
//                            return dist(pl, a) <= dist(pl, b);
                        }
                        return orient2d(pl, a, b) == 1;
                    });

                    ps.push_back(pl);
                    graham_hull(ps, pl, convexHull);
                    was_created = true;
                } else if (event.mouseButton.button == Mouse::Left and was_created) {
                    convexHull.clear();
                    generatePoints(points, ps, pl, n);
                    was_created = false;
                }
            }
        }
        window.clear();
        if (convexHull.getVertexCount() != 0) {
            window.draw(convexHull);
        }
        window.draw(points);
        window.display();
    }

    return 0;
}