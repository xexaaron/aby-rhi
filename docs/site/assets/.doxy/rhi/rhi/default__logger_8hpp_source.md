

# File default\_logger.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**interfaces**](dir_6277bbb7ca6e5761e568397cf4fde752.md) **>** [**default\_logger.hpp**](default__logger_8hpp.md)

[Go to the documentation of this file](default__logger_8hpp.md)


```C++
#pragma once
#include "interfaces/interfaces.hpp"

namespace aby::rhi {

    class DefaultLogger final : public ILogger {
    public:
        auto name() -> std::string_view override;
        auto log(ELogLevel level, const std::string& msg) -> void override;
    private:
    };

} // namespace aby::rhi
```


