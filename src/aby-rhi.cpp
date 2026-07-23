#include "aby-rhi.hpp"

namespace aby::rhi {

    auto Context::get() -> Context& {
        static Context context;
        return context;
    }  

    Context::Context() {

    }

    Context::~Context() {
        delete m_Logger;
    }


    auto Context::logger() -> ILogger* {
        return m_Logger;
    }


}