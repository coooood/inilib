//
// inilib.h - 库声明文件
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


#ifndef __INILIB_H__
#define __INILIB_H__

#ifndef _WINDOWS_
#include <windows.h>
#endif


const int  MAX_SEGNAME_LENGHT = 255;			// 段名字的最大长度（字符单位，不要小于255）
const int  MAX_VARNAME_LENGTH = 255;			// 变量名的最大长度（字符单位，不要小于255）


const TCHAR INIPARAMSEGNAME[] = _T("!INSIDE");	// INI内部参数段段名称


// 段属性定义
typedef enum
{
	SEGT_GOLABLENAME,		// 全局段
	SEGT_INSIDEPARANAME,	// 内参段
	SEGT_COMMSEGNAME,		// 注释段
	SEGT_VALSEGNAME,		// 数据段

	SEGT_MAX,				// 段属性个数
	SEGT_BOUNDARY,			// 边界

	SEGT_EC_NOTMEM,			// 错误代码：内存分配失败
	SEGT_EC_INVALIDSEGNAME	// 错误代码：无效的段名

} SEGTYPE, *LPSEGTYPE;

// 数据类型定义
typedef enum
{
	NTP_BYTE,				// byte数据
	NTP_INT,				// int数据
	NTP_BOOL,				// bool数据
	NTP_DOUBLE,				// double数据
	NTP_STRING,				// string数据
	NTP_DATE,				// date数据
	NTP_STRUCT,				// struct数据
	NTP_ARRAY_BYTE,			// byte[]数组数据
	NTP_ARRAY_INT,			// int[]数组数据
	NTP_ARRAY_BOOL,			// bool[]数组数据
	NTP_ARRAY_DOUBLE,		// double[]数组数据
	NTP_ARRAY_STRING,		// string[]数组数据
	NTP_ARRAY_DATE,			// date[]数组数据
	NTP_ARRAY_STRUCT,		// struct[]数组数据

	NTP_MAX,				// 数据类型个数

	NTP_VOID_LINE,			// 空行数据
	NTP_COMM_LINE,			// 注释数据

	NTP_SEGNAME_LINE,		// 段名数据

	NTP_BOUNDARY,			// 边界

	NTP_EC_NOTMEM,			// 错误代码：内存分配失败
	NTP_EC_SYNERR			// 错误代码：语法错误

} DATATYPE, *LPDATATYPE;




/////////////////////////////////////////////////////////////////////////////
// 预编译格式中使用的结构

#pragma pack(push, 1)

// 信息块块首结构 - [标志4][名字长度4][名字]
typedef struct
{
	DWORD	mark;					// 信息块类型标识
	int		namelen;				// 信息块名字长度（字符单位，不包含结尾的NULL）
	TCHAR	name[1];				// 信息块名称字符串（结尾有NULL字符）
} PCMIB_HEAD, *LPPCMIB_HEAD;

// byte 型变量数据部分结构
typedef struct
{
	BYTE	scale;					// [进制值]
	BYTE	value;					// [变量值]
} PCMIB_VT_BYTE, *LPPCMIB_VT_BYTE;

// byte 型数组变量数据部分结构
typedef struct
{
	BYTE	scale;					// [进制值]
	int		elecnt;					// [元素个数]
	BYTE	value[1];				// [变量值数组]...
} PCMIB_VT_BYTEARRAY, *LPPCMIB_VT_BYTEARRAY;

// int 型变量数据部分结构
typedef struct
{
	BYTE	scale;					// [进制值]
	int		value;					// [变量值]
} PCMIB_VT_INT, *LPPCMIB_VT_INT;

// int 型数组变量数据部分结构
typedef struct
{
	BYTE	scale;					// [进制值]
	int		elecnt;					// [元素个数]
	int		value[1];				// [变量值数组]...
} PCMIB_VT_INTARRAY, *LPPCMIB_VT_INTARRAY;

// bool 型变量数据部分结构
typedef struct
{
	BYTE	index;					// [描述索引值]
	BYTE	value;					// [变量值]
} PCMIB_VT_BOOL, *LPPCMIB_VT_BOOL;

