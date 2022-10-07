// Copyright 2020 DmitryDzz

#include "group_id.hpp"

using minunity::group_id;

int group_id::compare(group_id& lhs, group_id& rhs) {
    if (lhs.value() > rhs.value()) return 1;
    if (lhs.value() < rhs.value()) return -1;
    return 0;
}
