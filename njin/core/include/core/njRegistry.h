#pragma once
#include <string>
#include <unordered_map>

namespace njin::core {
    template<typename T>
    class njRegistry {
        public:
        void add(const std::string& name, const T& item);
        T* get(const std::string& name);
        const T* get(const std::string& name) const;

        private:
        std::unordered_map<std::string, T> registry_{};
    };
}  // namespace njin::core

#include "core/njRegistry.tpp"