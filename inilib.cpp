//
// inilib.h - 库实现文件
//-------------------------------------------------------------------------------------------------
//
// Copyright (c) 2014, YZ
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted 
// provided that the following conditions are met:
//
//		Redistributions of source code must retain the above copyright notice, this list of conditions 
//		and the following disclaimer.
//
//		Redistributions in binary form must reproduce the above copyright notice, this list of 
//		conditions and the following disclaimer in the documentation and/or other materials provided 
//		with the distribution.
//
//		Neither the name of the <ORGANIZATION> nor the names of its contributors may be used to endorse 
//		or promote products derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-------------------------------------------------------------------------------------------------
//
// 版权所有 (c) 2014，YZ
// 保留所有权利
//
// 在遵守以下条件的前提下，可再发布软件或以源代码及二进制形式使用软件，包括进行修改或不进行修改：
// 
//		源代码的再发布必须遵守上述版权通知、本条件列表以及以下免责声明。
//
//		以二进制形式再发布软件时必须复制上述版权通知、本条件列表以及文档下文中的免责声明和/或发布代码时
//		提供的其他信息。
//
//		未经事先书面批准的情况下，不得将名称“机构”或贡献者名字用于支持或推广该软件的衍生产品。
//
// 本软件为版权所有人和贡献者“按现状”为根据提供，不提供任何明确或暗示的保证，包括但不限于本软件针对特定
// 用途的可售性及适用性的暗示保证。在任何情况下，版权所有人或其贡献者均不对因使用本软件而以任何方式产生
// 的任何直接、间接、偶然、特殊、典型或因此而生的损失（包括但不限于采购替换产品或服务；使用价值、数据或
// 利润的损失；或业务中断）而根据任何责任理论，包括合同、严格责任或侵权行为（包括疏忽或其他）承担任何责
// 任，即使在已经提醒可能发生此类损失的情况下。
//
//-------------------------------------------------------------------------------------------------
//
//  如有任何建议或BUG，可用以下方式与我取得联系：
//
//		cooooocd@gmail.com
//		https://github.com/coooood/inilib
//


#include "stdafx.h"

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdarg.h>
#include <stddef.h>
#include <time.h>
#include <crtdbg.h>
#include <new>

#include "inilib.h"




////////////////////////////////////////////////////////////////////////
// 内部变量


// 内参段变量索引
typedef enum
{
	IPND_WRITER = 0,										// 作者
	IPND_LASTTIME,											// INI最后写入时间
	IPND_READONLY,											// 只读状态
	IPND_DOUBLE_PRECISION,									// double类型值小数点后保留的位数（有效值1~6）

	IPND_MAX												// 变量个数
} IPNDINDEX, *LPIPNDINDEX;

// 内参段变量名(串顺序必须于IPNDINDEX中的顺序相同)
LPCTSTR	INIPARAMNAME[IPND_MAX] = 
{
	_T("writer"),											// 创建INI文件的软件名称
	_T("last_time"), 										// INI最后一次的写入时间
	_T("read_only"),										// INI的内容是否只读
	_T("double_precision")									// double类型值小数点后保留的位数
};

// 各个数据类型的长度(字节)
const int INIDATATYPELENGTH[] = 
{
	sizeof(char), 
	sizeof(int), 
	sizeof(short), 
	sizeof(double), 
	sizeof(LPTSTR), 
	sizeof(__int64),
	sizeof(LPTSTR), 
	0
};

// 数据类型关键字
LPCTSTR	INIDATATYPESTRING[] = 
{
	_T("byte"), 
	_T("int"), 
	_T("bool"), 
	_T("double"), 
	_T("string"), 
	_T("date"),
	_T("struct"), 
	_T("byte"), 
	_T("int"), 
	_T("bool"), 
	_T("double"), 
	_T("string"), 
	_T("date"),
	_T("struct"), 
	_T("")
};


////////////////////////////////////////////////////////////////////////
// 错误信息的语言选择

#define	EM_LANGUAGE_CHS
//#define	EM_LANGUAGE_ENG


// 错误信息（信息长度不要超过_ICCBUFSIZE变量定义的字符个数）
#ifdef	EM_LANGUAGE_CHS												// 中文
	LPCTSTR	_ILEM_0 = _T("文件无法打开");
	LPCTSTR	_ILEM_1 = _T("内存申请失败");
	LPCTSTR	_ILEM_2 = _T("读文件时出现错误");
	LPCTSTR	_ILEM_3 = _T("文件编码不是unicode");
	LPCTSTR	_ILEM_4 = _T("系统异常");

	LPCTSTR	_ILEM_5 = _T("内参段段名非法，应该是[%s]，而不是[%s]");
	LPCTSTR	_ILEM_6 = _T("变量书写格式错误，请修改 \"%s\"");
	LPCTSTR	_ILEM_7 = _T("变量%s书写错误，它的值只能是%s，而不能是%s");
	LPCTSTR	_ILEM_8 = _T("未定义的内参变量 %s");

	LPCTSTR	_ILEM_9 = _T("变量名%s中含有非法字符");
	LPCTSTR	_ILEM_10 = _T("未指明数据类型");
	LPCTSTR	_ILEM_11 = _T("数据类型存在拼写错误（\"%s\"无法被识别）");
	LPCTSTR	_ILEM_12 = _T("数据类型书写格式错误（缺少:）");
	LPCTSTR	_ILEM_13 = _T("数组尺寸格式存在书写错误（应该是[n]的形式）");
	LPCTSTR	_ILEM_14 = _T("数组尺寸不合法（不能是0或负数）");	
	LPCTSTR	_ILEM_15 = _T("数据类型书写格式错误（缺少struct类型关键字）");
	LPCTSTR	_ILEM_16 = _T("数据类型关键字存在拼写错误（应该是\"struct\"，而不是\"%s\"）");
	LPCTSTR	_ILEM_17 = _T("成员类型关键字缺失（第%d个成员）");
	LPCTSTR	_ILEM_18 = _T("第%d个成员的数组尺寸存在书写错误");
	LPCTSTR	_ILEM_19 = _T("第%d个成员的数组尺寸不合法（不能是0或负数）");
	LPCTSTR	_ILEM_20 = _T("结构类型中又包含了结构类型（第%d个成员），这是非法的");
	LPCTSTR	_ILEM_21 = _T("第%d个成员的类型关键字拼写错误（\"%s\"不能被识别）");
	LPCTSTR	_ILEM_22 = _T("数组尺寸格式存在书写错误（应该是[n]的形式）");
	LPCTSTR	_ILEM_23 = _T("数组尺寸不合法（不能是0或负数）");
	
	LPCTSTR	_ILEM_24 = _T("数组缺少数据（应该有%d组数据，但实际只有%d组）");
	LPCTSTR	_ILEM_25 = _T("数组第%d个数据错误");
	LPCTSTR	_ILEM_26 = _T("缺少数据");
	LPCTSTR	_ILEM_27 = _T("数据错误，无法读取");
	
	LPCTSTR	_ILEM_28 = _T("第%d组数据缺少包裹符({ })");
	LPCTSTR	_ILEM_29 = _T("缺少包裹符({ })");
	LPCTSTR	_ILEM_30 = _T("缺少第%d个成员的数据");
	LPCTSTR	_ILEM_31 = _T("第%d个成员的数据错误，无法读取");
	LPCTSTR	_ILEM_32 = _T("第%d个成员的数据缺少数组数据包裹符（）");
	LPCTSTR	_ILEM_33 = _T("第%d个成员缺少数据（应该有%d个数据，但实际只有%d个）");
	LPCTSTR	_ILEM_34 = _T("第%d个成员的第%d个数据错误，无法读取");

	LPCTSTR	_ILEM_35 = _T("出现了无法解析的语句：\"%s\"");
	LPCTSTR	_ILEM_36 = _T("内参段只能有一个，而INI中出现了 %d 个内参段");
	LPCTSTR	_ILEM_37 = _T("非数组型结构变量不允许使用链表符(%s)");

#elif defined(EM_LANGUAGE_ENG)												// English

	LPCTSTR	_ILEM_0 = _T("The file cannot be opened");
	LPCTSTR	_ILEM_1 = _T("Allocation failure");
	LPCTSTR	_ILEM_2 = _T("An error occurred while reading the file");
	LPCTSTR	_ILEM_3 = _T("File encoding is not Unicode");
	LPCTSTR	_ILEM_4 = _T("System error");

	LPCTSTR	_ILEM_5 = _T("内参段段名非法，应该是[%s]，而不是[%s]");
	LPCTSTR	_ILEM_6 = _T("变量书写格式错误，请修改 \"%s\"");
	LPCTSTR	_ILEM_7 = _T("变量%s书写错误，它的值只能是%s，而不能是%s");
	LPCTSTR	_ILEM_8 = _T("未定义的内参变量 %s");

	LPCTSTR	_ILEM_9 = _T("变量名%s中含有非法字符");
	LPCTSTR	_ILEM_10 = _T("未指明数据类型");
	LPCTSTR	_ILEM_11 = _T("数据类型存在拼写错误（\"%s\"无法被识别）");
	LPCTSTR	_ILEM_12 = _T("数据类型书写格式错误（缺少:）");
	LPCTSTR	_ILEM_13 = _T("数组尺寸格式存在书写错误（应该是[n]的形式）");
	LPCTSTR	_ILEM_14 = _T("数组尺寸不合法（不能是0或负数）");	
	LPCTSTR	_ILEM_15 = _T("数据类型书写格式错误（缺少struct类型关键字）");
	LPCTSTR	_ILEM_16 = _T("数据类型关键字存在拼写错误（应该是\"struct\"，而不是\"%s\"）");
	LPCTSTR	_ILEM_17 = _T("成员类型关键字缺失（第%d个成员）");
	LPCTSTR	_ILEM_18 = _T("第%d个成员的数组尺寸存在书写错误");
	LPCTSTR	_ILEM_19 = _T("第%d个成员的数组尺寸不合法（不能是0或负数）");
	LPCTSTR	_ILEM_20 = _T("结构类型中又包含了结构类型（第%d个成员），这是非法的");
	LPCTSTR	_ILEM_21 = _T("第%d个成员的类型关键字拼写错误（\"%s\"不能被识别）");
	LPCTSTR	_ILEM_22 = _T("数组尺寸格式存在书写错误（应该是[n]的形式）");
	LPCTSTR	_ILEM_23 = _T("数组尺寸不合法（不能是0或负数）");
	
	LPCTSTR	_ILEM_24 = _T("数组缺少数据（应该有%d组数据，但实际只有%d组）");
	LPCTSTR	_ILEM_25 = _T("数组第%d个数据错误");
	LPCTSTR	_ILEM_26 = _T("缺少数据");
	LPCTSTR	_ILEM_27 = _T("数据错误，无法读取");
	
	LPCTSTR	_ILEM_28 = _T("第%d组数据缺少包裹符({ })");
	LPCTSTR	_ILEM_29 = _T("缺少包裹符({ })");
	LPCTSTR	_ILEM_30 = _T("缺少第%d个成员的数据");
	LPCTSTR	_ILEM_31 = _T("第%d个成员的数据错误，无法读取");
	LPCTSTR	_ILEM_32 = _T("第%d个成员的数据缺少数组数据包裹符（）");
	LPCTSTR	_ILEM_33 = _T("第%d个成员缺少数据（应该有%d个数据，但实际只有%d个）");
	LPCTSTR	_ILEM_34 = _T("第%d个成员的第%d个数据错误，无法读取");

	LPCTSTR	_ILEM_35 = _T("出现了无法解析的语句：\"%s\"");
	LPCTSTR	_ILEM_36 = _T("内参段只能有一个，而INI中出现了 %d 个内参段");
	LPCTSTR	_ILEM_37 = _T("非数组型结构变量不允许使用链表符(%s)");

#endif




////////////////////////////////////////////////////////////////////////
// UNICODE 相关设置 （此处UNICODE的具体编码格式为：X-Win UTF16LE BOM，即Windows系统UNICODE格式）

// 编译期提示
#ifdef _UNICODE
#pragma message("inilib is UNICODE state.")
#else
#pragma message("inilib is ANSI state.")
#endif

// UNICODE 文本文件 BOM 值
const unsigned short UNICODE_BOM = 0xfeff;

// UNICODE编码时，在需要换行的串尾部添加 “\r\n” ，ANSI编码时只添加 “\n”
// 因为UNICODE版本下，文件将以二进制方式打开，需手动添加\r\n，ANSI版本下
// ，文件以文本方式打开，系统在写入文件时，遇到\n字符，会自动在前面添加\r
// 字符。所以作此宏定义。
#ifdef _UNICODE
#define _T_CRLN(x)   L##x##L"\r\n"
#else
#define _T_CRLN(x)   x##"\n"
#endif




////////////////////////////////////////////////////////////////////////
// 串解析函数集
#include "inilib2.cpp"

// 内部测试函数集
//#include "inilib3.cpp"






////////////////////////////////////////////////////////////////////////
// INI库上下文类

// 压入错误信息
void _inicontextclass::push(LPCTSTR info)
{
	_tcscpy_s(m_data[m_cur++], _ICCBUFSIZE, info);
	_ASSERT(m_cur <= _ICCMAX_DPE);
};

// 弹出堆栈指针
void _inicontextclass::pop()
{
	m_cur--;
	_ASSERT(m_cur >= 0);
};

// 设置错误信息
//
// errtext	- IN - 错误原因的字符串，可带参数
//
// *注：参数只支持%d、%s两种，用法与printf()相同。
//
void _inicontextclass::seterror(LPCTSTR errtext, ...)
{
	LPCTSTR		format = errtext;
	LPTSTR		str;
	TCHAR		ptmp[256];
	TCHAR		c, ch;
	va_list		ap;

	m_buffer[0] = NULL;

	// 组合上下文路径
	for (int i=0; i<m_cur; i++)
	{
		_tcscat_s(m_buffer, _ICCMAX_DPE*_ICCBUFSIZE, m_data[i]);

		if ((i+1) < m_cur)
			_tcscat_s(m_buffer, _ICCMAX_DPE*_ICCBUFSIZE, _T("->"));
		else
			_tcscat_s(m_buffer, _ICCMAX_DPE*_ICCBUFSIZE, _T(" : "));
	}

	str = (LPTSTR)&m_buffer[_tcslen(m_buffer)];

	va_start(ap, errtext);

	// 根据可能有的可变参数填写错误信息字符串
	while((c = *format++))
	{
		switch(c)
		{
		case _T('%'):
			ch = *format++;
			switch(ch)
			{
			case _T('d'):		// int
				{
					int n = va_arg(ap, int);
					_itot_s(n, ptmp, 255, 10);
					*str = NULL;
					_tcscat_s(m_buffer, _ICCMAX_DPE*_ICCBUFSIZE, ptmp);
					str = (LPTSTR)&m_buffer[_tcslen(m_buffer)];
					break;
				}
			case _T('s'):		// string
				{
					LPCTSTR p = va_arg(ap, LPCTSTR);
					*str = NULL;
					_tcscat_s(m_buffer, _ICCMAX_DPE*_ICCBUFSIZE, p);
					str = (LPTSTR)&m_buffer[_tcslen(m_buffer)];
					break;
				}
			case _T('%'):
				*str++ = c;
				break;
			default:
				_ASSERT(0);
				break;
			}
			break;
		default:
			*str++ = c;
			break;
		}
	}  

	va_end(ap);  
	*str = NULL;

#ifdef _DEBUG
	int	rtp = _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	
	#ifdef _UNICODE
		_RPTW1(_CRT_WARN, L"\r\n%s\r\n", L"[inilib error information]");
		_RPTW0(_CRT_WARN, L"##------------------------------------------------------------------------------\r\n");
		_RPTW1(_CRT_WARN, L"##   %s\r\n", m_buffer);
		_RPTW0(_CRT_WARN, L"##------------------------------------------------------------------------------\r\n\r\n");
	#else
		_RPT1(_CRT_WARN, "\r\n%s\r\n", "[inilib error information]");
		_RPT0(_CRT_WARN, "##------------------------------------------------------------------------------\r\n");
		_RPT1(_CRT_WARN, "##   %s\r\n", m_buffer);
		_RPT0(_CRT_WARN, "##------------------------------------------------------------------------------\r\n\r\n");
	#endif

	_CrtSetReportMode(_CRT_WARN, rtp);
#endif	// _DEBUG
}



////////////////////////////////////////////////////////////////////////
// 变量基类实现

varbaseclass::varbaseclass()
{
	m_type		= NTP_MAX;
	m_name[0]	= _T('\0');
	m_pseg		= NULL;
	m_prev		= NULL;
	m_next		= NULL;

	for (int i=0; i<MAXVALSIZE; i++)
		m_data.m_buffer[i] = (size_t)0;
}

varbaseclass::~varbaseclass()
{
}


// tidy_vardec()函数内部用宏定义
#define __GSP_TIDY_VARDEC_FUNA(x)	ptidy[0] = _gsp_comp_attributemark(PCA_ATT_VAR, x); \
	ptidy[1] = varnamelen; \
	_tcscpy_s((LPTSTR)&ptidy[2], ptidy[1]+1, pbuffer); \
	pbcur += sizeof(DWORD)+sizeof(DWORD)+(ptidy[1]+1)*sizeof(TCHAR)

#define __GSP_TIDY_VARDEC_FUNB()	*((LPDWORD)pbcur) = (DWORD)arrsize;	\
	pbcur += sizeof(DWORD)


// 整理变量的声明行
//
// pvardectxt	- IN  - 变量声明行文本。
// tidytxtbegin	- IN  - 整理缓冲区写入地址
// tidytxtend	- OUT - 返回新的整理缓冲区写入地址
//
// 返回值：返回变量的类型值（包括错误代码）：
//
//			NTP_BYTE~NTP_ARRAY_STRUCT	变量类型码
//			NTP_EC_NOTMEM				内存申请失败
//			NTP_EC_SYNERR				原始文本语法错误
//			
// *注：pvardectxt中的文本不会被改动。
//		如果返回值是NTP_EC_NOTMEM或NTP_EC_SYNERR，tidytxtend指向的指针将被设为NULL。
//
DATATYPE varbaseclass::tidy_vardec(LPCTSTR pvardectxt, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	int				varnamelen, vartypelen, vararystrlen;	// 变量名长度、类型长度、数组尺寸串长度
	unsigned char	scale = 10;								// 进制，默认数值进制为十进制
	unsigned char	adtime = DATETYPE_DATE;					// date类型标识，0为简短型，1为完整型（DATE+TIME）
	DATATYPE		vartpe;									// 变量类型
	int				arrsize;								// 数组尺寸
	ILSUTMEMTPE		sutmemtype = ILSUTMEMTPE_ARRAY;			// 结构数组的内存形式(默认为数组形式)
	LPDWORD			ptidy = (LPDWORD)tidytxtbegin;			// 双字单位写入地址
	LPBYTE			pbcur = (LPBYTE)tidytxtbegin;			// 字节单位写入地址
	LPTSTR			parraysizemarkbegin;					// 数组[]标志位置
	LPTSTR			parraysizemarkend;


	// 新的写入地址先设为NULL
	*tidytxtend = NULL;

	// 确定赋值号的位置
	LPCTSTR pequ = _tcschr(pvardectxt, CT_ASSIGNMENT);
	_ASSERT(pequ);

	// 计算赋值号之前的字符个数
	int declen = (int)(pequ - pvardectxt);

	// 申请临时缓冲区
	LPTSTR pbuffer = new (std::nothrow) TCHAR[declen+1];

	if (pbuffer == NULL)
	{
		pict->seterror(_ILEM_1);
		return NTP_EC_NOTMEM;
	}

	// 复制赋值号以前的内容到临时缓冲区（尾部添加NULL符）
	memcpy((void*)pbuffer, (const void *)pvardectxt, (size_t)(declen*sizeof(TCHAR)));
	pbuffer[declen] = CT_NULL;

	// 搜索变量类型包裹符 <>
	LPTSTR pts = _tcschr(pbuffer, CT_VARNB);
	LPTSTR pte = _tcschr(pbuffer, CT_VARNE);

	_ASSERT(pts&&pte);

	*pts++ = CT_NULL;
	*pte++ = CT_NULL;

	// 去除变量名头尾的无用字符
	varnamelen= _gsp_tidy_string(pbuffer);

	pict->push(pbuffer);
	
	// 搜索结构类型变量的声明标志 ：
	LPTSTR	pstructmark = _tcschr(pts, CT_STRUCTMRK);

	// 因为复杂性的原因，以下将结构变量和非结构变量分开处理。

	if (pstructmark == NULL)	// 非结构变量
	{
		// 去除变量类型名头尾的无用字符
		vartypelen = _gsp_tidy_string(pts);

		// 检查类型名是否为空
		if (vartypelen == 0)
		{
			pict->seterror(_ILEM_10);
			delete []pbuffer;
			return NTP_EC_SYNERR;
		}

		// 判断是否存在数值进制符（byte、int两种类型可能会使用）
		if (pts[vartypelen-1] == CT_NUMTPE_HEX)
		{
			scale = 16;

			pts[vartypelen-1] = CT_NULL;
			vartypelen = _gsp_tidy_string(pts);

			if (vartypelen == 0)
			{
				pict->seterror(_ILEM_10);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			}
		}

		// 判断是否存在date类型的类型标识符（date类型可能会使用）
		if (pts[vartypelen-1] == CT_DATETIME)
		{
			adtime = DATETYPE_DATE_TIME;

			pts[vartypelen-1] = CT_NULL;
			vartypelen = _gsp_tidy_string(pts);

			if (vartypelen == 0)
			{
				pict->seterror(_ILEM_10);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			}
		}

		// 获取变量类型
		vartpe = _gsp_string_to_datatype(pts);

		if (vartpe == NTP_EC_SYNERR)
		{
			pict->seterror(_ILEM_11, pts);
			delete []pbuffer;
			return NTP_EC_SYNERR;
		}

		// 初始化数组尺寸值
		arrsize = 0;

		vararystrlen = _gsp_tidy_string(pte); 

		// 非结构、非数组的变量
		if (vararystrlen == 0)
		{
			switch (vartpe)
			{
			case	NTP_BYTE:				// [byte变量标志][变量名字长度][变量名字][进制值]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_BYTE);
				*pbcur++ = scale;
				break;
			case	NTP_INT:				// [int变量标志][变量名字长度][变量名字][进制值]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_INT);
				*pbcur++ = scale;
				break;
			case	NTP_BOOL:				// [bool变量标志][变量名字长度][变量名字]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_BOOL);
				break;
			case	NTP_DOUBLE:				// [double变量标志][变量名字长度][变量名字]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_DOUBLE);
				break;
			case	NTP_STRING:				// [string变量标志][变量名字长度][变量名字]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_STRING);
				break;
			case	NTP_DATE:				// [date变量标志][变量名字长度][变量名字][类型标识]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_DATE);
				*pbcur++ = adtime;
				break;
			case	NTP_STRUCT:
				pict->seterror(_ILEM_12);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			default:
				_ASSERT(0);
				pict->seterror(_ILEM_4);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			}
		}
		else	// 非结构、是数组的变量
		{
			// 搜索数组标志符 []
			parraysizemarkbegin = &pte[0];
			parraysizemarkend   = &pte[vararystrlen-1];

			// 初步判断[]符是否合法
			if ((parraysizemarkbegin[0] != CT_ARRYNB)||(parraysizemarkend[0] != CT_ARRYNE)||(vararystrlen < 3))
			{
				pict->seterror(_ILEM_13);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			}

			// 分隔元素个数数字
			*parraysizemarkbegin++ = CT_NULL;
			*parraysizemarkend++   = CT_NULL;

			vararystrlen = _gsp_tidy_string(parraysizemarkbegin);

			// 提取数组元素个数
			_stscanf_s(parraysizemarkbegin, _T("%d"), &arrsize);

			// 判断元素个数的合法性（0个元素或负个元素都是不合法的）
			if (arrsize < 1)
			{
				pict->seterror(_ILEM_14);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			}

			switch (vartpe)
			{
			case	NTP_BYTE:				// [byte数组标志][变量名字长度][变量名字][进制值][元素个数]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_BYTEARRAY);
				*pbcur++ = scale;
				__GSP_TIDY_VARDEC_FUNB();
				break;
			case	NTP_INT:				// [int数组标志][变量名字长度][变量名字][进制值][元素个数]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_INTARRAY);
				*pbcur++ = scale;
				__GSP_TIDY_VARDEC_FUNB();
				break;
			case	NTP_BOOL:				// [bool数组标志][变量名字长度][变量名字][元素个数]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_BOOLARRAY);
				__GSP_TIDY_VARDEC_FUNB();
				break;
			case	NTP_DOUBLE:				// [double数组标志][变量名字长度][变量名字][元素个数]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_DOUBLEARRAY);
				__GSP_TIDY_VARDEC_FUNB();
				break;
			case	NTP_STRING:				// [string数组标志][变量名字长度][变量名字][元素个数]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_STRINGARRAY);
				__GSP_TIDY_VARDEC_FUNB();
				break;
			case	NTP_DATE:				// [date数组标志][变量名字长度][变量名字][类型标识][元素个数]
				__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_DATEARRAY);
				*pbcur++ = adtime;
				__GSP_TIDY_VARDEC_FUNB();
				break;
			case	NTP_STRUCT:
				pict->seterror(_ILEM_12);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			default:
				_ASSERT(0);
				pict->seterror(_ILEM_4);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			}
		}

		delete []pbuffer;
		*tidytxtend = (LPTSTR)pbcur;

		// pict->pop();
		// 此处不放置上下文出栈指令，由tidy_data()函数执行出栈
		
		return (arrsize) ? (DATATYPE)(vartpe+NTP_ARRAY_BYTE) : vartpe;
	}
	else	// 结构变量(STRUCT)
	{
		*pstructmark++ = CT_NULL;

		// 去除变量类型名头尾的无用字符
		vartypelen = _gsp_tidy_string(pts);

		if (vartypelen == 0)
		{
			pict->seterror(_ILEM_15);
			delete []pbuffer;
			return NTP_EC_SYNERR;
		}

		// 获取结构在内存中的存放形式
		if (pts[vartypelen-1] == CT_SUTMEMTPE_LIST)
		{
			sutmemtype = ILSUTMEMTPE_LIST;

			pts[vartypelen-1] = CT_NULL;
			vartypelen = _gsp_tidy_string(pts);

			if (vartypelen == 0)
			{
				pict->seterror(_ILEM_10);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			}
		}

		// 获取变量类型
		vartpe = _gsp_string_to_datatype(pts);

		// 此处只能是结构类型，其他类型非法
		if (vartpe != NTP_STRUCT)
		{
			pict->seterror(_ILEM_16, pts);
			delete []pbuffer;
			return NTP_EC_SYNERR;
		}

		// [struct变量标志][变量名字长度][变量名字][内存形式][结构成员个数][变量表]

		// 写入结构变量标志、名称长度、变量名字。变量标志这里暂时使用PCA_ADD_VAR_STRUCT。
		__GSP_TIDY_VARDEC_FUNA(PCA_ADD_VAR_STRUCT);
		// 结构在内存中的存放形式
		LPDWORD	pmemtype= (LPDWORD)pbcur;
		// 获取结构成员个数的存放地址和变量表尺寸地址
		LPDWORD pmbcnt  = (pmemtype+1);
		LPDWORD pvtsize = (pmbcnt+1);
		// 指向变量表写入地址
		pbcur += sizeof(DWORD)*3;

		pmbcnt[0]  = 0;								// 结构中的变量个数
		pvtsize[0] = 0;								// 变量表尺寸（字节单位）

		LPTSTR	pnext = pstructmark;				// 下一个成员变量名的起始地址（往往是分隔符，）
		LPTSTR	pvari;								// 当前要处理的成员变量串

		do
		{
			pvari = pnext;
			pnext = _tcschr(pvari, CT_SPLIT);		// 搜索变量成员间的分隔符 ,

			if (pnext)
				*pnext++ = CT_NULL;

			// 去除成员变量串头尾的无用字符
			vartypelen = _gsp_tidy_string(pvari);

			if (vartypelen == 0)
			{
				pict->seterror(_ILEM_17, pmbcnt[0]+1);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			}

			arrsize = 0;

			// 判断成员变量是不是数组
			if (pvari[vartypelen-1] == CT_ARRYNE)
			{
				// 搜索数组标志符 []
				parraysizemarkbegin = _tcschr(pvari, CT_ARRYNB);
				parraysizemarkend   = &pvari[vartypelen-1];

				// 初步判断[]符是否合法
				if ((parraysizemarkbegin == NULL)||((parraysizemarkend-parraysizemarkbegin) < 2))
				{
					pict->seterror(_ILEM_18, pmbcnt[0]+1);
					delete []pbuffer;
					return NTP_EC_SYNERR;
				}

				// 分隔元素个数数字
				*parraysizemarkbegin++ = CT_NULL;
				*parraysizemarkend++   = CT_NULL;

				vararystrlen = _gsp_tidy_string(parraysizemarkbegin);

				// 提取数组元素个数
				_stscanf_s(parraysizemarkbegin, _T("%d"), &arrsize);

				// 判断元素个数的合法性（0个元素或负个元素都是不合法的）
				if (arrsize < 1)
				{
					pict->seterror(_ILEM_19, pmbcnt[0]+1);
					delete []pbuffer;
					return NTP_EC_SYNERR;
				}

				// 去除成员变量串头尾的无用字符
				vartypelen = _gsp_tidy_string(pvari);

				if (vartypelen == 0)
				{
					pict->seterror(_ILEM_17, pmbcnt[0]+1);
					delete []pbuffer;
					return NTP_EC_SYNERR;
				}
			}

			// 默认变量值都是十进制
			scale = 10;

			// 判断是否存在数值进制符
			if (pvari[vartypelen-1] == CT_NUMTPE_HEX)
			{
				scale = 16;
				pvari[vartypelen-1] = CT_NULL;

				vartypelen = _gsp_tidy_string(pvari);

				if (vartypelen == 0)
				{
					pict->seterror(_ILEM_17, pmbcnt[0]+1);
					delete []pbuffer;
					return NTP_EC_SYNERR;
				}
			}

			// 默认date都是简短类型
			adtime = DATETYPE_DATE;

			// 判断是否存在date的类型标识
			if (pvari[vartypelen-1] == CT_DATETIME)
			{
				adtime = DATETYPE_DATE_TIME;
				pvari[vartypelen-1] = CT_NULL;

				vartypelen = _gsp_tidy_string(pvari);

				if (vartypelen == 0)
				{
					pict->seterror(_ILEM_17, pmbcnt[0]+1);
					delete []pbuffer;
					return NTP_EC_SYNERR;
				}
			}

			// 获取变量类型
			vartpe = _gsp_string_to_datatype(pvari);

			// 写入变量表
			switch (vartpe)
			{
			case	NTP_BYTE:
				if (arrsize == 0)			// [00][进制1]
				{
					*pbcur++ = PCA_ADD_VAR_BYTE;
					*pbcur++ = scale;
				}
				else						// [07][进制1][元素个数4]
				{
					*pbcur++ = PCA_ADD_VAR_BYTEARRAY;
					*pbcur++ = scale;
					*((LPDWORD)pbcur) = arrsize;
					pbcur += sizeof(DWORD);
				}
				break;
			case	NTP_INT:
				if (arrsize == 0)			// [01][进制1]	
				{
					*pbcur++ = PCA_ADD_VAR_INT;
					*pbcur++ = scale;
				}
				else						// [08][进制1][元素个数4]
				{
					*pbcur++ = PCA_ADD_VAR_INTARRAY;
					*pbcur++ = scale;
					*((LPDWORD)pbcur) = arrsize;
					pbcur += sizeof(DWORD);
				}
				break;
			case	NTP_BOOL:
				if (arrsize == 0)			// [02]
				{
					*pbcur++ = PCA_ADD_VAR_BOOL;
				}
				else						// [09][元素个数4]
				{
					*pbcur++ = PCA_ADD_VAR_BOOLARRAY;
					*((LPDWORD)pbcur) = arrsize;
					pbcur += sizeof(DWORD);
				}
				break;
			case	NTP_DOUBLE:
				if (arrsize == 0)			// [03]
				{
					*pbcur++ = PCA_ADD_VAR_DOUBLE;
				}
				else						// [10][元素个数4]
				{
					*pbcur++ = PCA_ADD_VAR_DOUBLEARRAY;
					*((LPDWORD)pbcur) = arrsize;
					pbcur += sizeof(DWORD);
				}
				break;
			case	NTP_STRING:
				if (arrsize == 0)			// [04]
				{
					*pbcur++ = PCA_ADD_VAR_STRING;
				}
				else						// [11][元素个数4]
				{
					*pbcur++ = PCA_ADD_VAR_STRINGARRAY;
					*((LPDWORD)pbcur) = arrsize;
					pbcur += sizeof(DWORD);
				}
				break;
			case	NTP_DATE:
				if (arrsize == 0)			// [05][类型标识1]	
				{
					*pbcur++ = PCA_ADD_VAR_DATE;
					*pbcur++ = adtime;
				}
				else						// [12][类型标识1][元素个数4]
				{
					*pbcur++ = PCA_ADD_VAR_DATEARRAY;
					*pbcur++ = adtime;
					*((LPDWORD)pbcur) = arrsize;
					pbcur += sizeof(DWORD);
				}
				break;
			case	NTP_STRUCT:				// 结构中不能再包含结构成员
				pict->seterror(_ILEM_20, pmbcnt[0]+1);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			case	NTP_EC_SYNERR:
				pict->seterror(_ILEM_21, pmbcnt[0]+1, pvari);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			default:						// 代码设计错误
				_ASSERT(0);
				pict->seterror(_ILEM_4);
				delete []pbuffer;
				return NTP_EC_SYNERR;
			}

			pmbcnt[0]++;					// 成员变量个数累加

		} while (pnext);

		// 填写结构内存形式
		pmemtype[0] = sutmemtype;
		// 填写变量表尺寸（字节单位）
		pvtsize[0] = (DWORD)(pbcur-(LPBYTE)(pvtsize+1));
		// 整理可能存在的数组元素个数字符串
		vararystrlen = _gsp_tidy_string(pte);

		if (vararystrlen == 0)				// 非数组
		{
			delete []pbuffer;

			// 非数组结构不允许使用链表符
			if (sutmemtype == ILSUTMEMTPE_LIST)
			{
				pict->seterror(_ILEM_37, CT_SUTMEMTPE_LIST_STR);
				return NTP_EC_SYNERR;
			}

			*tidytxtend = (LPTSTR)pbcur;
			// pict->pop();
			// 此处不放置上下文出栈指令，由tidy_data()函数执行出栈
			return NTP_STRUCT;
		}

		// 搜索数组标志符 []
		parraysizemarkbegin = &pte[0];
		parraysizemarkend   = &pte[vararystrlen-1];

		// 初步判断[]符是否合法
		if ((parraysizemarkbegin[0] != CT_ARRYNB)||(parraysizemarkend[0] != CT_ARRYNE)||((parraysizemarkend-parraysizemarkbegin) < 2))
		{
			pict->seterror(_ILEM_22);
			delete []pbuffer;
			return NTP_EC_SYNERR;
		}

		// 分隔元素个数数字
		*parraysizemarkbegin++ = CT_NULL;
		*parraysizemarkend++   = CT_NULL;

		vararystrlen = _gsp_tidy_string(parraysizemarkbegin);

		// 提取数组元素个数
		_stscanf_s(parraysizemarkbegin, _T("%d"), &arrsize);

		// 判断元素个数的合法性（0个元素或负个元素都是不合法的）
		if (arrsize < 1)
		{
			pict->seterror(_ILEM_23);
			delete []pbuffer;
			return NTP_EC_SYNERR;
		}

		delete []pbuffer;

		// 写入数组元素个数
		*((LPDWORD)pbcur) = arrsize;
		pbcur += sizeof(DWORD);

		// 更新整理缓冲区的写入地址，及结构变量属性
		*tidytxtend = (LPTSTR)pbcur;
		ptidy[0] = _gsp_comp_attributemark(PCA_ATT_VAR, PCA_ADD_VAR_STRUCTARRAY);

		// pict->pop();
		// 此处不放置上下文出栈指令，由tidy_data()函数执行出栈
		
		return NTP_ARRAY_STRUCT;
	}
}


