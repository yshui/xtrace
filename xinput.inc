#define EOC {-1, NULL}

static const struct constant c_device_change[] = {
	{0,	"Added"},
	{1,	"Removed"},
	{2,	"Enabled"},
	{3,	"Disabled"},
	{4,	"Unrecoverable"},
	EOC
};

static const struct constant c_device_use[] = {
	{0,	"Pointer"},
	{1,	"Keyboard"},
	{2,	"ExtensionDevice"},
	{3,	"ExtensionKeyboard"},
	{4,	"ExtensionPointer"},
	EOC
};

/* there is only one set of constants in the header files for this,
   so I assume this are the correct values and what is described for most
   other status values in the XIproto.ps is just bogus */
static const struct constant c_device_status[] = {
	{0,	"Success"},
	{1,	"AlreadyGrabbed"},
	{2,	"InvalidTime"},
	{3,	"NotViewable"},
	{4,	"Frozen"},
	EOC
};

static const struct constant c_input_class_id[] = {
	{0,	"key"},
	{1,	"button"},
	{2,	"valuator"},
	{3,	"feedback"},
	{4,	"proximity"},
	{5,	"focus"},
	{6,	"other"},
	EOC
};

static const struct constant c_device_mode[] = {
	{0,	"relative"},
	{1,	"absolute"},
	EOC
};

static const struct constant c_classes_reported[] = {
	{0x01,	"reporting keys"},
	{0x02,	"reporting buttons"},
	{0x04,	"reporting valuators"},
	{0x40,	"absolute"},
	{0x80,	"out of proximity"},
	EOC
};

#undef EOC
#define EOP {0,NULL,0,NULL}
#define LC(len) {len,NULL,0,NULL}

static const struct parameter parameterXInputFake[] = {
	{	 0,	"unsupported",	ft_LISTofCARD8,	NULL},
	EOP
};

static const struct parameter returnXInputFake[] = {
	{	 0,	"unsupported",	ft_LISTofCARD8,	NULL},
	EOP
};

static const struct parameter parameterXInputGet[] = {
	EOP
};

static const struct parameter parameterXInputGetExtensionVersion[] = {
	{	 4,	"len",	ft_COUNT16,	NULL},
	{	 8,	"name",	ft_STRING8,	NULL},
	EOP
};

static const struct parameter returnXInputGetExtensionVersion[] = {
	{	 8,	"major_version",	ft_UINT16,	NULL},
	{	10,	"minor_version",	ft_UINT16,	NULL},
	{	12,	"present",	ft_BOOL},
	EOP
};

#define parameterXInputListInputDevices parameterXInputGet

static const struct parameter structAXISINFO[] = {
	LC(12),
	{	0,	"resolution",	ft_UINT32,	0},
	{	4,	"min",	ft_UINT32,	0},
	{	8,	"max",	ft_UINT32,	0},
	EOP
};
#define ft_LISTofAXISINFO	ft_LISTofStruct, (void*)structAXISINFO

static const struct parameter structpartKEYINFO[] = {
	{	2,	"min keycode",	ft_CARD8,	NULL},
	{	3,	"max keycode",	ft_CARD8,	NULL},
	{	4,	"#keys",	ft_UINT16,	NULL},
	{ 	8,	"",	ft_LASTMARKER,	NULL},
	EOP
};
static const struct parameter structpartBUTTONINFO[] = {
	{	2,	"#buttons",	ft_CARD16,	NULL},
	{ 	4,	"",	ft_LASTMARKER,	NULL},
	EOP
};
static const struct parameter structpartVALUATORINFO[] = {
	{	2,	"#axes",	ft_CARD16,	NULL},
	{	3,	"mode",		ft_ENUM8,	c_device_mode},
	{ 	4,	"motion buffer size",	ft_UINT32,	NULL},
	{	2,	"#axes",	ft_COUNT16,	NULL},
	{	8,	"axes",	ft_LISTofAXISINFO},
	EOP
};

