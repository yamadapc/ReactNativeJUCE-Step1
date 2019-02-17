#include "CJSObject.h"

namespace cpp_javascriptcore
{

Either<CJSValue, std::string>
CJSObject::callAsFunction (JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
{
    JSValueRef error;
    JSValueRef result = JSObjectCallAsFunction (context, object, thisObject, argumentCount, arguments, &error);

    if (result == nullptr)
    {
        CJSValue cerror (context, error);
        return right (cerror.get<std::string> ());
    }

    return left (CJSValue (context, result));
}

} // namespace cpp_javascriptcore