// 整理变量的数据部分
//
// pvarbegin	- IN  - 变量声明行文本。
// pvarend		- IN  - 变量结尾地址。
// pvartab		- IN  - 变量声明的已整理数据地址。
// tidytxtbegin	- IN  - 整理缓冲区写入地址
// tidytxtend	- OUT - 返回新的整理缓冲区写入地址
// pict			- IN  - 上下文类地址
//
// 返回值：
//			0  - 整理成功
//			-1 - 内存申请失败
//			-2 - 原始文本语法错误
//			-3 - 系统异常
//
// *注：
//
int varbaseclass::tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	BYTE	varattr, addition;
	int		retcode;

	// 入栈指令在tidy_vardec()函数中
	
	// 分解预编译数据中的数据属性标志
	if (_gsp_decomp_attributemark(((LPDWORD)pvartab)[0], &varattr, &addition) == false)
	{
		_ASSERT(0);
		pict->seterror(_ILEM_4);
		return -3;
	}

	_ASSERT(varattr == PCA_ATT_VAR);

	switch (addition)
	{
	case	PCA_ADD_VAR_BYTE:
		retcode = varbyte::tidy_data(pvarbegin, pvarend, pvartab, false, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_INT:
		retcode = varint::tidy_data(pvarbegin, pvarend, pvartab, false, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_BOOL:
		retcode = varbool::tidy_data(pvarbegin, pvarend, pvartab, false, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_DOUBLE:
		retcode = vardouble::tidy_data(pvarbegin, pvarend, pvartab, false, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_STRING:
		retcode = varstring::tidy_data(pvarbegin, pvarend, pvartab, false, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_DATE:
		retcode = vardate::tidy_data(pvarbegin, pvarend, pvartab, false, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_STRUCT:
		retcode = varstruct::tidy_data(pvarbegin, pvarend, pvartab, false, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_BYTEARRAY:
		retcode = varbyte::tidy_data(pvarbegin, pvarend, pvartab, true, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_INTARRAY:
		retcode = varint::tidy_data(pvarbegin, pvarend, pvartab, true, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_BOOLARRAY:
		retcode = varbool::tidy_data(pvarbegin, pvarend, pvartab, true, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_DOUBLEARRAY:
		retcode = vardouble::tidy_data(pvarbegin, pvarend, pvartab, true, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_STRINGARRAY:
		retcode = varstring::tidy_data(pvarbegin, pvarend, pvartab, true, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_DATEARRAY:
		retcode = vardate::tidy_data(pvarbegin, pvarend, pvartab, true, tidytxtbegin, tidytxtend, pict);
		break;
	case	PCA_ADD_VAR_STRUCTARRAY:
		retcode = varstruct::tidy_data(pvarbegin, pvarend, pvartab, true, tidytxtbegin, tidytxtend, pict);
		break;
	default:
		_ASSERT(0);
		pict->seterror(_ILEM_4);
		return -3;
	}
	
	// 出栈。
	if (retcode == 0)
		pict->pop();

	return retcode;
}


////////////////////////////////////////////////////////////////////////
// 空行类实现

linevoid::~linevoid()
{
	destroy();
}


// 整理输入文本
//
// tidytxtbegin - IN  - 整理缓冲区写入地址
// tidytxtend   - OUT - 返回的新的整理缓冲区写入地址
//
void linevoid::tidy(LPTSTR tidytxtbegin, LPTSTR *tidytxtend)
{
	// [空行标志]

	// 合成数据属性标志，并写入整理缓冲区
	*((LPDWORD)tidytxtbegin) = _gsp_comp_attributemark(PCA_ATT_VOID, 0);
	// 返回新的整理缓冲区写入地址
	*tidytxtend = (LPTSTR)(((LPDWORD)tidytxtbegin)+1);
}


// 初始化变量类
//
// pstr		- IN  - 变量预编译数据起始地址
// inseg	- IN  - 内参段地址
// pend		- OUT - 返回变量预编译数据结尾地址
//
// 返回值：0-成功、-1 - 内存不足、-3 - 系统异常
//
// *注：
// 
int linevoid::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	// [空行标志]

	LPDWORD			pmark = (LPDWORD)pstr;
	BYTE			attr, addition;

	// 分解数据属性标志
	bool bm = _gsp_decomp_attributemark(pmark[0], &attr, &addition);

	_ASSERT(bm);
	_ASSERT((attr == PCA_ATT_VOID)&&(addition == 0));

	if (bm == false)
		return -3;				// 系统异常

	m_type = NTP_VOID_LINE;		// 变量类型
	m_name[0] = _T('\0');		// 空行没有名字

	// 返回变量预编译数据结尾地址
	*pend = (LPCTSTR)&pmark[1];

	return 0;
}


// 销毁变量类中的内容
//
// *注：函数释放类中的相关资源，将类变为一个‘空白’变量类。
//		函数并不会把类从变量类链表中摘除，需要上一级的调用函数自行处理。
//
void linevoid::destroy()
{
	_ASSERT(m_type == NTP_VOID_LINE);

	m_name[0]	= _T('\0');
	m_type		= NTP_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功， -2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int linevoid::write(FILE *fp, insideseg* inseg)
{
	_ASSERT(m_type == NTP_VOID_LINE);		// 无效的类应该会已经从变量链表中摘除

	size_t	sl = _tcslen(CT_CRNL);
	
	if (m_type == NTP_VOID_LINE)
	{
		if (fwrite(CT_CRNL, sizeof(TCHAR), sl, fp) == sl)
			return 0;
		else
			return -2;
	}
	else
	{
		return -3;
	}
}



////////////////////////////////////////////////////////////////////////
// 注释类实现

linecomm::~linecomm()
{
	destroy();
}


// 整理输入文本
//
// str			- IN  - 注释文本
// tidytxtbegin	- IN  - 整理缓冲区写入地址
// tidytxtend	- OUT - 返回新的整理缓冲区写入地址
//
void linecomm::tidy(LPCTSTR str, LPTSTR tidytxtbegin, LPTSTR *tidytxtend)
{
	// [注释标志][注释长度][注释内容]

	LPDWORD	p = (LPDWORD)tidytxtbegin;

	// 向整理缓冲区写入注释标志、长度、注释内容
	p[0] = _gsp_comp_attributemark(PCA_ATT_COMM, PCA_ADD_LINECOMM);
	p[1] = (DWORD)_tcslen(str);
	_tcscpy_s((LPTSTR)&p[2], p[1]+1, str);

	// 返回新的整理缓冲区写入地址
	LPBYTE pb = (LPBYTE)tidytxtbegin;

	pb += sizeof(DWORD)+sizeof(DWORD)+(p[1]+1)*sizeof(TCHAR);
	*tidytxtend = (LPTSTR)pb;

}


// 初始化变量类
//
// pstr		- IN  - 变量预编译数据起始地址
// inseg	- IN  - 内参段地址
// pend		- OUT - 返回变量预编译数据结尾地址
//
// 返回值：0-成功、-1 - 内存不足、-3 - 系统异常
//
// *注：
// 
int linecomm::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	// [注释标志][注释长度4][注释内容]

	LPDWORD			pmark = (LPDWORD)pstr;
	int*			plen  = (int*)&pmark[1];
	LPCTSTR			pcomm = (LPCTSTR)&plen[1];
	BYTE			attr, addition;

	// 分解数据属性标志
	bool bm = _gsp_decomp_attributemark(pmark[0], &attr, &addition);

	_ASSERT(bm);
	_ASSERT((attr == PCA_ATT_COMM)&&(addition == PCA_ADD_LINECOMM));

	if (bm == false)
		return -3;				// 系统异常

	m_type = NTP_COMM_LINE;		// 变量类型
	m_name[0] = _T('\0');		// 注释行没有名字

	m_data.m_linecomm.len = *plen;
	m_data.m_linecomm.pvalue = new (std::nothrow) TCHAR[m_data.m_linecomm.len+1];
	if (m_data.m_linecomm.pvalue == NULL)
		return -1;

	_tcscpy_s(m_data.m_linecomm.pvalue, m_data.m_linecomm.len+1, pcomm);

	// 返回变量预编译数据结尾地址
	*pend = &pcomm[m_data.m_linecomm.len+1];

	return 0;
}


// 销毁变量类中的内容
//
// *注：函数释放类中的相关资源，将类变为一个‘空白’变量类。
//		函数并不会把类从变量类链表中摘除，需要上一级的调用函数自行处理。
//
void linecomm::destroy()
{
	_ASSERT(m_type == NTP_COMM_LINE);

	if (m_data.m_linecomm.pvalue)
	{
		delete []m_data.m_linecomm.pvalue;
		m_data.m_linecomm.pvalue = NULL;
	}

	m_data.m_linecomm.len = 0;

	m_name[0]	= _T('\0');
	m_type		= NTP_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功， -2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int linecomm::write(FILE *fp, insideseg* inseg)
{
	// 无效的类应该已经从变量链表中摘除
	_ASSERT(m_type == NTP_COMM_LINE);

	if (m_type == NTP_COMM_LINE)
	{
		// 不存在长度为0的注释（最起码会有注释符号//）
		if ((m_data.m_linecomm.len == 0)||(m_data.m_linecomm.pvalue == NULL))
			return -3;

		// 将注释字符串写入文件
		if (fwrite(m_data.m_linecomm.pvalue, (size_t)m_data.m_linecomm.len*sizeof(TCHAR), 1, fp) == 0)
			return -2;
		// 单行注释信息尾部没有回车换行符（预编译时期去掉了），需要单独加上
		if (fwrite(CT_CRNL, _tcslen(CT_CRNL)*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		return 0;
	}
	else
	{
		return -3;
	}
}



////////////////////////////////////////////////////////////////////////
// BYTE变量类实现

varbyte::~varbyte()
{
	destroy();
}

// 整理输入文本的数据部分
//
// pvarbegin		- IN  - 变量声明行首地址
// pvarend			- IN  - 变量定义结束地址（数据部分结尾）
// pvartab			- IN  - 变量整理后的参数起始地址
// isarray			- IN  - 变量是否为数组的标志，true:数组，false:非数组
// tidytxtbegin		- IN  - 整理缓冲区写入地址
// tidytxtend		- OUT - 返回整理缓冲区新的写入地址
// pict				- IN  - 上下文类地址
//
// 返回值： 0 - 成功读取
//		   -1 - 语法错误
//
int varbyte::tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	unsigned char bte = 0;
	unsigned char *pt = (unsigned char *)pvartab;
	unsigned char scale;
	LPCTSTR pdb;	// 数值开始位置
	LPCTSTR pde;	// 数值结束位置


	// 确定赋值号的位置
	LPCTSTR pequ = _tcschr(pvarbegin, CT_ASSIGNMENT);
	_ASSERT(pequ);

	// 指向赋值号后面的字符
	pequ++;

	if (isarray)
	{
		// [byte数组标志][变量名字长度][变量名字][进制值][元素个数]
		pt += sizeof(DWORD);
		pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);

		// 获取进制值
		scale = *pt++;
		// 获取数组元素个数
		int elecnt = *((LPDWORD)pt);

		pde = pequ;
		pt  = ((unsigned char *)tidytxtbegin);

		for (int i=0; i<elecnt; i++)
		{
			// 判断是否存在缺少元素的情况
			if (pde >= pvarend)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}

			// 跨过数值前面可能存在的空白字符
			if ((pdb=_gsp_strspn(pde, pvarend, GSP_WHITESPC3)) == NULL)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}
			// 找到数值结尾
			if ((pde=_gsp_strpbrk(pdb, pvarend, GSP_WHITESPC3)) == NULL)
				pde = pvarend;

			// 读取byte值
			if (_gsp_read_byte(pdb, scale, &bte) == false)
			{
				pict->seterror(_ILEM_25, i+1);
				return -1;	// 语法错误
			}

			// 写入整理缓冲区
			*pt++ = bte;
		}

		// 更新整理缓冲区写入地址
		*tidytxtend = (LPTSTR)pt;
	}
	else
	{
		// [byte变量标志][变量名字长度][变量名字][进制值]
		pt += sizeof(DWORD);
		pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);

		// 获取进制值
		scale = *pt;

		// 跨过数值前面可能存在的空白字符
		if ((pdb=_gsp_strspn(pequ, pvarend, GSP_WHITESPC2)) == NULL)
		{
			pict->seterror(_ILEM_26);
			return -1;
		}

		// 读取byte值
		if (_gsp_read_byte(pdb, scale, &bte) == false)
		{
			pict->seterror(_ILEM_27);
			return -1;	// 语法错误
		}

		// 写入整理缓冲区
		pt = ((unsigned char *)tidytxtbegin);

		*pt++ = bte;

		// 更新整理缓冲区写入地址
		*tidytxtend = (LPTSTR)pt;
	}

	return 0;
}


// 初始化变量类
//
// pstr		- IN  - 变量预编译数据起始地址
// inseg	- IN  - 内参段地址
// pend		- OUT - 返回变量预编译数据结尾地址
//
// 返回值：0-成功、-1 - 内存不足、-3 - 系统异常
//
// *注：
// 
int varbyte::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	// [byte变量标志][变量名字长度][变量名字][进制值1][变量值1]
	// [byte数组标志][变量名字长度][变量名字][进制值1][元素个数4][[变量值1],[变量值1],[变量值1],...]

	LPDWORD			pmark = (LPDWORD)pstr;
	int*			pnlen = (int*)&pmark[1];
	LPCTSTR			pname = (LPCTSTR)&pnlen[1];
	LPBYTE			pscale= (LPBYTE)&pname[pnlen[0]+1];
	int*			pele  = (int*)&pscale[1];
	LPBYTE			pval;
	BYTE			attr, addition;

	// 分解数据属性标志
	bool bm = _gsp_decomp_attributemark(pmark[0], &attr, &addition);

	_ASSERT(bm);
	_ASSERT((attr == PCA_ATT_VAR)&&((addition == PCA_ADD_VAR_BYTE)||(addition == PCA_ADD_VAR_BYTEARRAY)));

	if (bm == false)
		return -3;		// 系统异常

	m_type = (addition == PCA_ADD_VAR_BYTE) ? NTP_BYTE : NTP_ARRAY_BYTE;		// 变量类型
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, pname);								// 变量名字

	_ASSERT((pscale[0]==10)||(pscale[0]==16));

	if (m_type == NTP_BYTE)
	{
		m_data.m_byte.scale = pscale[0];
		m_data.m_byte.value = pscale[1];
		pval = &pscale[2];
	}
	else
	{
		m_data.m_bytearray.scale  = pscale[0];
		m_data.m_bytearray.elecnt = pele[0];
		m_data.m_bytearray.pvalue = new (std::nothrow) BYTE[m_data.m_bytearray.elecnt];		// 申请数组内存
		pval = (LPBYTE)&pele[1];

		if (m_data.m_bytearray.pvalue == NULL)
			return -1;	// 内存申请失败

		// 复制数组数据
		for (int i=0; i<m_data.m_bytearray.elecnt; i++)
		{
			m_data.m_bytearray.pvalue[i] = pval[i];
		}

		pval = &pval[m_data.m_bytearray.elecnt];
	}

	// 返回变量预编译数据结尾地址
	*pend = (LPCTSTR)pval;

	return 0;
}


// 销毁变量类中的内容
//
// *注：函数释放类中的相关资源，将类变为一个‘空白’变量类。
//		函数并不会把类从变量类链表中摘除，需要上一级的调用函数自行处理。
//
void varbyte::destroy()
{
	_ASSERT((m_type == NTP_BYTE)||(m_type == NTP_ARRAY_BYTE));

	if (m_type == NTP_BYTE)
	{
		_ASSERT((m_data.m_byte.scale==10)||(m_data.m_byte.scale==16));
		m_data.m_byte.scale = 0;
		m_data.m_byte.value = 0;
	}
	else
	{
		_ASSERT((m_data.m_bytearray.scale==10)||(m_data.m_bytearray.scale==16));
		m_data.m_bytearray.scale  = 0;
		m_data.m_bytearray.elecnt = 0;

		if (m_data.m_bytearray.pvalue)
			delete []m_data.m_bytearray.pvalue;			// 释放数组内存

		m_data.m_bytearray.pvalue = NULL;
	}

	m_name[0]	= _T('\0');
	m_type		= NTP_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功， -2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int varbyte::write(FILE *fp, insideseg* inseg)
{
	const size_t	BUFSIZE = (size_t)MAX_VARNAME_LENGTH*2;
	TCHAR			buffer[BUFSIZE];

	_ASSERT((m_type == NTP_BYTE)||(m_type == NTP_ARRAY_BYTE));

	if (m_type == NTP_BYTE)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					(m_data.m_byte.scale == 16) ? CT_NUMTPE_HEX_STR:_T(""),
					CT_VARDSNE, 
					_T("\t= "));

		// 转换数值
		if (m_data.m_byte.scale == 10)
			_itot_s((int)(char)m_data.m_byte.value, &buffer[_tcslen(buffer)], BUFSIZE-_tcslen(buffer), m_data.m_byte.scale);
		else if (m_data.m_byte.scale == 16)		// 十六进制的数值不能有负号，所以都按无符号数处理
			_itot_s((int)(unsigned char)m_data.m_byte.value, &buffer[_tcslen(buffer)], BUFSIZE - _tcslen(buffer), m_data.m_byte.scale);
		else
			return -3;

		_tcscat_s(buffer, BUFSIZE, CT_CRNL);

		// 写入文件
		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
		else
			return 0;
	}
	else if (m_type == NTP_ARRAY_BYTE)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s%s%d%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					(m_data.m_bytearray.scale == 16) ? CT_NUMTPE_HEX_STR:_T(""),
					CT_VARDSNE, 
					CT_ARRYSNB, 
					m_data.m_bytearray.elecnt, 
					CT_ARRYSNE, 
					_T("\t= "));

		size_t	deslen = _tcslen(buffer);

		// 写入变量声明
		if (fwrite(buffer, deslen*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		buffer[0] = CT_NULL;

		bool	isone = true;		// 是否为第一行
		int		width = 20;			// 折行的标准（2~50）

		// 写入数组数据
		for (int i=0; i<m_data.m_bytearray.elecnt; i++)
		{
			// 分行处理
			if ((i%width) == 0)
			{
				if (isone)
					isone = false;
				else
				{
					_tcscat_s(buffer, BUFSIZE, CT_CRNL);

					if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
						return -2;

					// 数据对齐
					_gsp_strset(buffer, BUFSIZE, CT_SPLIT2, deslen);
				}
			}

			if (m_data.m_bytearray.scale == 10)
				_itot_s((int)(char)(m_data.m_bytearray.pvalue[i]), &buffer[_tcslen(buffer)], BUFSIZE-_tcslen(buffer), m_data.m_bytearray.scale);
			else if (m_data.m_bytearray.scale == 16)
				_itot_s((int)(unsigned char)(m_data.m_bytearray.pvalue[i]), &buffer[_tcslen(buffer)], BUFSIZE - _tcslen(buffer), m_data.m_bytearray.scale);
			else
				return -3;
			
			// 分割符
			if ((i+1) < m_data.m_bytearray.elecnt)
				_tcscat_s(buffer, BUFSIZE, GSP_SPL_STR);
		}

		_tcscat_s(buffer, BUFSIZE, CT_CRNL);

		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		return 0;
	}
	else
	{
		return -3;
	}
}


// 生成一个从程序内部创建的变量对象
//
// name		- IN - 变量名称
// value	- IN - 变量值
// scale	- IN - 变量进制值
//
// 返回值：0 - 成功
//
// *注：
//
int varbyte::build(LPCTSTR name, BYTE value, BYTE scale)
{
	_ASSERT(m_type == NTP_MAX);		// 必须是一个空类
	_ASSERT(name&&_tcslen(name));	// 变量名称不能为空
	_ASSERT((scale == 10)||(scale == 16));		// 进制值只能是10或16

	m_type = NTP_BYTE;
	
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	m_data.m_byte.value = value;
	m_data.m_byte.scale = scale;
	
	return 0;
}


// 生成一个从程序内部创建的数组变量对象
//
// name		- IN - 变量名称
// elecnt	- IN - 数组尺寸值
// scale	- IN - 变量进制值
//
// 返回值：0 - 成功， -1 - 内存不足
//
// *注：数组缓冲区将会被初始化为0，调用者需要在本调用结束后填写数组内容
//
int varbyte::buildarray(LPCTSTR name, int elecnt, BYTE scale)
{
	_ASSERT(m_type == NTP_MAX);		// 必须是一个空类
	_ASSERT(name&&_tcslen(name));	// 变量名称不能为空
	_ASSERT(elecnt > 0);			// 数组尺寸不能为0
	_ASSERT((scale == 10)||(scale == 16));		// 进制值只能是10或16

	// 申请数组缓冲区
	LPBYTE	p = new (std::nothrow) BYTE[elecnt];

	if (p == NULL) return -1;

	// 初始化数组缓冲区
	memset((void*)p, 0, elecnt);

	m_type = NTP_ARRAY_BYTE;
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	m_data.m_bytearray.scale  = scale;
	m_data.m_bytearray.elecnt = elecnt;
	m_data.m_bytearray.pvalue = p;

	return 0;
}



////////////////////////////////////////////////////////////////////////
// int变量类实现

varint::~varint()
{
	destroy();
}

// 整理输入文本的数据部分
//
// pvarbegin		- IN  - 变量声明行首地址
// pvarend			- IN  - 变量定义结束地址（数据部分结尾）
// pvartab			- IN  - 变量整理后的参数起始地址
// isarray			- IN  - 变量是否为数组的标志，true:数组，false:非数组
// tidytxtbegin		- IN  - 整理缓冲区写入地址
// tidytxtend		- OUT - 返回整理缓冲区新的写入地址
// pict				- IN  - 上下文类地址
//
// 返回值： 0 - 成功读取
//		   -1 - 语法错误
//
int varint::tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	unsigned char *pt = (unsigned char *)pvartab;
	unsigned char scale;
	int		*pd;
	int		inter;
	LPCTSTR pdb;	// 数值开始位置
	LPCTSTR pde;	// 数值结束位置


	// 确定赋值号的位置
	LPCTSTR pequ = _tcschr(pvarbegin, CT_ASSIGNMENT);
	_ASSERT(pequ);

	// 指向赋值号后面的字符
	pequ++;

	if (isarray)
	{
		// [int数组标志][变量名字长度][变量名字][进制值][元素个数]
		pt += sizeof(DWORD);
		pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);

		// 获取进制值
		scale = *pt++;
		// 获取数组元素个数
		int elecnt = *((LPDWORD)pt);

		pde = pequ;
		pd  = ((int*)tidytxtbegin);

		for (int i=0; i<elecnt; i++)
		{
			// 判断是否存在缺少元素的情况
			if (pde >= pvarend)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}

			// 跨过数值前面可能存在的空白字符
			if ((pdb=_gsp_strspn(pde, pvarend, GSP_WHITESPC3)) == NULL)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}
			// 找到数值结尾
			if ((pde=_gsp_strpbrk(pdb, pvarend, GSP_WHITESPC3)) == NULL)
				pde = pvarend;

			// 读取int值
			if (_gsp_read_int(pdb, scale, &inter) == false)
			{
				pict->seterror(_ILEM_25, i+1);
				return -1;	// 语法错误
			}

			// 写入整理缓冲区
			*pd++ = inter;
		}

		// 更新整理缓冲区写入地址
		*tidytxtend = (LPTSTR)pd;
	}
	else
	{
		// [int变量标志][变量名字长度][变量名字][进制值]
		pt += sizeof(DWORD);
		pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);

		// 获取进制值
		scale = *pt;

		// 跨过数值前面可能存在的空白字符
		if ((pdb=_gsp_strspn(pequ, pvarend, GSP_WHITESPC2)) == NULL)
		{
			pict->seterror(_ILEM_26);
			return -1;
		}

		// 读取int值
		if (_gsp_read_int(pdb, scale, &inter) == false)
		{
			pict->seterror(_ILEM_27);
			return -1;	// 语法错误
		}

		// 写入整理缓冲区
		pd = ((int*)tidytxtbegin);

		*pd++ = inter;

		// 更新整理缓冲区写入地址
		*tidytxtend = (LPTSTR)pd;
	}

	return 0;
}


// 初始化变量类
//
// pstr		- IN  - 变量预编译数据起始地址
// inseg	- IN  - 内参段地址
// pend		- OUT - 返回变量预编译数据结尾地址
//
// 返回值：0-成功、-1 - 内存不足、-3 - 系统异常
//
// *注：
// 
int varint::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	// [int变量标志][变量名字长度][变量名字][进制值1][变量值4]
	// [int数组标志][变量名字长度][变量名字][进制值1][元素个数4][[变量值4],[变量值4],[变量值4],...]

	LPDWORD			pmark = (LPDWORD)pstr;
	int*			pnlen = (int*)&pmark[1];
	LPCTSTR			pname = (LPCTSTR)&pnlen[1];
	LPBYTE			pscale= (LPBYTE)&pname[pnlen[0]+1];
	int*			pele  = (int*)&pscale[1];
	int*			pval;
	BYTE			attr, addition;

	bool bm = _gsp_decomp_attributemark(pmark[0], &attr, &addition);

	_ASSERT(bm);
	_ASSERT((attr == PCA_ATT_VAR)&&((addition == PCA_ADD_VAR_INT)||(addition == PCA_ADD_VAR_INTARRAY)));

	if (bm == false)
		return -3;

	m_type = (addition == PCA_ADD_VAR_INT) ? NTP_INT : NTP_ARRAY_INT;
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, pname);

	_ASSERT((pscale[0]==10)||(pscale[0]==16));

	if (m_type == NTP_INT)
	{
		m_data.m_int.scale = pscale[0];
		pval = (int*)&pscale[1];
		m_data.m_int.value = pval[0];
		pval = &pval[1];
	}
	else
	{
		m_data.m_intarray.scale  = pscale[0];
		m_data.m_intarray.elecnt = pele[0];
		m_data.m_intarray.pvalue = new (std::nothrow) int[m_data.m_intarray.elecnt];
		pval = &pele[1];

		if (m_data.m_intarray.pvalue == NULL)
			return -1;

		for (int i=0; i<m_data.m_intarray.elecnt; i++)
		{
			m_data.m_intarray.pvalue[i] = pval[i];
		}

		pval = &pval[m_data.m_intarray.elecnt];
	}

	*pend = (LPCTSTR)pval;

	return 0;
}