/* that's what xspecs says it would like:

static const struct parameter structINPUTINFO[] = {
	LC(4),
//	{	0,	"class id",	ft_ENUM8,	c_class_id},
	{	0,	"\0",	ft_IF8,	(void*)structpartKEYINFO},
	{	0,	"\1",	ft_IF8,	(void*)structpartBUTTONINFO},
	{	0,	"\2",	ft_IF8,	(void*)structpartVALUATORINFO},
	{	1,	"len",	ft_COUNT8,	NULL},
	{	0,	"unsupported",	ft_LISTofCARD8,	NULL},
	EOP
};
#define ft_LISTofINPUTINFO	ft_LISTofVarStruct, (void*)structINPUTINFO

static const struct parameter structSTR[] = {
	LC(1),
	{	0,	"len",	ft_COUNT8,	NULL},
	{	1,	"value",	ft_STRING8, NULL},
	{ ROUND_32,	"",	ft_LASTMARKER,	NULL},
	EOP
};
#define ft_STR	ft_Struct, (void*)structSTR

static const struct parameter structDEVICEINFO[] = {
	LC(8),
	{	0,	"type",	ft_CARD32,	NULL},
	{	4,	"id",	ft_CARD8,	NULL},
	{	6,	"use",	ft_ENUM8,	c_device_use},
	{	5,	"#classes",	ft_COUNT8,	NULL},
	{	8,	"classes",	ft_LISTofINPUTINFO},
	{ OFS_LATER,	"name",	ft_STR},
	EOP
};
#define ft_LISTofDEVICEINFO	ft_LISTofVarStruct, (void*)structDEVICEINFO

static const struct parameter returnXInputListInputDevices[] = {
	{	 8,	"#devices",	ft_COUNT8,	NULL},
	{	32,	"devices",	ft_LISTofDEVICEINFO},
	EOP
};

but the implementation in reality it is totally different:

first come all the device structs, then all the INPUTINFO structs,
then all the names. To know how many INPUTINFO structs there are,
one would need to sum the numbers from the devices, which we cannot
do yet. So we assume that none will have a name with at most 2 characters:
 */

static const struct parameter structINPUTINFO[] = {
	LC(4),
	{	0,	"\0",	ft_IF8,	(void*)structpartKEYINFO},
	{	0,	"\1",	ft_IF8,	(void*)structpartBUTTONINFO},
	{	0,	"\2",	ft_IF8,	(void*)structpartVALUATORINFO},
/* otherwise we assume that it is time for the strings */
	{	0,	"len",	ft_COUNT8,	NULL},
	{	1,	"name",	ft_STRING8, NULL},
	EOP
};
#define ft_LISTofINPUTINFO	ft_LISTofVarStruct, (void*)structINPUTINFO

static const struct parameter structDEVICEINFO[] = {
	LC(8),
	{	0,	"type",	ft_CARD32,	NULL},
	{	4,	"id",	ft_CARD8,	NULL},
	{	5,	"#classes",	ft_UINT8,	NULL},
	{	6,	"use",	ft_ENUM8,	c_device_use},
	EOP
};
#define ft_LISTofDEVICEINFO	ft_LISTofStruct, (void*)structDEVICEINFO

static const struct parameter returnXInputListInputDevices[] = {
	{	 8,	"#devices",	ft_COUNT8,	NULL},
	{	32,	"devices",	ft_LISTofDEVICEINFO},
	RESET_COUNTER,
	{ OFS_LATER,	"infos",	ft_LISTofINPUTINFO},
	EOP
};

/* The rest I had no program to test with. Judging from the problems
   above, that might mean the actual code is totally different, too */

static const struct parameter parameterXInputDevice[] = {
	{	 4,	"device id",	ft_CARD8,	NULL},
	EOP
};
#define  parameterXInputOpenDevice parameterXInputDevice

