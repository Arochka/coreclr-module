#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wempty-body"
#pragma clang diagnostic ignored "-Wswitch"
#endif
#include "../../cpp-sdk/SDK.h"
#include <../../cpp-sdk/IScriptRuntime.h>
#include "CSharpResourceImpl.h"
#include "CoreClr.h"
#ifdef __clang__
#pragma clang diagnostic pop
#endif

class CSharpScriptRuntime : public alt::IScriptRuntime
{
    alt::IResource::Impl* CreateImpl(alt::IResource *resource) override;
    void DestroyImpl(alt::IResource::Impl *impl) override;

public:
    bool RequiresMain() const override;

public:
    void OnDispose() override;

private:

    void OnTick() override;

  public:
    CSharpScriptRuntime(alt::ICore* server);

    virtual ~CSharpScriptRuntime();

private:
    alt::ICore *core;
    CoreClr* coreClr;
};