// bool 型数组变量数据部分结构
typedef struct
{
	int				elecnt;			// [元素个数]
	PCMIB_VT_BOOL	value[1];		// [PCMIB_VT_BOOL]...
} PCMIB_VT_BOOLARRAY, *LPPCMIB_VT_BOOLARRAY;

// double 型变量数据部分结构
typedef struct
{
	double	value;					// [变量值]
} PCMIB_VT_DOUBLE, *LPPCMIB_VT_DOUBLE;

// double 型数组变量数据部分结构
typedef struct
{
	int		elecnt;					// [元素个数]
	double	value[1];				// [变量值数组]...
} PCMIB_VT_DOUBLEARRAY, *LPPCMIB_VT_DOUBLEARRAY;

// string 型变量数据部分结构
typedef struct
{
	int		len;					// [变量值长度]
	TCHAR	value[1];				// [变量值]
} PCMIB_VT_STRING, *LPPCMIB_VT_STRING;

// string 型数组变量数据部分结构
typedef struct
{
	int				elecnt;			// [元素个数]
	PCMIB_VT_STRING	value[1];		// [PCMIB_VT_STRING]...
} PCMIB_VT_STRINGARRAY, *LPPCMIB_VT_STRINGARRAY;

// date 型变量数据部分结构
typedef struct
{
	BYTE	adtime;					// [时间标志值]
	__int64	value;					// [变量值]
} PCMIB_VT_DATE, *LPPCMIB_VT_DATE;

// date 型数组变量数据部分结构
typedef struct
{
	BYTE	adtime;					// [时间标志值]
	int		elecnt;					// [元素个数]
	__int64	value[1];				// [变量值数组]...
} PCMIB_VT_DATEARRAY, *LPPCMIB_VT_DATEARRAY;

#pragma pack(pop)



/////////////////////////////////////////////////////////////////////////////
// 各数据类型面对调用者的返回结构


// 用于计算字节对齐值的测试用结构
typedef struct
{
	char	_begin;
	double	_end;
} SUTSIZECAL;

// 单行注释
typedef struct
{
	int		len;					// [注释长度]
	LPTSTR	pvalue;					// [注释]
} ILVT_COMM, *LPILVT_COMM;

// 变量存储结构

// byte
typedef struct
{
	BYTE	scale;					// [进制值]
	BYTE	value;					// [变量值]
} ILVT_BYTE, *LPILVT_BYTE;

// byte 型数组变量数据部分结构
typedef struct
{
	BYTE	scale;					// [进制值]
	int		elecnt;					// [元素个数]
	LPBYTE	pvalue;					// [变量值数组]...
} ILVT_BYTEARRAY, *LPILVT_BYTEARRAY;

// int 型变量数据部分结构
typedef struct
{
	BYTE	scale;					// [进制值]
	int		value;					// [变量值]
} ILVT_INT, *LPILVT_INT;

// int 型数组变量数据部分结构
typedef struct
{
	BYTE	scale;					// [进制值]
	int		elecnt;					// [元素个数]
	int*	pvalue;					// [变量值数组]...
} ILVT_INTARRAY, *LPILVT_INTARRAY;

// bool 型变量数据部分结构
typedef struct
{
	BYTE	index;					// [描述索引值]
	BYTE	value;					// [变量值]
} ILVT_BOOL, *LPILVT_BOOL;

// bool 型数组变量数据部分结构
typedef struct
{
	int				elecnt;			// [元素个数]
	LPILVT_BOOL		pvalue;			// [PCMIB_VT_BOOL]...
} ILVT_BOOLARRAY, *LPILVT_BOOLARRAY;

// double 型变量数据部分结构
typedef struct
{
	double	value;					// [变量值]
} ILVT_DOUBLE, *LPILVT_DOUBLE;

// double 型数组变量数据部分结构
typedef struct
{
	int		elecnt;					// [元素个数]
	double*	pvalue;					// [变量值数组]...
} ILVT_DOUBLEARRAY, *LPILVT_DOUBLEARRAY;

