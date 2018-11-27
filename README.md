# kyasu_string
"include" 1 file. C++ std::string wrapper class.

# example
```
#include <iostream>
#include "kyasu_string.h"

int main(int argc, const char * argv[]) {

    kyasu::string str = "ABC,def,GDE";

    std::cout << str.toUpper() << std::endl;
    std::cout << str.toLower() << std::endl;
    
    auto s = str.split(",");
    std::cout << s[0] << s[1] << s[2] << std::endl;

    std::cout << str.replace(",", ":") << std::endl;

    std::cout << kyasu::string::format( "test=%d %f %s", 10, 0.5f, "hoge" ) << std::endl;

    return 0;
}
```
# result
```
ABC,DEF,GDE
abc,def,gde
ABCdefGDE
ABC:def:GDE
test=10 0.500000 hoge
```
