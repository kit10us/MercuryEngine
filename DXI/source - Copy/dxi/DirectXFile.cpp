// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/DirectXFile.h>
#include <unify/Path.h>
#include <unify/Exception.h>
#include <cassert>

using namespace dxi;

unsigned int DirectXFile::m_D3DRM_XTEMPLATE_BYTES = 3278;
unsigned char DirectXFile::m_D3DRM_XTEMPLATES[] =
{
	0x78, 0x6f, 0x66, 0x20, 0x30, 0x33, 0x30, 0x32, 0x62, 
	0x69, 0x6e, 0x20, 0x30, 0x30, 0x36, 0x34, 0x1f, 0, 0x1, 
	0, 0x6, 0, 0, 0, 0x48, 0x65, 0x61, 0x64, 0x65, 
	0x72, 0xa, 0, 0x5, 0, 0x43, 0xab, 0x82, 0x3d, 0xda, 
	0x62, 0xcf, 0x11, 0xab, 0x39, 0, 0x20, 0xaf, 0x71, 0xe4, 
	0x33, 0x28, 0, 0x1, 0, 0x5, 0, 0, 0, 0x6d, 
	0x61, 0x6a, 0x6f, 0x72, 0x14, 0, 0x28, 0, 0x1, 0, 
	0x5, 0, 0, 0, 0x6d, 0x69, 0x6e, 0x6f, 0x72, 0x14, 
	0, 0x29, 0, 0x1, 0, 0x5, 0, 0, 0, 0x66, 
	0x6c, 0x61, 0x67, 0x73, 0x14, 0, 0xb, 0, 0x1f, 0, 
	0x1, 0, 0x6, 0, 0, 0, 0x56, 0x65, 0x63, 0x74, 
	0x6f, 0x72, 0xa, 0, 0x5, 0, 0x5e, 0xab, 0x82, 0x3d, 
	0xda, 0x62, 0xcf, 0x11, 0xab, 0x39, 0, 0x20, 0xaf, 0x71, 
	0xe4, 0x33, 0x2a, 0, 0x1, 0, 0x1, 0, 0, 0, 
	0x78, 0x14, 0, 0x2a, 0, 0x1, 0, 0x1, 0, 0, 
	0, 0x79, 0x14, 0, 0x2a, 0, 0x1, 0, 0x1, 0, 
	0, 0, 0x7a, 0x14, 0, 0xb, 0, 0x1f, 0, 0x1, 
	0, 0x8, 0, 0, 0, 0x43, 0x6f, 0x6f, 0x72, 0x64, 
	0x73, 0x32, 0x64, 0xa, 0, 0x5, 0, 0x44, 0x3f, 0xf2, 
	0xf6, 0x86, 0x76, 0xcf, 0x11, 0x8f, 0x52, 0, 0x40, 0x33, 
	0x35, 0x94, 0xa3, 0x2a, 0, 0x1, 0, 0x1, 0, 0, 
	0, 0x75, 0x14, 0, 0x2a, 0, 0x1, 0, 0x1, 0, 
	0, 0, 0x76, 0x14, 0, 0xb, 0, 0x1f, 0, 0x1, 
	0, 0x9, 0, 0, 0, 0x4d, 0x61, 0x74, 0x72, 0x69, 
	0x78, 0x34, 0x78, 0x34, 0xa, 0, 0x5, 0, 0x45, 0x3f, 
	0xf2, 0xf6, 0x86, 0x76, 0xcf, 0x11, 0x8f, 0x52, 0, 0x40, 
	0x33, 0x35, 0x94, 0xa3, 0x34, 0, 0x2a, 0, 0x1, 0, 
	0x6, 0, 0, 0, 0x6d, 0x61, 0x74, 0x72, 0x69, 0x78, 
	0xe, 0, 0x3, 0, 0x10, 0, 0, 0, 0xf, 0, 
	0x14, 0, 0xb, 0, 0x1f, 0, 0x1, 0, 0x9, 0, 
	0, 0, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x52, 0x47, 0x42, 
	0x41, 0xa, 0, 0x5, 0, 0xe0, 0x44, 0xff, 0x35, 0x7c, 
	0x6c, 0xcf, 0x11, 0x8f, 0x52, 0, 0x40, 0x33, 0x35, 0x94, 
	0xa3, 0x2a, 0, 0x1, 0, 0x3, 0, 0, 0, 0x72, 
	0x65, 0x64, 0x14, 0, 0x2a, 0, 0x1, 0, 0x5, 0, 
	0, 0, 0x67, 0x72, 0x65, 0x65, 0x6e, 0x14, 0, 0x2a, 
	0, 0x1, 0, 0x4, 0, 0, 0, 0x62, 0x6c, 0x75, 
	0x65, 0x14, 0, 0x2a, 0, 0x1, 0, 0x5, 0, 0, 
	0, 0x61, 0x6c, 0x70, 0x68, 0x61, 0x14, 0, 0xb, 0, 
	0x1f, 0, 0x1, 0, 0x8, 0, 0, 0, 0x43, 0x6f, 
	0x6c, 0x6f, 0x72, 0x52, 0x47, 0x42, 0xa, 0, 0x5, 0, 
	0x81, 0x6e, 0xe1, 0xd3, 0x35, 0x78, 0xcf, 0x11, 0x8f, 0x52, 
	0, 0x40, 0x33, 0x35, 0x94, 0xa3, 0x2a, 0, 0x1, 0, 
	0x3, 0, 0, 0, 0x72, 0x65, 0x64, 0x14, 0, 0x2a, 
	0, 0x1, 0, 0x5, 0, 0, 0, 0x67, 0x72, 0x65, 
	0x65, 0x6e, 0x14, 0, 0x2a, 0, 0x1, 0, 0x4, 0, 
	0, 0, 0x62, 0x6c, 0x75, 0x65, 0x14, 0, 0xb, 0, 
	0x1f, 0, 0x1, 0, 0xc, 0, 0, 0, 0x49, 0x6e, 
	0x64, 0x65, 0x78, 0x65, 0x64, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 
	0xa, 0, 0x5, 0, 0x20, 0xb8, 0x30, 0x16, 0x42, 0x78, 
	0xcf, 0x11, 0x8f, 0x52, 0, 0x40, 0x33, 0x35, 0x94, 0xa3, 
	0x29, 0, 0x1, 0, 0x5, 0, 0, 0, 0x69, 0x6e, 
	0x64, 0x65, 0x78, 0x14, 0, 0x1, 0, 0x9, 0, 0, 
	0, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x52, 0x47, 0x42, 0x41, 
	0x1, 0, 0xa, 0, 0, 0, 0x69, 0x6e, 0x64, 0x65, 
	0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x14, 0, 0xb, 0, 
	0x1f, 0, 0x1, 0, 0x7, 0, 0, 0, 0x42, 0x6f, 
	0x6f, 0x6c, 0x65, 0x61, 0x6e, 0xa, 0, 0x5, 0, 0xa0, 
	0xa6, 0x7d, 0x53, 0x37, 0xca, 0xd0, 0x11, 0x94, 0x1c, 0, 
	0x80, 0xc8, 0xc, 0xfa, 0x7b, 0x29, 0, 0x1, 0, 0x9, 
	0, 0, 0, 0x74, 0x72, 0x75, 0x65, 0x66, 0x61, 0x6c, 
	0x73, 0x65, 0x14, 0, 0xb, 0, 0x1f, 0, 0x1, 0, 
	0x9, 0, 0, 0, 0x42, 0x6f, 0x6f, 0x6c, 0x65, 0x61, 
	0x6e, 0x32, 0x64, 0xa, 0, 0x5, 0, 0x63, 0xae, 0x85, 
	0x48, 0xe8, 0x78, 0xcf, 0x11, 0x8f, 0x52, 0, 0x40, 0x33, 
	0x35, 0x94, 0xa3, 0x1, 0, 0x7, 0, 0, 0, 0x42, 
	0x6f, 0x6f, 0x6c, 0x65, 0x61, 0x6e, 0x1, 0, 0x1, 0, 
	0, 0, 0x75, 0x14, 0, 0x1, 0, 0x7, 0, 0, 
	0, 0x42, 0x6f, 0x6f, 0x6c, 0x65, 0x61, 0x6e, 0x1, 0, 
	0x1, 0, 0, 0, 0x76, 0x14, 0, 0xb, 0, 0x1f, 
	0, 0x1, 0, 0xc, 0, 0, 0, 0x4d, 0x61, 0x74, 
	0x65, 0x72, 0x69, 0x61, 0x6c, 0x57, 0x72, 0x61, 0x70, 0xa, 
	0, 0x5, 0, 0x60, 0xae, 0x85, 0x48, 0xe8, 0x78, 0xcf, 
	0x11, 0x8f, 0x52, 0, 0x40, 0x33, 0x35, 0x94, 0xa3, 0x1, 
	0, 0x7, 0, 0, 0, 0x42, 0x6f, 0x6f, 0x6c, 0x65, 
	0x61, 0x6e, 0x1, 0, 0x1, 0, 0, 0, 0x75, 0x14, 
	0, 0x1, 0, 0x7, 0, 0, 0, 0x42, 0x6f, 0x6f, 
	0x6c, 0x65, 0x61, 0x6e, 0x1, 0, 0x1, 0, 0, 0, 
	0x76, 0x14, 0, 0xb, 0, 0x1f, 0, 0x1, 0, 0xf, 
	0, 0, 0, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 
	0x46, 0x69, 0x6c, 0x65, 0x6e, 0x61, 0x6d, 0x65, 0xa, 0, 
	0x5, 0, 0xe1, 0x90, 0x27, 0xa4, 0x10, 0x78, 0xcf, 0x11, 
	0x8f, 0x52, 0, 0x40, 0x33, 0x35, 0x94, 0xa3, 0x31, 0, 
	0x1, 0, 0x8, 0, 0, 0, 0x66, 0x69, 0x6c, 0x65, 
	0x6e, 0x61, 0x6d, 0x65, 0x14, 0, 0xb, 0, 0x1f, 0, 
	0x1, 0, 0x8, 0, 0, 0, 0x4d, 0x61, 0x74, 0x65, 
	0x72, 0x69, 0x61, 0x6c, 0xa, 0, 0x5, 0, 0x4d, 0xab, 
	0x82, 0x3d, 0xda, 0x62, 0xcf, 0x11, 0xab, 0x39, 0, 0x20, 
	0xaf, 0x71, 0xe4, 0x33, 0x1, 0, 0x9, 0, 0, 0, 
	0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x52, 0x47, 0x42, 0x41, 0x1, 
	0, 0x9, 0, 0, 0, 0x66, 0x61, 0x63, 0x65, 0x43, 
	0x6f, 0x6c, 0x6f, 0x72, 0x14, 0, 0x2a, 0, 0x1, 0, 
	0x5, 0, 0, 0, 0x70, 0x6f, 0x77, 0x65, 0x72, 0x14, 
	0, 0x1, 0, 0x8, 0, 0, 0, 0x43, 0x6f, 0x6c, 
	0x6f, 0x72, 0x52, 0x47, 0x42, 0x1, 0, 0xd, 0, 0, 
	0, 0x73, 0x70, 0x65, 0x63, 0x75, 0x6c, 0x61, 0x72, 0x43, 
	0x6f, 0x6c, 0x6f, 0x72, 0x14, 0, 0x1, 0, 0x8, 0, 
	0, 0, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x52, 0x47, 0x42, 
	0x1, 0, 0xd, 0, 0, 0, 0x65, 0x6d, 0x69, 0x73, 
	0x73, 0x69, 0x76, 0x65, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x14, 
	0, 0xe, 0, 0x12, 0, 0x12, 0, 0x12, 0, 0xf, 
	0, 0xb, 0, 0x1f, 0, 0x1, 0, 0x8, 0, 0, 
	0, 0x4d, 0x65, 0x73, 0x68, 0x46, 0x61, 0x63, 0x65, 0xa, 
	0, 0x5, 0, 0x5f, 0xab, 0x82, 0x3d, 0xda, 0x62, 0xcf, 
	0x11, 0xab, 0x39, 0, 0x20, 0xaf, 0x71, 0xe4, 0x33, 0x29, 
	0, 0x1, 0, 0x12, 0, 0, 0, 0x6e, 0x46, 0x61, 
	0x63, 0x65, 0x56, 0x65, 0x72, 0x74, 0x65, 0x78, 0x49, 0x6e, 
	0x64, 0x69, 0x63, 0x65, 0x73, 0x14, 0, 0x34, 0, 0x29, 
	0, 0x1, 0, 0x11, 0, 0, 0, 0x66, 0x61, 0x63, 
	0x65, 0x56, 0x65, 0x72, 0x74, 0x65, 0x78, 0x49, 0x6e, 0x64, 
	0x69, 0x63, 0x65, 0x73, 0xe, 0, 0x1, 0, 0x12, 0, 
	0, 0, 0x6e, 0x46, 0x61, 0x63, 0x65, 0x56, 0x65, 0x72, 
	0x74, 0x65, 0x78, 0x49, 0x6e, 0x64, 0x69, 0x63, 0x65, 0x73, 
	0xf, 0, 0x14, 0, 0xb, 0, 0x1f, 0, 0x1, 0, 
	0xd, 0, 0, 0, 0x4d, 0x65, 0x73, 0x68, 0x46, 0x61, 
	0x63, 0x65, 0x57, 0x72, 0x61, 0x70, 0x73, 0xa, 0, 0x5, 
	0, 0xc0, 0xc5, 0x1e, 0xed, 0xa8, 0xc0, 0xd0, 0x11, 0x94, 
	0x1c, 0, 0x80, 0xc8, 0xc, 0xfa, 0x7b, 0x29, 0, 0x1, 
	0, 0xf, 0, 0, 0, 0x6e, 0x46, 0x61, 0x63, 0x65, 
	0x57, 0x72, 0x61, 0x70, 0x56, 0x61, 0x6c, 0x75, 0x65, 0x73, 
	0x14, 0, 0x34, 0, 0x1, 0, 0x9, 0, 0, 0, 
	0x42, 0x6f, 0x6f, 0x6c, 0x65, 0x61, 0x6e, 0x32, 0x64, 0x1, 
	0, 0xe, 0, 0, 0, 0x66, 0x61, 0x63, 0x65, 0x57, 
	0x72, 0x61, 0x70, 0x56, 0x61, 0x6c, 0x75, 0x65, 0x73, 0xe, 
	0, 0x1, 0, 0xf, 0, 0, 0, 0x6e, 0x46, 0x61, 
	0x63, 0x65, 0x57, 0x72, 0x61, 0x70, 0x56, 0x61, 0x6c, 0x75, 
	0x65, 0x73, 0xf, 0, 0x14, 0, 0xb, 0, 0x1f, 0, 
	0x1, 0, 0x11, 0, 0, 0, 0x4d, 0x65, 0x73, 0x68, 
	0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x43, 0x6f, 0x6f, 
	0x72, 0x64, 0x73, 0xa, 0, 0x5, 0, 0x40, 0x3f, 0xf2, 
	0xf6, 0x86, 0x76, 0xcf, 0x11, 0x8f, 0x52, 0, 0x40, 0x33, 
	0x35, 0x94, 0xa3, 0x29, 0, 0x1, 0, 0xe, 0, 0, 
	0, 0x6e, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x43, 
	0x6f, 0x6f, 0x72, 0x64, 0x73, 0x14, 0, 0x34, 0, 0x1, 
	0, 0x8, 0, 0, 0, 0x43, 0x6f, 0x6f, 0x72, 0x64, 
	0x73, 0x32, 0x64, 0x1, 0, 0xd, 0, 0, 0, 0x74, 
	0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x43, 0x6f, 0x6f, 0x72, 
	0x64, 0x73, 0xe, 0, 0x1, 0, 0xe, 0, 0, 0, 
	0x6e, 0x54, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x43, 0x6f, 
	0x6f, 0x72, 0x64, 0x73, 0xf, 0, 0x14, 0, 0xb, 0, 
	0x1f, 0, 0x1, 0, 0x10, 0, 0, 0, 0x4d, 0x65, 
	0x73, 0x68, 0x4d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61, 0x6c, 
	0x4c, 0x69, 0x73, 0x74, 0xa, 0, 0x5, 0, 0x42, 0x3f, 
	0xf2, 0xf6, 0x86, 0x76, 0xcf, 0x11, 0x8f, 0x52, 0, 0x40, 
	0x33, 0x35, 0x94, 0xa3, 0x29, 0, 0x1, 0, 0xa, 0, 
	0, 0, 0x6e, 0x4d, 0x61, 0x74, 0x65, 0x72, 0x69, 0x61, 
	0x6c, 0x73, 0x14, 0, 0x29, 0, 0x1, 0, 0xc, 0, 
	0, 0, 0x6e, 0x46, 0x61, 0x63, 0x65, 0x49, 0x6e, 0x64, 
	0x65, 0x78, 0x65, 0x73, 0x14, 0, 0x34, 0, 0x29, 0, 
	0x1, 0, 0xb, 0, 0, 0, 0x66, 0x61, 0x63, 0x65, 
	0x49, 0x6e, 0x64, 0x65, 0x78, 0x65, 0x73, 0xe, 0, 0x1, 
	0, 0xc, 0, 0, 0, 0x6e, 0x46, 0x61, 0x63, 0x65, 
	0x49, 0x6e, 0x64, 0x65, 0x78, 0x65, 0x73, 0xf, 0, 0x14, 
	0, 0xe, 0, 0x1, 0, 0x8, 0, 0, 0, 0x4d, 
	0x61, 0x74, 0x65, 0x72, 0x69, 0x61, 0x6c, 0xf, 0, 0xb, 
	0, 0x1f, 0, 0x1, 0, 0xb, 0, 0, 0, 0x4d, 
	0x65, 0x73, 0x68, 0x4e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x73, 
	0xa, 0, 0x5, 0, 0x43, 0x3f, 0xf2, 0xf6, 0x86, 0x76, 
	0xcf, 0x11, 0x8f, 0x52, 0, 0x40, 0x33, 0x35, 0x94, 0xa3, 
	0x29, 0, 0x1, 0, 0x8, 0, 0, 0, 0x6e, 0x4e, 
	0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x73, 0x14, 0, 0x34, 0, 
	0x1, 0, 0x6, 0, 0, 0, 0x56, 0x65, 0x63, 0x74, 
	0x6f, 0x72, 0x1, 0, 0x7, 0, 0, 0, 0x6e, 0x6f, 
	0x72, 0x6d, 0x61, 0x6c, 0x73, 0xe, 0, 0x1, 0, 0x8, 
	0, 0, 0, 0x6e, 0x4e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 
	0x73, 0xf, 0, 0x14, 0, 0x29, 0, 0x1, 0, 0xc, 
	0, 0, 0, 0x6e, 0x46, 0x61, 0x63, 0x65, 0x4e, 0x6f, 
	0x72, 0x6d, 0x61, 0x6c, 0x73, 0x14, 0, 0x34, 0, 0x1, 
	0, 0x8, 0, 0, 0, 0x4d, 0x65, 0x73, 0x68, 0x46, 
	0x61, 0x63, 0x65, 0x1, 0, 0xb, 0, 0, 0, 0x66, 
	0x61, 0x63, 0x65, 0x4e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x73, 
	0xe, 0, 0x1, 0, 0xc, 0, 0, 0, 0x6e, 0x46, 
	0x61, 0x63, 0x65, 0x4e, 0x6f, 0x72, 0x6d, 0x61, 0x6c, 0x73, 
	0xf, 0, 0x14, 0, 0xb, 0, 0x1f, 0, 0x1, 0, 
	0x10, 0, 0, 0, 0x4d, 0x65, 0x73, 0x68, 0x56, 0x65, 
	0x72, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x73, 
	0xa, 0, 0x5, 0, 0x21, 0xb8, 0x30, 0x16, 0x42, 0x78, 
	0xcf, 0x11, 0x8f, 0x52, 0, 0x40, 0x33, 0x35, 0x94, 0xa3, 
	0x29, 0, 0x1, 0, 0xd, 0, 0, 0, 0x6e, 0x56, 
	0x65, 0x72, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 
	0x73, 0x14, 0, 0x34, 0, 0x1, 0, 0xc, 0, 0, 
	0, 0x49, 0x6e, 0x64, 0x65, 0x78, 0x65, 0x64, 0x43, 0x6f, 
	0x6c, 0x6f, 0x72, 0x1, 0, 0xc, 0, 0, 0, 0x76, 
	0x65, 0x72, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 
	0x73, 0xe, 0, 0x1, 0, 0xd, 0, 0, 0, 0x6e, 
	0x56, 0x65, 0x72, 0x74, 0x65, 0x78, 0x43, 0x6f, 0x6c, 0x6f, 
	0x72, 0x73, 0xf, 0, 0x14, 0, 0xb, 0, 0x1f, 0, 
	0x1, 0, 0x4, 0, 0, 0, 0x4d, 0x65, 0x73, 0x68, 
	0xa, 0, 0x5, 0, 0x44, 0xab, 0x82, 0x3d, 0xda, 0x62, 
	0xcf, 0x11, 0xab, 0x39, 0, 0x20, 0xaf, 0x71, 0xe4, 0x33, 
	0x29, 0, 0x1, 0, 0x9, 0, 0, 0, 0x6e, 0x56, 
	0x65, 0x72, 0x74, 0x69, 0x63, 0x65, 0x73, 0x14, 0, 0x34, 
	0, 0x1, 0, 0x6, 0, 0, 0, 0x56, 0x65, 0x63, 
	0x74, 0x6f, 0x72, 0x1, 0, 0x8, 0, 0, 0, 0x76, 
	0x65, 0x72, 0x74, 0x69, 0x63, 0x65, 0x73, 0xe, 0, 0x1, 
	0, 0x9, 0, 0, 0, 0x6e, 0x56, 0x65, 0x72, 0x74, 
	0x69, 0x63, 0x65, 0x73, 0xf, 0, 0x14, 0, 0x29, 0, 
	0x1, 0, 0x6, 0, 0, 0, 0x6e, 0x46, 0x61, 0x63, 
	0x65, 0x73, 0x14, 0, 0x34, 0, 0x1, 0, 0x8, 0, 
	0, 0, 0x4d, 0x65, 0x73, 0x68, 0x46, 0x61, 0x63, 0x65, 
	0x1, 0, 0x5, 0, 0, 0, 0x66, 0x61, 0x63, 0x65, 
	0x73, 0xe, 0, 0x1, 0, 0x6, 0, 0, 0, 0x6e, 
	0x46, 0x61, 0x63, 0x65, 0x73, 0xf, 0, 0x14, 0, 0xe, 
	0, 0x12, 0, 0x12, 0, 0x12, 0, 0xf, 0, 0xb, 
	0, 0x1f, 0, 0x1, 0, 0x14, 0, 0, 0, 0x46, 
	0x72, 0x61, 0x6d, 0x65, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x66, 
	0x6f, 0x72, 0x6d, 0x4d, 0x61, 0x74, 0x72, 0x69, 0x78, 0xa, 
	0, 0x5, 0, 0x41, 0x3f, 0xf2, 0xf6, 0x86, 0x76, 0xcf, 
	0x11, 0x8f, 0x52, 0, 0x40, 0x33, 0x35, 0x94, 0xa3, 0x1, 
	0, 0x9, 0, 0, 0, 0x4d, 0x61, 0x74, 0x72, 0x69, 
	0x78, 0x34, 0x78, 0x34, 0x1, 0, 0xb, 0, 0, 0, 
	0x66, 0x72, 0x61, 0x6d, 0x65, 0x4d, 0x61, 0x74, 0x72, 0x69, 
	0x78, 0x14, 0, 0xb, 0, 0x1f, 0, 0x1, 0, 0x5, 
	0, 0, 0, 0x46, 0x72, 0x61, 0x6d, 0x65, 0xa, 0, 
	0x5, 0, 0x46, 0xab, 0x82, 0x3d, 0xda, 0x62, 0xcf, 0x11, 
	0xab, 0x39, 0, 0x20, 0xaf, 0x71, 0xe4, 0x33, 0xe, 0, 
	0x12, 0, 0x12, 0, 0x12, 0, 0xf, 0, 0xb, 0, 
	0x1f, 0, 0x1, 0, 0x9, 0, 0, 0, 0x46, 0x6c, 
	0x6f, 0x61, 0x74, 0x4b, 0x65, 0x79, 0x73, 0xa, 0, 0x5, 
	0, 0xa9, 0x46, 0xdd, 0x10, 0x5b, 0x77, 0xcf, 0x11, 0x8f, 
	0x52, 0, 0x40, 0x33, 0x35, 0x94, 0xa3, 0x29, 0, 0x1, 
	0, 0x7, 0, 0, 0, 0x6e, 0x56, 0x61, 0x6c, 0x75, 
	0x65, 0x73, 0x14, 0, 0x34, 0, 0x2a, 0, 0x1, 0, 
	0x6, 0, 0, 0, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x73, 
	0xe, 0, 0x1, 0, 0x7, 0, 0, 0, 0x6e, 0x56, 
	0x61, 0x6c, 0x75, 0x65, 0x73, 0xf, 0, 0x14, 0, 0xb, 
	0, 0x1f, 0, 0x1, 0, 0xe, 0, 0, 0, 0x54, 
	0x69, 0x6d, 0x65, 0x64, 0x46, 0x6c, 0x6f, 0x61, 0x74, 0x4b, 
	0x65, 0x79, 0x73, 0xa, 0, 0x5, 0, 0x80, 0xb1, 0x6, 
	0xf4, 0x3b, 0x7b, 0xcf, 0x11, 0x8f, 0x52, 0, 0x40, 0x33, 
	0x35, 0x94, 0xa3, 0x29, 0, 0x1, 0, 0x4, 0, 0, 
	0, 0x74, 0x69, 0x6d, 0x65, 0x14, 0, 0x1, 0, 0x9, 
	0, 0, 0, 0x46, 0x6c, 0x6f, 0x61, 0x74, 0x4b, 0x65, 
	0x79, 0x73, 0x1, 0, 0x6, 0, 0, 0, 0x74, 0x66, 
	0x6b, 0x65, 0x79, 0x73, 0x14, 0, 0xb, 0, 0x1f, 0, 
	0x1, 0, 0xc, 0, 0, 0, 0x41, 0x6e, 0x69, 0x6d, 
	0x61, 0x74, 0x69, 0x6f, 0x6e, 0x4b, 0x65, 0x79, 0xa, 0, 
	0x5, 0, 0xa8, 0x46, 0xdd, 0x10, 0x5b, 0x77, 0xcf, 0x11, 
	0x8f, 0x52, 0, 0x40, 0x33, 0x35, 0x94, 0xa3, 0x29, 0, 
	0x1, 0, 0x7, 0, 0, 0, 0x6b, 0x65, 0x79, 0x54, 
	0x79, 0x70, 0x65, 0x14, 0, 0x29, 0, 0x1, 0, 0x5, 
	0, 0, 0, 0x6e, 0x4b, 0x65, 0x79, 0x73, 0x14, 0, 
	0x34, 0, 0x1, 0, 0xe, 0, 0, 0, 0x54, 0x69, 
	0x6d, 0x65, 0x64, 0x46, 0x6c, 0x6f, 0x61, 0x74, 0x4b, 0x65, 
	0x79, 0x73, 0x1, 0, 0x4, 0, 0, 0, 0x6b, 0x65, 
	0x79, 0x73, 0xe, 0, 0x1, 0, 0x5, 0, 0, 0, 
	0x6e, 0x4b, 0x65, 0x79, 0x73, 0xf, 0, 0x14, 0, 0xb, 
	0, 0x1f, 0, 0x1, 0, 0x10, 0, 0, 0, 0x41, 
	0x6e, 0x69, 0x6d, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x4f, 0x70, 
	0x74, 0x69, 0x6f, 0x6e, 0x73, 0xa, 0, 0x5, 0, 0xc0, 
	0x56, 0xbf, 0xe2, 0xf, 0x84, 0xcf, 0x11, 0x8f, 0x52, 0, 
	0x40, 0x33, 0x35, 0x94, 0xa3, 0x29, 0, 0x1, 0, 0xa, 
	0, 0, 0, 0x6f, 0x70, 0x65, 0x6e, 0x63, 0x6c, 0x6f, 
	0x73, 0x65, 0x64, 0x14, 0, 0x29, 0, 0x1, 0, 0xf, 
	0, 0, 0, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 
	0x6e, 0x71, 0x75, 0x61, 0x6c, 0x69, 0x74, 0x79, 0x14, 0, 
	0xb, 0, 0x1f, 0, 0x1, 0, 0x9, 0, 0, 0, 
	0x41, 0x6e, 0x69, 0x6d, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0xa, 
	0, 0x5, 0, 0x4f, 0xab, 0x82, 0x3d, 0xda, 0x62, 0xcf, 
	0x11, 0xab, 0x39, 0, 0x20, 0xaf, 0x71, 0xe4, 0x33, 0xe, 
	0, 0x12, 0, 0x12, 0, 0x12, 0, 0xf, 0, 0xb, 
	0, 0x1f, 0, 0x1, 0, 0xc, 0, 0, 0, 0x41, 
	0x6e, 0x69, 0x6d, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x53, 0x65, 
	0x74, 0xa, 0, 0x5, 0, 0x50, 0xab, 0x82, 0x3d, 0xda, 
	0x62, 0xcf, 0x11, 0xab, 0x39, 0, 0x20, 0xaf, 0x71, 0xe4, 
	0x33, 0xe, 0, 0x1, 0, 0x9, 0, 0, 0, 0x41, 
	0x6e, 0x69, 0x6d, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0xf, 0, 
	0xb, 0, 0x1f, 0, 0x1, 0, 0xa, 0, 0, 0, 
	0x49, 0x6e, 0x6c, 0x69, 0x6e, 0x65, 0x44, 0x61, 0x74, 0x61, 
	0xa, 0, 0x5, 0, 0xa0, 0xee, 0x23, 0x3a, 0xb1, 0x94, 
	0xd0, 0x11, 0xab, 0x39, 0, 0x20, 0xaf, 0x71, 0xe4, 0x33, 
	0xe, 0, 0x1, 0, 0x6, 0, 0, 0, 0x42, 0x49, 
	0x4e, 0x41, 0x52, 0x59, 0xf, 0, 0xb, 0, 0x1f, 0, 
	0x1, 0, 0x3, 0, 0, 0, 0x55, 0x72, 0x6c, 0xa, 
	0, 0x5, 0, 0xa1, 0xee, 0x23, 0x3a, 0xb1, 0x94, 0xd0, 
	0x11, 0xab, 0x39, 0, 0x20, 0xaf, 0x71, 0xe4, 0x33, 0x29, 
	0, 0x1, 0, 0x5, 0, 0, 0, 0x6e, 0x55, 0x72, 
	0x6c, 0x73, 0x14, 0, 0x34, 0, 0x31, 0, 0x1, 0, 
	0x4, 0, 0, 0, 0x75, 0x72, 0x6c, 0x73, 0xe, 0, 
	0x1, 0, 0x5, 0, 0, 0, 0x6e, 0x55, 0x72, 0x6c, 
	0x73, 0xf, 0, 0x14, 0, 0xb, 0, 0x1f, 0, 0x1, 
	0, 0xf, 0, 0, 0, 0x50, 0x72, 0x6f, 0x67, 0x72, 
	0x65, 0x73, 0x73, 0x69, 0x76, 0x65, 0x4d, 0x65, 0x73, 0x68, 
	0xa, 0, 0x5, 0, 0x60, 0xc3, 0x63, 0x8a, 0x7d, 0x99, 
	0xd0, 0x11, 0x94, 0x1c, 0, 0x80, 0xc8, 0xc, 0xfa, 0x7b, 
	0xe, 0, 0x1, 0, 0x3, 0, 0, 0, 0x55, 0x72, 
	0x6c, 0x13, 0, 0x1, 0, 0xa, 0, 0, 0, 0x49, 
	0x6e, 0x6c, 0x69, 0x6e, 0x65, 0x44, 0x61, 0x74, 0x61, 0xf, 
	0, 0xb, 0, 0x1f, 0, 0x1, 0, 0x4, 0, 0, 
	0, 0x47, 0x75, 0x69, 0x64, 0xa, 0, 0x5, 0, 0xe0, 
	0x90, 0x27, 0xa4, 0x10, 0x78, 0xcf, 0x11, 0x8f, 0x52, 0, 
	0x40, 0x33, 0x35, 0x94, 0xa3, 0x29, 0, 0x1, 0, 0x5, 
	0, 0, 0, 0x64, 0x61, 0x74, 0x61, 0x31, 0x14, 0, 
	0x28, 0, 0x1, 0, 0x5, 0, 0, 0, 0x64, 0x61, 
	0x74, 0x61, 0x32, 0x14, 0, 0x28, 0, 0x1, 0, 0x5, 
	0, 0, 0, 0x64, 0x61, 0x74, 0x61, 0x33, 0x14, 0, 
	0x34, 0, 0x2d, 0, 0x1, 0, 0x5, 0, 0, 0, 
	0x64, 0x61, 0x74, 0x61, 0x34, 0xe, 0, 0x3, 0, 0x8, 
	0, 0, 0, 0xf, 0, 0x14, 0, 0xb, 0, 0x1f, 
	0, 0x1, 0, 0xe, 0, 0, 0, 0x53, 0x74, 0x72, 
	0x69, 0x6e, 0x67, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 
	0x79, 0xa, 0, 0x5, 0, 0xe0, 0x21, 0xf, 0x7f, 0xe1, 
	0xbf, 0xd1, 0x11, 0x82, 0xc0, 0, 0xa0, 0xc9, 0x69, 0x72, 
	0x71, 0x31, 0, 0x1, 0, 0x3, 0, 0, 0, 0x6b, 
	0x65, 0x79, 0x14, 0, 0x31, 0, 0x1, 0, 0x5, 0, 
	0, 0, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x14, 0, 0xb, 
	0, 0x1f, 0, 0x1, 0, 0xb, 0, 0, 0, 0x50, 
	0x72, 0x6f, 0x70, 0x65, 0x72, 0x74, 0x79, 0x42, 0x61, 0x67, 
	0xa, 0, 0x5, 0, 0xe1, 0x21, 0xf, 0x7f, 0xe1, 0xbf, 
	0xd1, 0x11, 0x82, 0xc0, 0, 0xa0, 0xc9, 0x69, 0x72, 0x71, 
	0xe, 0, 0x1, 0, 0xe, 0, 0, 0, 0x53, 0x74, 
	0x72, 0x69, 0x6e, 0x67, 0x50, 0x72, 0x6f, 0x70, 0x65, 0x72, 
	0x74, 0x79, 0xf, 0, 0xb, 0, 0x1f, 0, 0x1, 0, 
	0xe, 0, 0, 0, 0x45, 0x78, 0x74, 0x65, 0x72, 0x6e, 
	0x61, 0x6c, 0x56, 0x69, 0x73, 0x75, 0x61, 0x6c, 0xa, 0, 
	0x5, 0, 0xa0, 0x6a, 0x11, 0x98, 0xba, 0xbd, 0xd1, 0x11, 
	0x82, 0xc0, 0, 0xa0, 0xc9, 0x69, 0x72, 0x71, 0x1, 0, 
	0x4, 0, 0, 0, 0x47, 0x75, 0x69, 0x64, 0x1, 0, 
	0x12, 0, 0, 0, 0x67, 0x75, 0x69, 0x64, 0x45, 0x78, 
	0x74, 0x65, 0x72, 0x6e, 0x61, 0x6c, 0x56, 0x69, 0x73, 0x75, 
	0x61, 0x6c, 0x14, 0, 0xe, 0, 0x12, 0, 0x12, 0, 
	0x12, 0, 0xf, 0, 0xb, 0, 0x1f, 0, 0x1, 0, 
	0xb, 0, 0, 0, 0x52, 0x69, 0x67, 0x68, 0x74, 0x48, 
	0x61, 0x6e, 0x64, 0x65, 0x64, 0xa, 0, 0x5, 0, 0xa0, 
	0x5e, 0x5d, 0x7f, 0x3a, 0xd5, 0xd1, 0x11, 0x82, 0xc0, 0, 
	0xa0, 0xc9, 0x69, 0x72, 0x71, 0x29, 0, 0x1, 0, 0xc, 
	0, 0, 0, 0x62, 0x52, 0x69, 0x67, 0x68, 0x74, 0x48, 
	0x61, 0x6e, 0x64, 0x65, 0x64, 0x14, 0, 0xb, 0
};

