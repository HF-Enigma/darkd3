#ifndef _UI_STRUCTS_H_
#define _UI_STRUCTS_H_

#include "stdafx.h"
#include "Defines.h"

/*
	UI stuff taken from Zoxc Shade.
*/

struct UIComponent;

struct UIRect
{
	float left;
	float top;
	float right;
	float bottom;
};

//sizeof = 0x48
template<class K, class V> 
struct HashTable
{
	struct Pair
	{
		Pair *next;
        K key;
		V value;
	};

	DWORD mask;                         // 0x000
	DWORD u_1;                          // 0x004
	DWORD u_2;                          // 0x008
	DWORD u_3;                          // 0x00C
	Pair *table;                        // 0x010
};

/* UIEvent
	1.0.3.10235
	MainWndProc - 0x80E5F0: Setups this structure
	dispatch_ui_event - 0x94A8E0: Dispatches the event
*/
struct UIEvent
{
	enum Type
	{
		LeftMouseDown = 6,
		LeftMouseUp = 7,
		LeftMouseDblClick = 8,

		MiddleMouseDown = 11,
		MiddleMouseUp = 12,
		MiddleMouseDblClick = 13,

		RightMouseDown = 16,
		RightMouseUp = 17,
		RightMouseDblClick = 18,

	};

	Type type;      // 0x000

	void *u_0[6];   // 0x004
	int x;          // 0x01C
	int y;          // 0x020
	void *u_1[9];   // 0x024
};

//sizeof = 0x208
struct UIReference
{
	ULONGLONG hash;							// 0x000
	char name[0x200];						// 0x008
};

struct UIReference2
{
    DWORD align32;
    ULONGLONG hash;
};

struct UIHandler
{
	const void *name; // Ignored if this is 0
	DWORD hash;

	typedef void (*func_t)(UIReference *element);

	func_t execute;
};

struct UIComponentVirtualTable
{
	void *u_0[8];
	void (__thiscall *event)(UIComponent *self, int *, UIEvent *event);
	void *u_1[3];
	void (__thiscall *mouse_move)(UIComponent *self);
	void *u_2[4];
	void (__thiscall *mouse_enter)(UIComponent *self);
	void (__thiscall *mouse_leave)(UIComponent *self);
	void *u_3[14];
	void (__thiscall *set_text)(UIComponent *self, const char *text, int);
	void *u_4[2];
	void (__thiscall *switch_state)(UIComponent *self, int, int);
};

//sizeof = 0xCF0
struct UIComponent
{
	/* 1.0.3.10235 Vtable types:
		0x13ED3D8: UIShortcut > UIComponent
		0x13ED258: UIDrawHook > UIComponent
		0x13D7478: UIEvent > UIComponent
		0x13E3D80: UITimer > UIComponent
		0x13D2BD8: Unknown > UIContainer
		0x13E25B8: UIButton > UIControl
		0x13A2760: UILabel > UIControl
		0x13D4EB8: UIEdit > UIControl
		0x13EAFF0: UICheckbox > UIControl
		0x13ECFE0: UIScrollbarThumb > UIContainer
		0x13E8BF0: Unknown > UIContainer
		0x13EB408: Unknown > UIContainer
	*/

    UIComponentVirtualTable *v_table;			// 0x000
    void *u_0;									// 0x004
    UIHandler::func_t handler_0;				// 0x008
    void *u_1[1];								// 0x00C
    UIHandler::func_t handler_1;				// 0x010
    void *u_2[2];								// 0x014
    DWORD sound_0;								// 0x01C
    void *u_3[2];								// 0x020
    DWORD visible;								// 0x028
    DWORD u_4;									// 0x02C
    UIReference self;							// 0x030
    UIReference parent;							// 0x238
    DWORD pad_42C[8];							// 0x440
    DWORD addr_child1;							// 0x460
    DWORD pad_464[20];							// 0x464
    DWORD flag1;								// 0x4B4
    float pad_4E8[8];							// 0x4B8
    UIRect rect;								// 0x4D8
    float pad_518[15];							// 0x4E8
    DWORD click_handler;						// 0x524
    DWORD pad_558[3];							// 0x528
    DWORD mouse_over;							// 0x534
    DWORD pad_568[2];                           // 0x538
    DWORD rclick_handler;                       // 0x540
    float pad_574[325];							// 0x544
    DWORD text_ptr;								// 0xA58
    DWORD pad_A5C[3];							// 0xA5C 
    DWORD text_length;							// 0xA68
    DWORD pad_A6C[133];                         // 0xA6C
    DWORD val_C80;                              // 0xC80
    DWORD pad_C84[129];                         // 0xC84
    DWORD cb_index2;							// 0xE88    0xE98
    DWORD dwBaseAddr;							// 
};

struct UIContainer
{
	DWORD u_0[8];
	UIComponent **children;
	DWORD u_1;
	DWORD child_count;
};

struct UIControl
{
	void *u_0[30];
	DWORD state;
	void *u_1[1];
	DWORD flags; // Controls which UIRect is used
	void *u_2[6];
	UIRect rect_0;
	void *u_3[13];
	UIHandler::func_t click;
	void *u_4[323];
	UIRect rect_1;
	void *u_5[12];
	UIRect rect_2;
	void *u_6[7];
	const char *text;
	void *u_7[90];
	const char *text_dup;
};

typedef HashTable<UIReference2, UIComponent*> UIComponentMap;
typedef HashTable<DWORD, UIHandler *> UIHandlerMap;

struct UIManager
{
	UIComponentMap *component_map;      // 0x000
	void *u_0;                          // 0x004
	UIReference u_1[6];                 // 0x008
	DWORD u_2[1688];
	UIHandlerMap *handler_map;
};

#endif//_UI_STRUCTS_H_