#ifndef COMPONENT_HPP_INCLUDED
#define COMPONENT_HPP_INCLUDED

namespace minunity {

class GameObject;

class Component {
public:
    Component();
    virtual ~Component();

    virtual int get_id() const;
    virtual bool is_destroyed() const;

    void set_game_object(GameObject *game_object);
    GameObject *get_game_object();
    
    virtual void update() {};
    virtual void destroy();
private:
    int id_;
    bool destroyed_;
    GameObject *game_object_ = nullptr;
};

}

#endif // COMPONENT_HPP_INCLUDED
