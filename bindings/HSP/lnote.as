//=============================================================================
// LNote
//=============================================================================

#include "user32.as"

#ifndef __lnote__
#define __lnote__

#uselib "LNote.dll"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#define LN_MUSICTIMEBASE 768
//#define LN_EXCEPTION_WIN32_MESSAGE 0xA000

//-----------------------------------------------------------------------------
// enum
//-----------------------------------------------------------------------------
// LNBool
#const LN_FALSE 0
#const LN_TRUE 1

// LNResult
#const LN_OK 0
#const LN_ERROR_UNKNOWN -1
#const LN_ERROR_OUT_OF_MEMORY -2
#const LN_ERROR_IO -3
#const LN_ERROR_END_OF_STREAM -4
#const LN_ERROR_ARGUMENT -5
#const LN_ERROR_INVALID_OPERATION -6
#const LN_ERROR_NOT_SUPPORTED -7
#const LN_ERROR_FILE_NOT_FOUND -8
#const LN_ERROR_DIRECTORY_NOT_FOUND -9
#const LN_ERROR_INVALID_FORMAT -10
#const LN_ERROR_NOT_IMPLEMENTED -11
#const LN_ERROR_RUNTIME -12
#const LN_ERROR_ENCODING_FALLBACK -13
#const LN_ERROR_WIN32 -14
#const LN_ERROR_COM -15

// LNVariantType
#const LN_VARIANTTYPE_UNKNOWN 0
#const LN_VARIANTTYPE_BOOL 1
#const LN_VARIANTTYPE_INT 2
#const LN_VARIANTTYPE_FLOAT 3
#const LN_VARIANTTYPE_STRING 4
#const LN_VARIANTTYPE_LIST 5
#const LN_VARIANTTYPE_OBJECT 6
#const LN_VARIANTTYPE_SIZE_F 7

// LNFileAccessPriority
#const LN_FILEACCESSPRIORITY_ARCHIVE_FIRST 0
#const LN_FILEACCESSPRIORITY_DIRECTORY_FIRST 1
#const LN_FILEACCESSPRIORITY_ARCHIVE_ONLY 2

// LNFileType
#const LN_FILETYPE_BINARY 0
#const LN_FILETYPE_TEXT 1

// LNFileMode
#const LN_FILEMODE_OPEN 0
#const LN_FILEMODE_CREATE 1
#const LN_FILEMODE_APPEND 2

// LNFileAccess
#const LN_FILEACCESS_READ 0
#const LN_FILEACCESS_READWRITE 1
#const LN_FILEACCESS_WRITE 2

// LNSeekOrigin
#const LN_SEEK_BEGIN 0
#const LN_SEEK_CURRENT 1
#const LN_SEEK_END 2

// LNAsyncIOState
#const LN_ASYNCIOSTATE_IDLE 0
#const LN_ASYNCIOSTATE_READY 1
#const LN_ASYNCIOSTATE_PROCESSING 2
#const LN_ASYNCIOSTATE_COMPLETED 3
#const LN_ASYNCIOSTATE_FAILED 4

// LNRotationOrder
#const LN_ROTATIONORDER_XYZ 0
#const LN_ROTATIONORDER_XZY 1
#const LN_ROTATIONORDER_YXZ 2
#const LN_ROTATIONORDER_YZX 3
#const LN_ROTATIONORDER_ZXY 4
#const LN_ROTATIONORDER_ZYX 5

// LNKeyCode
#const LN_KEY_UNKNOWN 0
#const LN_KEY_A 1
#const LN_KEY_B 2
#const LN_KEY_C 3
#const LN_KEY_D 4
#const LN_KEY_E 5
#const LN_KEY_F 6
#const LN_KEY_G 7
#const LN_KEY_H 8
#const LN_KEY_I 9
#const LN_KEY_J 10
#const LN_KEY_K 11
#const LN_KEY_L 12
#const LN_KEY_M 13
#const LN_KEY_N 14
#const LN_KEY_O 15
#const LN_KEY_P 16
#const LN_KEY_Q 17
#const LN_KEY_R 18
#const LN_KEY_S 19
#const LN_KEY_T 20
#const LN_KEY_U 21
#const LN_KEY_V 22
#const LN_KEY_W 23
#const LN_KEY_X 24
#const LN_KEY_Y 25
#const LN_KEY_Z 26
#const LN_KEY_F10 46
#const LN_KEY_F11 47
#const LN_KEY_F12 48
#const LN_KEY_SPACE 49
#const LN_KEY_ESCAPE 50
#const LN_KEY_DOWN 52
#const LN_KEY_LEFT 53
#const LN_KEY_RIGHT 54
#const LN_KEY_LSHIFT 55
#const LN_KEY_RSHIFT 56
#const LN_KEY_LCTRL 57
#const LN_KEY_RCTRL 58
#const LN_KEY_LALT 59
#const LN_KEY_RALT 60
#const LN_KEY_TAB 61
#const LN_KEY_ENTER 62
#const LN_KEY_BACKSPACE 63
#const LN_KEY_INSERT 64
#const LN_KEY_DELETE 65
#const LN_KEY_PAGEUP 66
#const LN_KEY_PAGEDOWN 67
#const LN_KEY_HOME 68
#const LN_KEY_END 69
#const LN_KEY_COLON 70
#const LN_KEY_SEMICOLON 71
#const LN_KEY_COMMA 72
#const LN_KEY_PERIOD 73
#const LN_KEY_SLASH 74
#const LN_KEY_MINUS 75
#const LN_KEY_BACKSLASH 76
#const LN_KEY_YEN 77
#const LN_KEY_CARET 78
#const LN_KEY_LBRACKET 79
#const LN_KEY_RBRACKET 80
#const LN_KEY_ANDROID_PLUS 81