// 销毁变量类中的内容
//
// *注：函数释放类中的相关资源，将类变为一个‘空白’变量类。
//		函数并不会把类从变量类链表中摘除，需要上一级的调用函数自行处理。
//
void varint::destroy()
{
	_ASSERT((m_type == NTP_INT)||(m_type == NTP_ARRAY_INT));

	if (m_type == NTP_INT)
	{
		_ASSERT((m_data.m_int.scale==10)||(m_data.m_int.scale==16));
		m_data.m_int.scale = 0;
		m_data.m_int.value = 0;
	}
	else
	{
		_ASSERT((m_data.m_intarray.scale==10)||(m_data.m_intarray.scale==16));
		m_data.m_intarray.scale  = 0;
		m_data.m_intarray.elecnt = 0;

		if (m_data.m_intarray.pvalue)
			delete []m_data.m_intarray.pvalue;

		m_data.m_intarray.pvalue = NULL;
	}

	m_name[0]	= _T('\0');
	m_type		= NTP_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功， -2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int varint::write(FILE *fp, insideseg* inseg)
{
	const size_t	BUFSIZE = (size_t)MAX_VARNAME_LENGTH*3;
	TCHAR			buffer[BUFSIZE];

	_ASSERT((m_type == NTP_INT)||(m_type == NTP_ARRAY_INT));

	if (m_type == NTP_INT)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					(m_data.m_int.scale == 16) ? CT_NUMTPE_HEX_STR:_T(""),
					CT_VARDSNE, 
					_T("\t= "));

		// 转换数值
		_itot_s(m_data.m_int.value, &buffer[_tcslen(buffer)], BUFSIZE-_tcslen(buffer), m_data.m_int.scale);
		_tcscat_s(buffer, BUFSIZE, CT_CRNL);

		// 写入文件
		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
		else
			return 0;
	}
	else if (m_type == NTP_ARRAY_INT)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s%s%d%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					(m_data.m_intarray.scale == 16) ? CT_NUMTPE_HEX_STR:_T(""),
					CT_VARDSNE, 
					CT_ARRYSNB, 
					m_data.m_intarray.elecnt, 
					CT_ARRYSNE, 
					_T("\t= "));

		size_t	deslen = _tcslen(buffer);

		// 写入变量声明
		if (fwrite(buffer, deslen*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		buffer[0] = CT_NULL;

		bool	isone = true;		// 是否为第一行
		int		width = 20;			// 折行的标准（2~50）

		// 写入数组数据
		for (int i=0; i<m_data.m_intarray.elecnt; i++)
		{
			// 分行处理
			if ((i%width) == 0)
			{
				if (isone)
					isone = false;
				else
				{
					_tcscat_s(buffer, BUFSIZE, CT_CRNL);

					if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
						return -2;

					// 数据对齐
					_gsp_strset(buffer, BUFSIZE, CT_SPLIT2, deslen);
				}
			}

			_itot_s(m_data.m_intarray.pvalue[i], &buffer[_tcslen(buffer)], BUFSIZE-_tcslen(buffer), m_data.m_intarray.scale);
			
			// 分割符
			if ((i+1) < m_data.m_intarray.elecnt)
				_tcscat_s(buffer, BUFSIZE, GSP_SPL_STR);
		}

		_tcscat_s(buffer, BUFSIZE, CT_CRNL);

		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		return 0;
	}
	else
	{
		return -3;
	}
}


// 生成一个从程序内部创建的变量对象
//
// name		- IN - 变量名称
// value	- IN - 变量值
// scale	- IN - 变量进制值
//
// 返回值：0 - 成功
//
// *注：
//
int varint::build(LPCTSTR name, int value, BYTE scale)
{
	_ASSERT(m_type == NTP_MAX);				// 必须是一个空类
	_ASSERT(name&&_tcslen(name));			// 变量名称不能为空
	_ASSERT((scale == 10)||(scale == 16));	// 进制值只能是10或16

	m_type = NTP_INT;
	
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	m_data.m_int.value = value;
	m_data.m_int.scale = scale;
	
	return 0;
}


// 生成一个从程序内部创建的数组变量对象
//
// name		- IN - 变量名称
// elecnt	- IN - 数组尺寸值
// scale	- IN - 变量进制值
//
// 返回值：0 - 成功， -1 - 内存不足
//
// *注：数组缓冲区将会被初始化为0，调用者需要在本调用结束后填写数组内容
//
int varint::buildarray(LPCTSTR name, int elecnt, BYTE scale)
{
	_ASSERT(m_type == NTP_MAX);		// 必须是一个空类
	_ASSERT(name&&_tcslen(name));	// 变量名称不能为空
	_ASSERT(elecnt > 0);			// 数组尺寸不能为0
	_ASSERT((scale == 10)||(scale == 16));		// 进制值只能是10或16

	// 申请数组缓冲区
	int*	p = new (std::nothrow) int[elecnt];

	if (p == NULL) return -1;

	// 初始化数组缓冲区
	memset((void*)p, 0, sizeof(int)*elecnt);

	m_type = NTP_ARRAY_INT;
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	m_data.m_intarray.scale  = scale;
	m_data.m_intarray.elecnt = elecnt;
	m_data.m_intarray.pvalue = p;

	return 0;
}



////////////////////////////////////////////////////////////////////////
// bool变量类实现

varbool::~varbool()
{
	destroy();
}

// 整理输入文本的数据部分
//
// pvarbegin		- IN  - 变量声明行首地址
// pvarend			- IN  - 变量定义结束地址（数据部分结尾）
// pvartab			- IN  - 变量整理后的参数起始地址
// isarray			- IN  - 变量是否为数组的标志，true:数组，false:非数组
// tidytxtbegin		- IN  - 整理缓冲区写入地址
// tidytxtend		- OUT - 返回整理缓冲区新的写入地址
// pict				- IN  - 上下文类地址
//
// 返回值： 0 - 成功读取
//		   -1 - 语法错误
//
int varbool::tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	unsigned char *pt = (unsigned char *)pvartab;
	unsigned char bidx;
	unsigned char boolen;
	LPCTSTR pdb;	// 数值开始位置
	LPCTSTR pde;	// 数值结束位置


	// 确定赋值号的位置
	LPCTSTR pequ = _tcschr(pvarbegin, CT_ASSIGNMENT);
	_ASSERT(pequ);

	// 指向赋值号后面的字符
	pequ++;

	if (isarray)
	{
		// [bool数组标志][变量名字长度][变量名字][元素个数]
		// [描述索引值][变量值][描述索引值][变量值]...
		pt += sizeof(DWORD);
		pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);

		// 获取数组元素个数
		int elecnt = *((LPDWORD)pt);

		pde = pequ;
		pt  = ((unsigned char *)tidytxtbegin);

		for (int i=0; i<elecnt; i++)
		{
			// 判断是否存在缺少元素的情况
			if (pde >= pvarend)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}

			// 跨过数值前面可能存在的空白字符
			if ((pdb=_gsp_strspn(pde, pvarend, GSP_WHITESPC3)) == NULL)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}
			// 找到数值结尾
			if ((pde=_gsp_strpbrk(pdb, pvarend, GSP_WHITESPC3)) == NULL)
				pde = pvarend;

			// 读取bool值
			if (_gsp_read_bool(pdb, pde, &bidx, &boolen) == false)
			{
				pict->seterror(_ILEM_25, i+1);
				return -1;	// 语法错误
			}

			// 写入整理缓冲区（索引值和boole值都是BYTE类型，各占一个字节）
			*pt++ = bidx;
			*pt++ = boolen;
		}

		// 更新整理缓冲区写入地址
		*tidytxtend = (LPTSTR)pt;
	}
	else
	{
		// [bool数组标志][变量名字长度][变量名字]
		// 跨过数值前面可能存在的空白字符
		if ((pdb=_gsp_strspn(pequ, pvarend, GSP_WHITESPC2)) == NULL)
		{
			pict->seterror(_ILEM_26);
			return -1;
		}
		// 找到数值结尾
		if ((pde=_gsp_strpbrk(pdb, pvarend, GSP_WHITESPC3)) == NULL)
			pde = pvarend;

		// 读取bool值
		if (_gsp_read_bool(pdb, pde, &bidx, &boolen) == false)
		{
			pict->seterror(_ILEM_27);
			return -1;	// 语法错误
		}

		// 写入整理缓冲区
		pt = ((unsigned char *)tidytxtbegin);

		*pt++ = bidx;
		*pt++ = boolen;

		// 更新整理缓冲区写入地址
		*tidytxtend = (LPTSTR)pt;
	}

	return 0;
}


// 初始化变量类
//
// pstr		- IN  - 变量预编译数据起始地址
// inseg	- IN  - 内参段地址
// pend		- OUT - 返回变量预编译数据结尾地址
//
// 返回值：0-成功、-1 - 内存不足、-3 - 系统异常
//
// *注：
// 
int varbool::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	// [bool变量标志][变量名字长度][变量名字][描述索引值1][变量值1]
	// [bool数组标志][变量名字长度][变量名字][元素个数4][[描述索引值1][变量值1],[描述索引值1][变量值1],...]

	LPDWORD			pmark = (LPDWORD)pstr;
	int*			pnlen = (int*)&pmark[1];
	LPCTSTR			pname = (LPCTSTR)&pnlen[1];
	LPPCMIB_VT_BOOL	pbv   = (LPPCMIB_VT_BOOL)&pname[pnlen[0]+1];
	int*			pele  = (int*)&pname[pnlen[0]+1];
	int*			pval;
	BYTE			attr, addition;

	bool bm = _gsp_decomp_attributemark(pmark[0], &attr, &addition);

	_ASSERT(bm);
	_ASSERT((attr == PCA_ATT_VAR)&&((addition == PCA_ADD_VAR_BOOL)||(addition == PCA_ADD_VAR_BOOLARRAY)));

	if (bm == false)
		return -3;

	m_type = (addition == PCA_ADD_VAR_BOOL) ? NTP_BOOL : NTP_ARRAY_BOOL;
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, pname);

	if (m_type == NTP_BOOL)
	{
		m_data.m_bool.index = pbv->index;
		m_data.m_bool.value = pbv->value;
		pval = (int*)&pbv[1];
	}
	else
	{
		m_data.m_boolarray.elecnt = pele[0];
		m_data.m_boolarray.pvalue = new (std::nothrow) ILVT_BOOL[m_data.m_boolarray.elecnt];
		pbv = (LPPCMIB_VT_BOOL)&pele[1];

		if (m_data.m_boolarray.pvalue == NULL)
			return -1;

		for (int i=0; i<m_data.m_boolarray.elecnt; i++)
		{
			m_data.m_boolarray.pvalue[i].index = pbv[i].index;
			m_data.m_boolarray.pvalue[i].value = pbv[i].value;
		}

		pval = (int*)&pbv[m_data.m_boolarray.elecnt];
	}

	*pend = (LPCTSTR)pval;

	return 0;
}


// 销毁变量类中的内容
//
// *注：函数释放类中的相关资源，将类变为一个‘空白’变量类。
//		函数并不会把类从变量类链表中摘除，需要上一级的调用函数自行处理。
//
void varbool::destroy()
{
	_ASSERT((m_type == NTP_BOOL)||(m_type == NTP_ARRAY_BOOL));

	if (m_type == NTP_BOOL)
	{
		m_data.m_bool.index = 0;
		m_data.m_bool.value = 0;
	}
	else
	{
		m_data.m_boolarray.elecnt = 0;

		if (m_data.m_boolarray.pvalue)
			delete []m_data.m_boolarray.pvalue;

		m_data.m_boolarray.pvalue = NULL;
	}

	m_name[0]	= _T('\0');
	m_type		= NTP_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功， -2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int varbool::write(FILE *fp, insideseg* inseg)
{
	const size_t	BUFSIZE = (size_t)MAX_VARNAME_LENGTH*3;
	TCHAR			buffer[BUFSIZE];

	_ASSERT((m_type == NTP_BOOL)||(m_type == NTP_ARRAY_BOOL));

	if (m_type == NTP_BOOL)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					CT_VARDSNE, 
					_T("\t= "));

		// 对bool值数据进行错误检查
		if ((m_data.m_bool.index >= MAX_BOOLDES_TYPE)||(m_data.m_bool.index < 0))
		{
			_ASSERT(0);
			return -3;
		}
		if ((m_data.m_bool.value != 0)&&(m_data.m_bool.value != 1))
		{
			_ASSERT(0);
			return -3;
		}

		_tcscat_s(buffer, BUFSIZE, BOOLDES[m_data.m_bool.index].oo[m_data.m_bool.value]);
		_tcscat_s(buffer, BUFSIZE, CT_CRNL);

		// 写入文件
		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
		else
			return 0;
	}
	else if (m_type == NTP_ARRAY_BOOL)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s%d%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					CT_VARDSNE, 
					CT_ARRYSNB, 
					m_data.m_boolarray.elecnt, 
					CT_ARRYSNE, 
					_T("\t= "));

		size_t	deslen = _tcslen(buffer);

		// 写入变量声明
		if (fwrite(buffer, deslen*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		buffer[0] = CT_NULL;

		bool	isone = true;		// 是否为第一行
		int		width = 20;			// 折行的标准（2~50）

		// 写入数组数据
		for (int i=0; i<m_data.m_boolarray.elecnt; i++)
		{
			// 分行处理
			if ((i%width) == 0)
			{
				if (isone)
					isone = false;
				else
				{
					_tcscat_s(buffer, BUFSIZE, CT_CRNL);

					if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
						return -2;

					// 数据对齐
					_gsp_strset(buffer, BUFSIZE, CT_SPLIT2, deslen);
				}
			}

			_tcscat_s(buffer, BUFSIZE, BOOLDES[m_data.m_boolarray.pvalue[i].index].oo[m_data.m_boolarray.pvalue[i].value]);
			
			// 分割符
			if ((i+1) < m_data.m_boolarray.elecnt)
				_tcscat_s(buffer, BUFSIZE, GSP_SPL_STR);
		}

		_tcscat_s(buffer, BUFSIZE, CT_CRNL);

		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		return 0;
	}
	else
	{
		return -3;
	}
}


// 生成一个从程序内部创建的变量对象
//
// name		- IN - 变量名称
// value	- IN - bool变量值
// index	- IN - bool索引值
//
// 返回值：0 - 成功
//
// *注：
//
int varbool::build(LPCTSTR name, BYTE value, BYTE index)
{
	_ASSERT(m_type == NTP_MAX);				// 必须是一个空类
	_ASSERT(name&&_tcslen(name));			// 变量名称不能为空
	_ASSERT((value == 0)||(value == 1));				// bool值只能是0或1
	_ASSERT((index >= 0)&&(index < MAX_BOOLDES_TYPE));	// 索引值必须在0~MAX_BOOLDES_TYPE

	m_type = NTP_BOOL;
	
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	m_data.m_bool.value = value;
	m_data.m_bool.index = index;
	
	return 0;
}


// 生成一个从程序内部创建的数组变量对象
//
// name		- IN - 变量名称
// elecnt	- IN - 数组尺寸值
//
// 返回值：0 - 成功， -1 - 内存不足
//
// *注：数组缓冲区将会被初始化为0，调用者需要在本调用结束后填写数组内容
//
int varbool::buildarray(LPCTSTR name, int elecnt)
{
	_ASSERT(m_type == NTP_MAX);		// 必须是一个空类
	_ASSERT(name&&_tcslen(name));	// 变量名称不能为空
	_ASSERT(elecnt > 0);			// 数组尺寸不能为0

	// 申请数组缓冲区
	LPILVT_BOOL	p = new (std::nothrow) ILVT_BOOL[elecnt];

	if (p == NULL) return -1;

	// 初始化数组缓冲区
	memset((void*)p, 0, sizeof(ILVT_BOOL)*elecnt);

	m_type = NTP_ARRAY_BOOL;

	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	m_data.m_boolarray.elecnt = elecnt;
	m_data.m_boolarray.pvalue = p;

	return 0;
}



////////////////////////////////////////////////////////////////////////
// double变量类实现

vardouble::~vardouble()
{
	destroy();
}

// 整理输入文本的数据部分
//
// pvarbegin		- IN  - 变量声明行首地址
// pvarend			- IN  - 变量定义结束地址（数据部分结尾）
// pvartab			- IN  - 变量整理后的参数起始地址
// isarray			- IN  - 变量是否为数组的标志，true:数组，false:非数组
// tidytxtbegin		- IN  - 整理缓冲区写入地址
// tidytxtend		- OUT - 返回整理缓冲区新的写入地址
// pict				- IN  - 上下文类地址
//
// 返回值： 0 - 成功读取
//		   -1 - 语法错误
//
int vardouble::tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	unsigned char *pt = (unsigned char *)pvartab;
	double	*pd;
	double	dou;
	LPCTSTR pdb;	// 数值开始位置
	LPCTSTR pde;	// 数值结束位置

	// 确定赋值号的位置
	LPCTSTR pequ = _tcschr(pvarbegin, CT_ASSIGNMENT);
	_ASSERT(pequ);

	// 指向赋值号后面的字符
	pequ++;

	if (isarray)
	{
		// [double数组标志][变量名字长度][变量名字][元素个数]
		pt += sizeof(DWORD);
		pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);

		// 获取数组元素个数
		int elecnt = *((LPDWORD)pt);

		pde = pequ;
		pd  = ((double*)tidytxtbegin);

		for (int i=0; i<elecnt; i++)
		{
			// 判断是否存在缺少元素的情况
			if (pde >= pvarend)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}

			// 跨过数值前面可能存在的空白字符
			if ((pdb=_gsp_strspn(pde, pvarend, GSP_WHITESPC3)) == NULL)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}
			// 找到数值结尾
			if ((pde=_gsp_strpbrk(pdb, pvarend, GSP_WHITESPC3)) == NULL)
				pde = pvarend;

			// 读取double值
			if (_gsp_read_double(pdb, &dou) == false)
			{
				pict->seterror(_ILEM_25, i+1);
				return -1;	// 语法错误
			}

			// 写入整理缓冲区
			*pd++ = dou;
		}

		// 更新整理缓冲区写入地址
		*tidytxtend = (LPTSTR)pd;
	}
	else
	{
		// [double数组标志][变量名字长度][变量名字]
		// 跨过数值前面可能存在的空白字符
		if ((pdb=_gsp_strspn(pequ, pvarend, GSP_WHITESPC2)) == NULL)
		{
			pict->seterror(_ILEM_26);
			return -1;
		}

		// 读取double值
		if (_gsp_read_double(pdb, &dou) == false)
		{
			pict->seterror(_ILEM_27);
			return -1;	// 语法错误
		}

		// 写入整理缓冲区
		pd = ((double*)tidytxtbegin);

		*pd++ = dou;

		// 更新整理缓冲区写入地址
		*tidytxtend = (LPTSTR)pd;
	}

	return 0;
}


// 初始化变量类
//
// pstr		- IN  - 变量预编译数据起始地址
// inseg	- IN  - 内参段地址
// pend		- OUT - 返回变量预编译数据结尾地址
//
// 返回值：0-成功、-1 - 内存不足、-3 - 系统异常
//
// *注：
// 
int vardouble::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	// [double变量标志][变量名字长度][变量名字][变量值8]
	// [double数组标志][变量名字长度][变量名字][元素个数4][[变量值8],[变量值8],[变量值8],...]

	LPDWORD			pmark = (LPDWORD)pstr;
	int*			pnlen = (int*)&pmark[1];
	LPCTSTR			pname = (LPCTSTR)&pnlen[1];
	double*			pbv   = (double*)&pname[pnlen[0]+1];
	int*			pele  = (int*)&pname[pnlen[0]+1];
	double*			pval;
	BYTE			attr, addition;

	bool bm = _gsp_decomp_attributemark(pmark[0], &attr, &addition);

	_ASSERT(bm);
	_ASSERT((attr == PCA_ATT_VAR)&&((addition == PCA_ADD_VAR_DOUBLE)||(addition == PCA_ADD_VAR_DOUBLEARRAY)));

	if (bm == false)
		return -3;

	m_type = (addition == PCA_ADD_VAR_DOUBLE) ? NTP_DOUBLE : NTP_ARRAY_DOUBLE;
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, pname);

	if (m_type == NTP_DOUBLE)
	{
		m_data.m_double.value = pbv[0];
		pval = &pbv[1];
	}
	else
	{
		m_data.m_doublearray.elecnt = pele[0];
		m_data.m_doublearray.pvalue = new (std::nothrow) double[m_data.m_doublearray.elecnt];
		pbv = (double*)&pele[1];

		if (m_data.m_doublearray.pvalue == NULL)
			return -1;

		for (int i=0; i<m_data.m_doublearray.elecnt; i++)
		{
			m_data.m_doublearray.pvalue[i] = pbv[i];
		}

		pval = &pbv[m_data.m_doublearray.elecnt];
	}

	*pend = (LPCTSTR)pval;

	return 0;
}


// 销毁变量类中的内容
//
// *注：函数释放类中的相关资源，将类变为一个‘空白’变量类。
//		函数并不会把类从变量类链表中摘除，需要上一级的调用函数自行处理。
//
void vardouble::destroy()
{
	_ASSERT((m_type == NTP_DOUBLE)||(m_type == NTP_ARRAY_DOUBLE));

	if (m_type == NTP_DOUBLE)
	{
		m_data.m_double.value = 0;
	}
	else
	{
		m_data.m_doublearray.elecnt = 0;

		if (m_data.m_doublearray.pvalue)
			delete []m_data.m_doublearray.pvalue;

		m_data.m_doublearray.pvalue = NULL;
	}

	m_name[0]	= _T('\0');
	m_type		= NTP_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功， -2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int vardouble::write(FILE *fp, insideseg* inseg)
{
	const size_t	BUFSIZE = (size_t)MAX_VARNAME_LENGTH*8;
	TCHAR			buffer[BUFSIZE];
	TCHAR			ptd[16];

	_ASSERT((m_type == NTP_DOUBLE)||(m_type == NTP_ARRAY_DOUBLE));

	if (m_type == NTP_DOUBLE)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					CT_VARDSNE, 
					_T("\t= "));

		// 合成double小数位数（精度）的控制串
		_stprintf_s(ptd, 16, _T("%%.%df"), *(int*)inseg->io(INIPARAMNAME[IPND_DOUBLE_PRECISION], NULL));

		// 转换数值
		_stprintf_s(&buffer[_tcslen(buffer)], BUFSIZE-_tcslen(buffer), ptd, m_data.m_double.value);
		_tcscat_s(buffer, BUFSIZE, CT_CRNL);

		// 写入文件
		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
		else
			return 0;
	}
	else if (m_type == NTP_ARRAY_DOUBLE)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s%d%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					CT_VARDSNE, 
					CT_ARRYSNB, 
					m_data.m_doublearray.elecnt, 
					CT_ARRYSNE, 
					_T("\t= "));

		size_t	deslen = _tcslen(buffer);

		// 写入变量声明
		if (fwrite(buffer, deslen*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		// 合成double小数位数（精度）的控制串
		_stprintf_s(ptd, 16, _T("%%.%df"), *(int*)inseg->io(INIPARAMNAME[IPND_DOUBLE_PRECISION], NULL));

		buffer[0] = CT_NULL;

		bool	isone = true;		// 是否为第一行
		int		width = 10;			// 折行的标准（2~50）

		// 写入数组数据
		for (int i=0; i<m_data.m_doublearray.elecnt; i++)
		{
			// 分行处理
			if ((i%width) == 0)
			{
				if (isone)
					isone = false;
				else
				{
					_tcscat_s(buffer, BUFSIZE, CT_CRNL);

					if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
						return -2;

					// 数据对齐
					_gsp_strset(buffer, BUFSIZE, CT_SPLIT2, deslen);
				}
			}

			_stprintf_s(&buffer[_tcslen(buffer)], BUFSIZE-_tcslen(buffer), ptd, m_data.m_doublearray.pvalue[i]);
			
			// 分割符
			if ((i+1) < m_data.m_doublearray.elecnt)
				_tcscat_s(buffer, BUFSIZE, GSP_SPL_STR);
		}

		_tcscat_s(buffer, BUFSIZE, CT_CRNL);

		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		return 0;
	}
	else
	{
		return -3;
	}
}


// 生成一个从程序内部创建的变量对象
//
// name		- IN - 变量名称
// value	- IN - 变量值
//
// 返回值：0 - 成功
//
// *注：
//
int vardouble::build(LPCTSTR name, double value)
{
	_ASSERT(m_type == NTP_MAX);				// 必须是一个空类
	_ASSERT(name&&_tcslen(name));			// 变量名称不能为空

	m_type = NTP_DOUBLE;
	
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	m_data.m_double.value = value;
	
	return 0;
}


// 生成一个从程序内部创建的数组变量对象
//
// name		- IN - 变量名称
// elecnt	- IN - 数组尺寸值
//
// 返回值：0 - 成功， -1 - 内存不足
//
// *注：数组缓冲区将会被初始化为0，调用者需要在本调用结束后填写数组内容
//
int vardouble::buildarray(LPCTSTR name, int elecnt)
{
	_ASSERT(m_type == NTP_MAX);		// 必须是一个空类
	_ASSERT(name&&_tcslen(name));	// 变量名称不能为空
	_ASSERT(elecnt > 0);			// 数组尺寸不能为0

	// 申请数组缓冲区
	double*	p = new (std::nothrow) double[elecnt];

	if (p == NULL) return -1;

	// 初始化数组缓冲区
	memset((void*)p, 0, sizeof(double)*elecnt);

	m_type = NTP_ARRAY_DOUBLE;

	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	m_data.m_doublearray.elecnt = elecnt;
	m_data.m_doublearray.pvalue = p;

	return 0;
}



////////////////////////////////////////////////////////////////////////
// string变量类实现

varstring::~varstring()
{
	destroy();
}

// 整理输入文本的数据部分
//
// pvarbegin		- IN  - 变量声明行首地址
// pvarend			- IN  - 变量定义结束地址（数据部分结尾）
// pvartab			- IN  - 变量整理后的参数起始地址
// isarray			- IN  - 变量是否为数组的标志，true:数组，false:非数组
// tidytxtbegin		- IN  - 整理缓冲区写入地址
// tidytxtend		- OUT - 返回整理缓冲区新的写入地址
// pict				- IN  - 上下文类地址
//
// 返回值： 0 - 成功读取
//		   -1 - 语法错误
//
int varstring::tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	unsigned char *pt = (unsigned char *)pvartab;
	LPTSTR	ptidy;
	LPCTSTR pdb;	// 数值开始位置
	LPCTSTR pde;	// 数值结束位置

	// 确定赋值号的位置
	LPCTSTR pequ = _tcschr(pvarbegin, CT_ASSIGNMENT);
	_ASSERT(pequ);

	// 指向赋值号后面的字符
	pequ++;

	if (isarray)
	{
		// [string数组标志][变量名字长度][变量名字][元素个数]
		// [[变量值长度][变量值],[变量值长度][变量值], ...]
		pt += sizeof(DWORD);
		pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);

		// 获取数组元素个数
		int elecnt = *((LPDWORD)pt);

		pde   = pequ;
		ptidy = tidytxtbegin;

		for (int i=0; i<elecnt; i++)
		{
			// 判断是否存在缺少元素的情况
			if (pde >= pvarend)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}

			// 跨过数值前面可能存在的空白字符
			if ((pdb=_gsp_strspn(pde, pvarend, GSP_WHITESPC3)) == NULL)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}

			// 读string型变量（并写入整理缓冲区）
			if (_gsp_read_string(pdb, &pde, pvarend, ptidy, &ptidy) == false)
			{
				pict->seterror(_ILEM_25, i+1);
				return -1;	// 语法错误
			}
		}

		// 更新整理缓冲区写入地址
		*tidytxtend = ptidy;
	}
	else
	{
		// [string变量标志][变量名字长度][变量名字]
		// 跨过数值前面可能存在的空白字符
		if ((pdb=_gsp_strspn(pequ, pvarend, GSP_WHITESPC2)) == NULL)
		{
			pict->seterror(_ILEM_26);
			return -1;
		}

		ptidy = tidytxtbegin;

		// 读string型变量（并写入整理缓冲区）
		if (_gsp_read_string(pdb, &pde, pvarend, ptidy, &ptidy) == false)
		{
			pict->seterror(_ILEM_27);
			return -1;	// 语法错误
		}

		// 更新整理缓冲区写入地址
		*tidytxtend = ptidy;
	}

	return 0;
}


// 初始化变量类
//
// pstr		- IN  - 变量预编译数据起始地址
// inseg	- IN  - 内参段地址
// pend		- OUT - 返回变量预编译数据结尾地址
//
// 返回值：0-成功、-1 - 内存不足、-3 - 系统异常
//
// *注：
// 
int varstring::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	// [string变量标志][变量名字长度][变量名字][变量值长度4][变量值]
	// [string数组标志][变量名字长度][变量名字][元素个数4][[变量值长度4][变量值],[变量值长度][变量值], ...]

	LPDWORD			pmark = (LPDWORD)pstr;
	int*			pnlen = (int*)&pmark[1];
	LPCTSTR			pname = (LPCTSTR)&pnlen[1];
	LPPCMIB_VT_STRING	psv = (LPPCMIB_VT_STRING)&pname[pnlen[0]+1];
	int*			pele  = (int*)&pname[pnlen[0]+1];
	LPCTSTR			pval;
	BYTE			attr, addition;

	bool bm = _gsp_decomp_attributemark(pmark[0], &attr, &addition);

	_ASSERT(bm);
	_ASSERT((attr == PCA_ATT_VAR)&&((addition == PCA_ADD_VAR_STRING)||(addition == PCA_ADD_VAR_STRINGARRAY)));

	if (bm == false)
		return -3;

	m_type = (addition == PCA_ADD_VAR_STRING) ? NTP_STRING : NTP_ARRAY_STRING;
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, pname);

	if (m_type == NTP_STRING)
	{
		m_data.m_string.len    = psv[0].len;
		m_data.m_string.pvalue = new (std::nothrow) TCHAR[m_data.m_string.len+1];

		if (m_data.m_string.pvalue == NULL)
			return -1;
		_tcscpy_s(m_data.m_string.pvalue, m_data.m_string.len+1, psv[0].value);
		
		pval = (LPCTSTR)&psv[0].value[psv[0].len+1];
	}
	else
	{
		m_data.m_stringarray.elecnt = pele[0];
		m_data.m_stringarray.pvalue = new (std::nothrow) ILVT_STRING[m_data.m_stringarray.elecnt];
		psv = (LPPCMIB_VT_STRING)&pele[1];

		if (m_data.m_stringarray.pvalue == NULL)
			return -1;

		for (int i=0; i<m_data.m_stringarray.elecnt; i++)
		{
			m_data.m_stringarray.pvalue[i].len = psv->len;
			m_data.m_stringarray.pvalue[i].pvalue = new (std::nothrow) TCHAR[psv->len+1];
			
			if (m_data.m_stringarray.pvalue[i].pvalue == NULL)
			{
				for (int k=i; k>0; k--)
				{
					delete []m_data.m_stringarray.pvalue[k-1].pvalue;
				}
				
				delete []m_data.m_stringarray.pvalue;
				
				return -1;
			}

			_tcscpy_s(m_data.m_stringarray.pvalue[i].pvalue, psv->len+1, psv->value);
			psv = (LPPCMIB_VT_STRING)&psv->value[psv->len+1];
		}

		pval = (LPCTSTR)psv;
	}

	*pend = pval;

	return 0;
}


