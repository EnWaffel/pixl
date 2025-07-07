#include "pixl/reflect/Field.h"

using namespace px;

std::shared_ptr<FieldBase> px::FieldAccessor::_GetField(CREFSTR name)
{
    if (m_Fields.count(name) < 1)
    {
        Error::Throw(PX_R_ERROR_FIELD_NOT_FOUND, name + ": no such field");
        return nullptr;
    }
    return m_Fields.at(name);
}

std::shared_ptr<FieldBase> px::FieldAccessor::GetField(CREFSTR path)
{
    if (path.find_first_of('.') == std::string::npos) return _GetField(path);

    std::string thisFieldName = path.substr(0, path.find_first_of('.'));
    std::shared_ptr<FieldBase> field = _GetField(thisFieldName);

    if (!field->modifiable)
    {
        Error::Throw(PX_R_ERROR_FIELD_NOT_MODIFIABLE, thisFieldName + ": not a sub-class of FieldAccessor or not a class");
        return nullptr;
    }

    if (!field->ptr)
    {
        Error::Throw(PX_R_ERROR_FIELD_NULLPTR, thisFieldName + ": ptr is nullptr");
        return nullptr;
    }

    FieldAccessor* accessor = (FieldAccessor*)field->ptr;
    return accessor->GetField(path.substr(path.find_first_of('.') + 1));
}
