//
// Generated by Microsoft (R) D3DX9 Shader Compiler
//
//  Source: diffuse.fx
//  Flags: /E:VS /T:vs_1_1 
//

// Parameters:
//
//     float4x4 matWorld;
//     float4x4 matWorldViewProj;
//     float4 LightDir;
//     float4 IntPolFactor;
//
//
// Registers:
//
//     Name             Reg   Size
//     ---------------- ----- ----
//     matWorld         c0       3
//     matWorldViewProj c8       4
//     LightDir         c12      1
//     IntPolFactor     c37      1
//

    vs_1_1
    def c3, 1.96086, 0.5, 0, -1
    dcl_position v0
    dcl_normal v3
    dcl_normal1 v4
    dcl_texcoord v7
    dcl_tangent v8
    dcl_tangent1 v9
    m4x4 oPos, v0, c8
    mad r2.xyz, c3.x, v4, c3.w
    mad r9.xyz, c3.x, v3, c3.w
    add r11.xyz, r2, -r9
    mad r1.xyz, r11, c37.x, r9
    mad r3.xyz, c3.x, v9.zxyw, c3.w
    mad r5.xyz, c3.x, v8.zxyw, c3.w
    add r2.xyz, r3, -r5
    mad r9.xyz, r2, c37.x, r5
    mul r7.xyz, r1.yzxw, r9
    mad r8.xyz, r1.zxyw, r9.zxyw, -r7
    m3x3 r3.xyz, r8, c0
    add r5.xyz, -c12, v0
    m3x3 r2.xyz, r9.yzxw, c0
    m3x3 r4.xyz, r1, c0
    m3x3 r0.xyz, r5, r2
    dp3 r0.w, r0, r0
    rsq r0.w, r0.w
    mul r1.xyz, r0, r0.w
    mad oD0.xyz, r1, c3.y, c3.y
    mov oT0.xy, v7
    mov oT1.xy, v7

// approximately 33 instruction slots used


// 0000:  fffe0101  0035fffe  42415443  00000014  ......5_CTAB.___
// 0010:  000000b9  fffe0101  00000004  00000014  .___.....___.___
// 0020:  00000064  00250002  00000001  00000074  d___._%_.___t___
// 0030:  00000000  00000084  000c0002  00000001  ____.___._._.___
// 0040:  00000074  00000000  0000008d  00000002  t_______.___.___
// 0050:  00000003  00000098  00000000  000000a8  .___._______.___
// 0060:  00080002  00000004  00000098  00000000  ._._.___._______
// 0070:  50746e49  61466c6f  726f7463  ababab00  IntPolFactor_...
// 0080:  00030001  00040001  00000001  00000000  ._._._._._______
// 0090:  6867694c  72694474  74616d00  6c726f57  LightDir_matWorl
// 00a0:  abab0064  00030003  00040004  00000001  d_..._._._._.___
// 00b0:  00000000  5774616d  646c726f  77656956  ____matWorldView
// 00c0:  6a6f7250  44334400  53203958  65646168  Proj_D3DX9 Shade
// 00d0:  6f432072  6c69706d  ab007265  00000051  r Compiler_.Q___
// 00e0:  a00f0003  3ffafd8d  3f000000  00000000  ._.....?___?____
// 00f0:  bf800000  0000001f  80000000  900f0000  __...______.__..
// 0100:  0000001f  80000003  900f0003  0000001f  .___.__.._...___
// 0110:  80010003  900f0004  0000001f  80000005  ._..._...___.__.
// 0120:  900f0007  0000001f  80000006  900f0008  ._...___.__.._..
// 0130:  0000001f  80010006  900f0009  00000014  .___._..._...___
// 0140:  c00f0000  90e40000  a0e40008  00000004  __..__..._...___
// 0150:  80070002  a0000003  90e40004  a0ff0003  ._...__.._..._..
// 0160:  00000004  80070009  a0000003  90e40003  .___._...__.._..
// 0170:  a0ff0003  00000002  8007000b  80e40002  ._...___._..._..
// 0180:  81e40009  00000004  80070001  80e4000b  ._...___._..._..
// 0190:  a0000025  80e40009  00000004  80070003  %__.._...___._..
// 01a0:  a0000003  90d20009  a0ff0003  00000004  .__.._..._...___
// 01b0:  80070005  a0000003  90d20008  a0ff0003  ._...__.._..._..
// 01c0:  00000002  80070002  80e40003  81e40005  .___._..._..._..
// 01d0:  00000004  80070009  80e40002  a0000025  .___._..._..%__.
// 01e0:  80e40005  00000005  80070007  80c90001  ._...___._..._..
// 01f0:  80e40009  00000004  80070008  80d20001  ._...___._..._..
// 0200:  80d20009  81e40007  00000017  80070003  ._..._...___._..
// 0210:  80e40008  a0e40000  00000002  80070005  ._..__...___._..
// 0220:  a1e4000c  90e40000  00000017  80070002  ._..__...___._..
// 0230:  80c90009  a0e40000  00000017  80070004  ._..__...___._..
// 0240:  80e40001  a0e40000  00000017  80070000  ._..__..._____..
// 0250:  80e40005  80e40002  00000008  80080000  ._..._..._____..
// 0260:  80e40000  80e40000  00000007  80080000  __..__..._____..
// 0270:  80ff0000  00000005  80070001  80e40000  __...___._..__..
// 0280:  80ff0000  00000004  d0070000  80e40001  __..._____..._..
// 0290:  a0550003  a0550003  00000001  e0030000  ._U.._U.._____..
// 02a0:  90e40007  00000001  e0030001  90e40007  ._...___._..._..
// 02b0:  0000ffff                                ..__