// string 型变量数据部分结构
typedef struct
{
	int		len;					// [变量值长度]
	LPTSTR	pvalue;					// [变量值]
} ILVT_STRING, *LPILVT_STRING;

// string 型数组变量数据部分结构
typedef struct
{
	int				elecnt;			// [元素个数]
	LPILVT_STRING	pvalue;			// 
} ILVT_STRINGARRAY, *LPILVT_STRINGARRAY;

// date 型变量数据部分结构
typedef struct
{
	BYTE	adtime;					// [时间标志值]
	__int64	value;					// [变量值]
} ILVT_DATE, *LPILVT_DATE;

// date 型数组变量数据部分结构
typedef struct
{
	BYTE	adtime;					// [时间标志值]
	int		elecnt;					// [元素个数]
	__int64*	pvalue;				// [变量值数组]...
} ILVT_DATEARRAY, *LPILVT_DATEARRAY;


// *注：以下三个结构定义属于数据嵌套型结构，不能改变重叠部分的位序

// struct 型变量数据结构
typedef struct
{
	int		memtype;				// 结构在内存中的存放形式(见ILSUTMEMTPE含义)
	int		sutmemcnt;				// 结构中成员的个数
	LPBYTE	vartable;				// 变量表
	int*	varoffset;				// 各变量在结构中的偏移
	int		sutsize;				// 一个结构体在内存中会占用的字节数
	LPBYTE	pvalue;					// 变量值
} ILVT_STRUCT, *LPILVT_STRUCT;

// struct 型数组变量数据结构(Array形式)
typedef struct
{
	int		memtype;				// 结构在内存中的存放形式
	int		sutmemcnt;				// 结构中成员的个数
	LPBYTE	vartable;				// 变量表
	int*	varoffset;				// 各变量在结构中的偏移
	int		sutsize;				// 一个结构体在内存中会占用的字节数
	LPBYTE	pvalue;					// 变量值
	int		elecnt;					// 元素个数
} ILVT_STRUCTARRAY, *LPILVT_STRUCTARRAY;

// struct 型数组变量数据结构(List形式)
typedef struct
{
	int		memtype;				// 结构在内存中的存放形式
	int		sutmemcnt;				// 结构中成员的个数
	LPBYTE	vartable;				// 变量表
	int*	varoffset;				// 各变量在结构中的偏移
	int		sutsize;				// 一个结构体在内存中会占用的字节数
	void*	first;					// 链表第一个元素的地址
	int		elecnt;					// 元素个数
	void*	last;					// 链表最后一个元素的地址
} ILVT_STRUCTLIST, *LPILVT_STRUCTLIST;



/////////////////////////////////////////////////////////////////////////////


#pragma pack(push, 1)

typedef struct
{
	BYTE	index;					// 描述索引值
	BYTE	value;					// 变量值
} booln;

#pragma pack(pop)

// BOOL描述串对
typedef struct
{
	LPTSTR		oo[2];
} BOOLSTRSTRUCT, *LPBOOLSTRSTRUCT;

// BOOL型变量的文字描述种类数（可以添加自定义的布尔描述对）
const int  MAX_BOOLDES_TYPE = 7;

// BOOL型变量的文字描述种类（共MAX_BOOLDES_TYPE种）
const BOOLSTRSTRUCT	BOOLDES[MAX_BOOLDES_TYPE] = {
												{_T("false"), _T("true")}, 
												{_T("0"),     _T("1")},
												{_T("no"),    _T("yes")},
												{_T("关"),    _T("开")},
												{_T("不能"),  _T("能")},
												{_T("关闭"),  _T("开启")},
												{_T("否"),    _T("是")}};

// 结构数组在内存中的存放形式
typedef enum
{
	ILSUTMEMTPE_ARRAY = 0,				// 数组形式(默认)
	ILSUTMEMTPE_LIST  = 1				// 双向链表形式
} ILSUTMEMTPE, *LPILSUTMEMTPE;