// LNAcceleratorKey
#const LN_ACCELERATORKEY_TOGGLE_FULLSCREEN 0
#const LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS 1

// LNButton
#const LN_BUTTON_0 0
#const LN_BUTTON_1 1
#const LN_BUTTON_2 2
#const LN_BUTTON_3 3
#const LN_BUTTON_4 4
#const LN_BUTTON_5 5
#const LN_BUTTON_6 6
#const LN_BUTTON_7 7
#const LN_BUTTON_8 8
#const LN_BUTTON_9 9
#const LN_BUTTON_10 10
#const LN_BUTTON_11 11
#const LN_BUTTON_12 12
#const LN_BUTTON_13 13
#const LN_BUTTON_14 14
#const LN_BUTTON_15 15
#const LN_BUTTON_LEFT 16
#const LN_BUTTON_RIGHT 17
#const LN_BUTTON_UP 18
#const LN_BUTTON_DOWN 19
#const LN_BUTTON_AXIS_1X_MINUIS 20
#const LN_BUTTON_AXIS_1X_PLUS 21
#const LN_BUTTON_AXIS_1Y_MINUIS 22
#const LN_BUTTON_AXIS_1Y_PLUS 23
#const LN_BUTTON_AXIS_2X_MINUIS 24
#const LN_BUTTON_AXIS_2X_PLUS 25
#const LN_BUTTON_AXIS_2Y_MINUIS 26
#const LN_BUTTON_AXIS_2Y_PLUS 27

// LNDeviceButtonType
#const LN_DEVICEBUTTONTYPE_UNKNOWN 0
#const LN_DEVICEBUTTONTYPE_KEYBOARD 1
#const LN_DEVICEBUTTONTYPE_MOUSE 2
#const LN_DEVICEBUTTONTYPE_JOYPAD_BUTTON 3
#const LN_DEVICEBUTTONTYPE_JOYPAD_POV 4
#const LN_DEVICEBUTTONTYPE_JOYPAD_AXIS 5

// LNDeviceButton
#const LN_DEVICEBUTTON_UNKNOWN 0
#const LN_DEVICEBUTTON_MOUSE_1 256
#const LN_DEVICEBUTTON_MOUSE_2 257
#const LN_DEVICEBUTTON_MOUSE_3 258
#const LN_DEVICEBUTTON_MOUSE_4 259
#const LN_DEVICEBUTTON_MOUSE_5 260
#const LN_DEVICEBUTTON_MOUSE_6 261
#const LN_DEVICEBUTTON_MOUSE_7 262
#const LN_DEVICEBUTTON_MOUSE_8 263
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_1 264
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_2 265
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_3 266
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_4 267
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_5 268
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_6 269
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_7 270
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_8 271
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_9 272
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_10 273
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_11 274
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_12 275
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_13 276
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_14 277
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_15 278
#const LN_DEVICEBUTTON_JOYPAD_BUTTON_16 279
#const LN_DEVICEBUTTON_JOYPAD_POV_UP 280
#const LN_DEVICEBUTTON_JOYPAD_POV_RIGHT 281
#const LN_DEVICEBUTTON_JOYPAD_POV_DOWN 282
#const LN_DEVICEBUTTON_JOYPAD_POV_LEFT 283
#const LN_DEVICEBUTTON_JOYPAD_AXIS_1_MINUIS 284
#const LN_DEVICEBUTTON_JOYPAD_AXIS_1_PLUS 285
#const LN_DEVICEBUTTON_JOYPAD_AXIS_2_MINUIS 286
#const LN_DEVICEBUTTON_JOYPAD_AXIS_2_PLUS 287
#const LN_DEVICEBUTTON_JOYPAD_AXIS_3_MINUIS 288
#const LN_DEVICEBUTTON_JOYPAD_AXIS_3_PLUS 289
#const LN_DEVICEBUTTON_JOYPAD_AXIS_4_MINUIS 290
#const LN_DEVICEBUTTON_JOYPAD_AXIS_4_PLUS 291
#const LN_DEVICEBUTTON_JOYPAD_AXIS_5_MINUIS 292
#const LN_DEVICEBUTTON_JOYPAD_AXIS_5_PLUS 293
#const LN_DEVICEBUTTON_JOYPAD_AXIS_6_MINUIS 294
#const LN_DEVICEBUTTON_JOYPAD_AXIS_6_PLUS 295
#const LN_DEVICEBUTTON_JOYPAD_AXIS_7_MINUIS 296
#const LN_DEVICEBUTTON_JOYPAD_AXIS_7_PLUS 297
#const LN_DEVICEBUTTON_JOYPAD_AXIS_8_MINUIS 298
#const LN_DEVICEBUTTON_JOYPAD_AXIS_8_PLUS 299

