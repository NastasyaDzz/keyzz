#ifndef INPUT_HPP_INCLUDED
#define INPUT_HPP_INCLUDED

#include <ncurses.h>

namespace minunity {

class Input {
public:
    static void start();
    static void update();
    static bool is_key_pressed(int key);
};

} // namespace minunity

#endif // INPUT_HPP_INCLUDED