static const struct parameter structINPUTCLASSINFO[] = {
	LC(2),
	{	0,	"class",	ft_ENUM8,	c_input_class_id},
	{	1,	"event type base",	ft_CARD8,	NULL},
	EOP
};

#define ft_LISTofINPUTCLASSINFO	ft_LISTofStruct, (void*)structDEVICEINFO
static const struct parameter returnXInputOpenDevice[] = {
	{	 8,	"#classes",	ft_COUNT8,	NULL},
	{	32,	"classes",	ft_LISTofINPUTCLASSINFO},
	EOP
};

#define  parameterXInputCloseDevice parameterXInputDevice

static const struct parameter parameterXInputSetDeviceMode[] = {
	{	 4,	"device id",	ft_CARD8,	NULL},
	{	 5,	"mode",	ft_ENUM8,	c_device_mode},
	EOP
};

static const struct parameter returnXInputStatus[] = {
	{	 8,	"status",	ft_CARD8,	c_device_status},
	EOP
};
#define returnXInputSetDeviceMode returnXInputStatus

#define ft_LISTofEVENTCLASS ft_LISTofCARD32, NULL

static const struct parameter parameterXInputSelectExtensionEvent[] = {
	{	 4,	"window",	ft_WINDOW,	NULL},
	{	 8,	"count",	ft_UINT16,	NULL},
	{	12,	"desired events", ft_LISTofEVENTCLASS},
	EOP
};

static const struct parameter parameterXInputGetSelectedExtensionEvents[] = {
	{	 4,	"window",	ft_WINDOW,	NULL},
	EOP
};
static const struct parameter returnXInputGetSelectedExtensionEvents[] = {
	{	 8,	"#this client",	ft_COUNT16,	NULL},
	{	32,	"this client",	ft_LISTofEVENTCLASS},
	{	10,	"#all clients",	ft_COUNT16,	NULL},
	{ OFS_LATER,	"all clients",	ft_LISTofEVENTCLASS},
	EOP
};

static const struct parameter parameterXInputChangeDeviceDontPropagateList[] = {
	{	 4,	"window",	ft_WINDOW,	NULL},
	{	 8,	"count",	ft_COUNT16,	NULL},
	{	10,	"mode",	ft_ENUM8,	c_device_mode},
	{	12,	"desired events", ft_LISTofEVENTCLASS},
	EOP
};

#define parameterXInputGetDeviceDontPropagateList parameterXInputGetSelectedExtensionEvents

static const struct parameter returnXInputGetDeviceDontPropagateList[] = {
	{	 8,	"count",	ft_COUNT16,	NULL},
	{	32,	"list",	ft_LISTofEVENTCLASS},
	EOP
};

static const struct parameter parameterXInputGetDeviceMotionEvents[] = {
	{	 4,	"start-time",	ft_TIMESTAMP,	c_curtime},
	{	 8,	"stop-time",	ft_TIMESTAMP,	c_curtime},
	{	10,	"device-id",	ft_CARD8,	NULL},
	EOP
};

static const struct parameter structDEVICETIMECOORD[] = {
	LC(4),
	{	 0,	"time",	ft_TIMESTAMP,	NULL},
	{	 0,	"axes",	ft_GET,	NULL},
	{	 0,	"valuators",	ft_LISTofUINT32,	NULL},
	EOP
};
#define ft_LISTofDEVICETIMECOORD ft_LISTofVarStruct, (void*)structDEVICETIMECOORD

static const struct parameter returnXInputGetDeviceMotionEvents[] = {
	{	 8,	"#events",	ft_UINT32,	NULL},
	{	12,	"axes",	ft_UINT8,	NULL},
	{	12,	"axes",	ft_PUSH8,	NULL},
	{	13,	"mode",	ft_ENUM8,	c_device_mode},
	{ 	32,	"events",	ft_LISTofDEVICETIMECOORD},
	EOP
};