// date类型的INI文本表现形式。
const BYTE DATETYPE_DATE		= 0;	// 只有日期
const BYTE DATETYPE_DATE_TIME	= 1;	// 日期和时间


// 回调函数错误类型
typedef enum
{
	ILIOCBFM_MISS_SEG = 0x1,			// 缺失段
	ILIOCBFM_MISS_VAR = 0x2,			// 缺失变量

	ILIOCBFM_ERR_EXCEP	= 0x4,			// 库内部异常
	ILIOCBFM_ERR_MEMORY	= 0x8,			// 内存不足

} ILIOCBFM_MISSING, *LPILIOCBFM_MISSING;

// IO回调函数指针
typedef void* (*ILIO_CALLBACK_FUN)(ILIOCBFM_MISSING miss, LPCTSTR segname, LPCTSTR varname);



class segbaseclass;
class insideseg;


////////////////////////////////////////////////////////////////////////
// INI库上下文信息类

// 上下文最大深度
const int	_ICCMAX_DPE = 8;
// 每层堆栈缓冲区的尺寸
const int	_ICCBUFSIZE = 512;


class _inicontextclass
{
public:
	// 获取错误信息
	LPCTSTR get_errortxt(void){ return m_buffer; };

public:
	// 压入上下文环境描述文本
	void push(LPCTSTR info);
	// 弹出上下文环境描述文本
	void pop(void);
	// 清空堆栈
	void clear(void){m_cur=0; m_buffer[0]=NULL;};

	// 设置并合成错误信息
	void seterror(LPCTSTR errtext, ...);

public:
	_inicontextclass(){m_cur=0; m_buffer[0]=NULL;};
	~_inicontextclass(){;};

private:
	int		m_cur;										// 栈指针
	TCHAR	m_data[_ICCMAX_DPE][_ICCBUFSIZE];			// 上下文环境描述文本
	TCHAR	m_buffer[_ICCMAX_DPE*_ICCBUFSIZE];			// 错误信息合成缓冲区
};




/////////////////////////////////////////////////////////////////////////////
// 变量类定义

// 变量基类
class varbaseclass
{
public:
	varbaseclass();
	virtual ~varbaseclass() = 0;

	DATATYPE type(){ return m_type; };				// 获取变量类型
	LPCTSTR  name(){ return m_name; };				// 获取变量名
	void*    io(){ return (void*)&m_data; };		// 获取数据

	// 创建对象
	virtual int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend) = 0;
	// 销毁对象
	virtual void destroy(void) = 0;
	// 将对象写入文件
	virtual int write(FILE *fp, insideseg* inseg) = 0;

	// 整理变量声明部分
	static DATATYPE tidy_vardec(LPCTSTR pvardectxt, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);
	// 整理变量数据部分
	static int tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);

protected:
	static const int MAXVALSIZE = 8;				// 变量数据缓冲区尺寸

	typedef union
	{
		size_t				m_buffer[MAXVALSIZE];	// 占位符（在WIN32下，变量占用32字节。WIN64下将占用64字节）
		
		ILVT_BYTE			m_byte;					// byte 型变量数据部分结构
		ILVT_BYTEARRAY		m_bytearray;			// byte 型数组变量数据部分结构
		ILVT_INT			m_int;					// int 型变量数据部分结构
		ILVT_INTARRAY		m_intarray;				// int 型数组变量数据部分结构
		ILVT_BOOL			m_bool;					// bool 型变量数据部分结构
		ILVT_BOOLARRAY		m_boolarray;			// bool 型数组变量数据部分结构
		ILVT_DOUBLE			m_double;				// double 型变量数据部分结构
		ILVT_DOUBLEARRAY	m_doublearray;			// double 型数组变量数据部分结构
		ILVT_STRING			m_string;				// string 型变量数据部分结构
		ILVT_STRINGARRAY	m_stringarray;			// string 型数组变量数据部分结构
		ILVT_DATE			m_date;					// date 型变量数据部分结构
		ILVT_DATEARRAY		m_datearray;			// date 型数组变量数据部分结构
		ILVT_STRUCT			m_struct;				// struct 型变量数据结构
		ILVT_STRUCTARRAY	m_structarray;			// struct 型数组变量数据结构(Array)
		ILVT_STRUCTLIST		m_structlist;			// struct 型数组变量数据结构(List)
		
		ILVT_COMM			m_linecomm;				// 单行注释数据结构
		LPBYTE				m_void;					// X

	} DATAUNIONDEF;


	DATATYPE		m_type;							// 变量类型
	TCHAR			m_name[MAX_VARNAME_LENGTH+1];	// 变量的名字
	DATAUNIONDEF	m_data;							// 变量数据     

