#include "mvalue.h"
#include <iostream>

alt::MValue ToMValue(alt::MValueConst* val) {
    alt::ICore& core = alt::ICore::Instance();
    if (val == nullptr) return core.CreateMValueNil();
    std::cout << "1" << std::endl;
    auto mValue = val->Get();
    std::cout << "2" << std::endl;
    switch (mValue->GetType()) {
        case alt::IMValue::Type::NONE:
            return core.CreateMValueNone();
        case alt::IMValue::Type::NIL:
            return core.CreateMValueNil();
        case alt::IMValue::Type::BOOL:
            return core.CreateMValueBool(((alt::MValueBoolConst*)(val))->Get()->Value());
        case alt::IMValue::Type::INT:
            return core.CreateMValueInt(((alt::MValueIntConst*)(val))->Get()->Value());
        case alt::IMValue::Type::UINT:
            return core.CreateMValueUInt(((alt::MValueUIntConst*)(val))->Get()->Value());
        case alt::IMValue::Type::DOUBLE:
            return core.CreateMValueDouble(((alt::MValueDoubleConst*)(val))->Get()->Value());
        case alt::IMValue::Type::STRING:
            return core.CreateMValueString(((alt::MValueStringConst*)(val))->Get()->Value().CStr());
        case alt::IMValue::Type::LIST: {
            auto cVal = ((alt::MValueListConst*)(val))->Get();
            auto length = cVal->GetSize();
            alt::MValueList list = core.CreateMValueList(length);

            alt::RefBase<alt::RefStore<const alt::IMValue>> innerVal;
            for (uint32_t i = 0; i < length; ++i) {
                innerVal = cVal->Get(i);
                list->Set(i, ToMValue(&innerVal));
            }

            return list;
        }
        case alt::IMValue::Type::DICT:{
            auto cVal = ((alt::MValueDictConst*)(val))->Get();
            alt::MValueDict dict = core.CreateMValueDict();

            alt::RefBase<alt::RefStore<const alt::IMValue>> innerVal;
            alt::IMValueDict::Iterator* it = cVal->Begin();
            while (it != nullptr) {
                innerVal = it->GetValue();
                dict->Set(it->GetKey(), ToMValue(&innerVal));
                it = cVal->Next();
            }

            return dict;
        }
        case alt::IMValue::Type::BASE_OBJECT:
            return core.CreateMValueBaseObject(((alt::MValueBaseObjectConst*)(val))->Get()->Value());
        case alt::IMValue::Type::FUNCTION:
            return core.CreateMValueNone();//core.CreateMValueFunction(dynamic_cast<const alt::IMValueFunction*>(mValue)->Call);
        case alt::IMValue::Type::VECTOR3:
            return core.CreateMValueVector2(((alt::MValueVector3Const*)(val))->Get()->Value());
        case alt::IMValue::Type::RGBA:
            return core.CreateMValueRGBA(((alt::MValueRGBAConst*)(val))->Get()->Value());
        case alt::IMValue::Type::BYTE_ARRAY: {
            auto cVal = ((alt::MValueByteArrayConst*)(val))->Get();
            return core.CreateMValueByteArray(cVal->GetData(), cVal->GetSize());
        }
        case alt::IMValue::Type::VECTOR2:
            return core.CreateMValueVector2(((alt::MValueVector2Const*)(val))->Get()->Value());
        default:
            return core.CreateMValueNone();
    }
}

alt::MValueArgs MValuesToArgs(alt::MValueConst* args[], int size) {
    alt::Array<alt::MValueConst> mValues = alt::Array<alt::MValueConst>(size);
    for (int i = 0; i < size; i++) {
        mValues[i] = ToMValue(args[i]);
    }
    return mValues;
}

CustomInvoker* Invoker_Create(CSharpResourceImpl* resource, MValueFunctionCallback val) {
    auto invoker = new CustomInvoker(val);
    resource->invokers->Push(invoker);
    return invoker;
}

