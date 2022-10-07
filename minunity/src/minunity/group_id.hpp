#ifndef GROUP_ID_HPP_INCLUDED
#define GROUP_ID_HPP_INCLUDED

#include <iostream>

namespace minunity {

struct group_id {
public:
    group_id() {}
    group_id(const int value) : value_(value) {}

    int value() const { return value_; }

    friend std::ostream& operator << (std::ostream& os, const group_id& i) {
        os << i.value_;
        return os;
    }

    group_id& operator = (int value) {
        value_ = value;
        return *this;
    }

    operator int() { return value_; }

    static int compare(group_id& lhs, group_id& rhs);
private:
    int value_{0};
};

inline bool operator == (group_id& lhs, group_id& rhs) { return group_id::compare(lhs, rhs) == 0; }
inline bool operator != (group_id& lhs, group_id& rhs) { return group_id::compare(lhs, rhs) != 0; }
inline bool operator <  (group_id& lhs, group_id& rhs) { return group_id::compare(lhs, rhs) <  0; }
inline bool operator >  (group_id& lhs, group_id& rhs) { return group_id::compare(lhs, rhs) >  0; }
inline bool operator <= (group_id& lhs, group_id& rhs) { return group_id::compare(lhs, rhs) <= 0; }
inline bool operator >= (group_id& lhs, group_id& rhs) { return group_id::compare(lhs, rhs) >= 0; }

}

#endif // GROUP_ID_HPP_INCLUDED
