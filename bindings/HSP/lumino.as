
#ifndef __lumino__
#define __lumino__

#regcmd "_hsp3cmdinit@4","LuminoHSP.dll", 6

#const global LN_FALSE 0
#const global LN_TRUE 1

#const global LN_OK 0
#const global LN_ERROR_UNKNOWN -1
#const global LN_ERROR_VERIFY -2
#const global LN_ERROR_ARGUMENT -3
#const global LN_ERROR_INVALID_OPERATION -4
#const global LN_ERROR_NOT_IMPLEMENTED -5
#const global LN_ERROR_OUT_OF_MEMORY -6
#const global LN_ERROR_OUT_OF_RANGE -7
#const global LN_ERROR_KEY_NOT_FOUND -8
#const global LN_ERROR_OVERFLOW -9
#const global LN_ERROR_IO -10
#const global LN_ERROR_FILE_NOT_FOUND -11
#const global LN_ERROR_DIRECTORY_NOT_FOUND -12
#const global LN_ERROR_INVALID_FORMAT -13
#const global LN_ERROR_END_OF_STREAM -14
#const global LN_ERROR_ENCODING -15
#const global LN_ERROR_WIN32 -16
#const global LN_ERROR_COM -17

#const global LN_VARIANTTYPE_UNKNOWN 0
#const global LN_VARIANTTYPE_BOOL 1
#const global LN_VARIANTTYPE_INT 2
#const global LN_VARIANTTYPE_FLOAT 3
#const global LN_VARIANTTYPE_STRING 4
#const global LN_VARIANTTYPE_LIST 5
#const global LN_VARIANTTYPE_OBJECT 6
#const global LN_VARIANTTYPE_SIZE_F 7
#const global LN_VARIANTTYPE__TERMINATOR 8

#const global LN_FILEACCESSPRIORITY_ARCHIVE_FIRST 0
#const global LN_FILEACCESSPRIORITY_DIRECTORY_FIRST 1
#const global LN_FILEACCESSPRIORITY_ARCHIVE_ONLY 2
#const global LN_FILEACCESSPRIORITY__TERMINATOR 3

#const global LN_FILETYPE_BINARY 0
#const global LN_FILETYPE_TEXT 1
#const global LN_FILETYPE__TERMINATOR 2

#const global LN_FILEMODE_OPEN 0
#const global LN_FILEMODE_CREATE 1
#const global LN_FILEMODE_APPEND 2
#const global LN_FILEMODE__TERMINATOR 3

#const global LN_FILEACCESS_READ 0
#const global LN_FILEACCESS_READWRITE 1
#const global LN_FILEACCESS_WRITE 2
#const global LN_FILEACCESS__TERMINATOR 3

#const global LN_SEEK_BEGIN 0
#const global LN_SEEK_CURRENT 1
#const global LN_SEEK_END 2

#const global LN_ASYNCIOSTATE_IDLE 0
#const global LN_ASYNCIOSTATE_READY 1
#const global LN_ASYNCIOSTATE_PROCESSING 2
#const global LN_ASYNCIOSTATE_COMPLETED 3
#const global LN_ASYNCIOSTATE_FAILED 4
#const global LN_ASYNCIOSTATE__TERMINATOR 5

#const global LN_ROTATIONORDER_XYZ 0
#const global LN_ROTATIONORDER_XZY 1
#const global LN_ROTATIONORDER_YXZ 2
#const global LN_ROTATIONORDER_YZX 3
#const global LN_ROTATIONORDER_ZXY 4
#const global LN_ROTATIONORDER_ZYX 5
#const global LN_ROTATIONORDER__TERMINATOR 6