void Invoker_Destroy(CSharpResourceImpl* resource, CustomInvoker* val) {
    auto newInvokers = new alt::Array<CustomInvoker*>();
    for (alt::Size i = 0, length = resource->invokers->GetSize(); i < length; i++) {
        auto invoker = (*resource->invokers)[i];
        if (invoker != val) {
            newInvokers->Push(invoker);
        }
    }
    alt::Array<CustomInvoker*>* oldInvokers = resource->invokers;
    resource->invokers = newInvokers;
    delete val;
    delete oldInvokers;
}

void MValueConst_AddRef(alt::MValueConst* mValueConst) {
    (*mValueConst)->AddRef();
}

void MValueConst_RemoveRef(alt::MValueConst* mValueConst) {
    (*mValueConst)->RemoveRef();
}

uint8_t MValueConst_GetBool(alt::MValueConst* mValueConst) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::BOOL) {
        return dynamic_cast<const alt::IMValueBool*>(mValue)->Value();
    }
    return false;
}

int64_t MValueConst_GetInt(alt::MValueConst* mValueConst) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::INT) {
        return dynamic_cast<const alt::IMValueInt*>(mValue)->Value();
    }
    return 0;
}

uint64_t MValueConst_GetUInt(alt::MValueConst* mValueConst) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::UINT) {
        return dynamic_cast<const alt::IMValueUInt*>(mValue)->Value();
    }
    return 0;
}

double MValueConst_GetDouble(alt::MValueConst* mValueConst) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::DOUBLE) {
        return dynamic_cast<const alt::IMValueDouble*>(mValue)->Value();
    }
    return 0.0;
}

uint8_t MValueConst_GetString(alt::MValueConst* mValueConst, const char*&value, uint64_t &size) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::STRING) {
        auto stringView = dynamic_cast<const alt::IMValueString*>(mValue)->Value();
        value = stringView.CStr();
        size = stringView.GetSize();
        return true;
    }
    return false;
}

uint64_t MValueConst_GetListSize(alt::MValueConst* mValueConst) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::LIST) {
        auto list = dynamic_cast<const alt::IMValueList*>(mValue);
        if (list == nullptr) return 0;
        return list->GetSize();
    }
    return 0;
}

uint8_t MValueConst_GetList(alt::MValueConst* mValueConst, alt::MValueConst* values[]) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::LIST) {
        auto list = dynamic_cast<const alt::IMValueList*>(mValue);
        for (uint64_t i = 0, length = list->GetSize(); i < length; i++) {
            alt::MValueConst mValueElement = list->Get(i);
            values[i] = new alt::MValueConst(mValueElement);
        }
        return true;
    }
    return false;
}

uint64_t MValueConst_GetDictSize(alt::MValueConst* mValueConst) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::DICT) {
        auto dict = dynamic_cast<const alt::IMValueDict*>(mValue);
        if (dict == nullptr) return 0;
        return dict->GetSize();
    }
    return 0;
}

uint8_t MValueConst_GetDict(alt::MValueConst* mValueConst, const char* keys[],
                         alt::MValueConst* values[]) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::DICT) {
        auto dict = dynamic_cast<const alt::IMValueDict*>(mValue);
        auto next = dict->Begin();
        if (next == nullptr) return true;
        uint64_t i = 0;
        do {
            auto key = next->GetKey();
            auto keySize = key.GetSize();
            auto keyArray = new char[keySize + 1];
            memcpy(keyArray, key.CStr(), keySize);
            keyArray[keySize] = '\0';
            keys[i] = keyArray;
            alt::MValueConst mValueElement = next->GetValue();
            values[i] = new alt::MValueConst(mValueElement);
            i++;
        } while ((next = dict->Next()) != nullptr);
        return true;
    }
    return false;
}

