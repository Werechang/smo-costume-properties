#include <mallow/mallow.hpp>

#include "ActorFactory/actorPatches.h"

#include "Library/Base/StringUtil.h"
#include "Library/Controller/InputFunction.h"
#include "Library/File/FileUtil.h"
#include "Library/Resource/ResourceHolder.h"
#include "ModOptions.h"
#include "Player/PlayerActorBase.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerCostumeInfo.h"
#include "Player/PlayerFunction.h"
#include "Scene/StageScene.h"

using mallow::log::logLine;

struct ScenePlayHook : public mallow::hook::Trampoline<ScenePlayHook> {
    static void Callback(StageScene* thisPtr) {
        if (al::isPadTriggerL(-1) && mallow::config::getConfg<ModOptions>()->myModOption)
            logLine("You pressed L while the game was unpaused and myModOption was enabled");
        Orig(thisPtr);
    }
};

const char* bodyCostume = nullptr;

struct PlayerActorInitHook : public mallow::hook::Inline<PlayerActorInitHook> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        bodyCostume = reinterpret_cast<const char*>(ctx->X[2]);
    }
};

struct PlayerConstInitHook : public mallow::hook::Inline<PlayerConstInitHook> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        const char* costumeConstFile = "ObjectData/CostumePlayerConst";


        auto playerConstAppendix = reinterpret_cast<const char*>(ctx->X[0]);
        if (bodyCostume == nullptr) {
            ctx->X[0] =
                reinterpret_cast<u64>(PlayerFunction::createMarioConst(playerConstAppendix));
            return;
        }
        if (!al::isExistArchive(costumeConstFile)) {
            mallow::log::logLine("%s not found!", costumeConstFile);
            ctx->X[0] =
                reinterpret_cast<u64>(PlayerFunction::createMarioConst(playerConstAppendix));
            bodyCostume = nullptr;
            return;
        }
        al::StringTmp<0x80> bymlFile{"%s%s", bodyCostume, playerConstAppendix};
        u8* byml = al::tryGetBymlFromObjectResource(costumeConstFile, bymlFile);
        if (!byml) {
            mallow::log::logLine("PlayerConst Byaml not in archive for costume %s!", bodyCostume);
            ctx->X[0] =
                reinterpret_cast<u64>(PlayerFunction::createMarioConst(playerConstAppendix));
            bodyCostume = nullptr;
            return;
        }
        ctx->X[0] = reinterpret_cast<u64>(new PlayerConst(al::ByamlIter(byml)));
        bodyCostume = nullptr;
    }
};

struct PlayerAnimReplaceResourceHook : public mallow::hook::Inline<PlayerAnimReplaceResourceHook> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        const char* costumeAnimFile = "ObjectData/CostumeAnimations";


        const char* body = reinterpret_cast<const char*>(ctx->X[28]);
        if (!al::isExistArchive(costumeAnimFile)) {
            mallow::log::logLine("%s not found!", costumeAnimFile);
            bodyCostume = nullptr;
            return;
        }
        u8* byml = al::tryGetBymlFromObjectResource(costumeAnimFile, body);
        if (!byml) {
            mallow::log::logLine("Animation Byaml not in archive for costume %s!", body);
            bodyCostume = nullptr;
            return;
        }
        al::ByamlIter rootIt{byml};
        const char* animFile = nullptr;
        if (!rootIt.tryGetStringByKey(&animFile, "Name")) {
            mallow::log::logLine("Name property not found in byaml for %s", body);
            bodyCostume = nullptr;
            return;
        }
        ctx->X[2] = reinterpret_cast<u64>(animFile);
    }
};

struct PlayerAnimReplaceModelHook : public mallow::hook::Inline<PlayerAnimReplaceModelHook> {
    static void Callback(exl::hook::InlineCtx* ctx) {
        if (bodyCostume != nullptr)
            ctx->X[2] = reinterpret_cast<u64>(bodyCostume);
    }
};

extern "C" void userMain() {
    exl::hook::Initialize();
    mallow::init::installHooks();
    ca::actorPatches();
    PlayerConstInitHook::InstallAtOffset(0x41b708);
    exl::patch::CodePatcher patcher{0x41b704};
    patcher.WriteInst(exl::armv8::inst::Nop());

    ScenePlayHook::InstallAtSymbol("_ZN10StageScene7exePlayEv");
    PlayerActorInitHook::InstallAtOffset(0x41b4e4);
    PlayerAnimReplaceResourceHook::InstallAtOffset(0x4440c4);
    PlayerAnimReplaceModelHook::InstallAtOffset(0x444158);
}
