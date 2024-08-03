template <typename Resource, typename Identifier>
inline void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string &filename) 
{
    std::unique_ptr<Resource> resource(new Resource());

    if(!resource -> loadFromFile(filename)) {
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
    }
    //std::map<Identifier, std::unique_ptr<Resource>>::iterator ptr;
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

template <typename Resource, typename Identifier>
inline Resource &ResourceHolder<Resource, Identifier>::get(Identifier id) 
{
    auto ptr = mResourceMap.find(id);
    assert(ptr != mResourceMap.end());
    return *(ptr -> second);
}

template <typename Resource, typename Identifier>
inline const Resource &ResourceHolder<Resource, Identifier>::get(Identifier id) const 
{

    auto ptr = mResourceMap.find(id);
    assert(ptr != mResourceMap.end());
    return *(ptr -> second);
}