#define parameterXInputChangeKeyboardDevice parameterXInputDevice
#define returnXInputChangeKeyboardDevice returnXInputStatus

static const struct parameter parameterXInputChangePointerDevice[] = {
	{	 4,	"xaxis",	ft_CARD8,	NULL},
	{	 5,	"yaxis",	ft_CARD8,	NULL},
	{	 6,	"device id",	ft_CARD8,	NULL},
	EOP
};
#define returnXInputChangePointerDevice returnXInputStatus

static const struct parameter parameterXInputGrabDevice[] = {
	{	 4,	"window",	ft_WINDOW,	NULL},
	{	 8,	"time",	ft_TIMESTAMP,	c_curtime},
	{	12,	"#event",	ft_COUNT16,	NULL},
	{	14,	"this-device-mode",	ft_ENUM8,	c_synmode},
	{	15,	"other-device-mode",	ft_ENUM8,	c_synmode},
	{	16,	"owner-events",	ft_BOOL},
	{	17,	"device id",	ft_CARD8,	NULL},
	{	20,	"events",	ft_LISTofEVENTCLASS},
	EOP
};
#define returnXInputGrabDevice returnXInputStatus

static const struct parameter parameterXInputUngrabDevice[] = {
	{	 4,	"time",	ft_TIMESTAMP,	c_curtime},
	{	 8,	"device id",	ft_CARD8,	NULL},
	EOP
};

#define  parameterXInputGetDeviceFocus parameterXInputDevice
#define  parameterXInputGetFeedbackControl parameterXInputDevice
#define  parameterXInputGetDeviceButtonMapping parameterXInputDevice
#define  parameterXInputQueryDeviceState parameterXInputDevice


#define ra(a) { #a , parameterXInput ## a , NULL,NULL,NULL}
#define rb(a) { #a , parameterXInput ## a , returnXInput ## a ,NULL,NULL}
#define re(a) { #a , parameterXInputFake , NULL, NULL,NULL}
#define rf(a) { #a , parameterXInputFake , returnXInputFake, NULL,NULL}
static const struct request extensionXInput[] = {
	re(Nothing), /* 0 */
	rb(GetExtensionVersion), /* 1 */
	rb(ListInputDevices), /* 2 */
	rb(OpenDevice), /* 3 */
	ra(CloseDevice), /* 4 */
	rb(SetDeviceMode), /* 5 */
	ra(SelectExtensionEvent), /* 6 */
	rb(GetSelectedExtensionEvents), /* 7 */
	ra(ChangeDeviceDontPropagateList), /* 8 */
	rb(GetDeviceDontPropagateList), /* 9 */
	rb(GetDeviceMotionEvents), /* 10 */
	rb(ChangeKeyboardDevice), /* 11 */
	rb(ChangePointerDevice), /* 12 */
	rb(GrabDevice), /* 13 */
	ra(UngrabDevice), /* 14 */
	re(GrabDeviceKey), /* 15 */
	re(UngrabDeviceKey), /* 16 */
	re(GrabDeviceButton), /* 17 */
	re(UngrabDeviceButton), /* 18 */
	re(AllowDeviceEvents), /* 19 */
	rf(GetDeviceFocus), /* 20 */
	re(SetDeviceFocus), /* 21 */
	rf(GetFeedbackControl), /* 22 */
	re(ChangeFeedbackControl), /* 23 */
	rf(GetDeviceKeyMapping), /* 24 */
	re(ChangeDeviceKeyMapping), /* 25 */
	rf(GetDeviceModifierMapping), /* 26 */
	rf(SetDeviceModifierMapping), /* 27 */
	rf(GetDeviceButtonMapping), /* 28 */
	rf(SetDeviceButtonMapping), /* 29 */
	rf(QueryDeviceState), /* 30 */
	re(SendExtensionEvent), /* 31 */
	re(DeviceBell), /* 32 */
	rf(SetDeviceValuators), /* 33 */
	rf(GetDeviceControl), /* 34 */
	rf(ChangeDeviceControl) /* 35 */
};
#undef ra
#undef rb
#undef re
#undef rf