#const global LN_KEY_UNKNOWN 0
#const global LN_KEY_A 1
#const global LN_KEY_B 2
#const global LN_KEY_C 3
#const global LN_KEY_D 4
#const global LN_KEY_E 5
#const global LN_KEY_F 6
#const global LN_KEY_G 7
#const global LN_KEY_H 8
#const global LN_KEY_I 9
#const global LN_KEY_J 10
#const global LN_KEY_K 11
#const global LN_KEY_L 12
#const global LN_KEY_M 13
#const global LN_KEY_N 14
#const global LN_KEY_O 15
#const global LN_KEY_P 16
#const global LN_KEY_Q 17
#const global LN_KEY_R 18
#const global LN_KEY_S 19
#const global LN_KEY_T 20
#const global LN_KEY_U 21
#const global LN_KEY_V 22
#const global LN_KEY_W 23
#const global LN_KEY_X 24
#const global LN_KEY_Y 25
#const global LN_KEY_Z 26
#const global LN_KEY_D0 27
#const global LN_KEY_D1 28
#const global LN_KEY_D2 29
#const global LN_KEY_D3 30
#const global LN_KEY_D4 31
#const global LN_KEY_D5 32
#const global LN_KEY_D6 33
#const global LN_KEY_D7 34
#const global LN_KEY_D8 35
#const global LN_KEY_D9 36
#const global LN_KEY_F1 37
#const global LN_KEY_F2 38
#const global LN_KEY_F3 39
#const global LN_KEY_F4 40
#const global LN_KEY_F5 41
#const global LN_KEY_F6 42
#const global LN_KEY_F7 43
#const global LN_KEY_F8 44
#const global LN_KEY_F9 45
#const global LN_KEY_F10 46
#const global LN_KEY_F11 47
#const global LN_KEY_F12 48
#const global LN_KEY_SPACE 49
#const global LN_KEY_ESCAPE 50
#const global LN_KEY_UP 51
#const global LN_KEY_DOWN 52
#const global LN_KEY_LEFT 53
#const global LN_KEY_RIGHT 54
#const global LN_KEY_LSHIFT 55
#const global LN_KEY_RSHIFT 56
#const global LN_KEY_LCTRL 57
#const global LN_KEY_RCTRL 58
#const global LN_KEY_LALT 59
#const global LN_KEY_RALT 60
#const global LN_KEY_TAB 61
#const global LN_KEY_ENTER 62
#const global LN_KEY_BACKSPACE 63
#const global LN_KEY_INSERT 64
#const global LN_KEY_DELETE 65
#const global LN_KEY_PAGEUP 66
#const global LN_KEY_PAGEDOWN 67
#const global LN_KEY_HOME 68
#const global LN_KEY_END 69
#const global LN_KEY_COLON 70
#const global LN_KEY_SEMICOLON 71
#const global LN_KEY_COMMA 72
#const global LN_KEY_PERIOD 73
#const global LN_KEY_SLASH 74
#const global LN_KEY_MINUS 75
#const global LN_KEY_BACKSLASH 76
#const global LN_KEY_YEN 77
#const global LN_KEY_CARET 78
#const global LN_KEY_LBRACKET 79
#const global LN_KEY_RBRACKET 80
#const global LN_KEY_ANDROID_PLUS 81
#const global LN_KEY__TERMINATOR 256

#const global LN_ACCELERATORKEY_TOGGLE_FULLSCREEN 0
#const global LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS 1
#const global LN_ACCELERATORKEY__TERMINATOR 2

#const global LN_DIRECTMUSICMODE_NOT_USE 0
#const global LN_DIRECTMUSICMODE_NORMAL 1
#const global LN_DIRECTMUSICMODE_THREAD_WAIT 2
#const global LN_DIRECTMUSICMODE_THREAD_REQUEST 3
#const global LN_DIRECTMUSICMODE__TERMINATOR 4

#const global LN_SOUNDPLAYINGMODE_UNKNOWN 0
#const global LN_SOUNDPLAYINGMODE_AUTO 1
#const global LN_SOUNDPLAYINGMODE_ONMEMORY 2
#const global LN_SOUNDPLAYINGMODE_STREAMING 3
#const global LN_SOUNDPLAYINGMODE_MIDI 4
#const global LN_SOUNDPLAYINGTYPE__TERMINATOR 5

#const global LN_SOUNDPLAYINGSTATE_STOPPED 0
#const global LN_SOUNDPLAYINGSTATE_PLAYING 1
#const global LN_SOUNDPLAYINGSTATE_PAUSING 2
#const global LN_SOUNDPLAYINGSTATE__TERMINATOR 3

#const global LN_SOUNDFADEBEHAVIORBEHAVIOR_CONTINUE 0
#const global LN_SOUNDFADEBEHAVIOR_STOP 1
#const global LN_SOUNDFADEBEHAVIOR_STOP_RESET 2
#const global LN_SOUNDFADEBEHAVIOR_PAUSE 3
#const global LN_SOUNDFADEBEHAVIOR_PAUSE_RESET 4
#const global LN_SOUNDFADEBEHAVIOR__TERMINATOR 5

