#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"

#include <string>
#include <cstdint>
#include <typeinfo>
#include <unordered_map>
#include <type_traits>
#include <functional>
#include <memory>

// Use for custom setters and getters
#define PX_R_DECL_FIELD_CUSTOM(f, t, s, g) RegisterField<t>(#f, std::make_shared<px::Field<t>>(px::Field<t>(#f, typeid(f).hash_code(), &f, std::is_base_of<px::FieldAccessor, t>::value, s, g)))
#define PX_R_DECL_FIELD(f, t) RegisterField<t>(#f, std::make_shared<px::Field<t>>(px::Field<t>(#f, typeid(f).hash_code(), &f, std::is_base_of<px::FieldAccessor, t>::value, px::BasicFieldSetter<t>, px::BasicFieldGetter<t>)))
// Use this for a custom field name (the name cannot include dots!)
#define PX_R_DECL_FIELDn(f, t, n) RegisterField<t>(n, std::make_shared<px::Field<t>>(px::Field<t>(n, typeid(f).hash_code(), &f, std::is_base_of<px::FieldAccessor, t>::value, px::BasicFieldSetter<t>, px::BasicFieldGetter<t>)))
#define PX_R_VALID_FIELD(f) (f->typeHash != 0 && f->ptr != nullptr)
#define PX_R_AS_STR(s) (std::string)s

#define PX_R_ERROR_FIELD_MISMATCHED_TYPE 0x300
#define PX_R_ERROR_FIELD_NOT_MODIFIABLE 0x301
#define PX_R_ERROR_FIELD_NULLPTR 0x302
#define PX_R_ERROR_FIELD_NOT_FOUND 0x303
#define PX_R_ERROR_FIELD_INVALID 0x304

namespace px
{
    template<typename T>
    void BasicFieldSetter(T* ptr, T* value)
    {
        *ptr = *value;
    }

    template<typename T>
    T BasicFieldGetter(T* ptr)
    {
        return *ptr;
    }

    template<typename T>
    using FieldSetter = std::function<void(T*, T*)>;
    template<typename T>
    using FieldGetter = std::function<T(T*)>;

    struct FieldBase
    {
        std::string name;
        size_t typeHash;
        void* ptr;
        bool modifiable;

        virtual ~FieldBase() = default;
        virtual void Set(void* value) = 0;
        virtual void Get(void* value) = 0;
    };

    template<typename T>
    struct Field : FieldBase
    {
        FieldSetter<T> setter;
        FieldGetter<T> getter;

        Field(CREFSTR name, size_t typeHash, void* ptr, const bool modifiable, FieldSetter<T> setter, FieldGetter<T> getter)
        {
            this->name = name;
            this->typeHash = typeHash;
            this->ptr = ptr;
            this->modifiable = modifiable;
            this->setter = setter;
            this->getter = getter;
        }

        void Set(void* value) override
        {
            setter((T*)ptr, (T*)value);
        }

        void Get(void* value) override
        {
            *((T*)value) = getter((T*)ptr);
        }
    };

    class FieldAccessor
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<FieldBase>> m_Fields;
    public:
        FieldAccessor() = default;
        virtual ~FieldAccessor() = default;
    private:
        PX_API std::shared_ptr<FieldBase> _GetField(CREFSTR name);
    protected:
        template<typename T>
        void RegisterField(CREFSTR name, const std::shared_ptr<Field<T>>& field)
        {
            m_Fields.insert({ name, field });
        }
    public:
        PX_API std::shared_ptr<FieldBase> GetField(CREFSTR path);

        template<typename T>
        void SetField(CREFSTR path, T value)
        {
            std::shared_ptr<FieldBase> field = GetField(path);
            if (!PX_R_VALID_FIELD(field))
            {
                Error::Throw(PX_R_ERROR_FIELD_INVALID, path + ": not a valid field");
                return;
            }

            if (!field->ptr)
            {
                Error::Throw(PX_R_ERROR_FIELD_NULLPTR, field->name + ": ptr is nullptr");
                return;
            }

            if (typeid(value).hash_code() != field->typeHash)
            {
                Error::Throw(PX_R_ERROR_FIELD_MISMATCHED_TYPE, field->name + ": mismatched type (" + std::to_string(field->typeHash) + " -> " + std::to_string(typeid(T).hash_code()) + ")");
                return;
            }

            field->Set(&value);
        }

        template<typename T>
        T GetFieldValue(CREFSTR path, T defaultValue)
        {
            std::shared_ptr<FieldBase> field = GetField(path);
            if (!PX_R_VALID_FIELD(field))
            {
                Error::Throw(PX_R_ERROR_FIELD_INVALID, path + ": not a valid field");
                return defaultValue;
            }

            if (!field->ptr)
            {
                Error::Throw(PX_R_ERROR_FIELD_NULLPTR, field->name + ": ptr is nullptr");
                return defaultValue;
            }

            if (typeid(T).hash_code() != field->typeHash)
            {
                Error::Throw(PX_R_ERROR_FIELD_MISMATCHED_TYPE, field->name + ": mismatched type (" + std::to_string(field->typeHash) + " -> " + std::to_string(typeid(T).hash_code()) + ")");
                return defaultValue;
            }

            T value;
            field->Get(&value);
            return value;
        }
    };
}