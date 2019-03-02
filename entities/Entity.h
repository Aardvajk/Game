#ifndef ENTITY_H
#define ENTITY_H

#include <pcx/non_copyable.h>

class Entity : public pcx::non_copyable
{
public:
    virtual ~Entity(){ }
};

#endif // ENTITY_H