/* Events */

/* the top-most bit of the device-id (0x80) always means more events to follow.
   how to express that? */

static const struct parameter eparamXInputDeviceValuator[] = {
	{	 1,	"device-id",	ft_CARD8,	NULL},
	{	 4,	"state",	ft_SETofKEYBUTMASK},
	{	 6,	"#valuators",	ft_COUNT8,	NULL},
	{	 7,	"first axis",	ft_CARD8,	NULL},
	/* should be LISTofINT32? */
	{	 8,	"valuators",	ft_LISTofUINT32,	NULL},
	EOP
};

static const struct parameter eparamXInputKeyButtonPointer[] = {
	/* keycode, button, ... depending on actual event */
	{	 1,	"detail",	ft_CARD8,	NULL},
	{        4,     "timestamp",    ft_TIMESTAMP,   NULL},
	{	 8,	"root window",	ft_WINDOW,	NULL},
	{	12,	"event window",	ft_WINDOW,	NULL},
	{	16,	"child window",	ft_WINDOW,	c_none},
	{	20,	"root-x",	ft_INT16,	NULL},
	{	22,	"root-y",	ft_INT16,	NULL},
	{	24,	"event-x",	ft_INT16,	NULL},
	{	26,	"event-y",	ft_INT16,	NULL},
	{	28,	"state",	ft_SETofKEYBUTMASK},
	{	30,	"same-screen",	ft_BOOL},
	{	31,	"device-id",	ft_CARD8,	NULL},
	EOP
};
#define eparamXInputDeviceKeyPress eparamXInputKeyButtonPointer
#define eparamXInputDeviceKeyRelease eparamXInputKeyButtonPointer
#define eparamXInputDeviceButtonPress eparamXInputKeyButtonPointer
#define eparamXInputDeviceButtonRelease eparamXInputKeyButtonPointer

static const struct parameter eparamXInputDeviceMotionNotify[] = {
	{	 1,	"detail",	ft_ENUM8,	c_normalhint},
	{        4,     "timestamp",    ft_TIMESTAMP,   NULL},
	{	 8,	"root window",	ft_WINDOW,	NULL},
	{	12,	"event window",	ft_WINDOW,	NULL},
	{	16,	"child window",	ft_WINDOW,	c_none},
	{	20,	"root-x",	ft_INT16,	NULL},
	{	22,	"root-y",	ft_INT16,	NULL},
	{	24,	"event-x",	ft_INT16,	NULL},
	{	26,	"event-y",	ft_INT16,	NULL},
	{	28,	"state",	ft_SETofKEYBUTMASK},
	{	30,	"same-screen",	ft_BOOL},
	{	31,	"device-id",	ft_CARD8,	NULL},
	EOP
};

static const struct parameter eparamXInputProximity[] = {
	{        4,     "timestamp",    ft_TIMESTAMP,   NULL},
	{	 8,	"root window",	ft_WINDOW,	NULL},
	{	12,	"event window",	ft_WINDOW,	NULL},
	{	16,	"child window",	ft_WINDOW,	c_none},
	{	20,	"root-x",	ft_INT16,	NULL},
	{	22,	"root-y",	ft_INT16,	NULL},
	{	24,	"event-x",	ft_INT16,	NULL},
	{	26,	"event-y",	ft_INT16,	NULL},
	{	28,	"state",	ft_SETofKEYBUTMASK},
	{	30,	"same-screen",	ft_BOOL},
	{	31,	"device-id",	ft_CARD8,	NULL},
	EOP
};
#define eparamXInputProximityIn eparamXInputProximity
#define eparamXInputProximityOut eparamXInputProximity