// 销毁变量类中的内容
//
// *注：函数释放类中的相关资源，将类变为一个‘空白’变量类。
//		函数并不会把类从变量类链表中摘除，需要上一级的调用函数自行处理。
//
void varstring::destroy()
{
	_ASSERT((m_type == NTP_STRING)||(m_type == NTP_ARRAY_STRING));

	if (m_type == NTP_STRING)
	{
		m_data.m_string.len = 0;

		if (m_data.m_string.pvalue)
			delete []m_data.m_string.pvalue;

		m_data.m_string.pvalue = NULL;
	}
	else
	{
		m_data.m_stringarray.elecnt = 0;

		if (m_data.m_stringarray.pvalue)
		{
			for (int k=0; k<m_data.m_stringarray.elecnt; k++)
			{
				if (m_data.m_stringarray.pvalue[k].pvalue)
					delete []m_data.m_stringarray.pvalue[k].pvalue;
			}

			delete []m_data.m_stringarray.pvalue;
		}

		m_data.m_stringarray.pvalue = NULL;
	}

	m_name[0]	= _T('\0');
	m_type		= NTP_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功，-1 - 内存不足，-2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int varstring::write(FILE *fp, insideseg* inseg)
{
	const size_t	BUFSIZE = (size_t)MAX_VARNAME_LENGTH*8;
	TCHAR			buffer[BUFSIZE];

	_ASSERT((m_type == NTP_STRING)||(m_type == NTP_ARRAY_STRING));

	if (m_type == NTP_STRING)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					CT_VARDSNE, 
					_T("\t= \""));
		
		// 判断是否可以利用静态缓冲区，如果字符串太大，则申请内存。使用两种内存处理字符串的方式，本意是
		// 加快处理速度。在大多数情况下，字符串长度不会超过静态缓冲区的尺寸，这样就不必申请内存，节省了
		// 操作时间，并减少出错概率。
		//
		//		* m_data.m_string.len*2+1 的含义是假设字符串中都是需要转义的字符（最坏的情况），所以乘2。加一是为末尾的NULL字符留出位置。
		//		* BUFSIZE-MAX_VARNAME_LENGTH 的含义是去除变量名占用的一个MAX_VARNAME_LENGTH后剩余的空间尺寸。

		if ((m_data.m_string.len*2+1) > (BUFSIZE-MAX_VARNAME_LENGTH))	// 对于长字符串，采用申请内存的处理方式
		{
			LPTSTR	tmp = new (std::nothrow) TCHAR[m_data.m_string.len*2+4];	// +4是留出结尾引号、\r\n、NULL这些字符的位置

			if (tmp == NULL)
				return -1;

			// 对字符串进行转义
			_gsp_wst_cnv(m_data.m_string.pvalue, &m_data.m_string.pvalue[m_data.m_string.len], GSP_ESCAPED, tmp);
			_tcscat_s(tmp, m_data.m_string.len*2+4, CT_STRIDCH);
			_tcscat_s(tmp, m_data.m_string.len*2+4, CT_CRNL);

			// 写入文件
			if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			{
				delete []tmp;
				return -2;
			}

			if (fwrite(tmp, _tcslen(tmp)*sizeof(TCHAR), 1, fp) == 0)
			{
				delete []tmp;
				return -2;
			}

			delete []tmp;
			return 0;
		}
		else// 短字符串使用静态缓冲区的处理方式
		{
			// 对字符串进行转义
			_gsp_wst_cnv(m_data.m_string.pvalue, &m_data.m_string.pvalue[m_data.m_string.len], GSP_ESCAPED, &buffer[_tcslen(buffer)]);
			_tcscat_s(buffer, BUFSIZE, CT_STRIDCH);
			_tcscat_s(buffer, BUFSIZE, CT_CRNL);

			// 写入文件
			if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
				return -2;
			else
				return 0;
		}
	}
	else if (m_type == NTP_ARRAY_STRING)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s%d%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					CT_VARDSNE, 
					CT_ARRYSNB, 
					m_data.m_stringarray.elecnt, 
					CT_ARRYSNE, 
					_T("\t= "));

		size_t	deslen = _tcslen(buffer);

		// 写入变量声明
		if (fwrite(buffer, deslen*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		buffer[0] = CT_NULL;

		// 写入数组数据
		//		* 字符串数组将采用一个字符串占用一行的方式存放。
		//		* 字符串之间头部对齐。
		for (int i=0; i<m_data.m_stringarray.elecnt; i++)
		{
			if ((m_data.m_stringarray.pvalue[i].len*2+1) > (BUFSIZE-MAX_VARNAME_LENGTH))
			{
				LPTSTR	tmp = new (std::nothrow) TCHAR[m_data.m_stringarray.pvalue[i].len*2+5];	// 多出一个前缀引号

				if (tmp == NULL)
					return -1;

				// 对字符串进行转义
				tmp[0] = CT_STR;
				_gsp_wst_cnv(m_data.m_stringarray.pvalue[i].pvalue, &m_data.m_stringarray.pvalue[i].pvalue[m_data.m_stringarray.pvalue[i].len], GSP_ESCAPED, &tmp[1]);
				_tcscat_s(tmp, m_data.m_stringarray.pvalue[i].len*2+5, CT_STRIDCH);
				
				if ((i+1) < m_data.m_stringarray.elecnt)
					_tcscat_s(tmp, m_data.m_stringarray.pvalue[i].len*2+5, GSP_SPL_STR);

				_tcscat_s(tmp, m_data.m_stringarray.pvalue[i].len*2+5, CT_CRNL);

				// 写入文件
				if (fwrite(tmp, _tcslen(tmp)*sizeof(TCHAR), 1, fp) == 0)
				{
					delete []tmp;
					return -2;
				}

				delete []tmp;
			}
			else
			{
				// 对字符串进行转义
				buffer[0] = CT_STR;
				_gsp_wst_cnv(m_data.m_stringarray.pvalue[i].pvalue, &m_data.m_stringarray.pvalue[i].pvalue[m_data.m_stringarray.pvalue[i].len], GSP_ESCAPED, &buffer[1]);
				_tcscat_s(buffer, BUFSIZE, CT_STRIDCH);
				
				if ((i+1) < m_data.m_stringarray.elecnt)
					_tcscat_s(buffer, BUFSIZE, GSP_SPL_STR);

				_tcscat_s(buffer, BUFSIZE, CT_CRNL);

				// 写入文件
				if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
					return -2;
			}

			// 写入对齐空白
			if ((i+1) < m_data.m_stringarray.elecnt)
			{
				_gsp_strset(buffer, BUFSIZE, CT_SPLIT2, deslen);

				if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
					return -2;
			}
		}

		return 0;
	}
	else
	{
		return -3;
	}
}


// 生成一个从程序内部创建的变量对象
//
// name		- IN - 变量名称
// str		- IN - 变量值
//
// 返回值：0 - 成功，-1 - 内存不足
//
// *注：
//
int varstring::build(LPCTSTR name, LPCTSTR str)
{
	_ASSERT(m_type == NTP_MAX);				// 必须是一个空类
	_ASSERT(name&&_tcslen(name));			// 变量名称不能为空
	_ASSERT(str);

	int		len = _tcslen(str);
	LPTSTR	p = new (std::nothrow) TCHAR[len+1];

	if (p == NULL) return -1;

	m_type = NTP_STRING;
	
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	_tcscpy_s(p, len+1, str);
	m_data.m_string.len		= len;
	m_data.m_string.pvalue	= p;
	
	return 0;
}


// 生成一个从程序内部创建的数组变量对象
//
// name		- IN - 变量名称
// elecnt	- IN - 数组尺寸值
//
// 返回值：0 - 成功， -1 - 内存不足
//
// *注：数组缓冲区将会被初始化为0，调用者需要在本调用结束后填写数组内容
//
int varstring::buildarray(LPCTSTR name, int elecnt)
{
	_ASSERT(m_type == NTP_MAX);		// 必须是一个空类
	_ASSERT(name&&_tcslen(name));	// 变量名称不能为空
	_ASSERT(elecnt > 0);			// 数组尺寸不能为0

	// 申请数组缓冲区
	LPILVT_STRING	p = new (std::nothrow) ILVT_STRING[elecnt];

	if (p == NULL) return -1;

	// 初始化数组缓冲区
	memset((void*)p, 0, sizeof(ILVT_STRING)*elecnt);

	m_type = NTP_ARRAY_STRING;

	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);

	m_data.m_stringarray.elecnt = elecnt;
	m_data.m_stringarray.pvalue = p;

	return 0;
}



////////////////////////////////////////////////////////////////////////
// date变量类实现

vardate::~vardate()
{
	destroy();
}


// 整理输入文本的数据部分
//
// pvarbegin		- IN  - 变量声明行首地址
// pvarend			- IN  - 变量定义结束地址（数据部分结尾）
// pvartab			- IN  - 变量整理后的参数起始地址
// isarray			- IN  - 变量是否为数组的标志，true:数组，false:非数组
// tidytxtbegin		- IN  - 整理缓冲区写入地址
// tidytxtend		- OUT - 返回整理缓冲区新的写入地址
// pict				- IN  - 上下文类地址
//
// 返回值： 0 - 成功读取
//		   -1 - 语法错误
//
int vardate::tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	unsigned char *pt = (unsigned char *)pvartab;
	unsigned char adtime;
	__int64	*pd;
	__int64	value;
	LPCTSTR pdb;	// 数值开始位置
	LPCTSTR pde;	// 数值结束位置


	// 确定赋值号的位置
	LPCTSTR pequ = _tcschr(pvarbegin, CT_ASSIGNMENT);
	_ASSERT(pequ);

	// 指向赋值号后面的字符
	pequ++;

	if (isarray)
	{
		// [date数组标志][变量名字长度][变量名字][类型标识][元素个数]
		pt += sizeof(DWORD);
		pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);

		// 获取类型标识值
		adtime = *pt++;
		// 获取数组元素个数
		int elecnt = *((LPDWORD)pt);

		pde = pequ;
		pd  = ((__int64*)tidytxtbegin);

		for (int i=0; i<elecnt; i++)
		{
			// 判断是否存在缺少元素的情况
			if (pde >= pvarend)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}

			// 跨过数值前面可能存在的空白字符
			if ((pdb=_gsp_strspn(pde, pvarend, GSP_WHITESPC3)) == NULL)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}

			// 读date型变量
			if (_gsp_read_date(pdb, &pde, pvarend, adtime, &value) == false)
			{
				pict->seterror(_ILEM_25, i+1);
				return -1;	// 语法错误
			}

			// 写入整理缓冲区
			*pd++ = value;
		}

		// 更新整理缓冲区写入地址
		*tidytxtend = (LPTSTR)pd;
	}
	else
	{
		// [date变量标志][变量名字长度][变量名字][类型标识]
		pt += sizeof(DWORD);
		pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);

		// 获取类型标识值
		adtime = *pt++;

		// 跨过数值前面可能存在的空白字符
		if ((pdb=_gsp_strspn(pequ, pvarend, GSP_WHITESPC2)) == NULL)
		{
			pict->seterror(_ILEM_26);
			return -1;
		}

		// 读date型变量
		if (_gsp_read_date(pdb, &pde, pvarend, adtime, &value) == false)
		{
			pict->seterror(_ILEM_27);
			return -1;	// 语法错误
		}

		// 写入整理缓冲区
		pd = ((__int64*)tidytxtbegin);

		*pd++ = value;

		// 更新整理缓冲区写入地址
		*tidytxtend = (LPTSTR)pd;
	}

	return 0;
}


// 初始化变量类
//
// pstr		- IN  - 变量预编译数据起始地址
// inseg	- IN  - 内参段地址
// pend		- OUT - 返回变量预编译数据结尾地址
//
// 返回值：0-成功、-1 - 内存不足、-3 - 系统异常
//
// *注：
// 
int vardate::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	// [date变量标志][变量名字长度][变量名字][类型标识1][变量值8]
	// [date数组标志][变量名字长度][变量名字][类型标识1][元素个数4][[变量值8],[变量值8],[变量值8],...]

	LPDWORD			pmark   = (LPDWORD)pstr;
	int*			pnlen   = (int*)&pmark[1];
	LPCTSTR			pname   = (LPCTSTR)&pnlen[1];
	LPBYTE			padtime = (LPBYTE)&pname[pnlen[0]+1];
	int*			pele    = (int*)&padtime[1];
	__int64*		pval;
	BYTE			attr, addition;

	bool bm = _gsp_decomp_attributemark(pmark[0], &attr, &addition);

	_ASSERT(bm);
	_ASSERT((attr == PCA_ATT_VAR)&&((addition == PCA_ADD_VAR_DATE)||(addition == PCA_ADD_VAR_DATEARRAY)));

	if (bm == false)
		return -3;

	m_type = (addition == PCA_ADD_VAR_DATE) ? NTP_DATE : NTP_ARRAY_DATE;
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, pname);

	_ASSERT((padtime[0]==DATETYPE_DATE)||(padtime[0]==DATETYPE_DATE_TIME));

	if (m_type == NTP_DATE)
	{
		m_data.m_date.adtime = padtime[0];
		pval = (__int64*)&padtime[1];
		m_data.m_date.value  = pval[0];
		pval = &pval[1];
	}
	else
	{
		m_data.m_datearray.adtime = padtime[0];
		m_data.m_datearray.elecnt = pele[0];
		m_data.m_datearray.pvalue = new (std::nothrow) __int64[m_data.m_datearray.elecnt];
		pval = (__int64*)&pele[1];

		if (m_data.m_datearray.pvalue == NULL)
			return -1;

		for (int i=0; i<m_data.m_datearray.elecnt; i++)
		{
			m_data.m_datearray.pvalue[i] = pval[i];
		}

		pval = &pval[m_data.m_datearray.elecnt];
	}

	*pend = (LPCTSTR)pval;

	return 0;
}


// 销毁变量类中的内容
//
// *注：函数释放类中的相关资源，将类变为一个‘空白’变量类。
//		函数并不会把类从变量类链表中摘除，需要上一级的调用函数自行处理。
//
void vardate::destroy()
{
	_ASSERT((m_type == NTP_DATE)||(m_type == NTP_ARRAY_DATE));

	if (m_type == NTP_DATE)
	{
		_ASSERT((m_data.m_date.adtime==DATETYPE_DATE)||(m_data.m_date.adtime==DATETYPE_DATE_TIME));
		
		m_data.m_date.adtime = 0;
		m_data.m_date.value  = 0;
	}
	else
	{
		_ASSERT((m_data.m_datearray.adtime==DATETYPE_DATE)||(m_data.m_datearray.adtime==DATETYPE_DATE_TIME));

		m_data.m_datearray.adtime  = 0;
		m_data.m_datearray.elecnt  = 0;

		if (m_data.m_datearray.pvalue)
			delete []m_data.m_datearray.pvalue;

		m_data.m_datearray.pvalue = NULL;
	}

	m_name[0]	= _T('\0');
	m_type		= NTP_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功， -2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int vardate::write(FILE *fp, insideseg* inseg)
{
	const size_t	BUFSIZE = (size_t)MAX_VARNAME_LENGTH*3;
	TCHAR			buffer[BUFSIZE];
	struct tm		tms;

	_ASSERT((m_type == NTP_DATE)||(m_type == NTP_ARRAY_DATE));

	if (m_type == NTP_DATE)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					(m_data.m_date.adtime == DATETYPE_DATE_TIME) ? CT_DATETIME_STR:_T(""),
					CT_VARDSNE, 
					_T("\t= "));

		// 转换数值
		_localtime64_s(&tms, &m_data.m_date.value);

		// 输出日期字符串
		if (m_data.m_date.adtime == DATETYPE_DATE)
			_tcsftime(&buffer[_tcslen(buffer)], BUFSIZE-_tcslen(buffer), _T("\"%Y-%m-%d\""), &tms);
		else
			_tcsftime(&buffer[_tcslen(buffer)], BUFSIZE-_tcslen(buffer), _T("\"%Y-%m-%d %H:%M:%S\""), &tms);

		_tcscat_s(buffer, BUFSIZE, CT_CRNL);

		// 写入文件
		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
		else
			return 0;
	}
	else if (m_type == NTP_ARRAY_DATE)
	{
		// 合成变量声明
		_stprintf_s(buffer, BUFSIZE, _T("%s%s%s%s%s%s%d%s%s"), 
					m_name, 
					CT_VARDSNB, 
					INIDATATYPESTRING[m_type],
					(m_data.m_date.adtime == DATETYPE_DATE_TIME) ? CT_DATETIME_STR:_T(""),
					CT_VARDSNE, 
					CT_ARRYSNB, 
					m_data.m_datearray.elecnt, 
					CT_ARRYSNE, 
					_T("\t= "));

		size_t	deslen = _tcslen(buffer);

		// 写入变量声明
		if (fwrite(buffer, deslen*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		buffer[0] = CT_NULL;

		bool	isone = true;		// 是否为第一行
		int		width = 20;			// 折行的标准（2~50）

		// 写入数组数据
		for (int i=0; i<m_data.m_datearray.elecnt; i++)
		{
			// 分行处理
			if ((i%width) == 0)
			{
				if (isone)
					isone = false;
				else
				{
					_tcscat_s(buffer, BUFSIZE, CT_CRNL);

					if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
						return -2;

					// 数据对齐
					_gsp_strset(buffer, BUFSIZE, CT_SPLIT2, deslen);
				}
			}

			// 转换数值
			_localtime64_s(&tms, &m_data.m_datearray.pvalue[i]);

			// 输出日期字符串
			if (m_data.m_datearray.adtime == DATETYPE_DATE)
				_tcsftime(&buffer[_tcslen(buffer)], BUFSIZE-_tcslen(buffer), _T("\"%Y-%m-%d\""), &tms);
			else
				_tcsftime(&buffer[_tcslen(buffer)], BUFSIZE-_tcslen(buffer), _T("\"%Y-%m-%d %H:%M:%S\""), &tms);

			// 分割符
			if ((i+1) < m_data.m_datearray.elecnt)
				_tcscat_s(buffer, BUFSIZE, GSP_SPL_STR);
		}

		_tcscat_s(buffer, BUFSIZE, CT_CRNL);

		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		return 0;
	}
	else
	{
		return -3;
	}
}


// 生成一个从程序内部创建的变量对象
//
// name		- IN - 变量名称
// value	- IN - 变量值
// adtime	- IN - 类型标识
//
// 返回值：0 - 成功
//
// *注：
//
int vardate::build(LPCTSTR name, __int64 value, BYTE adtime)
{
	_ASSERT(m_type == NTP_MAX);				// 必须是一个空类
	_ASSERT(name&&_tcslen(name));			// 变量名称不能为空
	_ASSERT((adtime==DATETYPE_DATE)||(adtime==DATETYPE_DATE_TIME));

	m_type = NTP_DATE;
	
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	m_data.m_date.value  = value;
	m_data.m_date.adtime = adtime;
	
	return 0;
}


// 生成一个从程序内部创建的数组变量对象
//
// name		- IN - 变量名称
// elecnt	- IN - 数组尺寸值
// adtime	- IN - 类型标识
//
// 返回值：0 - 成功， -1 - 内存不足
//
// *注：数组缓冲区将会被初始化为0，调用者需要在本调用结束后填写数组内容
//
int vardate::buildarray(LPCTSTR name, int elecnt, BYTE adtime)
{
	_ASSERT(m_type == NTP_MAX);		// 必须是一个空类
	_ASSERT(name&&_tcslen(name));	// 变量名称不能为空
	_ASSERT(elecnt > 0);			// 数组尺寸不能为0
	_ASSERT((adtime==DATETYPE_DATE)||(adtime==DATETYPE_DATE_TIME));

	// 申请数组缓冲区
	__int64*	p = new (std::nothrow) __int64[elecnt];

	if (p == NULL) return -1;

	// 初始化数组缓冲区
	memset((void*)p, 0, sizeof(__int64)*elecnt);

	m_type = NTP_ARRAY_DATE;
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);
	m_data.m_datearray.adtime = adtime;
	m_data.m_datearray.elecnt = elecnt;
	m_data.m_datearray.pvalue = p;

	return 0;
}


////////////////////////////////////////////////////////////////////////
// struct变量类实现

varstruct::~varstruct()
{
	destroy();
}

// 整理输入文本
//
// pvarbegin		- IN  - 变量声明行首地址
// pvarend			- IN  - 变量定义结束地址（数据部分结尾）
// pvartab			- IN  - 变量整理后的参数起始地址
// isarray			- IN  - 变量是否为数组的标志，true:数组，false:非数组
// tidytxtbegin		- IN  - 整理缓冲区写入地址
// tidytxtend		- OUT - 返回整理缓冲区新的写入地址
// pict				- IN  - 上下文类地址
//
// 返回值： 0 - 成功读取
//		   -1 - 语法错误
//
int varstruct::tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	unsigned char *pt = (unsigned char *)pvartab;
	TCHAR	ptmp[256];
	LPTSTR	ptidy;
	LPCTSTR pdb;	// 数值开始位置
	LPCTSTR pde;	// 数值结束位置

	// 确定赋值号的位置
	LPCTSTR pequ = _tcschr(pvarbegin, CT_ASSIGNMENT);
	_ASSERT(pequ);

	// 指向赋值号后面的字符
	pequ++;

	if (isarray)
	{
		// [struct数组标志][变量名字长度][变量名字][内存形式][结构成员个数][变量表][元素个数]
		// [[变量序列], [变量序列], ...]
		pt += sizeof(DWORD);												// [struct数组标志]
		pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);			// [变量名字长度][变量名字]
		pt += sizeof(DWORD);												// [内存形式]
		pt += sizeof(DWORD);												// [结构成员个数]
		pt += *((LPDWORD)pt) + (int)sizeof(DWORD);							// [变量表]

		// 获取数组元素个数
		int elecnt = *((LPDWORD)pt);

		pde   = pequ;
		ptidy = tidytxtbegin;

		for (int i=0; i<elecnt; i++)
		{
			// 判断是否存在缺少元素的情况
			if (pde >= pvarend)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}

			// 跨过数值前面可能存在的空白字符
			if ((pdb=_gsp_strspn(pde, pvarend, GSP_WHITESPC3)) == NULL)
			{
				pict->seterror(_ILEM_24, elecnt, i);
				return -1;
			}

			LPCTSTR pab;
			LPCTSTR pae;

			// 搜索结构数据前后的标识符 "{ }"
			if (_gsp_i_find_structarea(pdb, pvarend, &pab, &pae) == false)
			{
				pict->seterror(_ILEM_28, i+1);
				return -1;
			}

			if (pdb != pab)
			{
				pict->seterror(_ILEM_28, i+1);
				return -1;	// pdb指向的字符应该就是'{'，如果不是，说明文本的语法错误
			}
			
			_stprintf_s(ptmp, 256, _T("[%d]"), i);
			pict->push(ptmp);

			// 读struct型变量（并写入整理缓冲区）
			if (_gsp_read_struct(pdb, pae, pvartab, ptidy, &ptidy, pict) == false)
				return -1;	// 语法错误

			pict->pop();
			
			pde = pae+1;
		}

		// 更新整理缓冲区写入地址
		*tidytxtend = ptidy;
	}
	else
	{
		// [string变量标志][变量名字长度][变量名字]
		// 跨过数值前面可能存在的空白字符
		if ((pdb=_gsp_strspn(pequ, pvarend, GSP_WHITESPC2)) == NULL)
		{
			pict->seterror(_ILEM_26);
			return -1;
		}

		ptidy = tidytxtbegin;

		LPCTSTR pab;
		LPCTSTR pae;

		// 搜索结构数据前后的标识符 "{ }"
		if (_gsp_i_find_structarea(pdb, pvarend, &pab, &pae) == false)
		{
			pict->seterror(_ILEM_29);
			return -1;
		}

		if (pdb != pab)
		{
			pict->seterror(_ILEM_29);
			return -1;	// pdb指向的字符应该就是'{'，如果不是，说明文本的语法错误
		}

		// 读struct型变量（并写入整理缓冲区）
		if (_gsp_read_struct(pdb, pae, pvartab, ptidy, &ptidy, pict) == false)
			return -1;	// 语法错误

		// 更新整理缓冲区写入地址
		*tidytxtend = ptidy;
	}

	return 0;
}