#const global LN_GRAPHICSRENDERINGTYPE_IMMEDIATE 0
#const global LN_GRAPHICSRENDERINGTYPE_THREADED 1
#const global LN_GRAPHICSRENDERINGTYPE__TERMINATOR 2

#const global LN_BACKBUFFERRESIZEMODE_SCALING 0
#const global LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX 1
#const global LN_BACKBUFFERRESIZEMODE_RESIZE 2
#const global LN_BACKBUFFERRESIZEMODE__TERMINATOR 3

#const global LN_FMT_UNKNOWN 0
#const global LN_FMT_A8R8G8B8 1
#const global LN_FMT_X8R8G8B8 2
#const global LN_FMT_A16B16G16R16F 3
#const global LN_FMT_A32B32G32R32F 4
#const global LN_FMT_D24S8 5
#const global LN_FMT__TERMINATOR 6

#const global LN_BLEND_NORMAL 0
#const global LN_BLEND_ALPHA 1
#const global LN_BLEND_ADD 2
#const global LN_BLEND_ADD_ALPHA_DISABLE 3
#const global LN_BLEND_SUB 4
#const global LN_BLEND_SUB_ALPHA_DISABLE 5
#const global LN_BLEND_MUL 6
#const global LN_BLEND_SCREEN 7
#const global LN_BLEND_REVERSE 8
#const global LN_BLEND__TERMINATOR 9

#const global LN_CULL_FRONT 0
#const global LN_CULL_BACK 1
#const global LN_CULL_DOUBLE 2
#const global LN_CULL__TERMINATOR 3

#const global LN_TEXTALIGN_LEFT 0
#const global LN_TEXTALIGN_CENTER 1
#const global LN_TEXTALIGN_RIGHT 2
#const global LN_TEXTALIGN__TERMINATOR 3

#const global LN_PLACEMENTTYPE_NORMAL 0
#const global LN_PLACEMENTTYPE_BILLBOARD 1
#const global LN_PLACEMENTTYPE_Y_FIXED 2
#const global LN_PLACEMENTTYPE__TERMINATOR 3

#const global LN_FRONTAXIS_X 0
#const global LN_FRONTAXIS_Y 1
#const global LN_FRONTAXIS_Z 2
#const global LN_FRONTAXIS_RX 3
#const global LN_FRONTAXIS_RY 4
#const global LN_FRONTAXIS_RZ 5
#const global LN_FRONTAXIS__TERMINATOR 6

#const global LN_MESHCREATEFLAGS_SOFTWARE_SKINNING 0x01
#const global LN_MESHCREATEFLAGS_COLLISION_MESH 0x02
#const global LN_MESHCREATEFLAGS_IGNORE_TEXTURE_NOT_FOUND 0x04
#const global LN_MESHCREATEFLAGS__TERMINATOR 5

#const global LN_CAMERATYPE_POSITION_AND_LOOKAT 0
#const global LN_CAMERATYPE_POSITION_AND_ANGLE 1
#const global LN_CAMERATYPE_TRANSFORM_MATRIX 2
#const global LN_CAMERATYPE__TERMINATOR 3

#const global LN_COORD_SPACE_2D 0
#const global LN_COORD_SPACE_3D 1
#const global LN_COORD__TERMINATOR 2

#const global LN_OBJECTTYPE_SOUND 0
#const global LN_OBJECTTYPE__TERMINATOR 1



#cmd LNSize $000A
#cmd LNVector2 $000B
#cmd LNVector3 $000C
#cmd LNVector4 $000D
#cmd LNMatrix $000E
#cmd LNQuaternion $000F