DirectXFile::DirectXFile()
: m_SearchStatus( SearchRoot )
, m_pDXFile( 0 )
, m_pDXEnum( 0 )
, m_iLevel( 0 )
, m_GUID( 0 )
{
}

DirectXFile::DirectXFile( const unify::Path & filePath )
: m_SearchStatus( SearchRoot )
, m_pDXFile( 0 )
, m_pDXEnum( 0 )
, m_iLevel( 0 )
, m_GUID( 0 )
{
	LoadFile( filePath );
}

DirectXFile::~DirectXFile()
{
	Release();
}

void DirectXFile::Release()
{
	m_CurrentPack.Release();
	m_ParentPack.Release();
	m_iLevel = 0;
}

void DirectXFile::LoadFile( const unify::Path & filePath )
{
	Release();

	if ( filePath.Exists() == false )
	{
		throw unify::Exception( "File not found! (\"" + filePath.ToString() + "\")" );
	}

	if( DirectXFileCreate( &m_pDXFile ) != DXFILE_OK )
	{
		throw unify::Exception( "Failed to create a DirectX File interface object!" );
	}

	if( FAILED(m_pDXFile->RegisterTemplates( (LPVOID)DirectXFile::GetXTemplates(), GetXTemplateBytes() )) )
	{
		if( m_pDXFile )
		{
			m_pDXFile->Release();
			m_pDXFile = 0;
		}
		throw unify::Exception( "Failed to register templates for DirectX file interface object!" );
	}

	if( FAILED(m_pDXFile->CreateEnumObject( (void*)filePath.ToString().c_str(), DXFILELOAD_FROMFILE, &m_pDXEnum )) )
	{
		if( m_pDXFile )
		{
			m_pDXFile->Release();
			m_pDXFile = 0;
		}
		throw unify::Exception( "Failed to create enum object for DirectX file interface object!" );
	}

	m_filePath = filePath;
}

