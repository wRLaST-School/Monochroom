#include "IScriptObject.h"
#include <ScriptComponent.h>
IComponent* IScriptObject::Parent()
{
    return body->Parent();
}

ScriptComponent* IScriptObject::This()
{
    return body;
}

std::string IScriptObject::GetClassString(void)
{
    return "IScriptObject";
}
