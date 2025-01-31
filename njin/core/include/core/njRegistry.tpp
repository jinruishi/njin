#include <format>
#include <iostream>
#include <string>

namespace njin::core {
    template<typename T>
    void njRegistry<T>::add(const std::string& name, const T& item) {
        if (registry_.contains(name)) {
            std::cout << std::format("Item with name '{}' already exists.",
                                     name)
                      << std::endl;
            return;
        }
        registry_.insert({ name, item });
    }

    template<typename T>
    T* njRegistry<T>::get(const std::string& name) {
        if (!registry_.contains(name)) {
            std::cout << std::format("Item with name '{}' does not exist.",
                                     name)
                      << std::endl;
            return nullptr;
        }
        return &registry_.at(name);
    }

    template<typename T>
    const T* njRegistry<T>::get(const std::string& name) const {
        if (!registry_.contains(name)) {
            std::cout << std::format("Item with name '{}' does not exist.",
                                     name)
                      << std::endl;
            return nullptr;
        }
        return &registry_.at(name);
    }
}  // namespace njin::core