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

Either<CJSValue, std::string>
CJSObject::callMethod (const std::string& methodName, size_t argumentCount, const JSValueRef arguments[])
{
    auto eitherMethod = getProperty (methodName).safeGet<CJSObject> ();
    return eitherMethod.leftFlatMap (
        [&](auto method) { return method.callAsFunction (object, argumentCount, arguments); });
}


} // namespace cpp_javascriptcore