public:
	segbaseclass	*m_pseg;						// 归属段
	varbaseclass	*m_prev;						// 前一个变量类地址
	varbaseclass	*m_next;						// 后一个变量类地址
};


// 空行类
class linevoid : public varbaseclass
{
public:
	linevoid(){};
	~linevoid();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	static void tidy(LPTSTR tidytxtbegin, LPTSTR *tidytxtend);
};

// 注释行（单行）类
class linecomm : public varbaseclass
{
public:
	linecomm(){};
	~linecomm();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	static void tidy(LPCTSTR str, LPTSTR tidytxtbegin, LPTSTR *tidytxtend);
};

// BYTE变量类
class varbyte : public varbaseclass
{
public:
	varbyte(){};
	~varbyte();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	int build(LPCTSTR name, BYTE value, BYTE scale);
	int buildarray(LPCTSTR name, int elecnt, BYTE scale);

	static int tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);
};

// int变量类
class varint : public varbaseclass
{
public:
	varint(){};
	~varint();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	int build(LPCTSTR name, int value, BYTE scale);
	int buildarray(LPCTSTR name, int elecnt, BYTE scale);

	static int tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);
};

// bool变量类
class varbool : public varbaseclass
{
public:
	varbool(){};
	~varbool();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	int build(LPCTSTR name, BYTE value, BYTE index);
	int buildarray(LPCTSTR name, int elecnt);

	static int tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);
};

// double变量类
class vardouble : public varbaseclass
{
public:
	vardouble(){};
	~vardouble();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	int build(LPCTSTR name, double value);
	int buildarray(LPCTSTR name, int elecnt);

	static int tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);
};

// string变量类
class varstring : public varbaseclass
{
public:
	varstring(){};
	~varstring();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	int build(LPCTSTR name, LPCTSTR str);
	int buildarray(LPCTSTR name, int elecnt);

	static int tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);
};

// date变量类
class vardate : public varbaseclass
{
public:
	vardate(){};
	~vardate();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	int build(LPCTSTR name, __int64 value, BYTE adtime);
	int buildarray(LPCTSTR name, int elecnt, BYTE adtime);

	static int tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);
};

// struct变量类
class varstruct : public varbaseclass
{
public:
	varstruct(){};
	~varstruct();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	int build(LPCTSTR name, LPILVT_STRUCT sut);
	int buildarray(LPCTSTR name, ILSUTMEMTPE type, void* psuta);

	static int tidy_data(LPCTSTR pvarbegin, LPCTSTR pvarend, LPCTSTR pvartab, bool isarray, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);

private:
	int compvardec(LPTSTR buffer, int bufsize, insideseg* inseg);						// 合成变量声明行文本
	int writestruct(LPTSTR buffer, int bufsize, FILE *fp, int index, insideseg* inseg);	// 向文件写入单个结构数据

};





///////////////////////////////////////////////////////////////////////////////
// 段类定义


// 段基类
class segbaseclass
{
public:
	segbaseclass(SEGTYPE segtype, LPCTSTR segname);
	virtual ~segbaseclass() = 0;

	SEGTYPE type(){ return m_segtype; };								// 获取段类型
	LPCTSTR name(){ return m_segname; };								// 获取段名字
	virtual void* io(LPCTSTR varname, ILIO_CALLBACK_FUN cbf = NULL);	// 获取段中指定变量的数据存放地址

	void regname(LPCTSTR* pdataname, int datanamecnt);					// 注册数据名
	void* operator[](int i);											// 重载数组操作符

