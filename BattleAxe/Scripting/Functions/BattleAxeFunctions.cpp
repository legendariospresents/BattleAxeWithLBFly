#include "BattleAxeFunctions.h"

JsValueRef CALLBACK theBattleFunctions::getCommandManager(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return reinterpret_cast<ContextObjects*>(callbackState)->commandManager;
}

JsValueRef CALLBACK theBattleFunctions::getModuleManager(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return reinterpret_cast<ContextObjects*>(callbackState)->moduleManager;
}

JsValueRef CALLBACK theBattleFunctions::getDrawUtils(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return reinterpret_cast<ContextObjects*>(callbackState)->drawUtils;
}