void* MValueConst_GetEntity(alt::MValueConst* mValueConst, alt::IBaseObject::Type &type) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::BASE_OBJECT) {
        auto entityPointer = dynamic_cast<const alt::IMValueBaseObject*>(mValue)->Value().Get();
        if (entityPointer != nullptr) {
            type = entityPointer->GetType();
            switch (type) {
                case alt::IBaseObject::Type::PLAYER:
                    return dynamic_cast<alt::IPlayer*>(entityPointer);
                case alt::IBaseObject::Type::VEHICLE:
                    return dynamic_cast<alt::IVehicle*>(entityPointer);
                case alt::IBaseObject::Type::BLIP:
                    return dynamic_cast<alt::IBlip*>(entityPointer);
                case alt::IBaseObject::Type::VOICE_CHANNEL:
                    return dynamic_cast<alt::IVoiceChannel*>(entityPointer);
                case alt::IBaseObject::Type::COLSHAPE:
                    return dynamic_cast<alt::IColShape*>(entityPointer);
                case alt::IBaseObject::Type::CHECKPOINT:
                    return dynamic_cast<alt::ICheckpoint*>(entityPointer);
                default:
                    return nullptr;
            }
        }
    }
    return nullptr;
}

alt::MValueConst* MValueConst_CallFunction(alt::ICore* core, alt::MValueConst* mValueConst, alt::MValueConst* val[], uint64_t size) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::FUNCTION) {
        alt::MValueArgs value = alt::Array<alt::MValueConst>(size);
        for (uint64_t i = 0; i < size; i++) {
            if (val == nullptr) {
                value[i] = core->CreateMValueNil();
            } else {
                value[i] = *val[i];
            }
        }
        auto mValueFunction = dynamic_cast<const alt::IMValueFunction*>(mValue);
        auto result = new alt::MValueConst(mValueFunction->Call(value));
        return result;
    }
    return nullptr;
}

void MValueConst_GetVector3(alt::MValueConst* mValueConst, position_t& position) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::VECTOR3) {
        auto vector = dynamic_cast<const alt::IMValueVector3*>(mValue)->Value();
        position.x = vector[0];
        position.y = vector[1];
        position.z = vector[2];
    }
}

void MValueConst_GetRGBA(alt::MValueConst* mValueConst, rgba_t& rgba) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::RGBA) {
        auto rgbaValue = dynamic_cast<const alt::IMValueRGBA*>(mValue)->Value();
        rgba.r = rgbaValue.r;
        rgba.g = rgbaValue.g;
        rgba.b = rgbaValue.b;
        rgba.a = rgbaValue.a;
    }
}

void MValueConst_GetByteArray(alt::MValueConst* mValueConst, uint64_t size, void* data) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::RGBA) {
        auto byteArrayMValue = dynamic_cast<const alt::IMValueByteArray*>(mValue);
        auto byteArraySize = byteArrayMValue->GetSize();
        if (byteArraySize < size) {
            size = byteArraySize;
        }
        auto byteArray = byteArrayMValue->GetData();
        memcpy(data, byteArray, size);
    }
}

uint64_t MValueConst_GetByteArraySize(alt::MValueConst* mValueConst) {
    auto mValue = mValueConst->Get();
    if (mValue != nullptr && mValue->GetType() == alt::IMValue::Type::BYTE_ARRAY) {
        return dynamic_cast<const alt::IMValueByteArray*>(mValue)->GetSize();
    }
    return 0;
}


/*void MValue_Dispose(alt::MValue* mValue) {
    mValue->Free();
}*/

void MValueConst_Delete(alt::MValueConst* mValueConst) {
    delete mValueConst;
}

uint8_t MValueConst_GetType(alt::MValueConst* mValueConst) {
    auto mValue = mValueConst->Get();
    if (mValue == nullptr) return (uint8_t) alt::IMValue::Type::NIL;
    return (uint8_t) mValue->GetType();
}