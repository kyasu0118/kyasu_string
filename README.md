# kyasu_string
"include" 1 file. C++ std::string wrapper class.

# example

```
#include <iostream>
#include "kyasu_string.h"

int main(int argc, const char * argv[]) {

    kyasu::string str = "ABC,def,GDE";

    std::cout << str.toUpperCase() << std::endl;
    std::cout << str.toLowerCase() << std::endl;
    
    auto s = str.split(",");
    std::cout << s[0] << s[1] << s[2] << std::endl;

    std::cout << str.replace(",", ":") << std::endl;

    return 0;
}
```