unsigned char* DirectXFile::GetXTemplates()
{
	return m_D3DRM_XTEMPLATES;
}

unsigned int DirectXFile::GetXTemplateBytes()
{
	return m_D3DRM_XTEMPLATE_BYTES;
}

std::string DirectXFile::GetGUIDName( const GUID* pGUID )
{
	if( !pGUID ) return "NULL";
	if( *pGUID == TID_D3DRMFrame ) return "Frame";
	if( *pGUID == TID_D3DRMFrameTransformMatrix ) return "FrameTransformMatrix";
	if( *pGUID == TID_D3DRMMesh ) return "Mesh";
	
	if( *pGUID == TID_D3DRMVertexDuplicationIndices ) return "VertexDuplicationIndices";
	if( *pGUID == TID_D3DRMSkinWeights ) return "SkinWeights";
	if( *pGUID == TID_D3DRMXSkinMeshHeader ) return "SkinMeshHeader";

	if( *pGUID == TID_D3DRMAnimationSet ) return "AnimationSet";
	if( *pGUID == TID_D3DRMAnimation ) return "Animation";
	if( *pGUID == TID_D3DRMAnimationKey ) return "AnimationKey";
	return "Unknown Type";
}

const GUID & DirectXFile::GetGUID() const
{
	m_GUID = 0;
	m_CurrentPack.GetGUID( (const GUID**)&m_GUID );
	return *m_GUID;
}