// 初始化变量类
//
// pstr		- IN  - 变量预编译数据起始地址
// inseg	- IN  - 内参段地址
// pend		- OUT - 返回变量预编译数据结尾地址
//
// 返回值：0-成功、-1 - 内存不足、-3 - 系统异常
//
// *注：
// 
int varstruct::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	// [struct变量标志][变量名字长度][变量名字][内存形式4][结构成员个数4][变量表][变量序列]
	// [struct数组标志][变量名字长度][变量名字][内存形式4][结构成员个数4][变量表][元素个数][[变量序列], [变量序列], ...]

	LPDWORD			pmark    = (LPDWORD)pstr;							// 标志
	int*			pnlen    = (int*)&pmark[1];							// 变量名字长度
	LPCTSTR			pname    = (LPCTSTR)&pnlen[1];						// 变量名字
	int*			pmemtype = (int*)&pname[pnlen[0]+1];				// 结构内存形式
	int*			pmemcnts = (int*)&pmemtype[1];						// 结构成员个数
	int*			pvtsize  = (int*)&pmemcnts[1];						// 变量表尺寸
	LPBYTE			pvtab    = (LPBYTE)&pvtsize[1];						// 变量表
	int*			pvssize;											// 变量序列尺寸
	LPBYTE			pvsei;												// 变量序列
	int*			pele;												// 元素个数
	LPCTSTR			pval;
	LPBYTE			pvalue;
	BYTE			attr, addition;
	void*			first;
	void*			last;

	bool bm = _gsp_decomp_attributemark(pmark[0], &attr, &addition);

	_ASSERT(bm);
	_ASSERT((attr == PCA_ATT_VAR)&&((addition == PCA_ADD_VAR_STRUCT)||(addition == PCA_ADD_VAR_STRUCTARRAY)));

	if (bm == false)
		return -3;

	m_type = (addition == PCA_ADD_VAR_STRUCT) ? NTP_STRUCT : NTP_ARRAY_STRUCT;
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, pname);

	if (m_type == NTP_STRUCT)
	{
		_ASSERT(pmemtype[0] == ILSUTMEMTPE_ARRAY);
		// 填写占位符（非数组的结构，不存在内存形式问题）
		m_data.m_struct.memtype		= ILSUTMEMTPE_ARRAY;
		// 填写结构成员个数
		m_data.m_struct.sutmemcnt	= pmemcnts[0];
		// 申请变量表空间
		m_data.m_struct.vartable	= new (std::nothrow) BYTE[*pvtsize+sizeof(int)];
		if (m_data.m_struct.vartable == NULL)
			return -1;
		memcpy((void*)m_data.m_struct.vartable, (void*)pvtsize, *pvtsize+sizeof(int));

		// 申请成员偏移空间
		m_data.m_struct.varoffset	= new (std::nothrow) int[m_data.m_struct.sutmemcnt];
		if (m_data.m_struct.varoffset == NULL)
		{
			delete []m_data.m_struct.vartable;
			m_data.m_struct.vartable = NULL;
			return -1;
		}
		// 填写成员偏移及结构尺寸信息
		m_data.m_struct.sutsize = _gsp_get_sutoffset_data((LPCTSTR)m_data.m_struct.vartable, m_data.m_struct.sutmemcnt, m_data.m_struct.varoffset);
		if (m_data.m_struct.sutsize == -1)
		{
			delete []m_data.m_struct.vartable;
			m_data.m_struct.vartable = NULL;
			delete []m_data.m_struct.varoffset;
			m_data.m_struct.varoffset = NULL;
			return -3;
		}
		// 申请结构体空间
		m_data.m_struct.pvalue		= new (std::nothrow) BYTE[m_data.m_struct.sutsize];
		if (m_data.m_struct.pvalue == NULL)
		{
			delete []m_data.m_struct.vartable;
			m_data.m_struct.vartable = NULL;
			delete []m_data.m_struct.varoffset;
			m_data.m_struct.varoffset = NULL;
			return -1;
		}

		pvssize  = (int*)&pvtab[pvtsize[0]];				// 变量序列尺寸
		pvsei    = (LPBYTE)&pvssize[1];						// 变量序列
		
		// 创建结构体
		switch (_gsp_get_sutbody(&m_data.m_struct, (LPCTSTR)pvsei))
		{
		case	0:
			break;
		case	-1:
			// 释放内存
			_gsp_i_freesutbody(&m_data.m_struct);

			delete []m_data.m_struct.vartable;
			m_data.m_struct.vartable = NULL;
			delete []m_data.m_struct.varoffset;
			m_data.m_struct.varoffset = NULL;
			delete []m_data.m_struct.pvalue;
			m_data.m_struct.pvalue = NULL;
			return -1;
		default:
			return -3;
		}
		
		pval = (LPCTSTR)&pvsei[*pvssize];
	}
	else
	{
		// 根据用户指定的内存存放形式分别处理
		if (pmemtype[0] == ILSUTMEMTPE_LIST)
		{
			pele  = (int*)&pvtab[pvtsize[0]];

			m_data.m_structlist.elecnt		= *pele;				// 数组元素个数
			m_data.m_structlist.memtype		= ILSUTMEMTPE_LIST;		// 内存形式
			m_data.m_structlist.sutmemcnt	= pmemcnts[0];			// 结构成员个数

			// 此处需要将变量表扩容，因为链表形式的结构需要在尾部添加两个指针，分别指向前一个结构和
			// 后一个结构。但这两个指针并不存在于用户编写的结构变量表中，所以这里需要库自动添加
			m_data.m_structlist.vartable	= new (std::nothrow) BYTE[*pvtsize+sizeof(int)+sizeof(BYTE)*2];
			if (m_data.m_structlist.vartable == NULL)
				return -1;
			// 复制用户变量表数据到变量表缓冲区
			memcpy((void*)m_data.m_structlist.vartable, (void*)pvtsize, *pvtsize+sizeof(int));
			// 自动添加两个指针变量（但在预编译数据中实际不存在对应的数据）
			// 使用PCA_ADD_VAR_STRING类型是因为它的内存形式是指针，与新加入的类型一致，所以先用它占位
			m_data.m_structlist.vartable[*pvtsize+sizeof(int)+0] = PCA_ADD_VAR_STRING;
			m_data.m_structlist.vartable[*pvtsize+sizeof(int)+1] = PCA_ADD_VAR_STRING;
			// 修正变量表尺寸值（字节单位）
			((int*)m_data.m_structlist.vartable)[0] = pvtsize[0]+sizeof(BYTE)*2;
			// 修正结构成员个数（加入新添加的两个指针变量）
			m_data.m_structlist.sutmemcnt	+= 2;

			// 申请结构成员内存偏移数组
			m_data.m_structlist.varoffset	= new (std::nothrow) int[m_data.m_structlist.sutmemcnt];
			if (m_data.m_structlist.varoffset == NULL)
			{
				delete []m_data.m_structlist.vartable;
				m_data.m_structlist.vartable = NULL;
				return -1;
			}
			
			// 填写结构成员偏移数据
			m_data.m_structlist.sutsize = _gsp_get_sutoffset_data((LPCTSTR)m_data.m_structlist.vartable, m_data.m_structlist.sutmemcnt, m_data.m_structlist.varoffset);
			if (m_data.m_structlist.sutsize == -1)
			{
				delete []m_data.m_structlist.vartable;
				m_data.m_structlist.vartable = NULL;
				delete []m_data.m_structlist.varoffset;
				m_data.m_structlist.varoffset = NULL;
				return -3;
			}

			// 初始化链表头尾指针
			m_data.m_structlist.first	= NULL;
			m_data.m_structlist.last	= NULL;
			
			// 第一个变量序列地址
			pvssize = &pele[1];
			// 链表头尾
			first	= NULL;
			last	= NULL;

			for (int i=0; i<m_data.m_structlist.elecnt; i++)
			{
				// 申请一个内存块
				pvalue = new (std::nothrow) BYTE[m_data.m_structlist.sutsize];

				if (pvalue == NULL)
				{
					while (first)
					{
						pvalue = *(LPBYTE*)&((LPBYTE)first)[m_data.m_structlist.varoffset[m_data.m_structlist.sutmemcnt-1]];
						m_data.m_structlist.first = first;
						_gsp_i_freesutbody((LPILVT_STRUCT)&m_data.m_structlist);
						delete []m_data.m_structlist.first;
						first = (void*)pvalue;
					}

					delete []m_data.m_structlist.vartable;
					m_data.m_structlist.vartable = NULL;
					delete []m_data.m_structlist.varoffset;
					m_data.m_structlist.varoffset = NULL;

					m_data.m_structlist.first = NULL;
					m_data.m_structlist.last  = NULL;

					return -1;
				}

				pvsei = (LPBYTE)&pvssize[1];					// 变量序列
				m_data.m_structlist.first = (void*)pvalue;
			
				// 创建结构体
				switch (_gsp_get_sutbody((LPILVT_STRUCT)&m_data.m_structlist, (LPCTSTR)pvsei))
				{
				case	0:
					break;
				case	-1:
					while (first)
					{
						pvalue = *(LPBYTE*)&((LPBYTE)first)[m_data.m_structlist.varoffset[m_data.m_structlist.sutmemcnt-1]];
						m_data.m_structlist.first = first;
						_gsp_i_freesutbody((LPILVT_STRUCT)&m_data.m_structlist);
						delete []m_data.m_structlist.first;
						first = (void*)pvalue;
					}

					delete []m_data.m_structlist.vartable;
					m_data.m_structlist.vartable = NULL;
					delete []m_data.m_structlist.varoffset;
					m_data.m_structlist.varoffset = NULL;

					m_data.m_structlist.first = NULL;
					m_data.m_structlist.last  = NULL;
					return -1;
				default:
					return -3;
				}

				// 将新的结构内存块加入链表尾部
				if (first == NULL)
				{
					_ASSERT(last == NULL);

					first = m_data.m_structlist.first;
					last  = m_data.m_structlist.first;

					*(LPBYTE*)&((LPBYTE)first)[m_data.m_structlist.varoffset[m_data.m_structlist.sutmemcnt-2]] = NULL;
					*(LPBYTE*)&((LPBYTE)first)[m_data.m_structlist.varoffset[m_data.m_structlist.sutmemcnt-1]] = NULL;
				}
				else
				{
					_ASSERT(last != NULL);

					*(LPBYTE*)&((LPBYTE)last)[m_data.m_structlist.varoffset[m_data.m_structlist.sutmemcnt-1]] = (LPBYTE)m_data.m_structlist.first;
					*(LPBYTE*)&((LPBYTE)m_data.m_structlist.first)[m_data.m_structlist.varoffset[m_data.m_structlist.sutmemcnt-2]] = (LPBYTE)last;
					*(LPBYTE*)&((LPBYTE)m_data.m_structlist.first)[m_data.m_structlist.varoffset[m_data.m_structlist.sutmemcnt-1]] = (LPBYTE)NULL;
					last = m_data.m_structlist.first;
				}

				pvssize = (int*)&pvsei[*pvssize];
			}

			// 填写结构体链表地址
			m_data.m_structlist.first	= first;
			m_data.m_structlist.last	= last;

			pval = (LPCTSTR)pvssize;
		}
		else
		{
			_ASSERT(pmemtype[0] == ILSUTMEMTPE_ARRAY);

			// 获取数组元素个数
			pele  = (int*)&pvtab[pvtsize[0]];
			m_data.m_structarray.elecnt = *pele;

			m_data.m_structarray.memtype	= ILSUTMEMTPE_ARRAY;
			m_data.m_structarray.sutmemcnt	= pmemcnts[0];
			m_data.m_structarray.vartable	= new (std::nothrow) BYTE[*pvtsize+sizeof(int)];
			if (m_data.m_structarray.vartable == NULL)
				return -1;
			memcpy((void*)m_data.m_structarray.vartable, (void*)pvtsize, *pvtsize+sizeof(int));

			m_data.m_structarray.varoffset	= new (std::nothrow) int[m_data.m_structarray.sutmemcnt];
			if (m_data.m_structarray.varoffset == NULL)
			{
				delete []m_data.m_structarray.vartable;
				m_data.m_structarray.vartable = NULL;
				return -1;
			}
			
			m_data.m_structarray.sutsize = _gsp_get_sutoffset_data((LPCTSTR)m_data.m_structarray.vartable, m_data.m_structarray.sutmemcnt, m_data.m_structarray.varoffset);
			if (m_data.m_structarray.sutsize == -1)
			{
				delete []m_data.m_structarray.vartable;
				m_data.m_structarray.vartable = NULL;
				delete []m_data.m_structarray.varoffset;
				m_data.m_structarray.varoffset = NULL;
				return -3;
			}
			
			m_data.m_structarray.pvalue		= new (std::nothrow) BYTE[m_data.m_structarray.sutsize*m_data.m_structarray.elecnt];
			if (m_data.m_structarray.pvalue == NULL)
			{
				delete []m_data.m_structarray.vartable;
				m_data.m_structarray.vartable = NULL;
				delete []m_data.m_structarray.varoffset;
				m_data.m_structarray.varoffset = NULL;
				return -1;
			}

			// 第一个变量序列地址
			pvssize = &pele[1];
			// 保存结构体首地址
			pvalue  = m_data.m_structarray.pvalue;

			for (int i=0; i<m_data.m_structarray.elecnt; i++)
			{
				pvsei    = (LPBYTE)&pvssize[1];			// 变量序列
				m_data.m_structarray.pvalue = &pvalue[m_data.m_structarray.sutsize*i];
			
				// 创建结构体
				switch (_gsp_get_sutbody((LPILVT_STRUCT)&m_data.m_structarray, (LPCTSTR)pvsei))
				{
				case	0:
					break;
				case	-1:
					for (int k=i; k>0; k--)
					{
						m_data.m_structarray.pvalue = &pvalue[m_data.m_structarray.sutsize*(k-1)];
						_gsp_i_freesutbody((LPILVT_STRUCT)&m_data.m_structarray);
					}
					
					delete []m_data.m_structarray.vartable;
					m_data.m_structarray.vartable = NULL;
					delete []m_data.m_structarray.varoffset;
					m_data.m_structarray.varoffset = NULL;
					delete []m_data.m_structarray.pvalue;
					m_data.m_structarray.pvalue = NULL;
					return -1;
				default:
					return -3;
				}

				pvssize = (int*)&pvsei[*pvssize];
			}

			// 恢复结构体地址
			m_data.m_structarray.pvalue = pvalue;

			pval = (LPCTSTR)pvssize;
		}
	}

	*pend = pval;

	return 0;
}