// LNDirectMusicInitMode
#const LN_DMINITMODE_NOT_USE 0
#const LN_DMINITMODE_NORMAL 1
#const LN_DMINITMODE_THREAD_WAIT 2
#const LN_DMINITMODE_THREAD_REQUEST 3

// LNSoundPlayingType
#const LN_SOUNDPLAYINGTYPE_UNKNOWN 0
#const LN_SOUNDPLAYINGTYPE_AUTO 1
#const LN_SOUNDPLAYINGTYPE_ONMEMORY 2
#const LN_SOUNDPLAYINGTYPE_STREAMING 3
#const LN_SOUNDPLAYINGTYPE_MIDI 4

// LNSoundPlayingState
#const LN_SOUNDPLAYINGSTATE_STOPPED 0
#const LN_SOUNDPLAYINGSTATE_PLAYING 1
#const LN_SOUNDPLAYINGSTATE_PAUSING 2

// LNSoundFadeState
#const LN_SOUNDFADE_CONTINUE 0
#const LN_SOUNDFADE_STOP 1
#const LN_SOUNDFADE_STOP_RESET 2
#const LN_SOUNDFADE_PAUSE 3
#const LN_SOUNDFADE_PAUSE_RESET 4

// LNInternalGameSound
#const LN_INTERNALGAMESOUND_BGM 0
#const LN_INTERNALGAMESOUND_BGS 1
#const LN_INTERNALGAMESOUND_ME 2

// LNBackbufferResizeMode
#const LN_BACKBUFFERRESIZEMODE_SCALING 0
#const LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX 1
#const LN_BACKBUFFERRESIZEMODE_RESIZE 2

// LNTextureFormat
#const LN_FMT_UNKNOWN 0
#const LN_FMT_A8R8G8B8 1
#const LN_FMT_X8R8G8B8 2
#const LN_FMT_A16B16G16R16F 3
#const LN_FMT_A32B32G32R32F 4
#const LN_FMT_D24S8 5

// LNBlendMode
#const LN_BLEND_NORMAL 0
#const LN_BLEND_ALPHA 1
#const LN_BLEND_ADD 2
#const LN_BLEND_ADD_ALPHA_DISABLE 3
#const LN_BLEND_SUB 4
#const LN_BLEND_SUB_ALPHA_DISABLE 5
#const LN_BLEND_MUL 6
#const LN_BLEND_SCREEN 7
#const LN_BLEND_REVERSE 8

// LNCullingMode
#const LN_CULL_FRONT 0
#const LN_CULL_BACK 1
#const LN_CULL_DOUBLE 2

// LNTextAlign
#const LN_TEXTALIGN_LEFT 0
#const LN_TEXTALIGN_CENTER 1
#const LN_TEXTALIGN_RIGHT 2

// LNPlacementType
#const LN_PLACEMENTTYPE_NORMAL 0
#const LN_PLACEMENTTYPE_BILLBOARD 1
#const LN_PLACEMENTTYPE_Y_FIXED 2

// LNFrontAxis
#const LN_FRONTAXIS_X 0
#const LN_FRONTAXIS_Y 1
#const LN_FRONTAXIS_Z 2
#const LN_FRONTAXIS_RX 3
#const LN_FRONTAXIS_RY 4
#const LN_FRONTAXIS_RZ 5