const std::string & DirectXFile::GetCurrentName() const
{
	m_sName = m_CurrentPack.GetName();
	return m_sName;
}

DXFType DirectXFile::GetObjectType() const
{
	return m_CurrentPack.type;
}

int DirectXFile::GetObjectLevel() const
{
	return m_iLevel;
}

bool DirectXFile::GetType( const GUID **ppguid ) const
{
	if( m_CurrentPack.GetGUID( ppguid ) ) return true;
	return FALSE;
	// return *m_CurrentPack.GetGUID( ppguid );
}

bool DirectXFile::GetData( const std::string & name, unsigned long & size, void ** data )
{
	return m_CurrentPack.GetData( name, size, data );
}

// Increment our level, set our backup parent, our parent and the new current.
bool DirectXFile::IncrementLevel( IDirectXFileObject* pNewObject )
{
	m_ParentPackBackup[m_iLevel] = m_CurrentPack;
	m_ParentPack = m_CurrentPack;
	m_CurrentPack.Build( pNewObject );
	m_iLevel += 1;
	return true;
}

// Move back to a previous level, setting parent, decrementing level, setting current.
bool DirectXFile::DecrementLevel( )
{
	assert( m_iLevel );
	m_iLevel--;
	m_CurrentPack.Release();
	m_CurrentPack = m_ParentPack;
	
	if( m_iLevel == 0 )
	{
		m_ParentPack = DataPack();
		//m_pCurrentParent = 0;
	}
	else 
	{
		m_ParentPack = m_ParentPackBackup[m_iLevel-1];
		//m_pParentObject = m_pObjectBack[m_iLevel-1];
		//m_pParentDataObject = m_pObjectDataBack[m_iLevel-1];
	}

	return true;
}

