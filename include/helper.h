#ifndef HELPER_H
#define HELPER_H

#include <QPushButton>
#include <QDebug>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>

enum class State { empty, obstacle, visited, start, goal, path };

class Tile : public QPushButton {
public:
    // x = width, y = height
    int x, y;
    State state;

    Tile(int x, int y, State state) : x(x), y(y), state(state) {}

    // Getter
    bool isEmpty() const { return this->state == State::empty; }
    bool isObstacle() const { return this->state == State::obstacle; }
    bool isVisited() const { return this->state == State::visited; }
    bool isStart() const { return this->state == State::start; }
    bool isGoal() const { return this->state == State::goal; }
    bool isPath() const { return this->state == State::path; }
};

struct Coordinates {
    int x, y;
    friend bool operator==(const Coordinates& a, const Coordinates& b) {
        return a.x == b.x && a.y == b.y;
    }
    friend bool operator!=(const Coordinates& a, const Coordinates& b) {
        return !(a == b);
    }
    friend bool operator<(const Coordinates& a, const Coordinates& b) {
        return std::tie(a.x, a.y) < std::tie(b.x, b.y);
    }
};

// "custom specialization of std::hash can be injected in namespace std"
// Implement hash to put Coordinates into an unordered_set
namespace std {
template<>
struct hash<Coordinates> {
    std::size_t operator()(const Coordinates& id) const noexcept {
        return std::hash<int>()(id.x ^ (id.y << 4));
    }
};
} // namespace std

template<typename T, typename priority_t>
struct PrioriyQueue {
    std::priority_queue<std::pair<priority_t, T>, std::vector<std::pair<priority_t, T>>,
        std::greater<std::pair<priority_t, T>>> elements;
    inline bool empty() const { return elements.empty(); }
    inline void put(T item, priority_t priority) { elements.emplace(priority, item); }
    T get() {
        T ret = elements.top().second;
        elements.pop();
        return ret;
    }
};

#endif // HELPER_H