// LNMeshCreateFlags
#const LN_MESHCREATEFLAGS_SOFTWARE_SKINNING 0x01
#const LN_MESHCREATEFLAGS_COLLISION_MESH 0x02
#const LN_MESHCREATEFLAGS_IGNORE_TEXTURE_NOT_FOUND 0x04

// LNCameraType
#const LN_CAMERATYPE_POSITION_AND_LOOKAT 0
#const LN_CAMERATYPE_POSITION_AND_ANGLE 1
#const LN_CAMERATYPE_TRANSFORM_MATRIX 2

// LNCoord
#const LN_COORD_SPACE_2D 0
#const LN_COORD_SPACE_3D 1

// LNObjectType
#const LN_OBJECTTYPE_SOUND 0



//-----------------------------------------------------------------------------
// funcs
//-----------------------------------------------------------------------------
#func _LNObject_Release "LNObject_Release" int
#define LNObject_Release(%1)
#func _LNObject_AddRef "LNObject_AddRef" int
#define LNObject_AddRef(%1)
#func _LNObject_GetRefCount "LNObject_GetRefCount" int, var
#define LNObject_GetRefCount(%1, %2)
#func _LNApplication_Initialize "LNApplication_Initialize" 
#define LNApplication_Initialize
#func _LNApplication_Update "LNApplication_Update" 
#define LNApplication_Update
#func _LNApplication_ResetFrameDelay "LNApplication_ResetFrameDelay" 
#define LNApplication_ResetFrameDelay
#func _LNApplication_IsEndRequested "LNApplication_IsEndRequested" var
#define LNApplication_IsEndRequested(%1)
#func _LNApplication_Terminate "LNApplication_Terminate" 
#define LNApplication_Terminate
#func __typeOverride_LNVector2_GetLength "LNVector2_GetLengthD" int, var
#define LNVector2_GetLength(%1, %2)
#func __typeOverride_LNVector2_GetSquareLength "LNVector2_GetSquareLengthD" int, var
#define LNVector2_GetSquareLength(%1, %2)
#func _LNVector2_Create "LNVector2_CreateD" double, double, var
#define LNVector2_Create(%1, %2, %3)
#func _LNVector2_Normalize "LNVector2_Normalize" int, var
#define LNVector2_Normalize(%1, %2)
#func _LNVector2_NormalizeV "LNVector2_NormalizeV" int
#define LNVector2_NormalizeV(%1)
#func __typeOverride_LNVector3_GetLength "LNVector3_GetLengthD" int, var
#define LNVector3_GetLength(%1, %2)
#func __typeOverride_LNVector3_GetSquareLength "LNVector3_GetSquareLengthD" int, var
#define LNVector3_GetSquareLength(%1, %2)
#func _LNVector3_Create "LNVector3_CreateD" double, double, double, var
#define LNVector3_Create(%1, %2, %3, %4)
#func _LNVector3_CreateVZ "LNVector3_CreateVZD" int, double, var
#define LNVector3_CreateVZ(%1, %2, %3)
#func _LNVector3_Normalize "LNVector3_Normalize" int, var
#define LNVector3_Normalize(%1, %2)
#func _LNVector3_NormalizeV "LNVector3_NormalizeV" int
#define LNVector3_NormalizeV(%1)
#func __typeOverride_LNVector3_Dot "LNVector3_DotD" int, int, var
#define LNVector3_Dot(%1, %2, %3)
#func _LNVector3_Cross "LNVector3_Cross" int, int, var
#define LNVector3_Cross(%1, %2, %3)
#func _LNVector3_Reflect "LNVector3_Reflect" int, int, var
#define LNVector3_Reflect(%1, %2, %3)
#func _LNVector3_Slide "LNVector3_Slide" int, int, var
#define LNVector3_Slide(%1, %2, %3)
#func _LNVector3_Lerp "LNVector3_LerpD" int, int, double, var
#define LNVector3_Lerp(%1, %2, %3, %4)
#func _LNVector3_CatmullRom "LNVector3_CatmullRomD" int, int, int, int, double, var
#define LNVector3_CatmullRom(%1, %2, %3, %4, %5, %6)
#func _LNVector3_Transform "LNVector3_Transform" int, int, var
#define LNVector3_Transform(%1, %2, %3)
#func _LNVector3_TransformCoord "LNVector3_TransformCoord" int, int, var
#define LNVector3_TransformCoord(%1, %2, %3)
#func _LNVector4_Create "LNVector4_CreateD" double, double, double, double, var
#define LNVector4_Create(%1, %2, %3, %4, %5)
#func _LNMatrix_GetRight "LNMatrix_GetRight" int, var
#define LNMatrix_GetRight(%1, %2)
#func _LNMatrix_GetUp "LNMatrix_GetUp" int, var
#define LNMatrix_GetUp(%1, %2)
#func _LNMatrix_GetFront "LNMatrix_GetFront" int, var
#define LNMatrix_GetFront(%1, %2)
#func _LNMatrix_GetPosition "LNMatrix_GetPosition" int, var
#define LNMatrix_GetPosition(%1, %2)
#func _LNMatrix_Identity "LNMatrix_Identity" var
#define LNMatrix_Identity(%1)
#func _LNMatrix_Translate "LNMatrix_TranslateD" int, double, double, double
#define LNMatrix_Translate(%1, %2, %3, %4)
#func _LNMatrix_TranslateVec3 "LNMatrix_TranslateVec3" int, int
#define LNMatrix_TranslateVec3(%1, %2)
#func _LNMatrix_RotateX "LNMatrix_RotateXD" int, double
#define LNMatrix_RotateX(%1, %2)
#func _LNMatrix_RotateY "LNMatrix_RotateYD" int, double
#define LNMatrix_RotateY(%1, %2)
#func _LNMatrix_RotateZ "LNMatrix_RotateZD" int, double
#define LNMatrix_RotateZ(%1, %2)
#func _LNMatrix_Rotate "LNMatrix_RotateD" int, double, double, double, int
#define LNMatrix_Rotate(%1, %2, %3, %4, %5=LN_ROTATIONORDER_XYZ)
#func _LNMatrix_RotateVec3 "LNMatrix_RotateVec3" int, int, int
#define LNMatrix_RotateVec3(%1, %2, %3=LN_ROTATIONORDER_XYZ)
#func _LNMatrix_RotateAxis "LNMatrix_RotateAxisD" int, int, double
#define LNMatrix_RotateAxis(%1, %2, %3)
#func _LNMatrix_RotateQuaternion "LNMatrix_RotateQuaternion" int, int
#define LNMatrix_RotateQuaternion(%1, %2)
#func _LNMatrix_Scale "LNMatrix_ScaleD" int, double
#define LNMatrix_Scale(%1, %2)
#func _LNMatrix_ScaleXYZ "LNMatrix_ScaleXYZD" int, double, double, double
#define LNMatrix_ScaleXYZ(%1, %2, %3, %4)
#func _LNMatrix_ScaleVec3 "LNMatrix_ScaleVec3" int, int
#define LNMatrix_ScaleVec3(%1, %2)
#func _LNMatrix_Multiply "LNMatrix_Multiply" int, int, var
#define LNMatrix_Multiply(%1, %2, %3)
#func _LNMatrix_Inverse "LNMatrix_Inverse" int, var
#define LNMatrix_Inverse(%1, %2)
#func _LNMatrix_Transpose "LNMatrix_Transpose" int, var
#define LNMatrix_Transpose(%1, %2)
#func _LNMatrix_ViewTransformLH "LNMatrix_ViewTransformLH" int, int, int, var
#define LNMatrix_ViewTransformLH(%1, %2, %3, %4)
#func _LNMatrix_ViewTransformRH "LNMatrix_ViewTransformRH" int, int, int, var
#define LNMatrix_ViewTransformRH(%1, %2, %3, %4)
#func _LNMatrix_PerspectiveFovLH "LNMatrix_PerspectiveFovLHD" double, double, double, double, var
#define LNMatrix_PerspectiveFovLH(%1, %2, %3, %4, %5)
#func _LNMatrix_PerspectiveFovRH "LNMatrix_PerspectiveFovRHD" double, double, double, double, var
#define LNMatrix_PerspectiveFovRH(%1, %2, %3, %4, %5)
#func _LNMatrix_OrthoLH "LNMatrix_OrthoLHD" double, double, double, double, var
#define LNMatrix_OrthoLH(%1, %2, %3, %4, %5)
#func _LNMatrix_OrthoRH "LNMatrix_OrthoRHD" double, double, double, double, var
#define LNMatrix_OrthoRH(%1, %2, %3, %4, %5)
#func _LNMatrix_GetEulerAngles "LNMatrix_GetEulerAngles" int, var
#define LNMatrix_GetEulerAngles(%1, %2)
#func _LNMatrix_Decompose "LNMatrix_Decompose" int, var, var, var
#define LNMatrix_Decompose(%1, %2, %3, %4)
#func _LNQuaternion_Create "LNQuaternion_CreateD" double, double, double, double, var
#define LNQuaternion_Create(%1, %2, %3, %4, %5)
#func _LNQuaternion_Identity "LNQuaternion_Identity" var
#define LNQuaternion_Identity(%1)
#func _LNQuaternion_RotationAxis "LNQuaternion_RotationAxisD" int, double, var
#define LNQuaternion_RotationAxis(%1, %2, %3)
#func _LNQuaternion_RotationMatrix "LNQuaternion_RotationMatrix" int, var
#define LNQuaternion_RotationMatrix(%1, %2)
#func _LNQuaternion_RotationYawPitchRoll "LNQuaternion_RotationYawPitchRollD" double, double, double, var
#define LNQuaternion_RotationYawPitchRoll(%1, %2, %3, %4)
#func _LNQuaternion_Normalize "LNQuaternion_Normalize" int, var
#define LNQuaternion_Normalize(%1, %2)
#func _LNQuaternion_Conjugate "LNQuaternion_Conjugate" int, var
#define LNQuaternion_Conjugate(%1, %2)
#func _LNQuaternion_Multiply "LNQuaternion_Multiply" int, int, var
#define LNQuaternion_Multiply(%1, %2, %3)
#func _LNQuaternion_Slerp "LNQuaternion_SlerpD" int, int, double, var
#define LNQuaternion_Slerp(%1, %2, %3, %4)
#func _LNAudio_PlayBGM "LNAudio_PlayBGM" str, int, int, int
#define LNAudio_PlayBGM(%1, %2=100, %3=100, %4=0)
#func _LNAudio_PlayBGMMem "LNAudio_PlayBGMMem" int, int, int, int, int
#define LNAudio_PlayBGMMem(%1, %2, %3=100, %4=100, %5=0)
#func _LNAudio_StopBGM "LNAudio_StopBGM" int
#define LNAudio_StopBGM(%1=0)
#func _LNAudio_PlayBGS "LNAudio_PlayBGS" str, int, int, int
#define LNAudio_PlayBGS(%1, %2=100, %3=100, %4=0)
#func _LNAudio_PlayBGSMem "LNAudio_PlayBGSMem" int, int, int, int, int
#define LNAudio_PlayBGSMem(%1, %2, %3=100, %4=100, %5=0)
#func _LNAudio_StopBGS "LNAudio_StopBGS" int
#define LNAudio_StopBGS(%1=0)
#func _LNAudio_PlayME "LNAudio_PlayME" str, int, int
#define LNAudio_PlayME(%1, %2=100, %3=100)
#func _LNAudio_PlayMEMem "LNAudio_PlayMEMem" int, int, int, int
#define LNAudio_PlayMEMem(%1, %2, %3=100, %4=100)
#func _LNAudio_StopME "LNAudio_StopME" 
#define LNAudio_StopME
#func _LNAudio_PlaySE "LNAudio_PlaySE" str, int, int
#define LNAudio_PlaySE(%1, %2=100, %3=100)
#func _LNAudio_PlaySE3D "LNAudio_PlaySE3DD" str, int, double, int, int
#define LNAudio_PlaySE3D(%1, %2, %3, %4=100, %5=100)
#func _LNAudio_PlaySE3DXYZ "LNAudio_PlaySE3DXYZD" str, double, double, double, double, int, int
#define LNAudio_PlaySE3DXYZ(%1, %2, %3, %4, %5, %6=100, %7=100)
#func _LNAudio_PlaySEMem "LNAudio_PlaySEMem" int, int, int, int
#define LNAudio_PlaySEMem(%1, %2, %3=100, %4=100)
#func _LNAudio_PlaySE3DMem "LNAudio_PlaySE3DMemD" int, int, int, double, int, int
#define LNAudio_PlaySE3DMem(%1, %2, %3, %4, %5=100, %6=100)
#func _LNAudio_PlaySE3DMemXYZ "LNAudio_PlaySE3DMemXYZD" int, int, double, double, double, double, int, int
#define LNAudio_PlaySE3DMemXYZ(%1, %2, %3, %4, %5, %6, %7=100, %8=100)
#func _LNAudio_StopSE "LNAudio_StopSE" 
#define LNAudio_StopSE
#func _LNAudio_SetMetreUnitDistance "LNAudio_SetMetreUnitDistanceD" double
#define LNAudio_SetMetreUnitDistance(%1)
#func _LNAudio_SetBGMVolume "LNAudio_SetBGMVolume" int, int
#define LNAudio_SetBGMVolume(%1, %2=0)
#func _LNAudio_SetBGSVolume "LNAudio_SetBGSVolume" int, int
#define LNAudio_SetBGSVolume(%1, %2=0)
#func _LNSoundListener_SetPosition "LNSoundListener_SetPosition" int
#define LNSoundListener_SetPosition(%1)
#func _LNSoundListener_SetPositionXYZ "LNSoundListener_SetPositionXYZD" double, double, double
#define LNSoundListener_SetPositionXYZ(%1, %2, %3)
#func _LNSoundListener_SetDirection "LNSoundListener_SetDirection" int
#define LNSoundListener_SetDirection(%1)
#func _LNSoundListener_SetDirectionXYZ "LNSoundListener_SetDirectionXYZD" double, double, double
#define LNSoundListener_SetDirectionXYZ(%1, %2, %3)
#func _LNSoundListener_SetUpDirection "LNSoundListener_SetUpDirection" int
#define LNSoundListener_SetUpDirection(%1)
#func _LNSoundListener_SetUpDirectionXYZ "LNSoundListener_SetUpDirectionXYZD" double, double, double
#define LNSoundListener_SetUpDirectionXYZ(%1, %2, %3)
#func _LNSoundListener_Velocity "LNSoundListener_Velocity" int
#define LNSoundListener_Velocity(%1)
#func _LNSoundListener_VelocityXYZ "LNSoundListener_VelocityXYZD" double, double, double
#define LNSoundListener_VelocityXYZ(%1, %2, %3)
#func _LNSound_GetVolume "LNSound_GetVolume" int, var
#define LNSound_GetVolume(%1, %2)
#func _LNSound_SetVolume "LNSound_SetVolume" int, int
#define LNSound_SetVolume(%1, %2)
#func _LNSound_GetPitch "LNSound_GetPitch" int, var
#define LNSound_GetPitch(%1, %2)
#func _LNSound_SetPitch "LNSound_SetPitch" int, int
#define LNSound_SetPitch(%1, %2)
#func _LNSound_SetLoop "LNSound_SetLoop" int, int
#define LNSound_SetLoop(%1, %2)
#func _LNSound_IsLoop "LNSound_IsLoop" int, var
#define LNSound_IsLoop(%1, %2)
#func _LNSound_SetLoopRange "LNSound_SetLoopRange" int, int, int
#define LNSound_SetLoopRange(%1, %2, %3)
#func _LNSound_GetPlayState "LNSound_GetPlayState" int, var
#define LNSound_GetPlayState(%1, %2)
#func _LNSound_GetUnitsPlayed "LNSound_GetUnitsPlayed" int, var
#define LNSound_GetUnitsPlayed(%1, %2)
#func _LNSound_GetTotalUnits "LNSound_GetTotalUnits" int, var
#define LNSound_GetTotalUnits(%1, %2)
#func _LNSound_GetUnitsParSecond "LNSound_GetUnitsParSecond" int, var
#define LNSound_GetUnitsParSecond(%1, %2)
#func _LNSound_Is3DSound "LNSound_Is3DSound" int, var
#define LNSound_Is3DSound(%1, %2)
#func _LNSound_SetEmitterPosition "LNSound_SetEmitterPosition" int, int
#define LNSound_SetEmitterPosition(%1, %2)
#func _LNSound_SetEmitterPositionXYZ "LNSound_SetEmitterPositionXYZD" int, double, double, double
#define LNSound_SetEmitterPositionXYZ(%1, %2, %3, %4)
#func _LNSound_SetEmitterVelocity "LNSound_SetEmitterVelocity" int, int
#define LNSound_SetEmitterVelocity(%1, %2)
#func _LNSound_SetEmitterVelocityXYZ "LNSound_SetEmitterVelocityXYZD" int, double, double, double
#define LNSound_SetEmitterVelocityXYZ(%1, %2, %3, %4)
#func _LNSound_SetEmitterDistance "LNSound_SetEmitterDistanceD" int, double
#define LNSound_SetEmitterDistance(%1, %2)
#func _LNSound_Create "LNSound_Create" str, var
#define LNSound_Create(%1, %2)
#func _LNSound_CreateMem "LNSound_CreateMem" int, int, var
#define LNSound_CreateMem(%1, %2, %3)
#func _LNSound_Play "LNSound_Play" int
#define LNSound_Play(%1)
#func _LNSound_Stop "LNSound_Stop" int
#define LNSound_Stop(%1)
#func _LNSound_Pause "LNSound_Pause" int, int
#define LNSound_Pause(%1, %2)
#func _LNSound_FadeVolume "LNSound_FadeVolume" int, int, int, int
#define LNSound_FadeVolume(%1, %2, %3, %4)


        const string ASFileFooter = @"
