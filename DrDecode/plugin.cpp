#include "plugin.h"

enum
{
	MENU_ENABLED,
};

static bool regstepEnabled = true;

PLUG_EXPORT void CBMENUENTRY(CBTYPE, PLUG_CB_MENUENTRY* info)
{
	switch (info->hEntry)
	{
	case MENU_ENABLED:
	{
		regstepEnabled = !regstepEnabled;
		BridgeSettingSetUint("regstep", "Enabled", regstepEnabled);
	}
	break;
	}
}

void OnDbgEvent(CBTYPE cbType, void* callbackInfo)
{
	if (!regstepEnabled)
		return;
	if (cbType == CB_EXCEPTION)
	{
		PLUG_CB_EXCEPTION* ex = (PLUG_CB_EXCEPTION*)callbackInfo;
		EXCEPTION_RECORD* record = &ex->Exception->ExceptionRecord;
		if (ex->Exception->ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT) // INT3
		{
			dprintf("===========================================\n");
			duint EIPValue = (duint)record->ExceptionAddress;
			dprintf(u8"x64dbg 捕获到 int3 异常，当前EIP地址为=0x%08X\n", EIPValue);
			BASIC_INSTRUCTION_INFO Assembly_Value = {};
			DbgDisasmFastAt(EIPValue, &Assembly_Value);
			// 判断是否是 int3
			if (!strcmp(Assembly_Value.instruction,"int3"))
			{
				duint NextAdr = EIPValue + 1;
				dprintf(u8"加1之后的地址为:0x%08X\n", NextAdr);
				BASIC_INSTRUCTION_INFO Assembly_Value_NEXT = {};
				DbgDisasmFastAt(NextAdr, &Assembly_Value_NEXT);
				duint xorValue = 0;
				DbgMemRead(NextAdr, &xorValue, 1);
				dprintf(u8"加1之后的汇编代码为:%s,对应的机器码为:0x%08X\n", Assembly_Value_NEXT.instruction, xorValue);
				duint Offset = xorValue ^ 0xA3;
				dprintf(u8"偏移为::0x%08X\n", Offset);
				duint StartAdr = EIPValue + 2;
				dprintf(u8"起始范围为:0x%08X\n", StartAdr);
				duint EndAdr = EIPValue - 1 + Offset;
				dprintf(u8"终止范围为:0x%08X\n", EndAdr);
				duint CheatEipValue = EIPValue + Offset;
				// 计算范围内是否还是int3
				for (auto i = StartAdr; i <= EndAdr; i++)
				{
					BASIC_INSTRUCTION_INFO Assembly_Value_RUN = {};
					DbgDisasmFastAt(i, &Assembly_Value_RUN);
					dprintf(u8"当前的地址为:0x%08X,对应的汇编代码为:%s\n", i, Assembly_Value_RUN.instruction);
					// 不等于 int3
					if (!strcmp(Assembly_Value_RUN.instruction, "int3")) {
						dprintf(u8"检测到:0x%08X地址的汇编代码为 int3 则不会写入EIP\n", i);
						return;
					}
				}
				CONTEXT ctx = {};
				ctx.ContextFlags = CONTEXT_FULL;
				if (GetThreadContext(DbgGetThreadHandle(), &ctx))
				{
					ctx.Eip = CheatEipValue; // x86
					SetThreadContext(DbgGetThreadHandle(), &ctx); // 写回
					dprintf(u8"写入解密EIP:0x%08X\n", CheatEipValue);
				}
				return;
			}
			return;
		}
		return;
	}
	return;
}
//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
	duint setting = regstepEnabled;
	BridgeSettingGetUint("regstep", "Enabled", &setting);
	regstepEnabled = !!setting;
	// 注册异常回调
	_plugin_registercallback(initStruct->pluginHandle, CB_EXCEPTION, OnDbgEvent);
	return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here.
void pluginStop()
{
}

//Do GUI/Menu related things here.
void pluginSetup()
{
	_plugin_menuaddentry(hMenu, MENU_ENABLED, "Enabled");
	_plugin_menuentrysetchecked(pluginHandle, MENU_ENABLED, regstepEnabled);

}
