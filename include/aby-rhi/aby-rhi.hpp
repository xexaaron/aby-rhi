#pragma once
#include "interfaces.hpp"
#include <type_traits>
#include <vector> 

namespace aby::rhi {

    class Context {
    public:
        static auto get() -> Context&;  
        ~Context();
        
        auto logger() -> ILogger*;

        template <typename T> requires(std::derived_from<IInterface, T>) 
        auto set_interface() -> void;
    protected:
        Context();
    private:
        ILogger* m_Logger;
    };

    template <typename T> requires(std::derived_from<IInterface, T>) 
    auto Context::set_interface() -> void {
        if constexpr (std::is_base_of_v<ILogger, T>) {
            m_Logger = new T();
        } else {
            static_assert(false, "unimplemented interface type");
        }
    } 


}