#func LException_SetEnableWrappedException ""LException_SetEnableWrappedException"" int
#func LHSPInternal_GetIntPtrStringLength ""LHSPInternal_GetIntPtrStringLength"" int, var
#func LHSPInternal_GetIntPtrString ""LHSPInternal_GetIntPtrString"" int, var

//------------------------------------------------------------------------------
// overrides
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
// LManager_Initialize
#deffunc _LManager_Initialize
	_LConfig_SetUserWindowHandle@ hwnd
	__LManager_Initialize@
	return stat
	
//--------------------------------------------------------------------------
// LManager_Update
#deffunc _LManager_Update
	__LManager_Update@
	await 0
	return stat
	
//--------------------------------------------------------------------------
// LFile_ExistDirectoryOrEXE(stat: 0=�Ȃ� 1=�f�B���N�g���ɑ���(�D��) 2=EXE�ɑ���)
#deffunc LFile_ExistDirectoryOrEXE str p1
	b = LN_FALSE@
	_LFile_Exists@ p1, b
	if b != LN_FALSE@ : return 1
	exist p1
	if strsize != -1 : return 2
	return 0

//--------------------------------------------------------------------------
// LManager_InitializeAudio
#deffunc _LManager_InitializeAudio
	_LConfig_SetUserWindowHandle@ hwnd
	__LManager_InitializeAudio@
	return