#cmd LNConfig_SetGraphicsRenderingType $0064
#cmd LNConfig_SetEngineLogEnabled $0065
#cmd LNConfig_RegisterArchive $0066
#cmd LNConfig_SetFileAccessPriority $0067
#cmd LNConfig_SetUserWindowHandle $0068
#cmd LNConfig_SetSoundCacheSize $0069
#cmd LNConfig_SetDirectMusicMode $006A
#cmd LNConfig_SetDirectMusicReverbLevel $006B
#cmd LNEngine_Initialize $006C
#cmd LNEngine_InitializeAudio $006D
#cmd LNEngine_UpdateFrame $006E
#cmd LNEngine_IsEndRequested $006F
#cmd LNEngine_Terminate $0070
#cmd LNVersion_GetMajor $0071
#cmd LNVersion_GetMinor $0072
#cmd LNVersion_GetRevision $0073
#cmd LNVersion_GetBuild $0074
#cmd LNVersion_GetString $0075
#cmd LNVersion_IsAtLeast $0076
#cmd LNError_GetLastErrorCode $0077
#cmd LNError_GetLastErrorMessage $0078
#cmd LNDiag_GetHandleCount $0079
#cmd LNObject_Release $007A
#cmd LNObject_AddRef $007B
#cmd LNObject_GetRefCount $007C
#cmd LNVector2_GetLength $007D
#cmd LNVector2_GetSquareLength $007E
#cmd LNVector2_Set $007F
#cmd LNVector2_Normalize $0080
#cmd LNVector2_NormalizeV $0081
#cmd LNVector3_GetLength $0082
#cmd LNVector3_GetSquareLength $0083
#cmd LNVector3_Set $0084
#cmd LNVector3_SetVZ $0085
#cmd LNVector3_Normalize $0086
#cmd LNVector3_NormalizeV $0087
#cmd LNVector3_Dot $0088
#cmd LNVector3_Cross $0089
#cmd LNVector3_Reflect $008A
#cmd LNVector3_Slide $008B
#cmd LNVector3_Lerp $008C
#cmd LNVector3_CatmullRom $008D
#cmd LNVector3_Transform $008E
#cmd LNVector3_TransformCoord $008F
#cmd LNVector4_Set $0090
#cmd LNMatrix_GetRight $0091
#cmd LNMatrix_GetUp $0092
#cmd LNMatrix_GetFront $0093
#cmd LNMatrix_GetPosition $0094
#cmd LNMatrix_Identity $0095
#cmd LNMatrix_Translate $0096
#cmd LNMatrix_TranslateVec3 $0097
#cmd LNMatrix_RotateX $0098
#cmd LNMatrix_RotateY $0099
#cmd LNMatrix_RotateZ $009A
#cmd LNMatrix_Rotate $009B
#cmd LNMatrix_RotateVec3 $009C
#cmd LNMatrix_RotateAxis $009D
#cmd LNMatrix_RotateQuaternion $009E
#cmd LNMatrix_Scale $009F
#cmd LNMatrix_ScaleXYZ $00A0
#cmd LNMatrix_ScaleVec3 $00A1
#cmd LNMatrix_Multiply $00A2
#cmd LNMatrix_Inverse $00A3
#cmd LNMatrix_Transpose $00A4
#cmd LNMatrix_ViewTransformLH $00A5
#cmd LNMatrix_ViewTransformRH $00A6
#cmd LNMatrix_PerspectiveFovLH $00A7
#cmd LNMatrix_PerspectiveFovRH $00A8
#cmd LNMatrix_OrthoLH $00A9
#cmd LNMatrix_OrthoRH $00AA
#cmd LNMatrix_GetEulerAngles $00AB
#cmd LNMatrix_Decompose $00AC
#cmd LNQuaternion_Set $00AD
#cmd LNQuaternion_Identity $00AE
#cmd LNQuaternion_RotationAxis $00AF
#cmd LNQuaternion_RotationMatrix $00B0
#cmd LNQuaternion_RotationYawPitchRoll $00B1
#cmd LNQuaternion_Normalize $00B2
#cmd LNQuaternion_Conjugate $00B3
#cmd LNQuaternion_Multiply $00B4
#cmd LNQuaternion_Slerp $00B5
#cmd LNGameAudio_PlayBGM $00B6
#cmd LNGameAudio_PlayBGMMem $00B7
#cmd LNGameAudio_StopBGM $00B8
#cmd LNGameAudio_PlayBGS $00B9
#cmd LNGameAudio_PlayBGSMem $00BA
#cmd LNGameAudio_StopBGS $00BB
#cmd LNGameAudio_PlayME $00BC
#cmd LNGameAudio_PlayMEMem $00BD
#cmd LNGameAudio_StopME $00BE
#cmd LNGameAudio_PlaySE $00BF
#cmd LNGameAudio_PlaySE3D $00C0
#cmd LNGameAudio_PlaySE3DXYZ $00C1
#cmd LNGameAudio_PlaySEMem $00C2
#cmd LNGameAudio_PlaySE3DMem $00C3
#cmd LNGameAudio_PlaySE3DMemXYZ $00C4
#cmd LNGameAudio_StopSE $00C5
#cmd LNGameAudio_SetBGMVolume $00C6
#cmd LNGameAudio_SetBGSVolume $00C7
#cmd LNSoundListener_SetPosition $00C8
#cmd LNSoundListener_SetDirection $00C9
#cmd LNSoundListener_SetUpDirection $00CA
#cmd LNSoundListener_SetVelocity $00CB
#cmd LNSoundListener_SetPositionXYZ $00CC
#cmd LNSoundListener_SetDirectionXYZ $00CD
#cmd LNSoundListener_SetUpDirectionXYZ $00CE
#cmd LNSoundListener_SetVelocityXYZ $00CF
#cmd LNSound_Create $00D0
#cmd LNSound_CreateMem $00D1
#cmd LNSound_GetVolume $00D2
#cmd LNSound_SetVolume $00D3
#cmd LNSound_GetPitch $00D4
#cmd LNSound_SetPitch $00D5
#cmd LNSound_IsLoopEnabled $00D6
#cmd LNSound_SetLoopEnabled $00D7
#cmd LNSound_Is3DEnabled $00D8
#cmd LNSound_Set3DEnabled $00D9
#cmd LNSound_GetPlayingMode $00DA
#cmd LNSound_SetPlayingMode $00DB
#cmd LNSound_GetPlayingState $00DC
#cmd LNSound_GetPlayedSamples $00DD
#cmd LNSound_GetTotalSamples $00DE
#cmd LNSound_GetSamplingRate $00DF
#cmd LNSound_SetEmitterPosition $00E0
#cmd LNSound_SetEmitterVelocity $00E1
#cmd LNSound_SetEmitterMaxDistance $00E2
#cmd LNSound_SetLoopRange $00E3
#cmd LNSound_Play $00E4
#cmd LNSound_Stop $00E5
#cmd LNSound_Pause $00E6
#cmd LNSound_Resume $00E7
#cmd LNSound_FadeVolume $00E8
#cmd LNSound_SetEmitterPositionXYZ $00E9
#cmd LNSound_SetEmitterVelocityXYZ $00EA
#cmd LNTexture_GetSize $00EB
#cmd LNTexture2D_Create $00EC
#cmd LNTexture2D_CreateFromFile $00ED
#cmd LNViewport_GetMainViewport $00EE
#cmd LNViewport_GetLayers $00EF
#cmd LNSceneNode_IsVisible $00F0
#cmd LNSceneNode_SetVisible $00F1
#cmd LNSceneNode_GetPosition $00F2
#cmd LNSceneNode_SetPosition $00F3
#cmd LNSceneNode_GetChildren $00F4
#cmd LNSprite_GetTexture $00F5
#cmd LNSprite_SetTexture $00F6
#cmd LNSprite2D_Create $00F7
#cmd LNSprite2D_CreateFromTexture $00F8
#cmd LNUINativeHostWindow_Create $00F9
#cmd LNUINativeHostWindow_Render $00FA
#cmd LNViewportLayerList_GetCount $00FB
#cmd LNViewportLayerList_SetAt $00FC
#cmd LNViewportLayerList_GetAt $00FD
#cmd LNViewportLayerList_Add $00FE
#cmd LNViewportLayerList_Clear $00FF
#cmd LNViewportLayerList_Insert $0100
#cmd LNViewportLayerList_Remove $0101
#cmd LNViewportLayerList_RemoveAt $0102
#cmd LNSceneNodeList_GetCount $0103
#cmd LNSceneNodeList_SetAt $0104
#cmd LNSceneNodeList_GetAt $0105
#cmd LNSceneNodeList_Add $0106
#cmd LNSceneNodeList_Clear $0107
#cmd LNSceneNodeList_Insert $0108
#cmd LNSceneNodeList_Remove $0109
#cmd LNSceneNodeList_RemoveAt $010A


#endif // __lumino__