bool DirectXFile::GotoNextObject()
{
	//
	// We search through the heirarchy for the next object or the end...
	//
	gotoRestartSearch:
	switch( m_SearchStatus )
	{
	default:
	case SearchEnd:		// No where else to search...
		return FALSE;

	case SearchRoot:
		return GotoRootObject();

	case SearchChild:
		if( !GotoChildObject() ) goto gotoRestartSearch;
		return true;		

	case SearchSibling:
		if( !GotoSiblingObject() ) goto gotoRestartSearch;
		return true;

	case SearchParent:
		GotoParentObject();
		goto gotoRestartSearch;
	}
}

// Goto either the FIRST or the NEXT root object.
bool DirectXFile::GotoRootObject()
{
	// Check for another root object...
	IDirectXFileData* pRootData;
	m_pDXEnum->GetNextDataObject( &pRootData );
	m_CurrentPack.Release();
	if( !m_CurrentPack.Build( pRootData ) )
	{
		m_SearchStatus = SearchEnd;
		return FALSE;
	}
	m_SearchStatus = SearchChild;
	return true;
}

bool DirectXFile::GotoChildObject()
{
	IDirectXFileObject* pObject;
	m_CurrentPack.GetNextObject( &pObject );

	if( !pObject )
	{
		m_SearchStatus = SearchSibling;
		return FALSE;
	}

	return IncrementLevel( pObject );		
}