//--------------------------------------------------------------------------
// LAudio_PlayBGM
#deffunc _LAudio_PlayBGM str p1, int p2, int p3, int p4
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayBGMMem@ buf, strsize, p2, p3, p4
	} else : if stat == 1 {
		__LAudio_PlayBGM@ p1, p2, p3, p4
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlayBGM""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlayBGS
#deffunc _LAudio_PlayBGS str p1, int p2, int p3, int p4
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayBGSMem@ buf, strsize, p2, p3, p4
	} else : if stat == 1 {
		__LAudio_PlayBGS@ p1, p2, p3, p4
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlayBGS""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlayME
#deffunc _LAudio_PlayME str p1, int p2, int p3
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayMEMem@ buf, strsize, p2, p3
	} else : if stat == 1 {
		__LAudio_PlayME@ p1, p2, p3
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlayME""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlaySE
#deffunc _LAudio_PlaySE str p1, int p2, int p3
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySEMem@ buf, strsize, p2, p3
	} else : if stat == 1 {
		__LAudio_PlaySE@ p1, p2, p3
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlaySE""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlaySE3D
#deffunc _LAudio_PlaySE3D str p1, var p2, double p3, int p4, int p5
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySE3DMem@ buf, strsize, p2, p3, p4, p5
	} else : if stat == 1 {
		__LAudio_PlaySE3D@ p1, p2, p3, p4, p5
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlaySE3D""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlaySE3D
#deffunc _LAudio_PlaySE3DXYZ str p1, double p2, double p3, double p4, double p5, int p6, int p7
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySE3DMemXYZ@ buf, strsize, p2, p3, p4, p5, p6, p7
	} else : if stat == 1 {
		__LAudio_PlaySE3DXYZ@ p1, p2, p3, p4, p5, p6, p7
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlaySE3D""
	}
	return

