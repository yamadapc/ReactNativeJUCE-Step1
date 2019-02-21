#include <string>

namespace cpp_javascriptcore
{

class CJSExportBuilder
{
public:
    template <typename Fn> CJSExportBuilder& method (std::string name, Fn implementation);
};

template <typename T> CJSExportBuilder class_ (std::string className)
{
}

}
