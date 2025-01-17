#include "AcespaceEngine.h"
#include "Actor.h"
#include "GameKernel.h"
#include <windows.h>

extern "C" {
    CGameKernel* GetKernel() { return CGameKernel::getInstancePtr(); }
    bool KernelInit(CGameKernel* ptr) { return ptr->initialize(GetModuleHandle(nullptr)); }
    void KernelDestroy(CGameKernel* ptr) { ptr->deleteInstance(); }
    bool KernelTick(CGameKernel* ptr) { return ptr->tick(); }
}