// 销毁变量类中的内容
//
// *注：函数释放类中的相关资源，将类变为一个‘空白’变量类。
//		函数并不会把类从变量类链表中摘除，需要上一级的调用函数自行处理。
//
void varstruct::destroy()
{
	_ASSERT((m_type == NTP_STRUCT)||(m_type == NTP_ARRAY_STRUCT));

	if (m_type == NTP_STRUCT)
	{
		// 释放外链内存及结构体
		if (m_data.m_struct.pvalue)
		{
			_gsp_i_freesutbody(&m_data.m_struct);
			delete []m_data.m_struct.pvalue;
			m_data.m_struct.pvalue = NULL;
		}

		// 释放变量表
		if (m_data.m_struct.vartable)
		{
			delete []m_data.m_struct.vartable;
			m_data.m_struct.vartable = NULL;
		}

		// 释放偏移表
		if (m_data.m_struct.varoffset)
		{
			delete []m_data.m_struct.varoffset;
			m_data.m_struct.varoffset = NULL;
		}

		m_data.m_struct.memtype   = ILSUTMEMTPE_ARRAY;
		m_data.m_struct.sutmemcnt = 0;
		m_data.m_struct.sutsize   = 0;
	}
	else
	{
		LPBYTE	pvalue;

		if (m_data.m_structarray.memtype == ILSUTMEMTPE_LIST)		// 清理链表
		{
			while (m_data.m_structlist.first)
			{
				pvalue = *(LPBYTE*)&((LPBYTE)m_data.m_structlist.first)[m_data.m_structlist.varoffset[m_data.m_structlist.sutmemcnt-1]];
				_gsp_i_freesutbody((LPILVT_STRUCT)&m_data.m_structlist);
				delete []m_data.m_structlist.first;
				m_data.m_structlist.first = (void*)pvalue;
			}
			
			m_data.m_structlist.first = NULL;
			m_data.m_structlist.last  = NULL;
		}
		else														// 清理数组
		{
			if (m_data.m_structarray.pvalue)
			{
				pvalue = m_data.m_structarray.pvalue;
				
				for (int k=0; k<m_data.m_structarray.elecnt; k++)
				{
					m_data.m_structarray.pvalue = &pvalue[m_data.m_structarray.sutsize*k];
					_gsp_i_freesutbody((LPILVT_STRUCT)&m_data.m_structarray);
				}

				delete []pvalue;
				m_data.m_structarray.pvalue = NULL;
			}
		}

		if (m_data.m_structarray.vartable)
		{
			delete []m_data.m_structarray.vartable;
			m_data.m_structarray.vartable = NULL;
		}

		if (m_data.m_structarray.varoffset)
		{
			delete []m_data.m_structarray.varoffset;
			m_data.m_structarray.varoffset = NULL;
		}

		m_data.m_structarray.memtype   = ILSUTMEMTPE_ARRAY;
		m_data.m_structarray.sutmemcnt = 0;
		m_data.m_structarray.sutsize   = 0;
		m_data.m_structarray.elecnt    = 0;
	}

	m_name[0]	= _T('\0');
	m_type		= NTP_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功，-1 - 内存不足，-2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int varstruct::write(FILE *fp, insideseg* inseg)
{
	const size_t	BUFSIZE = (size_t)MAX_VARNAME_LENGTH*8;
	TCHAR			buffer[BUFSIZE];
	int				retcode;

	_ASSERT((m_type == NTP_STRUCT)||(m_type == NTP_ARRAY_STRUCT));

	if (m_type == NTP_STRUCT)
	{
		// 合成变量声明行
		if (compvardec(buffer, BUFSIZE, inseg) < 0)
			return -3;

		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		// 写入结构体
		retcode = writestruct(buffer, BUFSIZE, fp, 0, inseg);

		if (retcode != 0)
			return retcode;

		// 写入结尾换行符
		if (fwrite(CT_CRNL, _tcslen(CT_CRNL)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
		else
			return 0;
	}
	else if (m_type == NTP_ARRAY_STRUCT)
	{
		// 合成变量声明
		retcode = compvardec(buffer, BUFSIZE, inseg);
		
		if (retcode < 0)
			return -3;

		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;

		size_t	deslen = (size_t)retcode;

		bool	isone = true;		// 是否为第一行

		if (m_data.m_structarray.memtype == ILSUTMEMTPE_LIST)
		{
			void*	first = m_data.m_structlist.first;
			void*	last  = m_data.m_structlist.last;
			void*	p;

			for (int i=0; i<m_data.m_structlist.elecnt; i++)
			{
				if (isone == false)
				{
					// 此处有两种对齐方式可选，一种是长空格方式，另一种是单个TAB符方式
					// _gsp_strset(buffer, BUFSIZE, CT_SPLIT2, deslen);
					_tcscpy_s(buffer, BUFSIZE, GSP_SPL2_STR);

					if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
					{
						m_data.m_structlist.first = first;
						m_data.m_structlist.last  = last;
						return -2;
					}
				}
				else
				{
					isone = false;
				}

				_ASSERT(m_data.m_structlist.first);
				// 指向下一个结构
				p = *(void**)&((LPBYTE)m_data.m_structlist.first)[m_data.m_structlist.varoffset[m_data.m_structlist.sutmemcnt-1]];
				// 模拟数组结构，写入文件（第四个参数一直是0，来模拟数组）
				retcode = writestruct(buffer, BUFSIZE, fp, 0, inseg);
				
				m_data.m_structlist.first = p;

				if (retcode != 0)
				{
					m_data.m_structlist.first = first;
					m_data.m_structlist.last  = last;
					return retcode;
				}
				
				// 分割符
				if ((i+1) < m_data.m_structlist.elecnt)
				{
					if (fwrite(GSP_SPL_STR, _tcslen(GSP_SPL_STR)*sizeof(TCHAR), 1, fp) == 0)
					{
						m_data.m_structlist.first = first;
						m_data.m_structlist.last  = last;
						return -2;
					}
				}

				if (fwrite(CT_CRNL, _tcslen(CT_CRNL)*sizeof(TCHAR), 1, fp) == 0)
				{
					m_data.m_structlist.first = first;
					m_data.m_structlist.last  = last;
					return -2;
				}
			}
			// 恢复现场
			m_data.m_structlist.first = first;
			m_data.m_structlist.last  = last;
		}
		else
		{
			for (int i=0; i<m_data.m_structarray.elecnt; i++)
			{
				if (isone == false)
				{
					// 此处有两种对齐方式可选，一种是长空格方式，另一种是单个TAB符方式
					// _gsp_strset(buffer, BUFSIZE, CT_SPLIT2, deslen);
					_tcscpy_s(buffer, BUFSIZE, GSP_SPL2_STR);

					if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
						return -2;
				}
				else
				{
					isone = false;
				}

				// 写入结构体
				retcode = writestruct(buffer, BUFSIZE, fp, i, inseg);

				if (retcode != 0)
					return retcode;
				
				// 分割符
				if ((i+1) < m_data.m_structarray.elecnt)
					if (fwrite(GSP_SPL_STR, _tcslen(GSP_SPL_STR)*sizeof(TCHAR), 1, fp) == 0)
						return -2;

				if (fwrite(CT_CRNL, _tcslen(CT_CRNL)*sizeof(TCHAR), 1, fp) == 0)
					return -2;
			}
		}

		return 0;
	}
	else
	{
		return -3;
	}
}


// 合成变量声明行文本
//
// buffer	- IN - 调用者提供的内存缓冲区
// bufsize	- IN - 内存缓冲区尺寸（字符单位）
// inseg	- IN - 内参段指针
// 
// 返回值：返回合成文本的长度（字符单位，正值）。-3 - 系统异常。
//
// *注：
//
int varstruct::compvardec(LPTSTR buffer, int bufsize, insideseg* inseg)
{
	if ((m_type != NTP_STRUCT)&&(m_type != NTP_ARRAY_STRUCT))
	{
		_ASSERT(0);
		return -3;
	}

	_stprintf_s(buffer, bufsize, _T("%s%s%s%s%s"), 
				m_name, 
				CT_VARDSNB, 
				INIDATATYPESTRING[m_type],
				((m_type == NTP_ARRAY_STRUCT)&&(m_data.m_structarray.memtype == ILSUTMEMTPE_LIST)) ? CT_SUTMEMTPE_LIST_STR:_T(""),
				_T(" : "));

	LPBYTE	pvt = (LPBYTE)&((int*)m_data.m_struct.vartable)[1];		// 变量表首地址
	int		smc;

	// 如果是链表形式的结构，则忽略结构尾部库自行添加的指针变量
	if ((m_type == NTP_ARRAY_STRUCT)&&(m_data.m_structarray.memtype == ILSUTMEMTPE_LIST))
		smc = m_data.m_structarray.sutmemcnt - 2;
	else
		smc = m_data.m_struct.sutmemcnt;

	for (int i=0; i<smc; i++)
	{
		switch (pvt[0])
		{
		case	NTP_BYTE:				// byte数据
		case	NTP_INT:				// int数据
			_tcscat_s(buffer, bufsize, INIDATATYPESTRING[pvt[0]]);
			if (pvt[1] == 16)
				_tcscat_s(buffer, bufsize, CT_NUMTPE_HEX_STR);
			pvt += sizeof(BYTE)*2;
			break;
		case	NTP_BOOL:				// bool数据
		case	NTP_DOUBLE:				// double数据
		case	NTP_STRING:				// string数据
			_tcscat_s(buffer, bufsize, INIDATATYPESTRING[pvt[0]]);
			pvt++;
			break;
		case	NTP_DATE:				// date数据
			_tcscat_s(buffer, bufsize, INIDATATYPESTRING[pvt[0]]);
			if (pvt[1] == DATETYPE_DATE_TIME)
				_tcscat_s(buffer, bufsize, CT_DATETIME_STR);
			pvt += sizeof(BYTE)*2;
			break;
		case	NTP_ARRAY_BYTE:			// byte[]数组数据
		case	NTP_ARRAY_INT:			// int[]数组数据
			_tcscat_s(buffer, bufsize, INIDATATYPESTRING[pvt[0]]);
			if (pvt[1] == 16)
				_tcscat_s(buffer, bufsize, CT_NUMTPE_HEX_STR);
			_stprintf_s(&buffer[_tcslen(buffer)], bufsize-(int)_tcslen(buffer), _T("[%d]"), *(int*)&pvt[2]);
			pvt += sizeof(BYTE)*2 + sizeof(int);
			break;
		case	NTP_ARRAY_BOOL:			// bool[]数组数据
		case	NTP_ARRAY_DOUBLE:		// double[]数组数据
		case	NTP_ARRAY_STRING:		// string[]数组数据
			_tcscat_s(buffer, bufsize, INIDATATYPESTRING[pvt[0]]);
			_stprintf_s(&buffer[_tcslen(buffer)], bufsize-(int)_tcslen(buffer), _T("[%d]"), *(int*)&pvt[1]);
			pvt += sizeof(BYTE) + sizeof(int);
			break;
		case	NTP_ARRAY_DATE:			// date[]数组数据
			_tcscat_s(buffer, bufsize, INIDATATYPESTRING[pvt[0]]);
			if (pvt[1] == DATETYPE_DATE_TIME)
				_tcscat_s(buffer, bufsize, CT_DATETIME_STR);
			_stprintf_s(&buffer[_tcslen(buffer)], bufsize-(int)_tcslen(buffer), _T("[%d]"), *(int*)&pvt[2]);
			pvt += sizeof(BYTE)*2 + sizeof(int);
			break;
		default:
			_ASSERT(0);
			return -3;
		}

		if ((i+1) < smc)
			_tcscat_s(buffer, bufsize, GSP_SPL_STR);
	}

	_tcscat_s(buffer, bufsize, CT_VARDSNE);

	// 添加数组元素个数
	if (m_type == NTP_ARRAY_STRUCT)
	{
		_stprintf_s(&buffer[_tcslen(buffer)], bufsize-_tcslen(buffer), _T("[%d]"), m_data.m_structarray.elecnt);
	}

	_tcscat_s(buffer, bufsize, _T("\t= "));

	return _tcslen(buffer);
}


// 向文件中写入一个结构
// 
// buffer	- IN - 调用者提供的缓冲区
// bufsize	- IN - 缓冲区尺寸（字符单位 ）
// fp		- IN - 文件句柄
// index	- IN - 待写的结构数据索引（非数组结构的索引总是0，数组的索引可以从0~n）
// inseg	- IN - 内参段指针
//
// 返回值：0表示成功，-1 - 内存不足，-2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int varstruct::writestruct(LPTSTR buffer, int bufsize, FILE *fp, int index, insideseg* inseg)
{
	LPBYTE		pcur = &m_data.m_struct.pvalue[m_data.m_struct.sutsize*index];	// 数据地址
	LPBYTE		pvt  = (LPBYTE)&((int*)m_data.m_struct.vartable)[1];			// 变量表首地址
	TCHAR		ptd[16];
	int			smc;
	
	LPBYTE		pbyte;
	LPINT		pint;
	booln		*pbool;
	double		*pdouble;
	LPCTSTR		*pstr;
	__int64		*pi64;
	LPTSTR		ptmp;


	// {
	if (fwrite(CT_STCTDSNB, _tcslen(CT_STCTDSNB)*sizeof(TCHAR), 1, fp) == 0)
		return -2;

	// 如果是链表形式的结构，则忽略结构尾部库自行添加的指针变量
	if ((m_type == NTP_ARRAY_STRUCT)&&(m_data.m_structarray.memtype == ILSUTMEMTPE_LIST))
		smc = m_data.m_structarray.sutmemcnt - 2;
	else
		smc = m_data.m_struct.sutmemcnt;

	for (int i=0; i<smc; i++)
	{
		switch (pvt[0])
		{
		case	NTP_BYTE:				// byte数据
			pbyte = &pcur[m_data.m_struct.varoffset[i]];

			if (pvt[1] == 16)
				_stprintf_s(buffer, bufsize, _T("%.2X"), pbyte[0]);
			else
				_stprintf_s(buffer, bufsize, _T("%d"), pbyte[0]);

			if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			pvt += sizeof(BYTE)*2;
			break;
		case	NTP_INT:				// int数据
			pint = (LPINT)&pcur[m_data.m_struct.varoffset[i]];

			if (pvt[1] == 16)
				_stprintf_s(buffer, bufsize, _T("%.8X"), pint[0]);
			else
				_stprintf_s(buffer, bufsize, _T("%d"), pint[0]);

			if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			pvt += sizeof(BYTE)*2;
			break;
		case	NTP_BOOL:				// bool数据
			pbool = (booln*)&pcur[m_data.m_struct.varoffset[i]];
			if (fwrite(BOOLDES[pbool->index].oo[pbool->value], _tcslen(BOOLDES[pbool->index].oo[pbool->value])*sizeof(TCHAR), 1, fp) == 0)
				return -2;
			pvt++;
			break;
		case	NTP_DOUBLE:				// double数据
			pdouble = (double*)&pcur[m_data.m_struct.varoffset[i]];
			// 合成double小数位数（精度）的控制串
			_stprintf_s(ptd, 16, _T("%%.%df"), *(int*)inseg->io(INIPARAMNAME[IPND_DOUBLE_PRECISION], NULL));
			// 写入double值
			_stprintf_s(buffer, bufsize, ptd, pdouble[0]);
			if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
				return -2;
			pvt++;
			break;
		case	NTP_STRING:				// string数据
			pstr = (LPCTSTR*)&pcur[m_data.m_struct.varoffset[i]];

			ptmp = new (std::nothrow) TCHAR[_tcslen(pstr[0])*2+3];
			if (ptmp == NULL)
				return -1;

			ptmp[0] = CT_STR;
			// 字符串转义
			_gsp_wst_cnv(pstr[0], &pstr[0][_tcslen(pstr[0])], GSP_ESCAPED, &ptmp[1]);
			_tcscat_s(ptmp, _tcslen(pstr[0])*2+3, CT_STRIDCH);

			if (fwrite(ptmp, _tcslen(ptmp)*sizeof(TCHAR), 1, fp) == 0)
			{
				delete []ptmp;
				return -2;
			}

			delete []ptmp;
			pvt++;
			break;
		case	NTP_DATE:				// date数据
			pi64 = (__int64*)&pcur[m_data.m_struct.varoffset[i]];
			// 将__int64的日期时间值转换为字符串
			_gsp_cnvdt_to_str(*pi64, pvt[1], buffer, bufsize);

			if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			pvt += sizeof(BYTE)*2;
			break;
		case	NTP_ARRAY_BYTE:			// byte[]数组数据
			pbyte = &pcur[m_data.m_struct.varoffset[i]];

			if (fwrite(CT_ARRYDSNB, _tcslen(CT_ARRYDSNB)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			for (int k=0; k<*((int*)&pvt[2]); k++)
			{
				if (pvt[1] == 16)
					_stprintf_s(buffer, bufsize, _T("%.2X"), pbyte[k]);
				else
					_stprintf_s(buffer, bufsize, _T("%d"), pbyte[k]);

				if ((k+1) < *((int*)&pvt[2]))
					_tcscat_s(buffer, bufsize, GSP_SPL_STR);

				if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
					return -2;
			}

			if (fwrite(CT_ARRYDSNE, _tcslen(CT_ARRYDSNE)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			pvt += sizeof(BYTE)*2 + sizeof(int);
			break;
		case	NTP_ARRAY_INT:			// int[]数组数据
			pint = (LPINT)&pcur[m_data.m_struct.varoffset[i]];

			if (fwrite(CT_ARRYDSNB, _tcslen(CT_ARRYDSNB)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			for (int k=0; k<*((int*)&pvt[2]); k++)
			{
				if (pvt[1] == 16)
					_stprintf_s(buffer, bufsize, _T("%.8X"), pint[k]);
				else
					_stprintf_s(buffer, bufsize, _T("%d"), pint[k]);

				if ((k+1) < *((int*)&pvt[2]))
					_tcscat_s(buffer, bufsize, GSP_SPL_STR);

				if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
					return -2;
			}

			if (fwrite(CT_ARRYDSNE, _tcslen(CT_ARRYDSNE)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			pvt += sizeof(BYTE)*2 + sizeof(int);
			break;
		case	NTP_ARRAY_BOOL:			// bool[]数组数据
			pbool = (booln*)&pcur[m_data.m_struct.varoffset[i]];

			if (fwrite(CT_ARRYDSNB, _tcslen(CT_ARRYDSNB)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			for (int k=0; k<*((int*)&pvt[1]); k++)
			{
				if (fwrite(BOOLDES[pbool[k].index].oo[pbool[k].value], _tcslen(BOOLDES[pbool[k].index].oo[pbool[k].value])*sizeof(TCHAR), 1, fp) == 0)
					return -2;
				if ((k+1) < *((int*)&pvt[1]))
					if (fwrite(GSP_SPL_STR, _tcslen(GSP_SPL_STR)*sizeof(TCHAR), 1, fp) == 0)
						return -2;
			}

			if (fwrite(CT_ARRYDSNE, _tcslen(CT_ARRYDSNE)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			pvt += sizeof(BYTE) + sizeof(int);
			break;
		case	NTP_ARRAY_DOUBLE:		// double[]数组数据
			pdouble = (double*)&pcur[m_data.m_struct.varoffset[i]];

			if (fwrite(CT_ARRYDSNB, _tcslen(CT_ARRYDSNB)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			_stprintf_s(ptd, 16, _T("%%.%df"), *(int*)inseg->io(INIPARAMNAME[IPND_DOUBLE_PRECISION], NULL));

			for (int k=0; k<*((int*)&pvt[1]); k++)
			{
				_stprintf_s(buffer, bufsize, ptd, pdouble[k]);

				if ((k+1) < *((int*)&pvt[1]))
					_tcscat_s(buffer, bufsize, GSP_SPL_STR);

				if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
					return -2;
			}

			if (fwrite(CT_ARRYDSNE, _tcslen(CT_ARRYDSNE)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			pvt += sizeof(BYTE) + sizeof(int);
			break;
		case	NTP_ARRAY_STRING:		// string[]数组数据
			pstr = (LPCTSTR*)&pcur[m_data.m_struct.varoffset[i]];

			if (fwrite(CT_ARRYDSNB, _tcslen(CT_ARRYDSNB)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			for (int k=0; k<*((int*)&pvt[1]); k++)
			{
				ptmp = new (std::nothrow) TCHAR[_tcslen(pstr[k])*2+5];
				if (ptmp == NULL)
					return -1;

				ptmp[0] = CT_STR;
				_gsp_wst_cnv(pstr[k], &pstr[k][_tcslen(pstr[k])], GSP_ESCAPED, &ptmp[1]);
				_tcscat_s(ptmp, _tcslen(pstr[k])*2+5, CT_STRIDCH);

				if ((k+1) < *((int*)&pvt[1]))
					_tcscat_s(ptmp, _tcslen(pstr[k])*2+5, GSP_SPL_STR);

				if (fwrite(ptmp, _tcslen(ptmp)*sizeof(TCHAR), 1, fp) == 0)
				{
					delete []ptmp;
					return -2;
				}

				delete []ptmp;
			}

			if (fwrite(CT_ARRYDSNE, _tcslen(CT_ARRYDSNE)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			pvt += sizeof(BYTE) + sizeof(int);
			break;
		case	NTP_ARRAY_DATE:			// date[]数组数据
			pi64 = (__int64*)&pcur[m_data.m_struct.varoffset[i]];

			if (fwrite(CT_ARRYDSNB, _tcslen(CT_ARRYDSNB)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			for (int k=0; k<*((int*)&pvt[2]); k++)
			{
				_gsp_cnvdt_to_str(pi64[k], pvt[1], buffer, bufsize);

				if ((k+1) < *((int*)&pvt[2]))
					_tcscat_s(buffer, bufsize, GSP_SPL_STR);

				if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
					return -2;
			}

			if (fwrite(CT_ARRYDSNE, _tcslen(CT_ARRYDSNE)*sizeof(TCHAR), 1, fp) == 0)
				return -2;

			pvt += sizeof(BYTE)*2 + sizeof(int);
			break;
		default:
			_ASSERT(0);
			return -3;
		}

		if ((i+1) < smc)
			if (fwrite(GSP_SPL_STR, _tcslen(GSP_SPL_STR)*sizeof(TCHAR), 1, fp) == 0)
				return -2;
	}

	// }
	if (fwrite(CT_STCTDSNE, _tcslen(CT_STCTDSNE)*sizeof(TCHAR), 1, fp) == 0)
		return -2;

	return 0;
}


// 生成一个从程序内部创建的变量对象
//
// name		- IN - 变量名称
// sut		- IN - 由调用者填写好的结构数据
//
// 返回值：0 - 成功
//
// *注：
//
int varstruct::build(LPCTSTR name, LPILVT_STRUCT sut)
{
	_ASSERT(m_type == NTP_MAX);				// 必须是一个空类
	_ASSERT(name&&_tcslen(name));			// 变量名称不能为空
	_ASSERT(sut);

	m_type = NTP_STRUCT;
	
	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);

	memcpy((void*)&m_data.m_struct, (void*)sut, sizeof(ILVT_STRUCT));
	
	return 0;
}


// 生成一个从程序内部创建的数组变量对象
//
// name		- IN - 变量名称
// type		- IN - 结构数组的内存存放形式
// psuta	- IN - 结构数组数据
//
// 返回值：0 - 成功
//
// *注：
//
int varstruct::buildarray(LPCTSTR name, ILSUTMEMTPE type, void* psuta)
{
	_ASSERT(m_type == NTP_MAX);		// 必须是一个空类
	_ASSERT(name&&_tcslen(name));	// 变量名称不能为空
	_ASSERT(psuta);

	m_type = NTP_ARRAY_STRUCT;

	_tcscpy_s(m_name, MAX_VARNAME_LENGTH, name);

	switch (type)
	{
	case	ILSUTMEMTPE_ARRAY:
		memcpy((void*)&m_data.m_structarray, (void*)psuta, sizeof(ILVT_STRUCTARRAY));
		break;
	case	ILSUTMEMTPE_LIST:
		memcpy((void*)&m_data.m_structlist, (void*)psuta, sizeof(ILVT_STRUCTLIST));
		break;
	}

	return 0;
}



////////////////////////////////////////////////////////////////////////
// 段基类实现

segbaseclass::segbaseclass(SEGTYPE segtype, LPCTSTR segname)
{
	m_segtype = segtype;

	_ASSERT(_tcslen(segname) <= MAX_SEGNAME_LENGHT);
	_tcscpy_s(m_segname, MAX_SEGNAME_LENGHT, segname);

	m_first = 0;
	m_last  = 0;
	m_count = 0;

	m_reg_dataname		= NULL;
	m_reg_datanamecnt	= 0;

	m_prev = 0;
	m_next = 0;
}


segbaseclass::~segbaseclass()
{
}


// 将一个数据对象加入到变量链表中
//
// vbc	- IN - 数据对象基指针
//
// 返回值：
//
// *注：函数将改变链表头尾指针和待加入类中的链指针及所属段指针
//
void segbaseclass::append(varbaseclass *vbc)
{
	_ASSERT(vbc);
	// 只有全局段和数据段才能使用到链表
	_ASSERT((m_segtype == SEGT_GOLABLENAME)||(m_segtype == SEGT_VALSEGNAME));

	if (m_count == 0)
	{
		_ASSERT(m_first == NULL);
		_ASSERT(m_last  == NULL);

		m_first = vbc; 
		m_last  = vbc;
		vbc->m_prev = NULL;
		vbc->m_next = NULL;
		vbc->m_pseg = this;
	}
	else
	{
		_ASSERT(m_first&&m_last);

		m_last->m_next = vbc;
		vbc->m_prev    = m_last;
		vbc->m_next	   = NULL;
		vbc->m_pseg    = this;
		m_last = vbc;
	}

	m_count++;
}


// 将一个在变量链表中的数据对象摘除
//
// vbc	- IN - 变量对象地址
//
// *注：函数将指定变量对象从变量链表中摘除，并修正链表头尾指针及个数值。
//		函数只是将变量对象摘除，并不会释放。
//		调用者必须保证vbc对象是变量链表中的一员。
//
void segbaseclass::remove(varbaseclass *vbc)
{
	_ASSERT(vbc);
	// 只有全局段和数据段才能使用到链表
	_ASSERT((m_segtype == SEGT_GOLABLENAME)||(m_segtype == SEGT_VALSEGNAME));

	if (m_first == vbc)
		m_first = vbc->m_next;
	if (m_last == vbc)
		m_last  = vbc->m_prev;
	if (vbc->m_prev)
		vbc->m_prev->m_next = vbc->m_next;
	if (vbc->m_next)
		vbc->m_next->m_prev = vbc->m_prev;

	m_count--;

	_ASSERT(m_count >= 0);
}


// 搜索指定名称的变量对象地址
//
// varname	- IN - 变量名称串
//
// 返回值：返回变量对象的地址。如果指定的变量不存在，返回NULL。
//
// *注：
//
varbaseclass* segbaseclass::find_varobj(LPCTSTR varname)
{
	varbaseclass*	p = m_first;

	// 函数不接受无名变量
	if ((varname == NULL)||(_tcslen(varname) == 0))
	{
		_ASSERT(0);
		return NULL;
	}

	// 搜索指定变量（大小写敏感）
	for (int i=0; i<m_count; i++, p=p->m_next)
	{
		_ASSERT(p);

		if ((p->type() < NTP_MAX)&&(_tcscmp(p->name(), varname) == 0))
		{
			return p;
		}
	}

	return NULL;
}


// 获取指定变量的数据存放地址
//
// varname	- IN - 变量名称串
// cbf		- IN - 回调函数指针
//
// 返回值：返回变量存放内存地址。如果指定的变量不存在，返回NULL。
//
// *注：
//
void* segbaseclass::io(LPCTSTR varname, ILIO_CALLBACK_FUN cbf)
{
	varbaseclass*	p = m_first;

	// 函数不接受无名变量
	if ((varname == NULL)||(_tcslen(varname) == 0))
	{
		_ASSERT(0);
		return NULL;
	}

	// 搜索指定变量（大小写敏感）
	for (int i=0; i<m_count; i++, p=p->m_next)
	{
		_ASSERT(p);

		if (_tcscmp(p->name(), varname) == 0)
		{
			return p->io();
		}
	}

	if (cbf)
		return cbf(ILIOCBFM_MISS_VAR, m_segname, varname);	// 回调错误处理函数
	else
		return NULL;
}


// 注册数据名
//
// pdataname	- IN - 注册的数据名（字符串数组指针）
// datanamecnt	- IN - 注册的数据名个数
//
// 返回值：无
//
// *注：
// 
void segbaseclass::regname(LPCTSTR* pdataname, int datanamecnt)
{
	_ASSERT(datanamecnt <= 65535);

	m_reg_dataname		= pdataname;
	m_reg_datanamecnt	= datanamecnt;
}


// 重载数组操作符
//
// i	- IN - 已注册的数据字符串的索引
//
// 返回值：返回对应变量的数据地址。参加io()函数。
//
// *注：
//
void* segbaseclass::operator[](int i)
{
	_ASSERT((i >= 0)&&(i < m_reg_datanamecnt));		// 判断数据名索引的有效性

	varbaseclass*	pv;

	LPCTSTR	pname = m_reg_dataname[i];

	pv = find_varobj(m_reg_dataname[i]);			// 获取对应变量的类地址

	if (pv == NULL) return NULL;

	return pv->io();
}



////////////////////////////////////////////////////////////////////////
// 全局段类实现
golableseg::golableseg() : segbaseclass (SEGT_GOLABLENAME, _T(""))
{
}

golableseg::~golableseg()
{
	destroy();
}

// 整理全局段文本
//
// segbegin		- IN  - 待整理文本的开始地址
// segend		- IN  - 待整理文本的结束地址（往往就是下一个段的开始，或就是原始文本的结尾）
// orgnext		- OUT - 返回下一个段的开始地址
// tidytxtbegin	- IN  - 预编译缓冲区当前写入地址
// tidytxtend	- OUT - 返回预编译缓冲区写入地址
// pict			- IN  - 上下文信息类指针
//
// 返回值:  0  - 全局段整理完毕
//		   -1  - 函数内部申请内存失败
//		   -2  - 原始文本存在语法错误
//		   -3  - 系统异常
// *注：
//
int golableseg::tidy(LPCTSTR segbegin, LPCTSTR segend, LPCTSTR *orgnext, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	LPCTSTR		pcur = segbegin;			// 当前原始文本读取位置
	LPCTSTR		pnextcur  = NULL;			// 下一行原始文本的读取位置

	LPDWORD		pd   = (LPDWORD)tidytxtbegin;

	// [段名标志][段名长度][段名字][有效项个数] - (*全局段没有名字，所以段名长度值是0，段名字只包含一个NULL字符)
	pd[0] = _gsp_comp_attributemark(PCA_ATT_SEGNAME, PCA_ADD_GOLSEG);
	pd[1] = 0;
	*(LPTSTR)&pd[2] = CT_NULL;
	LPDWORD		punt = (LPDWORD)((LPTSTR)(((LPTSTR)&pd[2])+1));

	LPTSTR		pctb = (LPTSTR)&punt[1];	// 当前整理数据的写入位置
	LPTSTR		pcnx = pctb;				// 下个整理数据的写入位置
	LPTSTR		poneline  = NULL;			// 读取一行原始文本

	int			unt = 0;					// 有效项计数

	pict->push(_T("[]"));

	while (pcur < segend)
	{
		// 读取一行原始文本
		switch (_gsp_fgets(pcur, &pnextcur, &poneline))
		{
		case	0:		// 空内容
			pict->seterror(_ILEM_4);
			return -2;
		case	-1:		// 内存申请失败
			pict->seterror(_ILEM_1);
			return -1;
		}

		// 去除串头尾的无用字符
		_gsp_tidy_string(poneline);

		if (_gsp_is_void_line(poneline))			// 判断是否为空行
		{
			linevoid::tidy(pctb, &pcnx);
		}
		else if (_gsp_is_comment(poneline))			// 判断是否为单行注释
		{
			linecomm::tidy(poneline, pctb, &pcnx);
		}
		else if (_gsp_is_variable(poneline))		// 判断是不是变量声明行
		{
			// 寻找变量文本的结尾
			LPCTSTR pvarend = _gsp_find_varend(pcur, segend);

			if (pvarend == NULL)
			{
				delete []poneline;
				pict->seterror(_ILEM_1);
				return -1;
			}

			// 整理变量声明部分
			DATATYPE vtpe = varbaseclass::tidy_vardec(pcur, pctb, &pcnx, pict);

			switch (vtpe)
			{
			case	NTP_EC_NOTMEM:			// 错误代码：内存分配失败
				delete []poneline;
				return -1;
			case	NTP_EC_SYNERR:			// 错误代码：语法错误
				delete []poneline;
				return -2;
			}

			LPCTSTR	pvtable = pctb;
			pctb = pcnx;

			// 整理变量数据部分
			int retcode = varbaseclass::tidy_data(pcur, pvarend, pvtable, pctb, &pcnx, pict);

			switch (retcode)
			{
			case	-1:						// 内存申请失败
				delete []poneline;
				return -1;
			case	-2:						// 原始文本语法错误
				delete []poneline;
				return -2;
			case	-3:						// 系统异常
				_ASSERT(0);
				delete []poneline;
				return -3;
			}

			pnextcur = (LPTSTR)pvarend;
		}
		else if (_gsp_get_segtype(poneline) == SEGT_GOLABLENAME)
		{
			// 段名字必然是第一个被处理的行数据（unt的值为0）
			_ASSERT(unt == 0);
			// 因为函数开头已经写入了段名信息，所以此处不再处理段名行数据
			unt--;
		}
		else										// 未知内容行
		{
			pict->seterror(_ILEM_35, poneline);
			delete []poneline;
			return -2;
		}

		delete []poneline;

		poneline = NULL;
		pcur = pnextcur;
		pctb = pcnx;
		unt++;
	}

	pict->pop();

	punt[0] = unt;

	*orgnext	= pcur;
	*tidytxtend	= pctb;

	return 0;
}


// 根据预编译数据创建全局段对象
//
// pstr		- IN  - 全局段预编译数据起始地址
// inseg	- IN  - 内参段
// pend		- OUT - 下一个段在预编译数据中的位置
//
// 返回值：0		- 创建成功
//		  -1	- 内存不足
//		  -3	- 系统异常
// *注：
//
int golableseg::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	LPPCMIB_HEAD	phead = (LPPCMIB_HEAD)pstr;
	BYTE			attr, addition;
	int*			pvarcnts;
	int				retcode;
	varbaseclass*	pvc;


	// 分解预编译数据中的数据属性标志
	if ((_gsp_decomp_attributemark(phead->mark, &attr, &addition) == false)||
		(attr != PCA_ATT_SEGNAME)||
		(addition != PCA_ADD_GOLSEG)||
		(phead->namelen != 0))
	{
		_ASSERT(0);
		return -3;
	}
	
	// 获取段中有效项的个数
	pvarcnts = (int*)&phead->name[phead->namelen+1];
	phead	 = (LPPCMIB_HEAD)&pvarcnts[1];

	// 创建各有效项
	for (int i=0; i<pvarcnts[0]; i++)
	{
		if (_gsp_decomp_attributemark(phead->mark, &attr, &addition) == false)
		{
			_ASSERT(0);
			return -3;
		}

		_ASSERT(attr != PCA_ATT_SEGNAME);

		// 申请对象类实体
		switch (attr)
		{
		case	PCA_ATT_VOID:					// 空行
			pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) linevoid);
			break;
		case	PCA_ATT_COMM:					// 注释
			pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) linecomm);
			break;
		case	PCA_ATT_VAR:					// 变量
			{
				switch (addition)
				{
				case	PCA_ADD_VAR_BYTE:
				case	PCA_ADD_VAR_BYTEARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) varbyte);
					break;
				case	PCA_ADD_VAR_INT:
				case	PCA_ADD_VAR_INTARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) varint);
					break;
				case	PCA_ADD_VAR_BOOL:
				case	PCA_ADD_VAR_BOOLARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) varbool);
					break;
				case	PCA_ADD_VAR_DOUBLE:
				case	PCA_ADD_VAR_DOUBLEARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) vardouble);
					break;
				case	PCA_ADD_VAR_STRING:
				case	PCA_ADD_VAR_STRINGARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) varstring);
					break;
				case	PCA_ADD_VAR_DATE:
				case	PCA_ADD_VAR_DATEARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) vardate);
					break;
				case	PCA_ADD_VAR_STRUCT:
				case	PCA_ADD_VAR_STRUCTARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) varstruct);
					break;
				default:
					_ASSERT(0);
					return -3;
				}
			}
			break;
		default:
			_ASSERT(0);
			return -3;
		}

		if (pvc == NULL) 
			return -1;

		// 根据预编译数据初始化类实体
		retcode = pvc->create((LPCTSTR)phead, inseg, (LPCTSTR*)&phead);

		if (retcode != 0)
			return retcode;

		// 将类实体加入链表尾部
		append(pvc);
	}

	// 填写下一个段的起始地址
	*pend = (LPCTSTR)phead;
	
	_ASSERT(_CrtCheckMemory());

	return 0;
}


// 销毁一个段中的所有数据
//
// *注：函数销毁段中所有的数据，使之成为一个’空白‘段类。
//
void golableseg::destroy()
{
	varbaseclass*	p;

	for (int i=0; i<m_count; i++)
	{
		p = m_first;
		m_first = m_first->m_next;
		_ASSERT(p);
		delete p;
	}

	_ASSERT(m_first == NULL);

	m_first = NULL;
	m_last  = NULL;
	m_count = 0;

	m_prev  = NULL;
	m_next  = NULL;

	m_segname[0] = _T('\0');
	m_segtype = SEGT_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功，-1 - 内存不足，-2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int golableseg::write(FILE *fp, insideseg* inseg)
{
	varbaseclass*	p = m_first;
	int				rc;

	// 将本段中所有的项写入文件
	for (int i=0; i<m_count; i++, p=p->m_next)
	{
		_ASSERT(p);

		rc = p->write(fp, inseg);

		if (rc != 0)
			return rc;
	}

	_ASSERT(_CrtCheckMemory());
	
	return 0;
}



////////////////////////////////////////////////////////////////////////
// 内参段类实现

insideseg::insideseg(LPCTSTR segname) : segbaseclass (SEGT_INSIDEPARANAME, segname)
{
	m_writer_mark = INPST_DEFAULT;
	m_writer[0]   = _T('\0');						// ini 文件的创作者

	m_last_time_mark = INPST_DEFAULT;
	m_last_time[0]   = _T('\0');					// ini 最后一次写入时间

	m_read_only_mark = INPST_DEFAULT;
	m_read_only		 = false;						// ini 只读标志

	m_double_precision_mark = INPST_DEFAULT;		// ini double类型值精度
	m_double_precision		= IPDEF_DOUPRE;
}

insideseg::~insideseg()
{
	destroy();
}


// 整理内参段文本
//
// segbegin		- IN  - 待整理文本的开始地址
// segend		- IN  - 待整理文本的结束地址（往往就是下一个段的开始，或就是原始文本的结尾）
// orgnext		- OUT - 返回下一个段的开始地址
// tidytxtbegin	- IN  - 预编译缓冲区当前写入地址
// tidytxtend	- OUT - 返回预编译缓冲区写入地址
// pict			- IN  - 上下文信息类指针
//
// 返回值:  0  - 全局段整理完毕
//		   -1  - 函数内部申请内存失败
//		   -2  - 原始文本存在语法错误
//		   -3  - 系统异常
// *注：
//
int insideseg::tidy(LPCTSTR segbegin, LPCTSTR segend, LPCTSTR *orgnext, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	LPCTSTR		pcur = segbegin;			// 当前原始文本读取位置
	LPCTSTR		pnextcur  = NULL;			// 下一行原始文本的读取位置

	LPTSTR		pctb = tidytxtbegin;		// 当前整理数据的写入位置
	LPTSTR		pcnx = tidytxtbegin;		// 下个整理数据的写入位置

	LPTSTR		poneline  = NULL;			// 读取一行原始文本
	LPDWORD		punt;
	int			unt = 0;					// 本段中处理的元素计数
	size_t		slen;

	LPBYTE		pb = NULL;
	LPDWORD		pd = (LPDWORD)pctb;


	// 获取本段第一行的字符串，也就是段名行文本
	switch (_gsp_fgets(pcur, &pnextcur, &poneline))
	{
	case	0:
		_ASSERT(0);
		pict->seterror(_ILEM_4);
		return -3;							// str是一个空串，说明系统异常
	case	-1:
		pict->seterror(_ILEM_1);
		return -1;							// 内存分配失败
	}

	// 去除串头尾的无用字符
	slen = _gsp_tidy_string(poneline);

	poneline[0]      = CT_SPACE;
	poneline[slen-1] = CT_SPACE;

	// 去除段名标志及无用字符
	slen = _gsp_tidy_string(poneline);

	_ASSERT(poneline[0] == CT_SEG_INSD);

	// [段名标志][段名长度][段名字][有效项个数]
	pd[0] = _gsp_comp_attributemark(PCA_ATT_SEGNAME, PCA_ADD_INSIDESEG);
	pd[1] = (DWORD)slen;
	_tcscpy_s((LPTSTR)&pd[2], slen+1, poneline);

	punt = (LPDWORD)((LPBYTE)(((LPBYTE)pctb)+sizeof(DWORD)+sizeof(DWORD)+sizeof(TCHAR)*(slen+1)));
	punt[0] = 0;

	// 检查内参段段名称是否合法
	if (_tcscmp(poneline, INIPARAMSEGNAME) != 0)
	{
		pict->seterror(_ILEM_5, INIPARAMSEGNAME, poneline);
		delete []poneline;
		return -2;
	}

	pict->push(poneline);

	delete []poneline;
	poneline = NULL;

	// 更新整理内容写入地址
	pctb = (LPTSTR)&punt[1];
	pcnx = pctb;
	pcur = pnextcur;


	while (pcur < segend)
	{
		// 读取一行原始文本
		switch (_gsp_fgets(pcur, &pnextcur, &poneline))
		{
		case	0:		// 空内容
			pict->seterror(_ILEM_4);
			return -2;
		case	-1:		// 内存申请失败
			pict->seterror(_ILEM_1);
			return -1;
		}

		// 去除串头尾的无用字符
		slen = _gsp_tidy_string(poneline);

		pd = (LPDWORD)pctb;

		if (_gsp_is_void_line(poneline))			// 判断是否为空行
		{
			pd[0] = -1;
			pcnx  = (LPTSTR)&pd[1];
		}
		else if (_gsp_is_comment(poneline))			// 判断是否为单行注释
		{
			pd[0] = -2;
			pd[1] = (DWORD)_tcslen(poneline);
			_tcscpy_s((LPTSTR)&pd[2], pd[1]+1, poneline);

			pb = (LPBYTE)&pd[0];
			pb += sizeof(DWORD)+sizeof(DWORD)+(pd[1]+1)*sizeof(TCHAR);
			pcnx = (LPTSTR)pb;
		}
		else										// 内参变量
		{
			const int	_IGSPC_BUFSIZE_ = 256;
			int			rc;
			TCHAR		buff[_IGSPC_BUFSIZE_];
			TCHAR		name[_IGSPC_BUFSIZE_];
			TCHAR		value[_IGSPC_BUFSIZE_];

			// 读入内参变量（名称和对应的值）
			rc = _stscanf_s(poneline, _T("%[^ =\t]%[ =\t]%[^ \t\r\n]"), 
				name, _IGSPC_BUFSIZE_, 
				buff, _IGSPC_BUFSIZE_, 
				value, _IGSPC_BUFSIZE_);

			// 必须存在名称和赋值号
			if ((rc < 2)||(_tcslen(name)==0)||(_tcslen(buff)==0)||(_tcspbrk(buff,_T("="))==NULL))
			{
				pict->seterror(_ILEM_6, poneline);
				delete []poneline;
				return -2;
			}

			// 判断变量类型
			switch (_gsp_find_iniparamstr(name, INIPARAMNAME, IPND_MAX))
			{
			case	IPND_WRITER:	// IPND_WRITER		- [字符串长度 4byte][作者名称串，NULL结尾]
				pd[0] = IPND_WRITER;

				if (rc == 3)
				{
					pd[1] = (DWORD)_tcslen(value);
					_tcscpy_s((LPTSTR)&pd[2], pd[1]+1, value);
				}
				else
				{
					pd[1] = 0;
					*(LPTSTR)&pd[2] = NULL;
				}

				pb = (LPBYTE)&pd[0];
				pb += sizeof(DWORD)+sizeof(DWORD)+(pd[1]+1)*sizeof(TCHAR);
				pcnx = (LPTSTR)pb;
				break;
			case	IPND_LASTTIME:	// IPND_LASTTIME	- [字符串长度 4byte][时间串，NULL结尾]
				pd[0] = IPND_LASTTIME;

				if (rc == 3)
				{
					pd[1] = (DWORD)_tcslen(value);
					_tcscpy_s((LPTSTR)&pd[2], pd[1]+1, value);
				}
				else
				{
					pd[1] = 0;
					*(LPTSTR)&pd[2] = NULL;
				}

				pb = (LPBYTE)&pd[0];
				pb += sizeof(DWORD)+sizeof(DWORD)+(pd[1]+1)*sizeof(TCHAR);
				pcnx = (LPTSTR)pb;
				break;
			case	IPND_READONLY:	// IPND_READONLY	- [0或1,1byte]
				pd[0] = IPND_READONLY;
				pb = (LPBYTE)&pd[1];

				if (rc < 3)
				{
					pb[0] = 0;		// 默认为非只读
				}
				else
				{
					if ((_tcslen(value) != 1)||((value[0] != _T('0'))&&(value[0] != _T('1'))))
					{
						pict->seterror(_ILEM_7, name, _T("0/1"), value);
						delete []poneline;
						return -2;
					}

					pb[0] = (value[0] == _T('0')) ? (BYTE)0 : (BYTE)1;
				}

				pb = (LPBYTE)&pd[0];
				pb += sizeof(DWORD)+sizeof(BYTE);
				pcnx = (LPTSTR)pb;
				break;
			case	IPND_DOUBLE_PRECISION:
				pd[0] = IPND_DOUBLE_PRECISION;
				
				if (rc == 3)
				{
					int	itmp;

					_stscanf_s(value, _T("%d"), &itmp);		// 提取精度值
					itmp = min(max(itmp,1),6);				// 将值框定在指定范围内
					
					pd[1] = (DWORD)itmp;
				}
				else
				{
					pd[1] = IPDEF_DOUPRE;
				}

				pb = (LPBYTE)&pd[0];
				pb += sizeof(DWORD)+sizeof(DWORD);
				pcnx = (LPTSTR)pb;
				break;
			default:				// 语法错误
				pict->seterror(_ILEM_8, name);
				delete []poneline;
				return -2;
			}
		}

		delete []poneline;

		poneline = NULL;
		pcur = pnextcur;
		pctb = pcnx;
		unt++;
	}

	pict->pop();
	
	punt[0] = unt;

	*orgnext	= pcur;
	*tidytxtend	= pctb;

	return 0;
}


// 获取段中指定变量的数据存放地址
//
// varname	- IN - 指定的变量名
// cbf		- IN - 回调函数指针
//
// 返回值：返回变量地址。
//
// *注：
//
void* insideseg::io(LPCTSTR varname, ILIO_CALLBACK_FUN cbf)
{
	_ASSERT((m_first==NULL)&&(m_last==NULL));

	// 搜索指定变量
	for (int i=0; i<IPND_MAX; i++)
	{
		if (_tcscmp(INIPARAMNAME[i], varname) == 0)
		{
			switch (i)
			{
			case	IPND_WRITER:									// 作者
				return (void*)m_writer;
			case	IPND_LASTTIME:									// INI最后写入时间
				return (void*)m_last_time;
			case	IPND_READONLY:									// 只读状态
				return (void*)&m_read_only;
			case	IPND_DOUBLE_PRECISION:							// double值精度
				return (void*)&m_double_precision;
			}
		}
	}

	if (cbf)
		return cbf(ILIOCBFM_MISS_VAR, m_segname, varname);
	else
		return NULL;
}


// 根据预编译数据创建内参段对象
//
// pstr		- IN  - 内参段预编译数据起始地址
// inseg	- IN  - 内参段(自身)
// pend		- OUT - 下一个段在预编译数据中的位置
//
// 返回值：0		- 创建成功
//		  -1	- 内存不足
//		  -3	- 系统异常
// *注：
//
int insideseg::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	LPPCMIB_HEAD	phead = (LPPCMIB_HEAD)pstr;
	BYTE			attr, addition;
	int*			pvarcnts;

	_ASSERT(inseg == this);
	_ASSERT(m_count == 0);

	// 分解预编译数据中的数据属性标志
	if ((_gsp_decomp_attributemark(phead->mark, &attr, &addition) == false)||
		(attr != PCA_ATT_SEGNAME)||
		(addition != PCA_ADD_INSIDESEG)||
		(phead->name[0] != _T('!')))
	{
		_ASSERT(0);
		return -3;
	}
	
	// 获取段中有效项的个数
	pvarcnts = (int*)&phead->name[phead->namelen+1];
	phead	 = (LPPCMIB_HEAD)&pvarcnts[1];

	// 创建各有效项
	for (int i=0; i<pvarcnts[0]; i++)
	{
		switch (phead->mark)
		{
		case	IPND_WRITER:			// writers
			if (phead->namelen >= INSEGPS_MAXWRITERLEN) return -1;
			_tcscpy_s(m_writer, INSEGPS_MAXWRITERLEN, phead->name);
			m_writer_mark = INPST_USERSET;
			m_count++;

			phead = (LPPCMIB_HEAD)&phead->name[phead->namelen+1];
			break;
		case	IPND_LASTTIME:			// last_time
			if (phead->namelen >= INSEGPS_MAXLTLEN) return -1;
			_tcscpy_s(m_last_time, INSEGPS_MAXLTLEN, phead->name);
			m_last_time_mark = INPST_USERSET;
			m_count++;

			phead = (LPPCMIB_HEAD)&phead->name[phead->namelen+1];
			break;
		case	IPND_READONLY:			// read_only
			m_read_only = *(LPBYTE)&phead->namelen ? true : false;
			m_read_only_mark = INPST_USERSET;
			m_count++;

			phead = (LPPCMIB_HEAD)&((LPBYTE)&phead->namelen)[1];
			break;
		case	IPND_DOUBLE_PRECISION:	// double_precision
			m_double_precision = phead->namelen;
			m_double_precision_mark = INPST_USERSET;
			m_count++;

			phead = (LPPCMIB_HEAD)&phead->name[0];
			break;
		case	-1:						// 空行 (注：内参段内的空行和注释将被忽略）
			phead = (LPPCMIB_HEAD)&phead->namelen;
			break;
		case	-2:						// 注释 (注：内参段内的空行和注释将被忽略）
			phead = (LPPCMIB_HEAD)&phead->name[phead->namelen+1];
			break;
		default:
			_ASSERT(0);
			return -3;
		}
	}

	// 填写下一个段的起始地址
	*pend = (LPCTSTR)phead;

	return 0;
}


// 销毁一个段中的所有数据
//
// *注：函数销毁段中所有的数据，使之成为一个’空白‘段类。
//
void insideseg::destroy()
{
	m_writer_mark = INPST_DEFAULT;
	m_writer[0]   = _T('\0');						// ini 文件的创作者

	m_last_time_mark = INPST_DEFAULT;
	m_last_time[0]   = _T('\0');					// ini 最后一次写入时间

	m_read_only_mark = INPST_DEFAULT;
	m_read_only		 = false;						// ini 只读标志

	m_double_precision_mark	= INPST_DEFAULT;
	m_double_precision		= IPDEF_DOUPRE;			// ini double精度值

	m_count = 0;

	m_prev  = NULL;
	m_next  = NULL;

	m_segname[0] = _T('\0');
	m_segtype = SEGT_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功，-2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int insideseg::write(FILE *fp, insideseg* inseg)
{
	const size_t	BUFSIZE = (size_t)MAX_SEGNAME_LENGHT*3;
	TCHAR			buffer[BUFSIZE];

	if (((m_writer_mark == INPST_USERSET)||(m_writer_mark == INPST_INNERSET))||
		((m_last_time_mark == INPST_USERSET)||(m_last_time_mark == INPST_INNERSET))||
		((m_read_only_mark == INPST_USERSET)||(m_read_only_mark == INPST_INNERSET))||
		((m_double_precision_mark == INPST_USERSET)||(m_double_precision_mark == INPST_INNERSET)))
	{
		// 只有存在需要保持的内参段变量时，才合成并写入段名字
		_stprintf_s(buffer, BUFSIZE, _T("%c%s%c%s"), CT_SEGNB, m_segname, CT_SEGNE, CT_CRNL);

		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
	}
	else
	{
		return 0;
	}

	// 将本段中有效的项（用户明确要记录的项）写入文件

	// 作者
	if ((m_writer_mark == INPST_USERSET)||(m_writer_mark == INPST_INNERSET))
	{
		// 如果宿主程序没有设置软件名称，则获取当前进程的文件名填写在作者字符串中
		if (_tcslen(m_writer) == 0)
		{
			TCHAR	path[MAX_PATH];
			TCHAR	fn[_MAX_FNAME];

			if (::GetModuleFileName(NULL, path, MAX_PATH) == 0)
				return -3;

			_tsplitpath_s(path, NULL, 0, NULL, 0, fn, _MAX_FNAME, NULL, 0);
			_tcscpy_s(m_writer, INSEGPS_MAXWRITERLEN, fn);
		}

		_stprintf_s(buffer, BUFSIZE, _T("%s\t= %s%s"), INIPARAMNAME[IPND_WRITER], m_writer, CT_CRNL);
		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
	}

	// 最后写入时间
	if ((m_last_time_mark == INPST_USERSET)||(m_last_time_mark == INPST_INNERSET))
	{
		tm			tinfo;
		__time64_t	t;

		_time64(&t);
		_localtime64_s(&tinfo, &t);
	
		// 根据语言区域决定日期的输出格式
#ifdef EM_LANGUAGE_CHS
		_tcsftime(buffer, BUFSIZE, _T("%Y/%m/%d %H:%M:%S"), &tinfo);
#elif defined(EM_LANGUAGE_ENG)
		_tcsftime(buffer, BUFSIZE, _T("%m/%d/%Y %H:%M:%S"), &tinfo);
#endif

		_tcscpy_s(m_last_time, INSEGPS_MAXLTLEN, buffer);

		_stprintf_s(buffer, BUFSIZE, _T("%s\t= %s%s"), INIPARAMNAME[IPND_LASTTIME], m_last_time, CT_CRNL);
		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
	}

	// 只读标志
	if ((m_read_only_mark == INPST_USERSET)||(m_read_only_mark == INPST_INNERSET))
	{
		_stprintf_s(buffer, BUFSIZE, _T("%s\t= %d%s"), INIPARAMNAME[IPND_READONLY], (m_read_only==true) ? 1:0, CT_CRNL);
		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
	}

	// double精度值
	if ((m_double_precision_mark == INPST_USERSET)||(m_double_precision_mark == INPST_INNERSET))
	{
		_stprintf_s(buffer, BUFSIZE, _T("%s\t= %d%s"), INIPARAMNAME[IPND_DOUBLE_PRECISION], m_double_precision, CT_CRNL);
		if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
			return -2;
	}

	// 合成段结尾空行（因内参段会忽略用户在本段中输入的注释行和空行，为清晰，所以段结尾加入两个空行）
	_stprintf_s(buffer, BUFSIZE, _T("%s%s"), CT_CRNL, CT_CRNL);

	if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
		return -2;

	return 0;
}



////////////////////////////////////////////////////////////////////////
// 注释段类实现

commseg::commseg(LPCTSTR segname) : segbaseclass (SEGT_COMMSEGNAME, segname)
{
	m_pcomm = NULL;
	m_count = 0;
}

commseg::~commseg()
{
	destroy();
}


// 整理注释段文本
//
// segbegin		- IN  - 待整理文本的开始地址
// segend		- IN  - 待整理文本的结束地址（往往就是下一个段的开始，或就是原始文本的结尾）
// orgnext		- OUT - 返回下一个段的开始地址
// tidytxtbegin	- IN  - 预编译缓冲区当前写入地址
// tidytxtend	- OUT - 返回预编译缓冲区写入地址
// pict			- IN  - 上下文信息类指针
//
// 返回值:  0  - 全局段整理完毕
//		   -1  - 函数内部申请内存失败
//		   -2  - 原始文本存在语法错误
//		   -3  - 系统异常
// *注：
//
int commseg::tidy(LPCTSTR segbegin, LPCTSTR segend, LPCTSTR *orgnext, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	LPCTSTR		pcur = segbegin;			// 当前原始文本读取位置
	LPCTSTR		pnextcur  = NULL;			// 下一行原始文本的读取位置

	LPTSTR		pctb = tidytxtbegin;		// 当前整理数据的写入位置
	LPTSTR		pcnx = tidytxtbegin;		// 下个整理数据的写入位置

	LPTSTR		poneline  = NULL;			// 读取一行原始文本
	LPDWORD		punt;
	int			unt = 0;					// 本段中处理的元素计数
	size_t		slen;

	LPDWORD		pd = (LPDWORD)pctb;


	// 获取本段第一行的字符串，也就是注释段的段名行文本
	switch (_gsp_fgets(pcur, &pnextcur, &poneline))
	{
	case	0:
		_ASSERT(0);
		pict->seterror(_ILEM_4);
		return -3;							// str是一个空串，说明系统异常
	case	-1:
		pict->seterror(_ILEM_1);
		return -1;							// 内存分配失败
	}

	// 去除串头尾的无用字符
	slen = _gsp_tidy_string(poneline);

	poneline[0]      = CT_SPACE;
	poneline[slen-1] = CT_SPACE;

	// 去除段名标志及无用字符
	slen = _gsp_tidy_string(poneline);

	_ASSERT(poneline[0] == CT_SEG_COMM);

	// [段名标志][段名长度][段名字][有效项个数]
	pd[0] = _gsp_comp_attributemark(PCA_ATT_SEGNAME, PCA_ADD_COMMSEG);
	pd[1] = (DWORD)slen;
	_tcscpy_s((LPTSTR)&pd[2], slen+1, poneline);

	punt = (LPDWORD)((LPBYTE)(((LPBYTE)pctb)+sizeof(DWORD)+sizeof(DWORD)+sizeof(TCHAR)*(slen+1)));
	punt[0] = 0;

	pict->push(poneline);
	
	delete []poneline;
	poneline = NULL;

	// 更新整理内容写入地址
	pctb = (LPTSTR)&punt[1];
	pcnx = pctb;
	pcur = pnextcur;


	while (pcur < segend)
	{
		// 获取一行文本的结尾地址
		pnextcur = _tcsstr(pcur, CT_CRNL);

		if (pnextcur == NULL)
			pnextcur = segend;
		else
			pnextcur += _tcslen(CT_CRNL);		// 包含结尾的\r\n

		LPDWORD	psl = (LPDWORD)pctb;
		pctb		= (LPTSTR)&psl[1];
		pcnx		= pctb;

		// 复制注释内容到整理缓冲区（同时过滤掉转移符）
		for (int i=0; i<(pnextcur-pcur); i++)
		{
			if (pcur[i] == CT_ESCP)
				continue;
			else
				*pcnx++ = pcur[i];
		}

		*pcnx = CT_NULL;
		psl[0] = (int)(pcnx - pctb);
		pcnx++;

		pcur = pnextcur;
		pctb = pcnx;
		unt++;
	}

	pict->pop();
	punt[0] = unt;

	*orgnext	= pcur;
	*tidytxtend	= pctb;

	return 0;
}


// 获取段中指定变量的数据存放地址
//
// varname	- IN - 指定的编号
// cbf		- IN - 回调函数指针
//
// 返回值：返回变量地址。
//
// *注：因为注释段中的内容没有名字，所以使用行索引的方式访问。调用时需要强制转换数字到指针类型。
//
void* commseg::io(LPCTSTR varname, ILIO_CALLBACK_FUN cbf)
{
	int	idx = (int)varname;

	// 超出正常索引范围
	if ((idx < 0)||(idx >= m_count)||(m_pcomm == NULL))
	{
		if (cbf)
			return cbf(ILIOCBFM_MISS_VAR, m_segname, varname);
		else
			return NULL;
	}

	return (void*)m_pcomm[idx];
}


// 根据预编译数据创建注释段对象
//
// pstr		- IN  - 注释段预编译数据起始地址
// inseg	- IN  - 内参段
// pend		- OUT - 下一个段在预编译数据中的位置
//
// 返回值：0		- 创建成功
//		  -1	- 内存不足
//		  -3	- 系统异常
// *注：
//
int commseg::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	LPPCMIB_HEAD	phead = (LPPCMIB_HEAD)pstr;
	BYTE			attr, addition;
	int*			pvarcnts;

	LPPCMIB_VT_STRING	pvs;

	_ASSERT(m_pcomm == NULL);
	_ASSERT(m_count == 0);

	// 分解预编译数据中的数据属性标志
	if ((_gsp_decomp_attributemark(phead->mark, &attr, &addition) == false)||
		(attr != PCA_ATT_SEGNAME)||
		(addition != PCA_ADD_COMMSEG)||
		(phead->name[0] != _T('~')))
	{
		_ASSERT(0);
		return -3;
	}
	
	// 获取段中有效项的个数
	pvarcnts = (int*)&phead->name[phead->namelen+1];
	pvs		 = (LPPCMIB_VT_STRING)&pvarcnts[1];

	// 申请注释信息指针数组
	m_pcomm = (LPCTSTR*) new (std::nothrow) LPCTSTR[pvarcnts[0]];
	if (m_pcomm == NULL)
		return -1;

	// 创建各有效项
	for (int i=0; i<pvarcnts[0]; i++)
	{
		m_pcomm[i] = new (std::nothrow) TCHAR[pvs->len+1];
		if (m_pcomm[i] == NULL)
			return -1;

		_tcscpy_s((LPTSTR)m_pcomm[i], pvs->len+1, pvs->value);

		m_count++;
		pvs = (LPPCMIB_VT_STRING)&pvs->value[pvs->len+1];
	}

	// 填写下一个段的起始地址
	*pend = (LPCTSTR)pvs;
	
	_ASSERT(_CrtCheckMemory());

	return 0;
}


// 销毁一个段中的所有数据
//
// *注：函数销毁段中所有的数据，使之成为一个’空白‘段类。
//
void commseg::destroy()
{
	if (m_pcomm)
	{
		for (int i=0; i<m_count; i++)
		{
			delete []m_pcomm[i];
		}

		delete []m_pcomm;
		m_pcomm = NULL;
	}

	m_count = 0;

	m_prev  = NULL;
	m_next  = NULL;

	m_segname[0] = _T('\0');
	m_segtype = SEGT_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功，-1 - 内存不足，-2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int commseg::write(FILE *fp, insideseg* inseg)
{
	const size_t	BUFSIZE = (size_t)MAX_SEGNAME_LENGHT*2;
	TCHAR			buffer[BUFSIZE];
	int				len;

	// 合成段名字
	_stprintf_s(buffer, BUFSIZE, _T("%c%s%c%s"), CT_SEGNB, m_segname, CT_SEGNE, CT_CRNL);

	// 写入文件
	if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
		return -2;

	// 将本段中所有的项写入文件
	for (int i=0; i<m_count; i++)
	{
		len = _tcslen(m_pcomm[i]);

		// 即使是空行也会包含\r\n，所以长度不会是0
		_ASSERT(len > 0);

		// 较短的行使用静态缓冲区，以提高速度。比较长的行申请内存缓冲区。
		if (len < (BUFSIZE/2))
		{
			_gsp_wst_cnv(m_pcomm[i], &m_pcomm[i][len], GSP_COMMSEG_ESCAPED, buffer);

			if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
				return -2;
		}
		else
		{
			LPTSTR	ptmp = new (std::nothrow) TCHAR[len*2+1];

			if (ptmp == NULL) return -1;

			_gsp_wst_cnv(m_pcomm[i], &m_pcomm[i][len], GSP_COMMSEG_ESCAPED, ptmp);

			if (fwrite(ptmp, _tcslen(ptmp)*sizeof(TCHAR), 1, fp) == 0)
			{
				delete []ptmp;
				return -2;
			}

			delete []ptmp;
		}
	}

	_ASSERT(_CrtCheckMemory());
	
	return 0;
}




////////////////////////////////////////////////////////////////////////
// 数据段类实现

norseg::norseg(LPCTSTR segname) : segbaseclass (SEGT_VALSEGNAME, segname)
{
}

norseg::~norseg()
{
	destroy();
}


// 整理变量段文本
//
// segbegin		- IN  - 待整理文本的开始地址
// segend		- IN  - 待整理文本的结束地址（往往就是下一个段的开始，或就是原始文本的结尾）
// orgnext		- OUT - 返回下一个段的开始地址
// tidytxtbegin	- IN  - 预编译缓冲区当前写入地址
// tidytxtend	- OUT - 返回预编译缓冲区写入地址
// pict			- IN  - 上下文信息类指针
//
// 返回值:  0  - 全局段整理完毕
//		   -1  - 函数内部申请内存失败
//		   -2  - 原始文本存在语法错误
//		   -3  - 系统异常
// *注：
//
int norseg::tidy(LPCTSTR segbegin, LPCTSTR segend, LPCTSTR *orgnext, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	LPCTSTR		pcur = segbegin;			// 当前原始文本读取位置
	LPCTSTR		pnextcur  = NULL;			// 下一行原始文本的读取位置

	LPTSTR		pctb = tidytxtbegin;		// 当前整理数据的写入位置
	LPTSTR		pcnx = tidytxtbegin;		// 下个整理数据的写入位置
	LPTSTR		poneline  = NULL;			// 读取一行原始文本

	LPDWORD		punt = NULL;				// 变量个数计数位置的指针
	int			unt = 0;					// 本段中处理的元素计数
	size_t		slen;


	while (pcur < segend)
	{
		// 读取一行原始文本
		switch (_gsp_fgets(pcur, &pnextcur, &poneline))
		{
		case	0:		// 空内容
			pict->seterror(_ILEM_4);
			return -3;
		case	-1:		// 内存申请失败
			pict->seterror(_ILEM_1);
			return -1;
		}

		// 去除串头尾的无用字符
		slen = _gsp_tidy_string(poneline);

		if (_gsp_is_void_line(poneline))			// 判断是否为空行
		{
			linevoid::tidy(pctb, &pcnx);
		}
		else if (_gsp_is_comment(poneline))			// 判断是否为单行注释
		{
			linecomm::tidy(poneline, pctb, &pcnx);
		}
		else if (_gsp_is_variable(poneline))		// 判断是不是变量声明行
		{
			// 寻找变量文本的结尾
			LPCTSTR pvarend = _gsp_find_varend(pcur, segend);

			if (pvarend == NULL)
			{
				delete []poneline;
				pict->seterror(_ILEM_1);
				return -1;
			}

			// 整理变量声明部分
			DATATYPE vtpe = varbaseclass::tidy_vardec(pcur, pctb, &pcnx, pict);

			switch (vtpe)
			{
			case	NTP_EC_NOTMEM:			// 错误代码：内存分配失败
				delete []poneline;
				return -1;
			case	NTP_EC_SYNERR:			// 错误代码：语法错误
				delete []poneline;
				return -2;
			}

			LPCTSTR	pvtable = pctb;
			pctb = pcnx;

			// 整理变量数据部分
			int retcode = varbaseclass::tidy_data(pcur, pvarend, pvtable, pctb, &pcnx, pict);

			switch (retcode)
			{
			case	-1:						// 内存申请失败
				delete []poneline;
				return -1;
			case	-2:						// 原始文本语法错误
				delete []poneline;
				return -2;
			case	-3:						// 系统异常
				_ASSERT(0);
				delete []poneline;
				return -3;
			}

			pnextcur = (LPTSTR)pvarend;
		}
		else if (_gsp_get_segtype(poneline) == SEGT_VALSEGNAME)
		{
			LPDWORD	pd = (LPDWORD)pctb;

			// 段名字必然是第一个被处理的行数据（unt的值为0）
			_ASSERT(unt == 0);

			// [段名标志][段名长度][段名字][有效项个数]
			pd[0] = _gsp_comp_attributemark(PCA_ATT_SEGNAME, PCA_ADD_NORSEG);

			poneline[0]      = CT_SPACE;
			poneline[slen-1] = CT_SPACE;

			// 去除段名标志及无用字符
			slen = _gsp_tidy_string(poneline);

			pd[1] = (DWORD)slen;
			_tcscpy_s((LPTSTR)&pd[2], slen+1, poneline);

			punt = (LPDWORD)((LPBYTE)(((LPBYTE)pctb)+sizeof(DWORD)+sizeof(DWORD)+sizeof(TCHAR)*(slen+1)));

			// 更新整理内容写入地址
			pcnx = (LPTSTR)(&punt[1]);
			// 变量段有效项计数减一，因为段名称本身不算有效项
			unt--;
			
			pict->push(poneline);
		}
		else										// 未知内容行
		{
			pict->seterror(_ILEM_35, poneline);
			delete []poneline;
			return -2;
		}

		delete []poneline;

		poneline = NULL;
		pcur = pnextcur;
		pctb = pcnx;
		unt++;
	}

	pict->pop();
	punt[0] = unt;

	*orgnext	= pcur;
	*tidytxtend	= pctb;

	return 0;
}


// 根据预编译数据创建数据段对象
//
// pstr		- IN  - 数据段预编译数据起始地址
// inseg	- IN  - 内参段
// pend		- OUT - 下一个段在预编译数据中的位置
//
// 返回值：0		- 创建成功
//		  -1	- 内存不足
//		  -3	- 系统异常
// *注：
//
int norseg::create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend)
{
	LPPCMIB_HEAD	phead = (LPPCMIB_HEAD)pstr;
	BYTE			attr, addition;
	int*			pvarcnts;
	int				retcode;
	varbaseclass*	pvc;

	// 数据段不允许合并
	_ASSERT(m_count == 0);

	// 分解预编译数据中的数据属性标志
	if ((_gsp_decomp_attributemark(phead->mark, &attr, &addition) == false)||
		(attr != PCA_ATT_SEGNAME)||
		(addition != PCA_ADD_NORSEG))
	{
		_ASSERT(0);
		return -3;
	}
	
	// 获取段中有效项的个数
	pvarcnts = (int*)&phead->name[phead->namelen+1];
	phead	 = (LPPCMIB_HEAD)&pvarcnts[1];

	// 创建各有效项
	for (int i=0; i<pvarcnts[0]; i++)
	{
		if (_gsp_decomp_attributemark(phead->mark, &attr, &addition) == false)
		{
			_ASSERT(0);
			return -3;
		}

		_ASSERT(attr != PCA_ATT_SEGNAME);

		// 申请对象类实体
		switch (attr)
		{
		case	PCA_ATT_VOID:					// 空行
			pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) linevoid);
			break;
		case	PCA_ATT_COMM:					// 注释
			pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) linecomm);
			break;
		case	PCA_ATT_VAR:					// 变量
			{
				switch (addition)
				{
				case	PCA_ADD_VAR_BYTE:
				case	PCA_ADD_VAR_BYTEARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) varbyte);
					break;
				case	PCA_ADD_VAR_INT:
				case	PCA_ADD_VAR_INTARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) varint);
					break;
				case	PCA_ADD_VAR_BOOL:
				case	PCA_ADD_VAR_BOOLARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) varbool);
					break;
				case	PCA_ADD_VAR_DOUBLE:
				case	PCA_ADD_VAR_DOUBLEARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) vardouble);
					break;
				case	PCA_ADD_VAR_STRING:
				case	PCA_ADD_VAR_STRINGARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) varstring);
					break;
				case	PCA_ADD_VAR_DATE:
				case	PCA_ADD_VAR_DATEARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) vardate);
					break;
				case	PCA_ADD_VAR_STRUCT:
				case	PCA_ADD_VAR_STRUCTARRAY:
					pvc = dynamic_cast<varbaseclass*>(new (std::nothrow) varstruct);
					break;
				default:
					_ASSERT(0);
					return -3;
				}
			}
			break;
		default:
			_ASSERT(0);
			return -3;
		}

		if (pvc == NULL) 
			return -1;

		// 根据预编译数据初始化类实体
		retcode = pvc->create((LPCTSTR)phead, inseg, (LPCTSTR*)&phead);

		if (retcode != 0)
			return retcode;

		// 将类实体加入链表尾部
		append(pvc);
	}

	// 填写下一个段的起始地址
	*pend = (LPCTSTR)phead;
	
	_ASSERT(_CrtCheckMemory());

	return 0;
}