bool DirectXFile::GotoSiblingObject()
{
	//
	// Means going to our parent, then accessing the parent's child.
	// We then goto the child object (will be after us).
	// If we have no parent, then we are the root, so we need to access the next root object.
	// If there is no next root object, we are at the end of the file SearchEnd.

	// Check if we are in the root...
	if( m_iLevel == 0 )
	{
		m_SearchStatus = SearchRoot;
		return GotoRootObject();
	}

	// Check if our parent has another child...
	IDirectXFileObject* pObject;
	m_ParentPack.GetNextObject( &pObject );
	if( pObject )
	{
		m_SearchStatus = SearchChild;
		m_CurrentPack.Release();
		return m_CurrentPack.Build( pObject );
	}
	else
	{
		m_SearchStatus = SearchParent;
		return FALSE;
	}
}

// Makes the current object the parent, and the parent it's parent, etc..
bool DirectXFile::GotoParentObject()
{
	if( DecrementLevel() ) 
	{
		m_SearchStatus = SearchChild;
		return true;
	}
	else
	{
		m_SearchStatus = SearchRoot;
		return FALSE;
	}
}

bool DirectXFile::EndOfFile()
{
	return m_SearchStatus == SearchEnd;
}

bool DirectXFile::GetData( std::string & dataString )
{
	unsigned long size;
	char ** ppszString = 0;
	GetData( "", size, (void**)&ppszString );
	dataString = *ppszString;
	return true;
}

bool DirectXFile::GetData( unify::Matrix & matrix )
{
	unsigned long size;
	void * data;
	GetData( "", size, (void**)&data );
	matrix = *(unify::Matrix*)data;
	return true;
}

bool DirectXFile::GetData( SkinWeights & skinWeights )
{
	unsigned long size;
	unsigned long * data;

	GetData( "", size, (void**)&data );
	skinWeights.frameName = *((char**)data);
	
	unsigned long count = *(unsigned long*)&data[ 1 ];
	unsigned long * vertices = (unsigned long*)&data[ 2 ];
	float * weights = (float *)&data[ 2 + count ];

	for( size_t i = 0; i != count; ++i )
	{
		skinWeights.vertices.push_back( vertices[ i ] );
		skinWeights.weights.push_back( weights[ i ] );
	}
	skinWeights.offset = *(unify::Matrix * )&data[ 2 + count * 2 ];

	return true;
}
