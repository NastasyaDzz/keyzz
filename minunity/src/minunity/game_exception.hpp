#ifndef GAME_EXCEPTION_HPP_INCLUDED
#define GAME_EXCEPTION_HPP_INCLUDED

#include <exception>
#include <stdexcept>
#include <string>

namespace minunity {

class GameException : public std::runtime_error {
private:
    const int code_;
public:
    static const int ENGINE_NOT_STARTED = -1001;
    static const int WRONG_SPRITE_SIZE = -1002;
    
    GameException(const int code, const std::string& message) : std::runtime_error(message),
        code_(code) {}
    
    virtual ~GameException() throw () {}
    
    virtual int get_code() {
        return this->code_;
    }
};

} // namespace

#endif // GAME_EXCEPTION_HPP_INCLUDED