	// 创建对象
	virtual int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend) = 0;
	// 销毁对象
	virtual void destroy(void) = 0;
	// 将对象写入文件
	virtual int write(FILE *fp, insideseg* inseg) = 0;

	void append(varbaseclass *vbc);								// 将一个数据对象加入到变量链表中
	void remove(varbaseclass *vbc);								// 将一个在变量链表中的数据对象摘除

	varbaseclass* find_varobj(LPCTSTR varname);					// 搜索指定名称的变量对象地址

protected:

	SEGTYPE			m_segtype;									// 段类型
	TCHAR			m_segname[MAX_SEGNAME_LENGHT+1];			// 段名字

	varbaseclass	*m_first;									// 变量表第一个元素的地址
	varbaseclass	*m_last;
	int				m_count;									// 变量表元素个数

	LPCTSTR*		m_reg_dataname;								// 注册的数据名
	int				m_reg_datanamecnt;							// 注册的数据名个数

public:
	segbaseclass	*m_prev;									// 前一个段类地址
	segbaseclass	*m_next;									// 后一个段类地址
};



/////////////////////////
// 全局段类
class golableseg : public segbaseclass
{
public:
	golableseg();
	~golableseg();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	static int tidy(LPCTSTR segbegin, LPCTSTR segend, LPCTSTR *orgnext, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);
};



/////////////////////////
// 内参段类

const int INSEGPS_MAXWRITERLEN = 512;					// 内参段作者串最大长度（字符单位）
const int INSEGPS_MAXLTLEN     = 64;					// 内存段最后写入时间串最大长度


class insideseg : public segbaseclass
{
public:
	insideseg(LPCTSTR segname);
	~insideseg();

	// 参数值来源
	typedef enum
	{
		INPST_DEFAULT = 0,			// 未经设定，默认值
		INPST_INNERSET,				// 程序内部设定
		INPST_USERSET				// 客户外部设定
	} INPSTATE, *LPINPSTATE;

	typedef enum
	{
		IPDEF_DOUPRE	= 2			// double精度默认值
	} IPDEF, *LPIPDEF;


	void* io(LPCTSTR varname, ILIO_CALLBACK_FUN cbf = NULL);	// 获取段中指定变量的数据存放地址

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	static int tidy(LPCTSTR segbegin, LPCTSTR segend, LPCTSTR *orgnext, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);

private:

	int		m_writer_mark;
	TCHAR	m_writer[INSEGPS_MAXWRITERLEN];			// ini 文件的创作者

	int		m_last_time_mark;
	TCHAR	m_last_time[INSEGPS_MAXLTLEN];			// ini 最后一次写入时间

	int		m_read_only_mark;
	bool	m_read_only;							// ini 只读标志

	int		m_double_precision_mark;				// ini double类型值精度
	int		m_double_precision;
};



/////////////////////////
// 注释段类
class commseg : public segbaseclass
{
public:
	commseg(LPCTSTR segname);
	~commseg();

	void* io(LPCTSTR varname, ILIO_CALLBACK_FUN cbf = NULL);	// 获取段中指定变量的数据存放地址
	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	static int tidy(LPCTSTR segbegin, LPCTSTR segend, LPCTSTR *orgnext, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);

private:

	LPCTSTR*	m_pcomm;										// 注释信息指针数组
};



/////////////////////////
// 数据段类
class norseg : public segbaseclass
{
public:
	norseg(LPCTSTR segname);
	~norseg();

	int create(LPCTSTR pstr, insideseg* inseg, LPCTSTR *pend);
	void destroy(void);
	int write(FILE *fp, insideseg* inseg);

	static int tidy(LPCTSTR segbegin, LPCTSTR segend, LPCTSTR *orgnext, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);
};



/////////////////////////
// 预编译数据摘要类

// 每个段缓冲区的指针个数
const int	PCSUMMARY_STEP	= 512;

class pcsummary
{
public:
	pcsummary();
	~pcsummary();

