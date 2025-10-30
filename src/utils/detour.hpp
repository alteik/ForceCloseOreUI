//
// Created by alteik on 30.10.2025.
//

#pragma once

#include <minhook/minhook.h>

class detour
{
public:
    template <typename T, typename C>
    detour(T target, C callback)
    {
        mTarget = cast(target);
        mCallback = cast(callback);
        MH_CreateHook(mTarget, mCallback, &mTrampoline);
    }

    ~detour()
    {
        MH_RemoveHook(mTarget);
    }

    void enable() const
    {
        MH_EnableHook(mTarget);
    }

    void disable() const
    {
        MH_DisableHook(mTarget);
    }

    template<auto callback>
    auto getOriginal() const { return reinterpret_cast<decltype(callback)>(mTrampoline); }

private:
    void* mTarget = nullptr;
    void* mCallback = nullptr;
    void* mTrampoline = nullptr;

    template<typename T>
    static void* cast(T ptr)
    {
        if constexpr (std::is_pointer_v<T>) return const_cast<void*>(static_cast<const void*>(ptr));
        else return reinterpret_cast<void*>(ptr);
    }
};