//--------------------------------------------------------------------------
// LMatrix_Identity
#deffunc _LMatrix_Identity var p1
	if (length(p1) < 16) : dim p1, 16
    __LMatrix_Identity@ p1
    return

//--------------------------------------------------------------------------
// LQuaternion_Identity
#deffunc _LQuaternion_Identity var p1
	if (length(p1) < 4) : dim p1, 4
    __LQuaternion_Identity@ p1
    return

//--------------------------------------------------------------------------
// LRandom_SetSeed
#deffunc _LRandom_SetSeed var p1, int p2
	if (length(p1) < 4) : dim p1, 4
    __LRandom_SetSeed@ p1, p2
    return

__INTERNAL_DEFILES__

#global

// ��O�������ɃR�[���o�b�N����郉�x���̓o�^
#define LException_SetHandler(%1) oncmd gosub %1, LN_EXCEPTION_WIN32_MESSAGE

goto *lexception_default_label_dummy

// �f�t�H���g�̗�O�W�����v
*lexception_default_label
	LException_ProcDefault
	// HSP�O������� SendMessage() �ɂ��R�[���o�b�N���ł́A
	// end �ЂƂ������ƏI���ł��Ȃ��݂����B
	// (�ЂƂ�:PostQuit �� �ӂ���:Dispatch �̂悤�ɗ���Ă���H)
	end
	end
	return

*lexception_default_label_dummy

// �f�t�H���g�o�^
//LException_SetHandler *lexception_default_label


#endif  // __lnote__