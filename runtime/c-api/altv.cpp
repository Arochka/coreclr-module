#include "altv.h"

#include <string.h>
#include "version/version.h"
#include "../cpp-sdk/version/version.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wempty-body"
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif

using alt::Array;

void FreeUIntArray(alt::Array<uint32_t>* array) {
    array->~Array<uint32_t>();
}

/*void FreeStringViewArray(alt::Array<std::stringView>* array) {
    array->~Array<std::stringView>();
}*/

/*void FreeStringArray(alt::Array<std::string>* array) {
    array->~Array<std::string>();
}*/

/*void FreeMValueArray(alt::Array<alt::MValue>* array) {
    array->~Array<alt::MValue>();
}*/

/*void FreePlayerPointerArray(alt::Array<alt::IPlayer*>* array) {
    array->~Array<alt::IPlayer*>();
}*/

void FreeCharArray(char charArray[]) {
    delete[] charArray;
}

void FreeUInt32Array(uint32_t uInt32Array[]) {
    delete[] uInt32Array;
}

void FreeUInt8Array(uint8_t uInt8Array[]) {
    delete[] uInt8Array;
}

void FreeVoidPointerArray(void* voidPointerArray[]) {
    delete[] voidPointerArray;
}

void FreeString(const char* string) {
    delete[] string;
}

void FreeStringArray(const char** stringArray, uint32_t size) {
    for (int i = 0; i < size; i++) delete[] stringArray[i];
    delete[] stringArray;
}

void FreeResourceArray(alt::IResource** resourceArray) {
    delete[] resourceArray;
}

void FreeMValueConstArray(alt::MValueConst** mValueConstArray) {
    delete[] mValueConstArray;
}

const char* GetVersionStatic(int32_t &size) {
    return AllocateString(alt::ICore::Instance().GetVersion(), size);
}

const char* GetBranchStatic(int32_t &size) {
    return AllocateString(alt::ICore::Instance().GetBranch(), size);
}

uint8_t IsDebugStatic() {
    return alt::ICore::Instance().IsDebug();
}

const char* GetCApiVersion(int32_t &size) {
    return AllocateString(CSHARP_VERSION, size);
}

const char* GetSDKVersion(int32_t &size) {
    return AllocateString(ALT_SDK_VERSION, size);
}

#ifdef ALT_CLIENT_API
void FreeRmlElementArray(alt::IRmlElement** rmlElementArray) {
    delete[] rmlElementArray;
}
#endif