static const struct parameter eparamXInputDeviceFocus[] = {
	{	 1,	"detail",	ft_ENUM8,	c_enterdetail},
	{        4,     "timestamp",    ft_TIMESTAMP,   NULL},
	{	 8,	"event window",	ft_WINDOW,	NULL},
	{	12,	"mode",		ft_ENUM8,	c_grabmode},
	{	13,	"device-id",	ft_CARD8,	NULL},
	EOP
};
#define eparamXInputDeviceFocusOut eparamXInputDeviceFocus
#define eparamXInputDeviceFocusIn eparamXInputDeviceFocus

static const struct parameter eparamXInputDeviceStateNotify[] = {
	{	 1,	"device-id",	ft_CARD8,	NULL},
	{        4,     "timestamp",    ft_TIMESTAMP,   NULL},
	{	11,	"reported",	ft_BITMASK8,	c_classes_reported},
	{	 8,	"#keys",	ft_COUNT8,	NULL},
	{	12,	"keys",		ft_LISTofCARD8,	NULL},
	{	 9,	"#buttons",	ft_COUNT8,	NULL},
	{ OFS_LATER,	"buttons",	ft_LISTofCARD8,	NULL},
	{	10,	"#valuators",	ft_COUNT8,	NULL},
	{ OFS_LATER,	"valuators",	ft_LISTofCARD32,	NULL},
	EOP
};

static const struct parameter eparamXInputDeviceKeystateNotify[] = {
	{	 1,	"device-id",	ft_CARD8,	NULL},
	{	 4,	"keys",		ft_LISTofCARD8,	NULL},
	EOP
};

static const struct parameter eparamXInputDeviceButtonstateNotify[] = {
	{	 1,	"device-id",	ft_CARD8,	NULL},
	{	 4,	"buttons",	ft_LISTofCARD8,	NULL},
	EOP
};

static const struct parameter eparamXInputDeviceMappingNotify[] = {
	{	 1,	"device-id",	ft_CARD8,	NULL},
	{	 4,	"request",	ft_CARD8,	NULL},
	{	 5,	"first key code",	ft_KEYCODE,	NULL},
	{	 6,	"count",	ft_UINT8,	NULL},
	{        8,     "timestamp",    ft_TIMESTAMP,   NULL},
	EOP
};

static const struct parameter eparamXInputChangeDeviceNotify[] = {
	{	 1,	"device-id",	ft_CARD8,	NULL},
	{        4,     "timestamp",    ft_TIMESTAMP,   NULL},
	{	 8,	"request",	ft_CARD8,	NULL},
	EOP
};

static const struct parameter eparamXInputDevicePresenceNotify[] = {
	{        4,     "timestamp",    ft_TIMESTAMP,   NULL},
	{	 8,	"change",	ft_ENUM8,	c_device_change},
	{	 9,	"device-id",	ft_CARD8,	NULL},
	{	10,	"control",	ft_CARD16,	NULL},
	EOP
};

#define ev(name) { "XINPUT " #name , eparamXInput ## name }
static const struct event eventsXInput[16] = {
	ev(DeviceValuator),
	ev(DeviceKeyPress),
	ev(DeviceKeyRelease),
	ev(DeviceButtonPress),
	ev(DeviceButtonRelease),
	ev(DeviceMotionNotify),
	ev(DeviceFocusIn),
	ev(DeviceFocusOut),
	ev(ProximityIn),
	ev(ProximityOut),
	ev(DeviceStateNotify),
	ev(DeviceMappingNotify),
	ev(ChangeDeviceNotify),
	ev(DeviceKeystateNotify),
	ev(DeviceButtonstateNotify),
	ev(DevicePresenceNotify)
};
#undef ev

static const char *errorsXInput[5] = {
	"BadDevice",
	"BadEvent",
	"BadMode",
	"DeviceBusy",
	"BadClass"
};
#undef EOP