	// 初始化摘要类内部变量
	int initialization(void);
	// 记录一个段在预编译数据中的起始、结束地址
	int add(SEGTYPE type, LPBYTE begin, LPBYTE end);

	// 获取指定类型段的个数
	int getcnt(SEGTYPE type);
	// 获取指定类型段指定索引在预编译数据中的起始/结束地址
	int getaddr(SEGTYPE type, int index, LPBYTE* pb, LPBYTE* pe);

	void setbegin(LPBYTE pb){m_precombegin = pb;};
	void setend(LPBYTE pe){m_precomend = pe;};
	LPBYTE getbegin(void){return m_precombegin;};
	LPBYTE getend(void){return m_precomend;};

private:
	int		m_insegcnt;				// 段计数
	int		m_maxisc;				// 最大可以容纳的段个数
	LPBYTE*	m_insegaddr;			// 缓冲区

	int		m_comsegcnt;
	int		m_maxcsc;
	LPBYTE*	m_comsegaddr;

	int		m_golsegcnt;
	int		m_maxgsc;
	LPBYTE*	m_golsegaddr;

	int		m_norsegcnt;
	int		m_maxnsc;
	LPBYTE*	m_norsegaddr;

	LPBYTE	m_precombegin;			// 预编译数据开始地址
	LPBYTE	m_precomend;			// 预编译数据结束地址
};



///////////////////////////////////////////////////////////////////////////////
// INI类定义（用户接口


class inifile
{
public:
	inifile();
	~inifile();

public:

	bool read(LPCTSTR filename);							// 读入INI数据
	int  write(LPCTSTR filename = 0);						// 保存INI数据
	void* io(LPCTSTR segname, LPCTSTR varname);				// 获取数据

	void regname(LPCTSTR* psegname, int segnamecnt, LPCTSTR* pdataname, int datanamecnt);	// 注册段名和数据名
	segbaseclass& operator[](int i);

	int addseg(LPCTSTR segname);							// 添加一个数据段
	int delseg(LPCTSTR segname);							// 删除一个数据段
	int addvar(LPCTSTR segname, LPCTSTR varname, DATATYPE dtp, ...);	// 向指定数据段中添加一个变量
	int delvar(LPCTSTR segname, LPCTSTR varname);			// 删除指定数据段中的一个变量

	norseg* enum_norseg(norseg* prev_norseg);				// 枚举数据段

	LPCTSTR geterrmsg(void);								// 获取错误信息
	ILIO_CALLBACK_FUN setcallback(ILIO_CALLBACK_FUN newcbf);// 设置IO回调函数


protected:
	// 向链表中添加一个段类
	void append(segbaseclass *seg);
	// 从链表中摘除一个段类
	void remove(segbaseclass *seg);

	// 预编译
	int precompile(LPCTSTR otbegin, LPCTSTR otend, LPTSTR tidybegin, LPTSTR tidyend, pcsummary* pcsum);
	// 创建
	int create(pcsummary* pcsum);
	// 销毁
	void destroy(void);

	// 获取内参段指针
	insideseg*  find_inside_seg(void);
	// 获取全局段指针
	golableseg* find_golable_seg(void);
	// 获取指定名称数据的地址
	norseg*     find_data_seg(LPCTSTR segname);

private:
	TCHAR				m_filename[MAX_PATH];				// INI文件名称
	bool				m_flag;								// 数据读入成功的标记
	bool				m_isregularity;						// 文件内容是否是规整数据（true-是规整数据，false-不是）
															// *如果文件内容是本类的写函数（write）生成的，则是规整数据。
															//  被其它软件打开并修改过的内容，则不是规整数据。

	segbaseclass		*m_first;							// 段链表首指针
	segbaseclass		*m_last;							// 段链表尾指针
	int					m_count;							// 段计数

	ILIO_CALLBACK_FUN	m_cbf;								// IO回调函数指针
	_inicontextclass	m_ics;								// 上下文类

	LPCTSTR*			m_reg_segname;						// 注册的段名
	int					m_reg_segnamecnt;					// 注册的段名个数

};

#endif //__INILIB_H__

