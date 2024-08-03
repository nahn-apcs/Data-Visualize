#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cassert>
#include <map>
#include <memory>
#include <iostream>

template <typename Resource, typename Identifier> 
class ResourceHolder 
{
public:
    void load(Identifier id, const std::string &filename);
    Resource &get(Identifier id);
    const Resource &get(Identifier id) const;


private:
    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#include <ResourceHolder.inl>



#endif //RESOURCE_HOLDER_HPP