// 销毁一个段中的所有数据
//
// *注：函数销毁段中所有的数据，使之成为一个’空白‘段类。
//
void norseg::destroy()
{
	varbaseclass*	p;

	for (int i=0; i<m_count; i++)
	{
		p = m_first;
		m_first = m_first->m_next;
		_ASSERT(p);
		delete p;
	}
	
	_ASSERT(m_first == NULL);

	m_first = NULL;
	m_last  = NULL;
	m_count = 0;

	m_prev  = NULL;
	m_next  = NULL;

	m_segname[0] = _T('\0');
	m_segtype = SEGT_MAX;
}


// 将类内容写入指定文件
//
// fp		- IN - 文件指针
// inseg	- IN - 内参段地址
//
// 返回值：0 - 写入成功，-1 - 内存不足，-2 - 写操作失败，-3 - 系统异常
//
// *注：
//
int norseg::write(FILE *fp, insideseg* inseg)
{
	const size_t	BUFSIZE = (size_t)MAX_SEGNAME_LENGHT*2;
	TCHAR			buffer[BUFSIZE];
	varbaseclass*	p = m_first;
	int				rc;

	// 合成段名字
	_stprintf_s(buffer, BUFSIZE, _T("%c%s%c%s"), CT_SEGNB, m_segname, CT_SEGNE, CT_CRNL);

	// 写入文件
	if (fwrite(buffer, _tcslen(buffer)*sizeof(TCHAR), 1, fp) == 0)
		return -2;

	// 将本段中所有的项写入文件
	for (int i=0; i<m_count; i++, p=p->m_next)
	{
		_ASSERT(p);

		rc = p->write(fp, inseg);

		if (rc != 0)
			return rc;
	}

	_ASSERT(_CrtCheckMemory());
	
	return 0;
}



/////////////////////////
// 预编译数据摘要类

pcsummary::pcsummary()
{
	m_insegcnt	= 0;
	m_insegaddr	= NULL;
	m_comsegcnt	= 0;
	m_comsegaddr= NULL;
	m_golsegcnt	= 0;
	m_golsegaddr= NULL;
	m_norsegcnt	= 0;
	m_norsegaddr= NULL;

	m_maxisc	= 0;
	m_maxcsc	= 0;
	m_maxgsc	= 0;
	m_maxnsc	= 0;

	m_precombegin	= NULL;
	m_precomend		= NULL;
}


pcsummary::~pcsummary()
{
	m_precombegin	= NULL;
	m_precomend		= NULL;

	m_maxisc	= 0;
	m_maxcsc	= 0;
	m_maxgsc	= 0;
	m_maxnsc	= 0;

	m_insegcnt	= 0;
	if (m_insegaddr != NULL) 
	{
		delete []m_insegaddr;
		m_insegaddr = NULL;
	}
	m_comsegcnt	= 0;
	if (m_comsegaddr != NULL)
	{
		delete []m_comsegaddr;
		m_comsegaddr = NULL;
	}
	m_golsegcnt	= 0;
	if (m_golsegaddr != NULL)
	{
		delete []m_golsegaddr;
		m_golsegaddr = NULL;
	}
	m_norsegcnt	= 0;
	if (m_norsegaddr != NULL)
	{
		delete []m_norsegaddr;
		m_norsegaddr = NULL;
	}
}

// 初始化摘要类内部变量
//
// 返回值：0 - 成功，-1 - 内存不足
//
int pcsummary::initialization()
{
	m_insegcnt	= 0;
	m_insegaddr	= new (std::nothrow) LPBYTE[PCSUMMARY_STEP];
	m_comsegcnt	= 0;
	m_comsegaddr= new (std::nothrow) LPBYTE[PCSUMMARY_STEP];
	m_golsegcnt	= 0;
	m_golsegaddr= new (std::nothrow) LPBYTE[PCSUMMARY_STEP];
	m_norsegcnt	= 0;
	m_norsegaddr= new (std::nothrow) LPBYTE[PCSUMMARY_STEP];

	m_maxisc	= PCSUMMARY_STEP/2;
	m_maxcsc	= PCSUMMARY_STEP/2;
	m_maxgsc	= PCSUMMARY_STEP/2;
	m_maxnsc	= PCSUMMARY_STEP/2;

	m_precombegin	= NULL;
	m_precomend		= NULL;

	if ((m_insegaddr==NULL)||(m_comsegaddr==NULL)||(m_golsegaddr==NULL)||(m_norsegaddr==NULL))
		return -1;
	else
		return 0;
}


// 记录一个段在预编译数据中的起始、结束地址
//
// type		- IN - 段的类型
// begin	- IN - 起始地址
// end		- IN - 结束地址
//
// 返回值：0 - 成功，-1 - 内存不足，-2 - 系统异常
int pcsummary::add(SEGTYPE type, LPBYTE begin, LPBYTE end)
{
	LPBYTE*	ptmp;

	_ASSERT(begin&&end);

	switch (type)
	{
	case	SEGT_GOLABLENAME:		// 全局段
		// 判断缓冲区是否溢出，如果溢出，则重新申请大一倍的内存
		if (m_golsegcnt == m_maxgsc)
		{
			ptmp = new (std::nothrow) LPBYTE[m_maxgsc*2*2];
			if (ptmp == NULL) return -1;
			memmove_s(ptmp, m_maxgsc*2*2*sizeof(LPBYTE), m_golsegaddr, m_maxgsc*2*sizeof(LPBYTE));
			delete m_golsegaddr;
			m_golsegaddr = ptmp;
			m_maxgsc = m_maxgsc*2;
		}

		m_golsegaddr[m_golsegcnt*2]   = begin;
		m_golsegaddr[m_golsegcnt*2+1] = end;

		m_golsegcnt++;
		break;
	case	SEGT_INSIDEPARANAME:	// 内参段
		if (m_insegcnt == m_maxisc)
		{
			ptmp = new (std::nothrow) LPBYTE[m_maxisc*2*2];
			if (ptmp == NULL) return -1;
			memmove_s(ptmp, m_maxisc*2*2*sizeof(LPBYTE), m_insegaddr, m_maxisc*2*sizeof(LPBYTE));
			delete m_insegaddr;
			m_insegaddr = ptmp;
			m_maxisc = m_maxisc*2;
		}

		m_insegaddr[m_insegcnt*2]   = begin;
		m_insegaddr[m_insegcnt*2+1] = end;

		m_insegcnt++;
		break;
	case	SEGT_COMMSEGNAME:		// 注释段
		if (m_comsegcnt == m_maxcsc)
		{
			ptmp = new (std::nothrow) LPBYTE[m_maxcsc*2*2];
			if (ptmp == NULL) return -1;
			memmove_s(ptmp, m_maxcsc*2*2*sizeof(LPBYTE), m_comsegaddr, m_maxcsc*2*sizeof(LPBYTE));
			delete m_comsegaddr;
			m_comsegaddr = ptmp;
			m_maxcsc = m_maxcsc*2;
		}

		m_comsegaddr[m_comsegcnt*2]   = begin;
		m_comsegaddr[m_comsegcnt*2+1] = end;

		m_comsegcnt++;
		break;
	case	SEGT_VALSEGNAME:		// 数据段
		if (m_norsegcnt == m_maxnsc)
		{
			ptmp = new (std::nothrow) LPBYTE[m_maxnsc*2*2];
			if (ptmp == NULL) return -1;
			memmove_s(ptmp, m_maxnsc*2*2*sizeof(LPBYTE), m_norsegaddr, m_maxnsc*2*sizeof(LPBYTE));
			delete m_norsegaddr;
			m_norsegaddr = ptmp;
			m_maxnsc = m_maxnsc*2;
		}

		m_norsegaddr[m_norsegcnt*2]   = begin;
		m_norsegaddr[m_norsegcnt*2+1] = end;

		m_norsegcnt++;
		break;
	default:
		_ASSERT(0);
		return -2;					// 系统异常
	}

	return 0;
}


// 获取指定类型段的个数
//
// type		- IN - 段类型
//
// 返回值：0~n - 段的个数，-3 - 系统异常
//
// *注：
//
int pcsummary::getcnt(SEGTYPE type)
{
	switch (type)
	{
	case	SEGT_GOLABLENAME:		// 全局段
		return m_golsegcnt;
	case	SEGT_INSIDEPARANAME:	// 内参段
		return m_insegcnt;
	case	SEGT_COMMSEGNAME:		// 注释段
		return m_comsegcnt;
	case	SEGT_VALSEGNAME:		// 数据段
		return m_norsegcnt;
	default:
		_ASSERT(0);
		return -3;					// 系统异常
	}
}


// 获取指定类型段指定索引在预编译数据中的起始/结束地址
//
// type		- IN  - 段类型
// index	- IN  - 索引（0基）
// pb		- OUT - 返回在预编译数据中的起始地址
// pe		- OUT - 返回在预编译数据中的结束地址
//
// 返回值：0 - 成功获取， -3 系统异常
//
// *注：
//
int pcsummary::getaddr(SEGTYPE type, int index, LPBYTE* pb, LPBYTE* pe)
{
	_ASSERT(pb&&pe);

	*pb = NULL;
	*pe = NULL;

	switch (type)
	{
	case	SEGT_GOLABLENAME:		// 全局段
		if (index >= m_golsegcnt)
		{
			_ASSERT(0);
			return -3;
		}
		*pb = m_golsegaddr[index*2];
		*pe = m_golsegaddr[index*2+1];
		break;
	case	SEGT_INSIDEPARANAME:	// 内参段
		if (index >= m_insegcnt)
		{
			_ASSERT(0);
			return -3;
		}
		*pb = m_insegaddr[index*2];
		*pe = m_insegaddr[index*2+1];
		break;
	case	SEGT_COMMSEGNAME:		// 注释段
		if (index >= m_comsegcnt)
		{
			_ASSERT(0);
			return -3;
		}
		*pb = m_comsegaddr[index*2];
		*pe = m_comsegaddr[index*2+1];
		break;
	case	SEGT_VALSEGNAME:		// 数据段
		if (index >= m_norsegcnt)
		{
			_ASSERT(0);
			return -3;
		}
		*pb = m_norsegaddr[index*2];
		*pe = m_norsegaddr[index*2+1];
		break;
	default:
		_ASSERT(0);
		return -3;					// 系统异常
	}

	return 0;
}





////////////////////////////////////////////////////////////////////////
// INI类实现

inifile::inifile()
{
	m_filename[0] = _T('\0');
	m_flag = false;
	m_isregularity = false;

	m_first = 0;
	m_last  = 0;
	m_count = 0;
	m_cbf	= NULL;

	m_reg_segname	= NULL;
	m_reg_segnamecnt= 0;

	// 设置调试信息显示模式
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);

}


inifile::~inifile()
{
	destroy();
}


