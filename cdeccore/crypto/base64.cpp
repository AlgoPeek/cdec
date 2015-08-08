#include "stdafx.h"

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //
const char BASE64_CHAR_MAP[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

UINT32 _encodeGetLength(UINT32 srcSize)
{
	return (((srcSize + (3 - 1)) / 3) * 4);	
}

UINT32 _encode(void* dest, const void* src, UINT32 srcSize)
{
	ASSERT(dest);
	ASSERT(src);

	static const char BASE64_TABLE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	BYTE* pSrc = (BYTE*)src;
	BYTE* pDest = (BYTE*)dest;

	UINT32 destSize = 0;

	while (srcSize >= 1)
	{
		UINT32 blockSize = std::min<UINT32>(srcSize, 3);		// BASE64_INPUT
		ASSERT(1 <= blockSize && blockSize <= 3);

		// Encode inputs...
		BYTE n1, n2 = 0, n3 = 0, n4 = 0;
		switch (blockSize)
		{
		case 3:
			n4  =  (pSrc[ 2 ] & 0x3f);
			n3  = ((pSrc[ 2 ] & 0xc0) >> 6);
		case 2:
			n3 |= ((pSrc[ 1 ] & 0x0f) << 2);
			n2  = ((pSrc[ 1 ] & 0xf0) >> 4);
		case 1:
			n2 |= ((pSrc[ 0 ] & 0x03) << 4);
			n1  = ((pSrc[ 0 ] & 0xfc) >> 2);
			break;

		default:
			ASSERT(false);
		}
		pSrc += blockSize;
		srcSize -= blockSize;

		// Validate...
		ASSERT( 0 <= n1 && n1 <= 63 );
		ASSERT( 0 <= n2 && n2 <= 63 );
		ASSERT( 0 <= n3 && n3 <= 63 );
		ASSERT( 0 <= n4 && n4 <= 63 );

		// Padding...
		switch (blockSize)
		{
		case 1: n3 = 64;
		case 2: n4 = 64;
		case 3: break;
		default: ASSERT( false );
		}

		// 4 outputs...
		*pDest++ = BASE64_TABLE[ n1 ];
		*pDest++ = BASE64_TABLE[ n2 ];
		*pDest++ = BASE64_TABLE[ n3 ];
		*pDest++ = BASE64_TABLE[ n4 ];
		destSize += 4; //BASE64_OUTPUT
	}

	return destSize;
}

UINT32 _decodeGetLength(UINT32 srcSize)
{
	if (srcSize % 4 != 0)
		return -1;
	return (((srcSize + (4 - 1)) / 4) * 3);
}

UINT32 _decode(void* dest, const void* src, UINT32 srcSize)
{
	ASSERT(dest);
	ASSERT(src);

	if (srcSize % 4 != 0)
		return -1;

	static const BYTE BASE64_TABLE[ 0x80 ] = {
		/*00-07*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		/*08-0f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		/*10-17*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		/*18-1f*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		/*20-27*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		/*28-2f*/ 0xFF, 0xFF, 0xFF, 0x3e, 0xFF, 0xFF, 0xFF, 0x3f, //2 = '+' and '/'
		/*30-37*/ 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, //8 = '0'-'7'
		/*38-3f*/ 0x3c, 0x3d, 0xFF, 0xFF, 0xFF, 0x40, 0xFF, 0xFF, //2 = '8'-'9' and '='
		/*40-47*/ 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //7 = 'A'-'G'
		/*48-4f*/ 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, //8 = 'H'-'O'
		/*50-57*/ 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, //8 = 'P'-'W'
		/*58-5f*/ 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //3 = 'X'-'Z'
		/*60-67*/ 0xFF, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, //7 = 'a'-'g'
		/*68-6f*/ 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, //8 = 'h'-'o'
		/*70-77*/ 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, //8 = 'p'-'w'
		/*78-7f*/ 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  //3 = 'x'-'z'
	};

	BYTE* pSrc = (BYTE*)src;
	BYTE* pDest = (BYTE*)dest;
	UINT32 destSize = 0;

	while (srcSize >= 1)
	{
		// 4 inputs...
		BYTE in1 = *pSrc++;
		BYTE in2 = *pSrc++;
		BYTE in3 = *pSrc++;
		BYTE in4 = *pSrc++;
		srcSize -= 4; //BASE64_INPUT;

		// Validate ascii...
		ASSERT( 0 <= in1 && in1 <= 0x7f );
		ASSERT( 0 <= in2 && in2 <= 0x7f );
		ASSERT( 0 <= in1 && in3 <= 0x7f );
		ASSERT( 0 <= in2 && in4 <= 0x7f );

		// Convert ascii to base16...
		in1 = BASE64_TABLE[ in1 ];
		in2 = BASE64_TABLE[ in2 ];
		in3 = BASE64_TABLE[ in3 ];
		in4 = BASE64_TABLE[ in4 ];

		// Validate base16...
		ASSERT( in1 != 0xff );
		ASSERT( in2 != 0xff );
		ASSERT( in3 != 0xff );
		ASSERT( in4 != 0xff );
		ASSERT( 0 <= in1 && in1 <= 63 );
		ASSERT( 0 <= in2 && in2 <= 63 );
		ASSERT( 0 <= in3 && in3 <= 64 ); //possible padding
		ASSERT( 0 <= in4 && in4 <= 64 ); //possible padding

		// 3 outputs...
		*pDest++ = ((in1 & 0x3f) << 2) | ((in2 & 0x30) >> 4);
		*pDest++ = ((in2 & 0x0f) << 4) | ((in3 & 0x3c) >> 2);
		*pDest++ = ((in3 & 0x03) << 6) | (in4 & 0x3f);
		destSize += 3;	// BASE64_OUTPUT

		// Padding...
		if (in4 == 64)
		{
			--destSize;
			if (in3 == 64)
				--destSize;
		}
	}

	return destSize;
}

stringx Base64Encode(const BYTE* p, int len)
{
	ref<ByteArray> destArr = gc_new<ByteArray>(_encodeGetLength(len));
	BYTE* pDest = destArr->GetBuffer().ptr();

	UINT32 destLen = _encode(pDest, p, len);

	return Encoding::get_UTF8()->GetString(destArr, 0, destLen);
	
#if 0
	std::wstring16 s;
	while (len >= 3)
	{
		// aaaaaabb bbbbcccc ccdddddd
		BYTE c1 = p[0], c2 = p[1], c3 = p[2];
		s.push_back(BASE64_CHAR_MAP[c1 >> 2]);
		s.push_back(BASE64_CHAR_MAP[((c1 & 3) << 4) | (c2 >> 4)]);
		s.push_back(BASE64_CHAR_MAP[((c2 & 15) << 2) | (c3 >> 6)]);
		s.push_back(BASE64_CHAR_MAP[c3 & 0x3F]);
		p += 3;
		len -= 3;
	}

	if (len == 1)
	{
		BYTE c1 = p[0];
		s.push_back(BASE64_CHAR_MAP[c1 >> 2]);
		s.push_back(BASE64_CHAR_MAP[(c1 & 3) << 4]);
		s.push_back('=');
		s.push_back('=');
	}
	else if (len == 2)
	{
		BYTE c1 = p[0], c2 = p[1];
		s.push_back(BASE64_CHAR_MAP[c1 >> 2]);
		s.push_back(BASE64_CHAR_MAP[((c1 & 3) << 4) | (c2 >> 4)]);
		s.push_back(BASE64_CHAR_MAP[(c2 & 15) << 2]);
		s.push_back('=');
	}

	return stringx(s);
#endif

}

ref<ByteArray> Base64Decode(stringx data)
{
	ASSERT(data != NULL);

	ref<ByteArray> srcArr = Encoding::get_UTF8()->GetBytes(data);
	BYTE* pSrc = srcArr->GetBuffer().ptr();
	UINT32 len = srcArr->Count();

	ref<ByteArray> destArr = gc_new<ByteArray>(_decodeGetLength(len));
	UINT destLen = _decode(destArr->GetBuffer().ptr(), pSrc, len);
	
	return destArr->GetRange(0, destLen);
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