// 读入INI文件
//
// filename		- IN - 待读入的INI文件名称
//
// 返回值：读入并解析成功返回true，失败返回false。
//
// *注：
//
bool inifile::read(LPCTSTR filename)
{
	// 不能对已有数据的inifile类重复调用本函数
	_ASSERT(m_filename[0] == _T('\0'));
	_ASSERT(m_flag == false);

	/////////////////////////////////////////////////////////////////
	// 填写上下文信息
	TCHAR	fname[_MAX_FNAME+_MAX_EXT];
	TCHAR	ext[_MAX_EXT];

	// 分解出文件名称
	_tsplitpath_s(filename, NULL, 0, NULL, 0, fname, _MAX_FNAME, ext, _MAX_EXT);
	_tcscat_s(fname, _MAX_FNAME+_MAX_EXT, ext);

	m_ics.clear();
	m_ics.push(fname);


	/////////////////////////////////////////////////////////////////
	// 打开文件，并处理UNICODE编码标准

	FILE	*fp;

	_tcscpy_s(m_filename, MAX_PATH, filename);

#ifdef _UNICODE

	if (_tfopen_s(&fp, filename, _T("rb,ccs=UNICODE")))
	{
		m_ics.seterror(_ILEM_0);
		return false;
	}

	unsigned short unibom;

	// 读取UNICODE BOM
	if (fread((void*)&unibom, sizeof(UNICODE_BOM), 1, fp) == 0)
	{
		m_ics.seterror(_ILEM_2);
		return false;
	}

	// 判断INI文件编码，如果不是UNICODE编码，返回失败
	if (unibom != UNICODE_BOM)
	{
		fclose(fp);
		m_ics.seterror(_ILEM_3);
		return false;
	}
#else
	if (_tfopen_s(&fp, filename, _T("rb")))
	{
		m_ics.seterror(_ILEM_0);
		return false;
	}
#endif

	/////////////////////////////////////////////////////////////////
	// 原始文本预处理

	// 获取文件属性，用以判断其内容是否是规整数据
	DWORD	fileattr = GetFileAttributes(filename);

	if (fileattr & FILE_ATTRIBUTE_ARCHIVE)
		m_isregularity = false;		// 说明文件在上一次保存后被人修改过，不能保证其内容是规整数据
	else
		m_isregularity = true;		// 说明文件内容是规整数据


	// 获取文件尺寸（字节单位）
	long filelen = _filelength(_fileno(fp));

	if (filelen == -1L)
	{
		fclose(fp);
		m_ics.seterror(_ILEM_4);
		return false;
	}

#ifdef _UNICODE
	filelen -= sizeof(UNICODE_BOM);		// UNICODE编码文件头部的BOM不计算在文件尺寸之内
#endif

	// 申请缓冲区，用于存放原始文本。+sizeof(TCHAR)是留出结尾添加'\0'的位置
	unsigned char *porgtxt = new (std::nothrow) unsigned char[filelen+sizeof(TCHAR)];

	if (porgtxt == NULL)
	{
		fclose(fp);
		m_ics.seterror(_ILEM_1);
		return false;
	}

	// 读入原始文本（UNICODE编码将不包括头部的BOM标志）
	if (fread((void*)porgtxt, filelen, 1, fp) == 0)
	{
		fclose(fp);
		delete []porgtxt;
		m_ics.seterror(_ILEM_2);
		return false;
	}

	// 关闭原文件（文件内容已经读入porgtxt，不再需要文件句柄）
	fclose(fp);

	// 原始文本末尾添加0
#ifdef _UNICODE
	porgtxt[filelen+1] = 0;
	porgtxt[filelen]   = 0;
#else
	porgtxt[filelen]   = 0;
#endif

	// 申请缓冲区，用于存放预编译数据。预编译数据的尺寸定为原始文本的二倍，如果不足，可设为三倍。
	long			PBUFLEN	 = filelen*2;
	unsigned char *	ptidytxt = new (std::nothrow) unsigned char[PBUFLEN];

	if (ptidytxt == NULL)
	{
		delete []porgtxt;
		m_ics.seterror(_ILEM_1);
		return false;
	}

	_ASSERT(_CrtCheckMemory());

	/////////////////////////////////////////////////////////////////
	// 预编译
	pcsummary	pcsum;

	// 初始化摘要类
	if (pcsum.initialization() == -1)
	{
		m_ics.seterror(_ILEM_1);
		delete []porgtxt;
		delete []ptidytxt;
		return false;
	}

	// 预编译
	if (precompile((LPCTSTR)porgtxt, (LPCTSTR)&porgtxt[filelen], (LPTSTR)ptidytxt, (LPTSTR)&ptidytxt[PBUFLEN], &pcsum) != 0)
	{
		delete []porgtxt;
		delete []ptidytxt;
		return false;
	}

	// 释放原始文本缓冲区
	delete []porgtxt;

	_ASSERT(_CrtCheckMemory());


	/////////////////////////////////////////////////////////////////
	// 创建

	if (create(&pcsum) != 0)
	{
		delete []ptidytxt;
		return false;
	}

	delete []ptidytxt;

	_ASSERT(_CrtCheckMemory());

	m_ics.pop();

	// 设置读入成功标志
	m_flag = true;

	return true;
}


// 写INI文件
// 
// filename		- IN - 写入文件的名称
//
// 返回值：0 - 写入成功，-1 - 内存不足，-2 - 文件未能打开，-3 - 内部数据异常
//
// *注：入口参数filename可以设置为NULL，此时函数会将数据保存到读入的INI文件当中（覆盖）。
//
int inifile::write(LPCTSTR filename)
{
	FILE			*fp;
	int				rc;
	golableseg		*pgol = find_golable_seg();
	insideseg		*pinseg = find_inside_seg();
	segbaseclass	*ptmp = m_first;

	// 空类不能执行写入操作
	if (m_flag == false)
		return -3;

	// 读入成功必然会存在内参段，如果没有，说明内部数据异常，不能执行写入操作
	if ((pgol == NULL) || (pinseg == NULL))
		return -3;

	bool onlyread = *(bool*)pinseg->io(INIPARAMNAME[IPND_READONLY], NULL);

	// 判断INI是否允许写入操作
	if (onlyread == false)
	{
		if (filename == 0)
			filename = m_filename;

#ifdef _UNICODE
		_tfopen_s(&fp, filename, _T("wb+,ccs=UNICODE"));

		// 写入UNICODE BOM
		fwrite((const void *)&UNICODE_BOM, sizeof(UNICODE_BOM), 1, fp);
#else
		_tfopen_s(&fp, filename, _T("w+"));
#endif

		if (fp == NULL) return -2;

		//=================================================================
		// 段的存放顺序是：全局段、其它段。。。、内参段。全局段在文件头，内参段在文件尾。
		// *内参段放置在文件尾的原因是使段中的last_time值尽量贴近文件属性中的最后修改时间。

		// 优先写入全局段		
		if ((rc = pgol->write(fp, pinseg)) != 0)
		{
			fclose(fp);
			return rc;
		}

		// 写入其它段（不包括全局段和内参段）
		while (ptmp)
		{
			if ((ptmp != pgol) && (ptmp != pinseg))
			{
				rc = ptmp->write(fp, pinseg);

				if (rc != 0)
				{
					fclose(fp);
					return rc;
				}
			}

			ptmp = ptmp->m_next;
		}

		// 在文件尾部写入内参段
		if ((rc = pinseg->write(fp, pinseg)) != 0)
		{
			fclose(fp);
			return rc;
		}

		fclose(fp);

		// 取消文件的归档属性（表明该文件的内容是规整数据），用于以后再打开它时，判断文件是否被修改过
		::SetFileAttributes(filename, FILE_ATTRIBUTE_NORMAL);
	}

	return 0;
}

// 添加一个段类
//
// seg	- IN - 段对象地址
//
// *注：函数将一个段对象添加到段链表的尾部，并修正链表头尾指针及段个数值。
//
void inifile::append(segbaseclass *seg)
{
	if (m_count == 0)
	{
		_ASSERT(m_first == 0);
		m_first = seg; 
		m_last  = seg;
		seg->m_prev = 0;
		seg->m_next = 0;
	}
	else
	{
		m_last->m_next = seg;
		seg->m_prev = m_last;
		seg->m_next = 0;
		m_last = seg;
	}

	m_count++;
}


// 从链表中摘除一个段类
//
// seg	- IN - 段对象地址
//
// *注：函数将指定段对象从段链表中摘除，并修正段链表头尾指针及段个数值。
//		函数只是将段对象摘除，并不会释放。
//		调用者必须保证seg段对象是段链表中的一员。
//
void inifile::remove(segbaseclass *seg)
{
	if (m_first == seg)
		m_first = seg->m_next;
	if (m_last == seg)
		m_last  = seg->m_prev;
	if (seg->m_prev)
		seg->m_prev->m_next = seg->m_next;
	if (seg->m_next)
		seg->m_next->m_prev = seg->m_prev;

	m_count--;

	_ASSERT(m_count >= 0);
}


// 获取错误信息
//
// 返回值：返回错误信息字符串的地址
//
// *注：返回的字符串内存不需要调用者释放
//
LPCTSTR inifile::geterrmsg()
{
	return m_ics.get_errortxt();
}


// 预编译
//
// otbegin		- IN - 原始文本开始地址
// otend		- IN - 原始文本结束地址
// tidybegin	- IN - 预编译数据起始存放地址
// tidyend		- IN - 预编译数据结束存放地址
// pcsum		- IN - 预编译数据摘要类
// 
// 返回值：  0 - 成功
//			-1 - 失败，内存不足
//			-2 - 语法错误
//			-3 - 系统异常
//
// *注：
//
int inifile::precompile(LPCTSTR otbegin, LPCTSTR otend, LPTSTR tidybegin, LPTSTR tidyend, pcsummary* pcsum)
{
	LPCTSTR	psegbegin	= otbegin;					// 当前原始文本读取位置
	LPCTSTR	psegend		= NULL;						// 下一个段的段名行首地址
	LPCTSTR	pnext		= otbegin;					// 下一行原始文本读取位置
	LPTSTR	ptidycur	= (LPTSTR)tidybegin;		// 整理后文本的开始写入地址
	LPTSTR	ptidynext	= ptidycur;					// 整理后文本的下一个写入地址

	SEGTYPE segtp;									// 段类型
	int		rcode;									// 返回代码

	// 记录预编译数据起始地址
	pcsum->setbegin((LPBYTE)ptidycur);

	// 获取第一行文本的属性。如果是段名，则返回段类型。如果是文件头部的全局数据，则修改段类型为全局段类型
	switch (segtp = _gsp_get_segtype(psegbegin))
	{
	case	SEGT_EC_NOTMEM:
		m_ics.seterror(_ILEM_1);
		return -1;						// 内存不足
	case	SEGT_EC_INVALIDSEGNAME:
		segtp = SEGT_GOLABLENAME;
	case	SEGT_GOLABLENAME:			// 全局段
	case	SEGT_INSIDEPARANAME:		// 内参段
	case	SEGT_COMMSEGNAME:			// 注释段
	case	SEGT_VALSEGNAME:			// 数据段
		break;
	default:
		_ASSERT(0);
		m_ics.seterror(_ILEM_4);
		return -3;						// 系统异常
	}

	// 对原始文本进行预编译
	do
	{
		// 计算当前段的区间
		LPCTSTR	pnextline = _gsp_nextline(psegbegin);

		if (pnextline == NULL)
			psegend = otend;
		else
		{
			// 搜索下一个段的首地址
			switch (_gsp_find_nextseg(pnextline, &psegend))
			{
			case	-1:				// 内存分配失败
				m_ics.seterror(_ILEM_1);
				return -1;
			case	0:				// 之后没有段了
				psegend = otend;
				break;
			case	1:				// 成功找到下一个段
				break;
			}
		}

		// 预编译
		switch (segtp)
		{
		case	SEGT_GOLABLENAME:		// 全局段
			rcode = golableseg::tidy(psegbegin, psegend, &pnext, ptidycur, &ptidynext, &m_ics);
			break;
		case	SEGT_INSIDEPARANAME:	// 内参段
			rcode = insideseg::tidy(psegbegin, psegend, &pnext, ptidycur, &ptidynext, &m_ics);
			break;
		case	SEGT_COMMSEGNAME:		// 注释段
			rcode = commseg::tidy(psegbegin, psegend, &pnext, ptidycur, &ptidynext, &m_ics);
			break;
		case	SEGT_VALSEGNAME:		// 数据段
			rcode = norseg::tidy(psegbegin, psegend, &pnext, ptidycur, &ptidynext, &m_ics);
			break;
		case	SEGT_MAX:				// 程序设计错误
		default:
			_ASSERT(0);
			m_ics.seterror(_ILEM_4);
			return -3;
		};

		// 如果在预编译过程中出现错误，返回-2
		if (rcode != 0) return -2;

		// 记录摘要信息
		pcsum->add(segtp, (LPBYTE)ptidycur, (LPBYTE)ptidynext);

		// 更新原始文本读取地址和整理文本写入地址
		psegbegin	= psegend;
		ptidycur	= ptidynext;

		// 预测的段结尾应和实际的段结尾相同
		_ASSERT(psegend == pnext);

		// 解析下一个段名行
	} while (((segtp=_gsp_get_segtype(psegbegin)) != SEGT_EC_NOTMEM) && (segtp != SEGT_EC_INVALIDSEGNAME));

	// 检查是否出现溢出
	if (ptidycur > tidyend)
		return -1;
	// 检查内参段的个数
	if (pcsum->getcnt(SEGT_INSIDEPARANAME) > 1)
	{
		m_ics.seterror(_ILEM_36);
		return -2;
	}

	// 记录预编译数据结束地址
	pcsum->setend((LPBYTE)ptidycur);

	return 0;
}


// 创建
//
// pcsum	- IN - 预编译数据摘要类
// 
// 返回值：  0 - 成功
//			-1 - 失败，内存不足
//			-2 - 语法错误
//			-3 - 系统异常
//
// *注：
//
int inifile::create(pcsummary* pcsum)
{
	// 申请全局段对象
	golableseg	*pgolseg = new (std::nothrow) golableseg;

	if (pgolseg == 0) return -1;

	// 申请内参段对象
	insideseg	*pinsideseg = new (std::nothrow) insideseg(INIPARAMSEGNAME);

	if (pinsideseg == 0)
	{
		delete pgolseg;
		return -1;
	}

	LPBYTE	pbegin, pend, ptmp;
	int		retcode;

	// 优先处理内参段
	if (pcsum->getcnt(SEGT_INSIDEPARANAME))
	{
		_ASSERT(pcsum->getcnt(SEGT_INSIDEPARANAME) == 1);

		if (pcsum->getaddr(SEGT_INSIDEPARANAME, 0, &pbegin, &pend) != 0)
		{
			delete pinsideseg;
			delete pgolseg;
			return -3;
		}

		retcode = pinsideseg->create((LPCTSTR)pbegin, pinsideseg, (LPCTSTR*)&pend);

		if (retcode != 0)
		{
			delete pinsideseg;
			delete pgolseg;
			return -3;
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// 各个段在链表中的存放顺序是：全局段、内参段、其它段。。。

	// 优先加入全局段和内参段
	append(dynamic_cast<segbaseclass*>(pgolseg));
	append(dynamic_cast<segbaseclass*>(pinsideseg));

	// 获取预编译数据的起始、结束地址
	pbegin = pcsum->getbegin();
	pend   = pcsum->getend();

	LPPCMIB_HEAD	pcur = (LPPCMIB_HEAD)pbegin;
	BYTE			attr, addition;
	bool			golmk = false, insmk = false;
	segbaseclass*	pseg;

	// 顺序处理各个段
	while (reinterpret_cast<LPBYTE>(pcur) < pend)
	{
		if (_gsp_decomp_attributemark(pcur->mark, &attr, &addition) == false)
		{
			retcode = -3;
			break;
		}

		_ASSERT(attr == PCA_ATT_SEGNAME);

		switch (addition)
		{
		case	PCA_ADD_GOLSEG:				// 全局段
			golmk = true;

			retcode = pgolseg->create((LPCTSTR)pcur, pinsideseg, (LPCTSTR*)&pcur);

			break;
		case	PCA_ADD_NORSEG:				// 数据段
			pseg = dynamic_cast<segbaseclass*>(new (std::nothrow) norseg(pcur->name));
			
			if (pseg == NULL)
			{
				retcode = -1;
				break;
			}
			append(dynamic_cast<segbaseclass*>(pseg));
			retcode = pseg->create((LPCTSTR)pcur, pinsideseg, (LPCTSTR*)&pcur);

			break;
		case	PCA_ADD_INSIDESEG:			// 内参段
			insmk = true;

			if (pcsum->getaddr(SEGT_INSIDEPARANAME, 0, &ptmp, (LPBYTE*)&pcur) != 0)
			{
				retcode = -3;
				break;
			}
			retcode = 0;

			break;
		case	PCA_ADD_COMMSEG:			// 注释段
			pseg = dynamic_cast<segbaseclass*>(new (std::nothrow) commseg(pcur->name));
			
			if (pseg == NULL)
			{
				retcode = -1;
				break;
			}
			append(dynamic_cast<segbaseclass*>(pseg));
			retcode = pseg->create((LPCTSTR)pcur, pinsideseg, (LPCTSTR*)&pcur);

			break;
		default:
			_ASSERT(0);
			retcode = -3;
			break;
		}

		if (retcode != 0) break;
	}

	if (reinterpret_cast<LPBYTE>(pcur) < pend)
	{
		_ASSERT(retcode != 0);
		destroy();
		return retcode;
	}

	return 0;
}


// 销毁INI中的内容
void inifile::destroy()
{
	segbaseclass*	p;

	for (int i=0; i<m_count; i++)
	{
		p = m_first;
		m_first = m_first->m_next;
		_ASSERT(p);
		delete p;
	}

	m_reg_segname	= NULL;
	m_reg_segnamecnt= 0;

	_ASSERT(m_first == NULL);

	m_first = NULL;
	m_last  = NULL;
	m_count = 0;

	m_flag	= false;
	m_filename[0] = CT_NULL;
}


// 获取内参段指针
//
// 返回值：返回内参段地址，如果没有找到内参段，返回NULL。
//
// *注：内参段只会有一个。
//
insideseg* inifile::find_inside_seg()
{
	segbaseclass*	p = m_first;

	while (p)
	{
		if (p->type() == SEGT_INSIDEPARANAME)
			return dynamic_cast<insideseg*>(p);
		else
			p = p->m_next;
	}

	return NULL;
}


// 获取全局段指针
//
// 返回值：返回全局段地址
//
// *注：全局段只会有一个
//
golableseg* inifile::find_golable_seg()
{
	segbaseclass*	p = m_first;

	while (p)
	{
		if (p->type() == SEGT_GOLABLENAME)
			return dynamic_cast<golableseg*>(p);
		else
			p = p->m_next;
	}

	return NULL;
}


// 获取指定名称数据段的地址
//
// segname	- IN - 指定的段名称
//
// 返回值：返回指定名称数据段的地址，如果没有找到，则返回NULL。
//
// *注：
//
norseg* inifile::find_data_seg(LPCTSTR segname)
{
	segbaseclass*	p = m_first;

	while (p)
	{
		if ((p->type() == SEGT_VALSEGNAME)&&(_tcscmp(p->name(), segname) == 0))
			return dynamic_cast<norseg*>(p);
		else
			p = p->m_next;
	}

	return NULL;
}


// 获取数据
//
// segname	- IN - 数据所在段的段名字
// varname	- IN - 数据名字
//
// 返回值：返回数据存放地址，如果没有找到指定的数据，函数返回NULL。
//
// *注：全局段没有名字，所以在获取全局段中的数据时，segname参数需要设置为NULL。
//
void* inifile::io(LPCTSTR segname, LPCTSTR varname)
{
	segbaseclass*	p = m_first;

	// 搜索全局段。（全局段没有段名字，所以需要特殊处理）
	if (segname == NULL)
	{
		while (p)
		{
			if (p->type() == SEGT_GOLABLENAME)		// 搜索全局段（全局段只有一个）
				break;
			else
				p = p->m_next;
		}

		if (p == NULL)
		{
			// 全局段是必然有的，如果此处没有，说明系统异常
			if (m_cbf)
				return m_cbf(ILIOCBFM_ERR_EXCEP, NULL, NULL);
			else
				return NULL;
		}

		return p->io(varname, m_cbf);
	}

	// 搜索其它段
	for (int i=0; i<m_count; i++, p=p->m_next)
	{
		if (_tcscmp(p->name(), segname) == 0)
		{
			return p->io(varname, m_cbf);
		}
	}

	if (m_cbf)
		return m_cbf(ILIOCBFM_MISS_SEG, segname, NULL);	// 回调错误处理函数
	else
		return NULL;
}


// 添加一个数据段
//
// segname	- IN - 新段的名字
//
// 返回值：
//		0	- 添加新段成功
//		-1	- 内存不足
//		-2	- 段名称不合法
//		-3	- 已存在同名段
//
// *注：函数只能添加数据段，其它类型的段不能添加。新添加的段会被放置在段链表尾部。新段对象是空的。
//		不能添加全局段
//
int inifile::addseg(LPCTSTR segname)
{
	// 判断段名称是否合法
	if ((segname == NULL)||(segname[0] == CT_NULL)||(segname[0] == CT_SEG_INSD)||(segname[0] == CT_SEG_COMM))
		return -2;
		
	// 判断是否存在同名的数据段
	if (find_data_seg(segname) != NULL)
		return -3;

	// 申请段对象
	norseg*	p = new (std::nothrow) norseg(segname);

	if (p == NULL) return -1;

	// 将段对象加入链表尾部
	append(p);

	return 0;
}


// 删除一个数据段
//
// segname	- IN - 待删除段的名字
//
// 返回值：
//		0	- 删除段成功
//		-2	- 指定名称的段不存在
//
// *注：全局段不能删除
//
int inifile::delseg(LPCTSTR segname)
{
	// 搜索段对象地址
	norseg* p = find_data_seg(segname);

	// 如果不存在指定名称的数据段，返回-2
	if (p == NULL) return -2;

	// 将段对象从段链表中摘除
	remove(p);

	// 销毁段
	delete p;

	return 0;
}


// 向指定数据段中添加一个变量
// 
// segname	- IN - 段名称
// varname	- IN - 变量名称
// dtp		- IN - 变量类型
//
// 返回值：0 - 成功，-1 - 内存不足，-2 - 没有指定的段，-3 - 已经存在指定名称的变量
//
// *注：全局段属于数据段的一种
//
int inifile::addvar(LPCTSTR segname, LPCTSTR varname, DATATYPE dtp, ...)
{
	va_list		ap;

	segbaseclass* ps;

	// 搜索段地址
	if (segname == NULL)
		ps = (segbaseclass*)find_golable_seg();
	else
		ps = (segbaseclass*)find_data_seg(segname);

	// 如果不存在指定名称的数据段，返回-2
	if (ps == NULL) return -2;
	// 如果已经存在指定名称的变量，返回-3
	if (ps->io(varname, NULL)) return -3;

	varbaseclass* pv = NULL;

	// 申请变量对象
	switch (dtp)
	{
	case	NTP_BYTE:				// byte数据
	case	NTP_ARRAY_BYTE:			// byte[]数组数据
		pv = dynamic_cast<varbaseclass*>(new (std::nothrow) varbyte);
		break;
	case	NTP_INT:				// int数据
	case	NTP_ARRAY_INT:			// int[]数组数据
		pv = dynamic_cast<varbaseclass*>(new (std::nothrow) varint);
		break;
	case	NTP_BOOL:				// bool数据
	case	NTP_ARRAY_BOOL:			// bool[]数组数据
		pv = dynamic_cast<varbaseclass*>(new (std::nothrow) varbool);
		break;
	case	NTP_DOUBLE:				// double数据
	case	NTP_ARRAY_DOUBLE:		// double[]数组数据
		pv = dynamic_cast<varbaseclass*>(new (std::nothrow) vardouble);
		break;
	case	NTP_STRING:				// string数据
	case	NTP_ARRAY_STRING:		// string[]数组数据
		pv = dynamic_cast<varbaseclass*>(new (std::nothrow) varstring);
		break;
	case	NTP_DATE:				// date数据
	case	NTP_ARRAY_DATE:			// date[]数组数据
		pv = dynamic_cast<varbaseclass*>(new (std::nothrow) vardate);
		break;
	case	NTP_STRUCT:				// struct数据
	case	NTP_ARRAY_STRUCT:		// struct[]数组数据
		pv = dynamic_cast<varbaseclass*>(new (std::nothrow) varstruct);
		break;
	default:
		_ASSERT(0);
		pv = NULL;
		break;
	}

	// 内存不足
	if (pv == NULL) return -1;

	int		rc = 0;
	va_start(ap, dtp);

	switch (dtp)
	{
	case	NTP_BYTE:				// byte数据
		{
			BYTE	b1 = va_arg(ap,BYTE);	// value
			BYTE	b2 = va_arg(ap,BYTE);	// scale
			rc = dynamic_cast<varbyte*>(pv)->build(varname, b1, b2);
		}
		break;
	case	NTP_ARRAY_BYTE:			// byte[]数组数据
		{
			int		i1 = va_arg(ap,int);	// elecnt
			BYTE	b1 = va_arg(ap,BYTE);	// scale
			rc = dynamic_cast<varbyte*>(pv)->buildarray(varname, i1, b1);
		}
		break;
	case	NTP_INT:				// int数据
		{
			int		i1 = va_arg(ap,int);	// value
			BYTE	b1 = va_arg(ap,BYTE);	// scale
			rc = dynamic_cast<varint*>(pv)->build(varname, i1, b1);
		}
		break;
	case	NTP_ARRAY_INT:			// int[]数组数据
		{
			int		i1 = va_arg(ap,int);	// elecnt
			BYTE	b1 = va_arg(ap,BYTE);	// scale
			rc = dynamic_cast<varint*>(pv)->buildarray(varname, i1, b1);
		}
		break;
	case	NTP_BOOL:				// bool数据
		{
			BYTE	b1 = va_arg(ap,BYTE);	// value
			BYTE	b2 = va_arg(ap,BYTE);	// index
			rc = dynamic_cast<varbool*>(pv)->build(varname, b1, b2);
		}
		break;
	case	NTP_ARRAY_BOOL:			// bool[]数组数据
		{
			int		i1 = va_arg(ap,int);	// elecnt
			rc = dynamic_cast<varbool*>(pv)->buildarray(varname, i1);
		}
		break;
	case	NTP_DOUBLE:				// double数据
		{
			double	d1 = va_arg(ap,double);	// value
			rc = dynamic_cast<vardouble*>(pv)->build(varname, d1);
		}
		break;
	case	NTP_ARRAY_DOUBLE:		// double[]数组数据
		{
			int		i1 = va_arg(ap,int);	// elecnt
			rc = dynamic_cast<vardouble*>(pv)->buildarray(varname, i1);
		}
		break;
	case	NTP_STRING:				// string数据
		{
			LPCTSTR	s1 = va_arg(ap,LPCTSTR);
			rc = dynamic_cast<varstring*>(pv)->build(varname, s1);
		}
		break;
	case	NTP_ARRAY_STRING:		// string[]数组数据
		{
			int		i1 = va_arg(ap,int);	// elecnt
			rc = dynamic_cast<varstring*>(pv)->buildarray(varname, i1);
		}
		break;
	case	NTP_DATE:				// date数据
		{
			__int64	i1 = va_arg(ap,__int64);// value
			BYTE	b1 = va_arg(ap,BYTE);	// adtime
			rc = dynamic_cast<vardate*>(pv)->build(varname, i1, b1);
		}
		break;
	case	NTP_ARRAY_DATE:			// date[]数组数据
		{
			int		i1 = va_arg(ap,int);	// elecnt
			BYTE	b1 = va_arg(ap,BYTE);	// adtime
			rc = dynamic_cast<vardate*>(pv)->buildarray(varname, i1, b1);
		}
		break;
	case	NTP_STRUCT:				// struct数据
		{
			LPILVT_STRUCT	s1 = va_arg(ap,LPILVT_STRUCT);
			rc = dynamic_cast<varstruct*>(pv)->build(varname, s1);
		}
		break;
	case	NTP_ARRAY_STRUCT:		// struct[]数组数据
		{
			ILSUTMEMTPE	i1 = va_arg(ap,ILSUTMEMTPE);	// type
			void*		p1 = va_arg(ap,void*);			// value
			rc = dynamic_cast<varstruct*>(pv)->buildarray(varname, i1, p1);
		}
		break;
	}

	va_end(ap);

	if (rc != 0)
	{
		delete pv;
		return -1;
	}

	// 将变量加入指定段变量链表中
	ps->append(pv);

	return 0;
}


// 删除指定数据段中的一个变量
//
// segname	- IN - 数据段的名称
// varname	- IN - 待删除变量的名称
//
// 返回值：0 - 删除成功，-2 - 指定的段不存在，-3 - 指定的变量不存在
//
// *注：全局段属于数据段的一种
//
int inifile::delvar(LPCTSTR segname, LPCTSTR varname)
{
	segbaseclass* ps;

	// 搜索段地址
	if (segname == NULL)
		ps = (segbaseclass*)find_golable_seg();
	else
		ps = (segbaseclass*)find_data_seg(segname);

	// 如果不存在指定名称的数据段，返回-2
	if (ps == NULL) return -2;

	// 搜索指定名称的变量
	varbaseclass* pv = ps->find_varobj(varname);

	// 指定的变量不存在
	if (pv == NULL) return -3;

	// 摘除并释放
	ps->remove(pv);
	delete pv;
	
	return 0;
}


// 枚举数据段
//
// prev_norseg	- IN - 前一个数据段的类地址，如果该参数填写NULL，则函数将返回INI中第一个数据段的类地址。
//
// 返回值：函数返回prev_norseg指向的数据段的后一个数据段，如果没有后续数据段，函数返回NULL。
//
// *注：函数不会返回全局段的类地址。更不会返回内参段、注释段的类地址。
//
norseg* inifile::enum_norseg(norseg* prev_norseg)
{
	segbaseclass*	p;

	if (prev_norseg == NULL)
		p = m_first;
	else
		p = prev_norseg->m_next;

	while (p)
	{
		if (p->type() == SEGT_VALSEGNAME)		// 判断是否是数据段
			break;
		else
			p = p->m_next;
	}

	if (p == NULL)
		return NULL;
	else
		return dynamic_cast<norseg*>(p);
}


// 设置IO回调函数
//
// newcbf	- IN - 新的IO回调函数
//
// 返回值：以前的回调函数
//
// *注：
//
ILIO_CALLBACK_FUN inifile::setcallback(ILIO_CALLBACK_FUN newcbf)
{
	ILIO_CALLBACK_FUN oldcbf = m_cbf;

	_ASSERT(newcbf);
	m_cbf = newcbf;

	return oldcbf;
}


// 注册段名和数据名
//
// psegname		- IN - 注册的段名（字符串数组指针）
// segnamecnt	- IN - 注册的段名个数
// pdataname	- IN - 注册的数据名（字符串数组指针）
// datanamecnt	- IN - 注册的数据名个数
//
// 返回值：无
//
// *注：
// 
void inifile::regname(LPCTSTR* psegname, int segnamecnt, LPCTSTR* pdataname, int datanamecnt)
{
	_ASSERT(segnamecnt <= 65535);

	m_reg_segname		= psegname;
	m_reg_segnamecnt	= segnamecnt;

	segbaseclass*	p	= m_first;

	while (p)
	{
		p->regname(pdataname, datanamecnt);		// 注册数据名
		p = p->m_next;
	}
}


// 重载[]操作符
//
// i	- IN - 已注册的段名字符串的索引
//
// 返回值：返回对应段的地址
//
// *注：数组下标方式只对全局段和数据段中的变量IO有效。
//
segbaseclass& inifile::operator[](int i)
{
	_ASSERT((i >= 0)&&(i < m_reg_segnamecnt));	// 判断段名索引的有效性

	segbaseclass*	ps;

	LPCTSTR	pname = m_reg_segname[i];

	if (pname == NULL)
		ps = dynamic_cast<segbaseclass*>(find_golable_seg());
	else
		ps = dynamic_cast<segbaseclass*>(find_data_seg(pname));

	return *ps;
}

