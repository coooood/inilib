//
// inilib.h - 字符串操作函数集
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



//////////////////////////////////////////////////////////////////////////////////
// 本文件中的函数

LPCTSTR _gsp_strpbrk (LPCTSTR string, LPCTSTR strend, LPCTSTR control);		// 在给定的一段文本中搜索指定字符集中的字符（本函数为strpbrk()函数的变体）
LPCTSTR _gsp_strspn(LPCTSTR string, LPCTSTR strend, LPCTSTR control);		// 在给定的一段文本中搜索不在指定字符集中的字符（本函数为strspn()函数的变体）

void _gsp_remove_left(LPTSTR str, TCHAR ft1, TCHAR ft2);		// 去除串头部的指定字符（ft1/ft2－可指定去除两个不同的字符）如果串头部存在指定的字符，函数将移动串的内容
void _gsp_remove_right(LPTSTR str, TCHAR ft1, TCHAR ft2);		// 去除串尾部的指定字符
size_t _gsp_tidy_string(LPTSTR str);							// 去除串头部的空格、TAB字符和尾部的\r\n及空格TAB字符
int _gsp_fgets(LPCTSTR str, LPCTSTR *strend, LPTSTR *rel);		// 从指定地址获取一个文本行的串
void _gsp_strset(LPTSTR str, TCHAR c, int count);				// 将字符串头部填充指定个数的特定符号
int _gsp_wst_cnv(LPCTSTR pstrbegin, LPCTSTR pstrend, LPCTSTR pescpset, LPTSTR ptidy);				// 将内存格式的字符串转换为INI文件中的格式

LPCTSTR _gsp_prevline(LPCTSTR sp, LPCTSTR str);					// 获取指定文本所在行的上一行的首地址
LPCTSTR _gsp_currline(LPCTSTR sp, LPCTSTR str);					// 获取指定文本所在行的首地址
LPCTSTR _gsp_nextline(LPCTSTR str);								// 获取下一行的首地址

int _gsp_find_nextseg(LPCTSTR str, LPCTSTR *prul);				// 获取下一个段的段名行地址
LPCTSTR _gsp_find_nextvar(LPCTSTR str, LPCTSTR nextseg);		// 获取本段中下一个变量的首行地址
LPCTSTR _gsp_find_varend(LPCTSTR str, LPCTSTR nextseg);			// 获取本段中当前变量的结尾地址

SEGTYPE _gsp_get_segtype(LPCTSTR str);							// 判断段名行的属性

inline bool _gsp_is_void_line(LPCTSTR str);						// 判断指定串是不是空行
inline bool _gsp_is_comment(LPCTSTR str);						// 判断指定串是不是单行注释
bool _gsp_is_variable(LPCTSTR str);								// 判断指定串是不是变量声明行

DWORD _gsp_comp_attributemark(BYTE attr, BYTE addition);					// 合成数据属性标志
bool _gsp_decomp_attributemark(DWORD amk, BYTE *pattr, BYTE *paddition);	// 分解数据属性标志

DATATYPE _gsp_string_to_datatype(LPCTSTR str);								// 将关键字描述串转换为数据类型

bool _gsp_read_byte(LPCTSTR str, int scale, unsigned char *bte);			// 读取byte型变量
bool _gsp_read_int(LPCTSTR str, int scale, int *inter);						// 读int型变量
bool _gsp_read_bool(LPCTSTR boolstrbegin, LPCTSTR boolstrend, LPBYTE pdesindex, LPBYTE pboolean);	// 读bool型变量
bool _gsp_read_double(LPCTSTR str, double *doub);							// 读double型变量
bool _gsp_read_string(LPCTSTR pstrbegin, LPCTSTR *pstrend, LPCTSTR pvarend, LPTSTR tidytxtbegin, LPTSTR *tidytxtend);	// 读string型变量
bool _gsp_read_date(LPCTSTR pstrbegin, LPCTSTR *pstrend, LPCTSTR pvarend, BYTE adtime, __int64* dtv);					// 读date型变量
bool _gsp_read_struct(LPCTSTR pstrbegin, LPCTSTR pstrend, LPCTSTR pvartab, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict);	// 读struct型变量

bool _gsp_i_find_arrayarea(LPCTSTR pstrbegin, LPCTSTR pstrend, LPCTSTR *pab, LPCTSTR *pae);			// 在文本中搜索数组数据的头尾地址
bool _gsp_i_find_structarea(LPCTSTR pstrbegin, LPCTSTR pstrend, LPCTSTR *pab, LPCTSTR *pae);		// 在文本中搜索结构数据的头尾地址

int _gsp_find_iniparamstr(LPCTSTR str, LPCTSTR *pp, int cntpp);				// 获取str串在pp串数组中的索引
bool _gsp_cnvdt_to_str(__int64 dt, BYTE adtime, LPTSTR buff, size_t buffsize);// 将日期时间值转换为字符串

int _gsp_get_sutoffset_data(LPCTSTR pvtab, int sutmemcnts, int *poff);		// 获取结构的各成员的存放偏移量
int _gsp_get_sutali_value(void);											// 获取结构字节对齐的默认值
int _gsp_get_sutbody(LPILVT_STRUCT pvpt, LPCTSTR pdat);						// 创建一个结构的实体
int _gsp_i_freesutbody(LPILVT_STRUCT pvpt);									// 释放结构体中的内存




////////////////////////////////////////////////////////////////
// 字符常量定义

const TCHAR CT_SPACE = _T(' ');
const TCHAR CT_TAB   = _T('\t');
const TCHAR CT_CR    = _T('\r');
const TCHAR CT_NL    = _T('\n');
const TCHAR CT_NULL  = _T('\0');
const TCHAR CT_ESCP  = _T('\\');
const TCHAR CT_STR   = _T('\"');
const TCHAR CT_COMM  = _T('/');

const TCHAR CT_SEGNB = _T('[');
const TCHAR CT_SEGNE = _T(']');
const TCHAR CT_VARNB = _T('<');
const TCHAR CT_VARNE = _T('>');

const TCHAR CT_STRUCTMRK  = _T(':');
const TCHAR CT_ASSIGNMENT = _T('=');
const TCHAR CT_ARRYNB     = _T('[');
const TCHAR CT_ARRYNE     = _T(']');
const TCHAR CT_ARRYDATANB = _T('(');
const TCHAR CT_ARRYDATANE = _T(')');
const TCHAR CT_STRUCTNB   = _T('{');
const TCHAR CT_STRUCTNE   = _T('}');

const LPCTSTR CT_STRIDCH  = _T("\"");
const LPCTSTR CT_ARRYDSNB = _T("(");
const LPCTSTR CT_ARRYDSNE = _T(")");
const LPCTSTR CT_VARDSNB  = _T("<");
const LPCTSTR CT_VARDSNE  = _T(">");
const LPCTSTR CT_ARRYSNB  = _T("[");
const LPCTSTR CT_ARRYSNE  = _T("]");
const LPCTSTR CT_STCTDSNB = _T("{");
const LPCTSTR CT_STCTDSNE = _T("}");
const LPCTSTR CT_CRNL     = _T("\r\n");

const TCHAR CT_SEG_COMM   = _T('~');
const TCHAR CT_SEG_INSD   = _T('!');

const TCHAR CT_SPLIT      = _T(',');
const TCHAR CT_SPLIT2     = _T(' ');

const TCHAR CT_NUMTPE_HEX			= _T('&');
const LPCTSTR CT_NUMTPE_HEX_STR		= _T("&");
const TCHAR	CT_DATETIME				= _T('+');
const LPCTSTR CT_DATETIME_STR		= _T("+");
const TCHAR CT_SUTMEMTPE_LIST		= _T('#');
const LPCTSTR CT_SUTMEMTPE_LIST_STR	= _T("#");


const TCHAR	GSP_INVSNC[]		= _T(" []{}<>():;,.+-*/=\t\r\n\\\'\"");		// 段名字中不能出现的字符
const size_t GSP_INVSNC_SIZE	= sizeof(GSP_INVSNC)/sizeof(TCHAR);			// 字符的个数

const TCHAR	GSP_INVVNC[]  = _T(" []{}<>()/~`!@#$%^&*:;,.+-=\t\r\n\\\'\"");	// 变量名字中不能出现的字符
const size_t GSP_INVVNC_SIZE   = sizeof(GSP_INVVNC)/sizeof(TCHAR);			// 字符的个数

const TCHAR GSP_ESCAPED[] = _T("[]{}<>()/\\\"\t\r\n");						// 字符串中需转义的字符
const size_t  GSP_ESCAPED_SIZE = sizeof(GSP_ESCAPED)/sizeof(TCHAR);			// 字符的个数

const TCHAR GSP_COMMSEG_ESCAPED[] = _T("[]\\");								// 注释段中需转义的字符
const size_t  GSP_COMMSEG_ESCAPED_SIZE = sizeof(GSP_COMMSEG_ESCAPED)/sizeof(TCHAR);

const TCHAR	GSP_WHITESPC1[] = _T(" \t");									// 空白字符常量
const TCHAR	GSP_WHITESPC2[] = _T(" \t\r\n");
const TCHAR	GSP_WHITESPC3[] = _T(" \t\r\n,");




////////////////////////////////////////////////////////////////
// 预编译常量定义
const BYTE PCA_MAG0				= 0x4d;				// 'M'	- 数据属性标志
const BYTE PCA_MAG1				= 0x4b;				// 'K'

// 数据属性定义
const BYTE PCA_ATT_SEGNAME		= 0;				// 段名
const BYTE PCA_ATT_VOID			= 1;				// 空行
const BYTE PCA_ATT_COMM			= 2;				// 注释
const BYTE PCA_ATT_VAR			= 3;				// 变量

// 附加信息定义
const BYTE PCA_ADD_GOLSEG		= (BYTE)SEGT_GOLABLENAME;			// 全局段
const BYTE PCA_ADD_INSIDESEG	= (BYTE)SEGT_INSIDEPARANAME;		// 内参段
const BYTE PCA_ADD_COMMSEG		= (BYTE)SEGT_COMMSEGNAME;			// 注释段
const BYTE PCA_ADD_NORSEG		= (BYTE)SEGT_VALSEGNAME;			// 数据段

const BYTE PCA_ADD_LINECOMM		= 0;								// 其他段中的单行注释信息

// 数据类型代码
const BYTE PCA_ADD_VAR_BYTE			= (BYTE)NTP_BYTE;				// byte数据
const BYTE PCA_ADD_VAR_INT			= (BYTE)NTP_INT;				// int数据
const BYTE PCA_ADD_VAR_BOOL			= (BYTE)NTP_BOOL;				// bool数据
const BYTE PCA_ADD_VAR_DOUBLE		= (BYTE)NTP_DOUBLE;				// double数据
const BYTE PCA_ADD_VAR_STRING		= (BYTE)NTP_STRING;				// string数据
const BYTE PCA_ADD_VAR_DATE			= (BYTE)NTP_DATE;				// date数据
const BYTE PCA_ADD_VAR_STRUCT		= (BYTE)NTP_STRUCT;				// struct数据

const BYTE PCA_ADD_VAR_BYTEARRAY	= (BYTE)NTP_ARRAY_BYTE;			// byte[]数组数据
const BYTE PCA_ADD_VAR_INTARRAY		= (BYTE)NTP_ARRAY_INT;			// int[]数组数据
const BYTE PCA_ADD_VAR_BOOLARRAY	= (BYTE)NTP_ARRAY_BOOL;			// bool[]数组数据
const BYTE PCA_ADD_VAR_DOUBLEARRAY	= (BYTE)NTP_ARRAY_DOUBLE;		// double[]数组数据
const BYTE PCA_ADD_VAR_STRINGARRAY	= (BYTE)NTP_ARRAY_STRING;		// string[]数组数据
const BYTE PCA_ADD_VAR_DATEARRAY	= (BYTE)NTP_ARRAY_DATE;			// date[]数组数据
const BYTE PCA_ADD_VAR_STRUCTARRAY	= (BYTE)NTP_ARRAY_STRUCT;		// struct[]数组数据




////////////////////////////////////////////////////////////////
// 函数定义



// 在给定的一段文本中搜索样本字符集中的字符（本函数为strpbrk()函数的变体）
// 
// string	- IN - 搜索的起始地址
// strend	- IN - 搜索的结束地址
// control	- IN - 样本字符集，NULL结尾
//
// 返回值：在给定文本中找到样本字符集中的任意字符时，返回该字符出现在给定文本中的位置。
//			如果没有找到，返回NULL。
//
// *注：入口参数都不能为NULL。
//
LPCTSTR _gsp_strpbrk(LPCTSTR string, LPCTSTR strend, LPCTSTR control)
{
	_ASSERT(string&&strend&&control);

	while (string < strend)
	{
		for (LPCTSTR set = (LPCTSTR)control; *set; set++)
		{
			if (*set == *string)
			{
				return string;
			}
		}
		string++;
	}

	return NULL;
}


// 在给定的一段文本中搜索不在样本字符集中的字符（本函数为strspn()函数的变体）
// 
// string	- IN - 搜索的起始地址
// strend	- IN - 搜索的结束地址
// control	- IN - 样本字符集，NULL结尾
//
// 返回值：在给定文本中找到不在样本字符集中的任意字符时，返回该字符出现在给定文本的位置。
//			如果给定文本中的字符都在字符集中，返回NULL。
//
// *注：返回值的类型与CRT函数strspn()不同，后者是返回索引值。
//		入口参数都不能为NULL。
//
LPCTSTR _gsp_strspn(LPCTSTR string, LPCTSTR strend, LPCTSTR control)
{
	LPCTSTR str = string;
	LPCTSTR ctl;

	_ASSERT(string&&strend&&control);

	while (str < strend)
	{
		for (ctl = control; *ctl != *str; ctl++)
		{
			if (*ctl == (TCHAR)0)
			{
				return str;
			}
		}
		str++;
	}

	return NULL;
}


// 去除串头部的指定字符
//
// str     - IN - 待处理的文本
// ft1/ft2 - IN - 指定去除的两个字符（比如 ' '、'\t'、'\r' 等等）
//
// *注：如果串头部存在指定的字符，函数将向前移动串的内容，指定的字符将被覆盖（串尾部的0将一同向前拷贝）。
//      如果只想去除一种字符，可以将ft1和ft2填写成相同的内容。
//
void _gsp_remove_left(LPTSTR str, TCHAR ft1, TCHAR ft2)
{
	LPTSTR first = str;

	while ((*str != CT_NULL)&&((*str == ft1)||(*str == ft2)))
		str++;

	if (first != str)
		memmove((void*)first, (const void *)str, (_tcslen(str)+1)*sizeof(TCHAR));	// 串尾部的0一并拷贝
}


// 去除串尾部的指定字符
//
// str     - IN - 待处理的文本
// ft1/ft2 - IN - 指定去除的两个字符（比如 ' '、'\t'、'\r' 等等）
//
// *注：函数不会移动串内容，只是将串尾部最左边的指定字符替换成NULL。
//
void _gsp_remove_right(LPTSTR str, TCHAR ft1, TCHAR ft2)
{
	int i = _tcslen(str);

	while((i>0)&&(*(str+i-1)==ft1||*(str+i-1)==ft2))
		i--;

	*(str+i) = CT_NULL;
}


// 去除串头部的空格、TAB字符和尾部的\r\n及空格TAB字符
//
// str - IN - 待处理的文本
// 
// 返回值：去除头尾指定字符后串的长度。
//
// *注：本函数是_gsp_remove_left()函数和_gsp_remove_right()函数的组合体，但效率会比它们的组合高。
//      str 指向的串必须以NULL结尾。
//
size_t _gsp_tidy_string(LPTSTR str)
{
	LPTSTR first = str;

	// 去除串头部的空格和TAB字符
	while ((*str != CT_NULL)&&((*str == CT_SPACE)||(*str == CT_TAB)))
		str++;

	if (first != str)
		memmove((void*)first, (const void *)str, (_tcslen(str)+1)*sizeof(TCHAR));	// 向前移动串

	int i = _tcslen(str=first);

	// 去除串尾部的回车换行符和空格、TAB字符
	while((i>0)&&(*(str+i-1)==CT_SPACE||*(str+i-1)==CT_TAB||*(str+i-1)==CT_CR||*(str+i-1)==CT_NL))
		i--;

	*(str+i) = CT_NULL;

	// 返回整理后的串的长度（以字符为单位）
	return _tcslen(first);
}


// 从指定地址获取一个文本行的串，并将串内容复制到一个新申请的缓冲区并返回给调用者
//
// str    - IN  - 读取原始文本的地址
// strend - OUT - 函数成功读取后，下一行的首地址
// rel    - OUT - 函数成功读取后，返回的由本函数申请的缓冲区，里面存放了获取的一行的文本，NULL结尾
//
// 返回值：如果获取成功，返回一行文本的长度（字符数）。
//         如果str中已经没有字符（str指向的内存中是0），返回0（此时，入口参数strend和rel将被填入NULL）。
//         如果本函数申请内存失败，返回-1。
//
// *注：如果函数失败，strend、rel将被写入NULL。
//      如果遇到原始文本的结尾，即使未遇到换行符，也将结束，并返回之前已读取的文本。
//      本函数以字符 '\n' 作为一行结束的标志，只有遇到这个字符后读取才会结束。
//      本函数返回的缓冲区在不需要时，应使用delete []操作删除。
//      新缓冲区中读取的行内容将保持原样，结尾的 '\n' 字符会被保留。
//      新缓冲区中读取的行内容将以NULL结尾。
//      入口参数中的原始文本应该以NULL结尾。
//
int _gsp_fgets(LPCTSTR str, LPCTSTR *strend, LPTSTR *rel)
{
	_ASSERT(str);

	// 初始化返回值
	*strend = NULL;
	*rel    = NULL;

	// 判断原始文本是否为空，没有字符返回0
	if (str[0] == CT_NULL)
		return 0;

	// 搜索换行符 '\n'
	LPCTSTR pdes = _tcschr(str, CT_NL);
	LPTSTR	p;
	size_t	ts;

	if (pdes)
		ts = (size_t)(pdes-str+1);
	else
		ts = (size_t)_tcslen(str);

	// 申请新文本缓冲区（+1是为串结尾留出写0的位置）
	if ((p = new TCHAR[ts+1]) == NULL)
		return -1;			// 内存申请失败，返回-1

	// 复制串内容（包括尾部的 '\n' 字符）
	if (pdes)
		memmove((void*)p, (void*)str, ts*sizeof(TCHAR));
	else
		_tcscpy_s(p, ts+1, str);

	// 填写结尾0字符
	p[ts] = CT_NULL;

	// 填写返回值
	*strend = (LPTSTR)(str+ts);
	*rel    = p;

	return ts;	// 返回一行的字符数
}


// 向字符串头部填充指定个数的特定符号
//
// str		- IN - 待填充的字符串缓冲区
// strsize	- IN - str的内存尺寸（字符单位）
// c		- IN - 填充字符
// count	- IN - 填充的字符个数
//
// 返回值：无
//
// *注：函数模仿CRT函数strset()，不同点是本函数不会理会str中的NULL字符。如果count的尺寸超过了
//		str中原有的NULL字符，函数会继续填充。
//		函数会在填充结尾处加入NULL字符。
//
void _gsp_strset(LPTSTR str, int strsize, TCHAR c, int count)
{
	// 防溢出检查
	if ((count+1) > strsize)
	{
		_ASSERT(0);
		count = strsize-1;
	}

	for (int i=0; i<count; i++)
	{
		str[i] = c;
	}

	str[count] = _T('\0');
}


// 获取指定文本所在行的上一行的首地址
//
// sp  - IN - str前方指定的搜索终点
// str - IN - 搜索的地址，函数将从本地址向前搜索换行符
// 
// 返回值：如果找到上一行首地址，则返回该地址。如果没有找到，返回NULL
//
// * 注：换行符必须是 /r/n 组合。
//		 sp不能为NULL。sp不要指向/r/n组合的中间。
//
LPCTSTR _gsp_prevline(LPCTSTR sp, LPCTSTR str)
{
	_ASSERT(sp&&str);

	LPCTSTR	prn = NULL;

	// 查找上一行的结尾
	for (int i=(int)(str-sp)-1; i>=0; i--)
	{
		if ((sp[i]==CT_NL)&&(sp[i-1]==CT_CR))
		{
			prn = &sp[i-2];
			break;
		}
	}

	if (prn == NULL) return NULL;

	// 查找上上行的结尾
	for (int i=(int)(prn-sp); i>=0; i--)
	{
		if ((sp[i]==CT_NL)&&(sp[i-1]==CT_CR))
		{
			return &sp[i+1];
		}
	}

	return NULL;
}


// 获取指定文本所在行的首地址
//
// sp  - IN - str前方指定的搜索终点
// str - IN - 搜索的地址，函数将从本地址向前搜索换行符
// 
// 返回值：如果找到上一行结尾的换行符，则返回换行符后面本行的首地址。
//			如果没有找到上一行的换行符，返回NULL
//
// * 注：换行符必须是 /r/n 组合。
//		 sp不能为NULL。sp不要指向/r/n组合的中间。
//
LPCTSTR _gsp_currline(LPCTSTR sp, LPCTSTR str)
{
	_ASSERT(sp&&str);

	for (int i=(int)(str-sp)-1; i>=0; i--)
	{
		if ((sp[i]==CT_NL)&&(sp[i-1]==CT_CR)) return (LPCTSTR) &sp[i+1];
	}

	return NULL;
}


// 获取下一行的首地址
//
// str - IN - 开始搜索的地址
// 
// 返回值：如果找到下一行，则返回下一行的首地址。如果没有下一行了，返回NULL
//
// * 注：换行符必须是 /r/n 组合。
//		 str串必须以NULL字符结尾。
//
LPCTSTR _gsp_nextline(LPCTSTR str)
{
	_ASSERT(str);

	while (LPCTSTR pd = _tcschr(str, CT_CR))
	{
		if (pd[1] == CT_NL)
			return (LPCTSTR) &pd[2];
		else
			str = &pd[1];
	}

	return NULL;
}


// 合成预编译数据中的数据属性标志
//
// attr		- IN - 数据属性
// addition - IN - 附加信息
//
// 返回值：返回合成后的4字节属性标志
//
DWORD _gsp_comp_attributemark(BYTE attr, BYTE addition)
{
	DWORD	mag = 0;
	LPBYTE	p = (LPBYTE)&mag;

	p[0] = PCA_MAG0;
	p[1] = PCA_MAG1;
	p[2] = attr;
	p[3] = addition;

	return mag;
}


// 分解预编译数据中的数据属性标志
//
// amk		 - IN  - 待分解的4字节属性标志
// pattr	 - OUT - 返回分解出来的数据属性
// paddition - OUT - 返回分解出来的附加信息
//
// 返回值：如果分解成功，返回true。如果amk中不是数据属性标志，则返回false。
//
bool _gsp_decomp_attributemark(DWORD amk, BYTE *pattr, BYTE *paddition)
{
	_ASSERT(pattr&&paddition);

	LPBYTE	p = (LPBYTE)&amk;

	if ((p[0] == PCA_MAG0)&&(p[1] == PCA_MAG1))
	{
		*pattr		= p[2];
		*paddition	= p[3];

		return true;
	}

	return false;
}


// 将变量类型文本串转换为数据类型
//
// str - IN - 变量类型文本（去除头尾无用字符的、NULL结尾的串）
//
// 返回值：返回变量类型。如果文本不是合法的变量类型，则返回NTP_EC_SYNERR。
//
// *注：变量类型文本大小写敏感。
//		本函数只判断基本的变量类型，无法判断是不是数组
//
DATATYPE _gsp_string_to_datatype(LPCTSTR str)
{
	for (int i=0; i<NTP_ARRAY_BYTE; i++)
	{
		if (_tcscmp(str, INIDATATYPESTRING[i]) == 0)
		{
			return (DATATYPE)i;
		}
	}

	return NTP_EC_SYNERR;		// 非法的变量类型描述串
}


// 从指定位置开始，搜索下一个段的段名行地址
//
// str  - IN  - 开始搜索的地址
// prul - OUT - 如果在str之后没有段了，本变量将被设置为NULL。否则是下一个段的段名行首地址。
//
// 返回值：
//			-1	- 内存分配失败
//			 0	- str之后没有段了
//			 1	- str之后有段，并成功找到
//
// *注：str指向的串必须以NULL结尾。
// 
int _gsp_find_nextseg(LPCTSTR str, LPCTSTR *prul)
{
	_ASSERT(str&&prul);

	*prul = NULL;

	LPCTSTR	ps = str;
	LPCTSTR	pd, pis;

	while (pd = _tcschr(ps, CT_SEGNB))
	{
		int slen = (int)(pd-ps);			// 计算起点到出现段名前缀标志的距离（字符单位）
		int i;

		// 如果段名标志前带有转义符，说明是字符串或是注释段中的内容，忽略它
		if ((pd > str)&&(pd[-1] == CT_ESCP))
		{
			ps = (LPCTSTR)(pd+1);
			continue;
		}

		// 从段名前缀开始向前搜索\r\n组合
		for (i=slen-1; i>=0; i--)
			if ((ps[i] == CT_NL)&&(ps[i-1] == CT_CR)) break;

		// 计算段名行的起始地址
		if (i < 0)
			pis = ps;
		else
			pis = &ps[i+1];

		// 判断这一行是不是段名行
		switch (_gsp_get_segtype(pis))
		{
		case	SEGT_EC_NOTMEM:				// 内存分配失败
			return -1;
		case	SEGT_GOLABLENAME:			// 全局段
		case	SEGT_INSIDEPARANAME:		// 内参段
		case	SEGT_COMMSEGNAME:			// 注释段
		case	SEGT_VALSEGNAME:			// 数据段（变量段）
			*prul = pis;					// 找到后续段，返回段名行起始地址
			return 1;
		case	SEGT_MAX:
		case	SEGT_BOUNDARY:
			_ASSERT(0);
			break;
		case	SEGT_EC_INVALIDSEGNAME:
			break;
		}

		ps = (LPCTSTR)(pd+1);				// 更新搜索地址，指向段名前缀的后面一个字符
	}

	return 0;	// 没有后续段，返回0
}


// 获取本段中下一个变量的首行地址
//
// str     - IN - 当前变量定义区间的任意地址
// nextseg - IN - 下一个段的段名行首地址（作为搜索终点）
//
// 返回值：如果在当前段中找到下一个变量，则返回该变量的行首地址。否则返回NULL。
//
// *注：str串必须以NULL结尾。
//		函数将从str所在行的下一行开始搜索。
//		如果没有下一个段，nextseg参数可填写NULL，函数将以str结尾的NULL作为搜索终点。
//
LPCTSTR _gsp_find_nextvar(LPCTSTR str, LPCTSTR nextseg)
{
	_ASSERT(str);

	LPCTSTR	ps = _gsp_nextline(str);			// 获取下一行首地址（从下一行开始搜索）

	if (ps == NULL)
		return NULL;

	if ((nextseg != NULL)&&(ps >= nextseg))		// 如果下一行是在下一个段中，返回NULL
		return NULL;

	// 向后搜索变量标志符
	while (LPCTSTR pd = _tcschr(ps, CT_VARNB))
	{
		// 如果它出现在下一个段中，返回NULL
		if ((nextseg != NULL)&&(pd >= nextseg))
			return NULL;

		// 更新搜索位置
		ps = &pd[1];

		// 如果变量标志前带有转义符，说明是字符串中的内容，忽略它
		if (pd[-1] == CT_ESCP)
			continue;

		LPCTSTR pnl = _gsp_nextline(pd);		// 获取下一行的首地址

		if (pnl == NULL)
			pnl = &pd[_tcslen(pd)];				// 如果没有下一行，用串结尾的NULL地址代替

		// 搜索同一行中是否存在所有的变量声明标志
		LPCTSTR pe = _tcschr(pd, CT_VARNE);
		LPCTSTR pq = _tcschr(pd, CT_ASSIGNMENT);

		// 排除各种可能的非变量情况
		if ((pe==NULL)||(pq==NULL))
			continue;
		if ((pe >= pnl)||(pq >= pnl))
			continue;
		if ((pe[-1] == CT_ESCP)||(pq[-1] == CT_ESCP))
			continue;
		if (pe > pq)
			continue;

		// 获取行首地址
		LPCTSTR plb = _gsp_currline(str, pd);
		// 找到第一个非空格和TAB的字符的位置
		size_t comidx = _tcsspn(plb, _T(" \t"));
		// 判断该行是不是单行注释
		if (_gsp_is_comment(&plb[comidx]) == true)
			continue;

		return plb;			// 返回下一个变量所在行的首地址
	}

	return NULL;
}


// 获取当前变量的结尾地址
//
// pvr     - IN - 当前变量行首地址（即变量声明行的行首地址）
// nextseg - IN - 搜索终点（下一个段的段名行首地址）
//
// 返回值:当前变量的结尾地址，也就是变量定义最后一行的\r\n字符后面的地址(下一行的行首地址)。
//        如果函数中的内存申请操作失败，函数将返回NULL。
//
// *注：pvr串必须以NULL结尾。
//		如果没有下一个段，nextseg参数可填写NULL，函数将以pvr结尾的NULL作为搜索终点。
//      pvr/nextseg都必须指向行首地址。
//
LPCTSTR _gsp_find_varend(LPCTSTR pvr, LPCTSTR nextseg)
{
	_ASSERT(pvr);

	// 搜索终点前下一个变量的行首地址
	LPCTSTR pnextvar = _gsp_find_nextvar(pvr, nextseg);
	// 获取下一行行首地址
	LPCTSTR	pnextline = _gsp_nextline(pvr);

	bool	tailrn = true;							// INI最后一行的结尾是否有\r\n字符的标志

	if (pnextvar == NULL)
	{
		if (nextseg == NULL)
			pnextvar = &pvr[_tcslen(pvr)];
		else
			pnextvar = nextseg;

		if (pnextline == NULL)					// 如果当前变量行是INI的最后一行，返回结尾指针
			return pnextvar;

		if ((pnextvar[-1]==CT_NL)&&(pnextvar[-2]==CT_CR))
			tailrn = true;
		else
			tailrn = false;						// 最后一行没有\r\n字符的标志
	}
	else
	{
		if (pnextvar == pnextline)					// 下一行就是下一个变量
			return pnextvar;
	}

	LPCTSTR prevline = NULL;
	LPCTSTR pendline = pnextvar;
	LPCTSTR	pvarend  = NULL;

	LPCTSTR	pnx;
	LPTSTR	ptxt;

	while (true)
	{
		if (tailrn)
		{
			// 获取上一行行首地址
			prevline = _gsp_prevline(pvr, pendline);

			if (prevline == NULL)
			{
				pvarend = _gsp_currline(pvr, pendline);
				break;
			}
		}
		else
		{
			// 将当前行模拟为上一行
			prevline = _gsp_currline(pvr, pendline);
			tailrn = true;
		}

		// 获取该地址的文本
		if (_gsp_fgets(prevline, &pnx, &ptxt) <= 0)
		{
			return NULL;	// 内存申请失败
		}

		_ASSERT(pnx == pendline);

		// 去除串头部的空格、TAB字符和尾部的\r\n及空格TAB字符
		size_t tl = _gsp_tidy_string(ptxt);

		// 如果该行是空行（长度是0），或者是单行注释，则继续向上搜索
		if ((tl == 0)||(_gsp_is_comment(ptxt)))
		{
			delete []ptxt;
			pendline = prevline;
			continue;
		}

		delete []ptxt;
		pvarend = pendline;
		break;
	}

	return pvarend;
}


// 判断段名行的属性
//
// str  - IN - 指向段名行文本的起始地址（串结尾必须有NULL）
//
// 返回值：
//		 	SEGT_GOLABLENAME		全局段
//			SEGT_INSIDEPARANAME		内参段
//			SEGT_COMMSEGNAME		注释段
//			SEGT_VALSEGNAME			数据段（变量段）
//
//			SEGT_EC_NOTMEM			错误代码：内存分配失败
//			SEGT_EC_INVALIDSEGNAME	错误代码：无效的段名
//
// *注：函数不会修改str中的内容。
//
SEGTYPE _gsp_get_segtype(LPCTSTR str)
{
	LPCTSTR	ped;
	LPTSTR	prel;
	TCHAR   c;
	size_t  slen;

	// 从指定地址获取段名串
	switch (_gsp_fgets(str, &ped, &prel))
	{
	case	0:
		return SEGT_EC_INVALIDSEGNAME;		// str是一个空串，不是有效的段名字
	case	-1:
		return SEGT_EC_NOTMEM;				// 内存分配失败
	}

	// 去除串头尾的无用字符
	slen = _gsp_tidy_string(prel);

	// 判断是否为段名行是以串头尾部的 [] 符号来确定的。如果[]符号
	// 是出现在串中间，则视为无效的段名

	if ((prel[0] != CT_SEGNB)||(prel[slen-1] != CT_SEGNE))
	{
		delete []prel;
		return SEGT_EC_INVALIDSEGNAME;		// 不是有效的段名字（缺少段名字标志符[]）
	}

	prel[0]      = CT_SPACE;
	prel[slen-1] = CT_SPACE;

	// 去除段名标志及无用字符
	slen = _gsp_tidy_string(prel);

	if (slen == 0)	
	{
		delete []prel;
		return SEGT_GOLABLENAME;			// 没有段名的为全局段
	}

	if (_tcspbrk(prel, GSP_INVSNC))
	{
		delete []prel;
		return SEGT_EC_INVALIDSEGNAME;		// 不是有效的段名字（名字中含有非法字符）
	}

	// 保留段属性标志位
	c = prel[0];
	// 释放_gsp_fgets()函数申请的内存
	delete []prel;

	if (c == CT_SEG_COMM)
		return SEGT_COMMSEGNAME;
	else if (c == CT_SEG_INSD)
		return SEGT_INSIDEPARANAME;
	else
		return SEGT_VALSEGNAME;
}


// 判断指定串是不是空行
//
// str - IN - 待判断的串
//
// 返回值：返回true表示是空行，否则不是。
//
// *注：str指向的串必须是经过去除头尾无用字符操作的。
//		函数以判断串的长度是否为 0 作为空行的判断标准。
//
inline bool _gsp_is_void_line(LPCTSTR str)
{
	return (_tcslen(str) == 0) ? true : false;
}


// 判断指定串是不是单行注释
//
// str - IN - 待判断的串
//
// 返回值：返回true表示是单行注释，否则不是。
//
// *注：str指向的串必须去除头部无用字符
//		单行注释的标志是 // 
//
inline bool _gsp_is_comment(LPCTSTR str)
{
	return ((str[0]==CT_COMM)&&(str[1]==CT_COMM)) ? true : false;
}


// 判断指定串是不是变量声明行
//
// str - IN - 待判断的串
//
// 返回值：返回true表示是变量声明行，否则不是。
//
// *注：str指向的串必须去除头尾部的无用字符
//		本函数只是大致判断一个文本行是不是变量的声明部分，并不会去判断声明的语法对错
//		变量声明行是指赋值号及之前的文本。
//
bool _gsp_is_variable(LPCTSTR str)
{
	// 如果是空行或单行注释，返回false
	if (_gsp_is_void_line(str)||_gsp_is_comment(str))
		return false;

	// 搜索变量类型标志 < >
	LPCTSTR pts = _tcschr(str, CT_VARNB);
	LPCTSTR pte = _tcschr(str, CT_VARNE);

	if ((pts==0)||(pte==0)||(pts==str)||(pte<pts)||((pte-pts)==1))
		return false;

	if ((pts[-1] == CT_ESCP)||(pte[-1] == CT_ESCP))
		return false;

	// 搜索赋值符 =
	LPCTSTR pequ= _tcschr(pte, CT_ASSIGNMENT);

	if ((pequ==NULL)||(pequ<pte))
		return false;

	return true;
}


// 读取byte型变量
//
// str		- IN  - 待读取的字符串
// scale	- IN  - 进制说明（只支持10进制和16进制）
// bte		- OUT - 存放读取值的变量指针
//
// 返回：如果读取成功，返回true，失败返回false。
//
// *注：str串不需要NULL结尾。
//		如果函数读取失败（返回false），bte只向的变量不会被改动。
//
bool _gsp_read_byte(LPCTSTR str, int scale, unsigned char *bte)
{
	int tmp;

	switch (scale)
	{
	case	10:
		if (_stscanf_s(str, _T("%d"), &tmp) == 0) return false;
		break;
	case	16:
		if (_stscanf_s(str, _T("%x"), &tmp) == 0) return false;
		break;
	default:
		_ASSERT(0);
		return false;
	}

	*bte = (unsigned char)(tmp&0xff);

	return true;
}


// 读取int型变量
//
// str		- IN  - 待读取的字符串
// scale	- IN  - 进制说明（只支持10进制和16进制）
// inter	- OUT - 存放读取值的变量指针
//
// 返回：如果读取成功，返回true，失败返回false。
//
// *注：str串不需要NULL结尾。
//		如果函数读取失败（返回false），inter指向的变量不会被改动。
//
bool _gsp_read_int(LPCTSTR str, int scale, int *inter)
{
	int tmp;

	switch (scale)
	{
	case	10:
		if (_stscanf_s(str, _T("%d"), &tmp) == 0) return false;
		break;
	case	16:
		if (_stscanf_s(str, _T("%x"), &tmp) == 0) return false;
		break;
	default:
		_ASSERT(0);		// 进制只支持10和16两种
		return false;
	}

	*inter = tmp;

	return true;
}


// 读bool型变量
//
// boolstrbegin	- IN  - 布尔值文本描述串开始地址
// boolstrend	- IN  - 布尔值文本描述串结束地址
// pdesindex	- OUT - 返回对应的描述索引
// pboolean		- OUT - 返回布尔值
//
// 返回：转换成功返回true，pdesindex和pboolean中返回对应的数据。失败返回false。
//
// *注：如果函数失败，pdesindex和pboolean指向的值不会被修改。
//
bool _gsp_read_bool(LPCTSTR boolstrbegin, LPCTSTR boolstrend, LPBYTE pdesindex, LPBYTE pboolean)
{
	for (int i=0; i<MAX_BOOLDES_TYPE; i++)
	{
		for (int k=0; k<2; k++)
		{
			if (_tcslen(BOOLDES[i].oo[k]) == (boolstrend-boolstrbegin))
			{
#ifdef _UNICODE
				if (wmemcmp(BOOLDES[i].oo[k], boolstrbegin, (boolstrend-boolstrbegin)) == 0)
				{
					*pboolean  = (BYTE)k;
					*pdesindex = (BYTE)i;

					return true;
				}
#else
				if (memcmp(BOOLDES[i].oo[k], boolstrbegin, (boolstrend-boolstrbegin)) == 0)
				{
					*pboolean  = (BYTE)k;
					*pdesindex = (BYTE)i;

					return true;
				}
#endif
			}
		}
	}

	return false;
}


// 读double型变量
//
// str	- IN  - 小数字符串
// doub	- OUT - 返回读取的double值
//
// 返回值：读取成功返回true，doub中返回读取的值。失败返回false，doub指向的值不会被改变。
//
bool _gsp_read_double(LPCTSTR str, double *doub)
{
	double	tmp;

	if (_stscanf_s(str, _T("%lf"), &tmp) == 0)
		return false;
	else
		*doub = tmp;

	return true;
}


// 将内存格式的字符串转换为INI文件中的格式
//
// pstrbegin	- IN - 内存字符串开始地址
// pstrend		- IN - 内存字符串结尾地址（即结尾NULL的地址）
// pescpset		- IN - 转义字符集
// ptidy		- IN - 转换后的字符串存放地址
//
// 返回值：返回转换后字符串的长度（不包括结尾的NULL）。
//
// *注：转换后的字符串结尾会被添加NULL字符。
//		转义字符集是一个字符串，结尾有NULL字符。
//		转义字符串类似于 : "[]{}<>()/\\\"\t\r\n"
//
int _gsp_wst_cnv(LPCTSTR pstrbegin, LPCTSTR pstrend, LPCTSTR pescpset, LPTSTR ptidy)
{
	LPCTSTR	pstr = pstrbegin;
	size_t	loc, i;
	int		len	 = 0;					// 转换后字符串长度值

	// 复制字符串到整理缓冲区，并根据字符集添加转义符
	while (pstr < pstrend)
	{
		// 搜索需要转义的字符
		loc = _tcscspn(pstr, pescpset);

		for (i=0; i<loc; i++)
		{
			*ptidy++ = *pstr++;
		}

		_ASSERT(pstr <= pstrend);

		if (pstr < pstrend)
		{
			*ptidy++ = CT_ESCP;

			switch (*pstr)
			{
			case	_T('\t'):
				*ptidy++ = _T('t');
				break;
			case	_T('\r'):
				*ptidy++ = _T('r');
				break;
			case	_T('\n'):
				*ptidy++ = _T('n');
				break;
			default:
				*ptidy++ = *pstr;
				break;
			}

			pstr++;
			len += (int)(loc+1+1);
		}
		else
		{
			len += (int)loc;
		}
	}

	*ptidy = NULL;		// 结尾的NULL字符

	return len;
}


// 读string型变量
//
// pstrbegin	- IN  - 字符串开始地址（即字符串开始的引号的地址）
// pstrend		- OUT - 返回字符串结尾地址（字符串结尾引号后面的地址）
// pvarend		- IN  - 变量结尾地址
// tidytxtbegin	- IN  - 整理缓冲区的写入地址
// tidytxtend	- OUT - 返回新的整理缓冲区写入地址
//
// 返回值：读取成功返回true，失败返回false。
//
// *注：函数会过滤掉原始文本中的转移符。
//		空字符串（“”）是有效的，整理后是0长度的字符串。
// 
bool _gsp_read_string(LPCTSTR pstrbegin, LPCTSTR *pstrend, LPCTSTR pvarend, LPTSTR tidytxtbegin, LPTSTR *tidytxtend)
{
	if (pstrbegin[0] != CT_STR)		// 串的第一个字符必须是引号 "
		return false;

	LPCTSTR	pstr = &pstrbegin[1];
	LPCTSTR	pde  = NULL;

	// 搜索串定义的结尾
	while (pstr=_gsp_strpbrk(pstr, pvarend, CT_STRIDCH))
	{
		if (pstr[-1] == CT_ESCP)
		{
			pstr++;
			continue;				// 忽略转移符
		}
		else
		{
			pde = pstr;
			break;
		}
	}

	if (pde == NULL)				// 串没有结尾的引号
		return false;

	LPDWORD	plen = (LPDWORD)tidytxtbegin;		// 字符串长度值写入地址
	LPTSTR	pbuf = (LPTSTR)(plen+1);			// 字符串写入地址
	LPTSTR	p    = pbuf;

	pstr = &pstrbegin[1];

	// 复制字符串到整理缓冲区，并过滤掉转移符
	for (int i=0; i<(pde-pstr); i++)
	{
		if (pstr[i] == CT_ESCP)
		{
			switch (pstr[i+1])
			{
			case	_T('t'):
				*p++ = CT_TAB;
				i++;
				break;
			case	_T('r'):
				*p++ = CT_CR;
				i++;
				break;
			case	_T('n'):
				*p++ = CT_NL;
				i++;
				break;
			default:
				break;
			}
			continue;
		}
		else
			*p++ = pstr[i];
	}

	// 写入字符串长度值（不包括结尾NULL字符）
	*plen = (DWORD)(p-pbuf);
	// 写入串结尾NULL字符
	*p++ = CT_NULL;
	// 返回原始文本中字符串的结尾引号后面的地址
	*pstrend = &pde[1];
	// 返回整理缓冲区新的写入地址
	*tidytxtend = p;

	return true;
}


// 读date型变量
//
// pstrbegin	- IN  - DATE字符串开始地址（即字符串开始的引号的地址）
// pstrend		- OUT - 返回串结尾地址（字符串结尾引号后面的地址）
// pvarend		- IN  - DATE变量数据的结尾地址
// adtime		- IN  - 类型标识
// dtv			- OUT - 返回日期时间值（UTC秒值）
//
// 返回值：读取成功返回true，失败返回false。
//
// *注：
// 
bool _gsp_read_date(LPCTSTR pstrbegin, LPCTSTR *pstrend, LPCTSTR pvarend, BYTE adtime, __int64* dtv)
{
	if (pstrbegin[0] != CT_STR)		// 串的第一个字符必须是引号 "
		return false;

	LPCTSTR	pstr = &pstrbegin[1];
	LPCTSTR	pde  = NULL;

	// 搜索串定义的结尾
	pde = _gsp_strpbrk(pstr, pvarend, CT_STRIDCH);

	if (pde == NULL)				// 串没有结尾的引号
		return false;

	int			year, mon, day, hour, min, sec, rc;
	TCHAR		buff[256];
	struct tm	tms;
	__int64		dt;

	switch (adtime)
	{
	case	DATETYPE_DATE:			// 只有日期
		// 提取年月日数据
		rc = _stscanf_s(pstr, _T("%d%[^0-9]%d%[^0-9]%d"), 
			&year, 
			buff, 256,
			&mon,
			buff, 256,
			&day);

		// 如果缺少内容，返回false
		if (rc != 5)
			return false;

		// 检查年月日的值是否合法
		if ((year < 1970)||(year > 3000)||(mon < 0)||(mon > 11)||(day < 1)||(day > 31))
			return false;

		// 初始化tm结构
		tms.tm_year		= year-1900;
		tms.tm_mon		= mon-1;
		tms.tm_mday		= day;
		tms.tm_hour		= 0;
		tms.tm_min		= 0;
		tms.tm_sec		= 0;
		tms.tm_wday		= 0;
		tms.tm_yday		= 0; 
		tms.tm_isdst	= 0;

		// 转换为UTC的秒值
		dt = _mktime64(&tms);

		if (dt == (__int64)-1)
			return false;

		break;
	case	DATETYPE_DATE_TIME:		// 日期和时间
		// 提取年月日时分秒数据
		rc = _stscanf_s(pstr, _T("%d%[^0-9]%d%[^0-9]%d%[^0-9]%d%[^0-9]%d%[^0-9]%d"), 
			&year, 
			buff, 256,
			&mon,
			buff, 256,
			&day,
			buff, 256,
			&hour,
			buff, 256,
			&min,
			buff, 256,
			&sec );

		// 如果缺少内容，返回false
		if (rc != 11)
			return false;

		// 检查年月日时分秒的值是否合法
		if ((year < 1970)||(year > 3000)||(mon < 0)||(mon > 11)||(day < 1)||(day > 31)|| \
			(hour < 0)||(hour > 23)||(min < 0)||(min > 59)||(sec < 0)||(sec > 59))
			return false;

		// 初始化tm结构
		tms.tm_year		= year-1900;
		tms.tm_mon		= mon-1;
		tms.tm_mday		= day;
		tms.tm_hour		= hour;
		tms.tm_min		= min;
		tms.tm_sec		= sec;
		tms.tm_wday		= 0;
		tms.tm_yday		= 0; 
		tms.tm_isdst	= 0;

		// 转换为UTC的秒值
		dt = _mktime64(&tms);

		if (dt == (__int64)-1)
			return false;

		break;
	default:
		_ASSERT(0);
		return false;
	}

	// 返回原始文本中日期串的结尾引号后面的地址
	*pstrend = &pde[1];
	// 返回日期时间值
	*dtv = dt;

	return true;
}


// 读struct型变量
//
// pstrbegin	- IN  - 结构字符串开始地址（即‘{’的地址）
// pstrend		- IN  - 结构字符串结尾地址（即‘}’的地址）
// pvartab		- IN  - 结构参数的起始地址
// tidytxtbegin	- IN  - 整理缓冲区的写入地址
// tidytxtend	- OUT - 返回新的整理缓冲区写入地址
// pict			- IN  - 上下文类地址
//
// 返回值：读取成功返回true，失败返回false。
//
// *注：
// 
bool _gsp_read_struct(LPCTSTR pstrbegin, LPCTSTR pstrend, LPCTSTR pvartab, LPTSTR tidytxtbegin, LPTSTR *tidytxtend, _inicontextclass *pict)
{
	if (pstrbegin[0] != CT_STRUCTNB)	// 串的第一个字符必须是结构开始的花括号 ‘{’
	{
		pict->seterror(_ILEM_29);
		return false;
	}

	_ASSERT(pstrend[0] == CT_STRUCTNE);

	// [struct变量标志][变量名字长度][变量名字][内存形式]
	unsigned char *pt = (unsigned char *)pvartab;
	pt += sizeof(DWORD);
	pt += (*((LPDWORD)pt) + 1)*sizeof(TCHAR) + sizeof(DWORD);
	pt += sizeof(DWORD);

	// [结构成员个数][变量表]
	LPDWORD	pd			= (LPDWORD)pt;
	int	varelecnt		= (int)pd[0];				// 获取结构成员个数
	int	vtsize			= (int)pd[1];				// 获取变量表尺寸（字节单位）
	pt					= (LPBYTE)&pd[2];			// 获取变量表起始地址
	LPDWORD	varsersize	= (LPDWORD)tidytxtbegin;
	varsersize[0]		= 0;						// 变量序列尺寸值初始化为0
	LPBYTE	ptidy		= (LPBYTE)&varsersize[1];	// 整理缓冲区写入地址
	LPCTSTR	pde			= (pstrbegin+1);
	LPCTSTR	pdb			= NULL;

	// 根据变量表的指示读取各变量的数据
	for (int k=0; k<varelecnt; k++)
	{
		// 跨过可能存在的空白字符
		if ((pdb=_gsp_strspn(pde, pstrend, GSP_WHITESPC3)) == NULL)
		{
			pict->seterror(_ILEM_30, k+1);
			return false;
		}
		// 找到数值结尾
		if ((pde=_gsp_strpbrk(pdb, pstrend, GSP_WHITESPC3)) == NULL)
			pde = pstrend;

		// 提取成员变量属性值，并根据属性值读取后续数据
		switch ((int)*pt++)
		{
		case	NTP_BYTE:				// byte数据
			{
				unsigned char bte;
				unsigned char scale = *pt++;

				if (_gsp_read_byte(pdb, scale, &bte) == false)
				{
					pict->seterror(_ILEM_31, k+1);	// 显示语法错误信息，k+1是将0基的索引变为1基索引
					return false;
				}

				*ptidy++ = bte;
				varsersize[0] += (int)sizeof(unsigned char);
				break;
			}
		case	NTP_INT:				// int数据
			{
				int				inter;
				unsigned char	scale = *pt++;

				if (_gsp_read_int(pdb, scale, &inter) == false)
				{
					pict->seterror(_ILEM_31, k+1);
					return false;
				}

				*((int*)ptidy) = inter;
				ptidy += sizeof(int);
				varsersize[0] += (int)sizeof(int);
				break;
			}
		case	NTP_BOOL:				// bool数据
			{
				unsigned char bidx;
				unsigned char boolen;

				if (_gsp_read_bool(pdb, pde, &bidx, &boolen) == false)
				{
					pict->seterror(_ILEM_31, k+1);
					return false;
				}

				*ptidy++ = bidx;
				*ptidy++ = boolen;
				varsersize[0] += (int)sizeof(unsigned char)*2;
				break;
			}
		case	NTP_DOUBLE:				// double数据
			{
				double	dou;

				if (_gsp_read_double(pdb, &dou) == false)
				{
					pict->seterror(_ILEM_31, k+1);
					return false;
				}

				*((double*)ptidy) = dou;
				ptidy += sizeof(double);
				varsersize[0] += (int)sizeof(double);
				break;
			}
		case	NTP_STRING:				// string数据
			{
				LPTSTR	ptb = (LPTSTR)ptidy;

				// 读string型变量（并写入整理缓冲区）
				if (_gsp_read_string(pdb, &pde, pstrend, ptb, (LPTSTR*)&ptidy) == false)
				{
					pict->seterror(_ILEM_31, k+1);
					return false;
				}

				varsersize[0] += (int)(ptidy-(unsigned char *)ptb);
				break;
			}
		case	NTP_DATE:				// date数据
			{
				__int64			inter;
				unsigned char	adtime = *pt++;

				if (_gsp_read_date(pdb, &pde, pstrend, adtime, &inter) == false)
				{
					pict->seterror(_ILEM_31, k+1);
					return false;
				}

				*((__int64*)ptidy) = inter;
				ptidy += sizeof(__int64);
				varsersize[0] += (int)sizeof(__int64);
				break;
			}
		case	NTP_ARRAY_BYTE:			// byte[]数组数据
			{
				unsigned char	bte;
				unsigned char	scale = *pt++;
				int				elecnt = *((LPDWORD)pt);
				LPCTSTR			pab;
				LPCTSTR			pae;

				// 搜索数组数据前后标识符“()”
				if (_gsp_i_find_arrayarea(pdb, pstrend, &pab, &pae) == false)
				{
					pict->seterror(_ILEM_32, k+1);
					return false;
				}

				pde = pab+1;

				for (int i=0; i<elecnt; i++)
				{
					// 判断是否存在缺少元素的情况
					if (pde >= pae)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}

					// 跨过数值前面可能存在的空白字符
					if ((pdb=_gsp_strspn(pde, pae, GSP_WHITESPC3)) == NULL)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}
					// 找到数值结尾
					if ((pde=_gsp_strpbrk(pdb, pae, GSP_WHITESPC3)) == NULL)
						pde = pae;

					if (_gsp_read_byte(pdb, scale, &bte) == false)
					{
						pict->seterror(_ILEM_34, k+1, i+1);
						return false;
					}

					*ptidy++ = bte;
				}

				pt += sizeof(DWORD);
				varsersize[0] += elecnt*(int)sizeof(unsigned char);
				pde = pae+1;
				break;
			}
		case	NTP_ARRAY_INT:			// int[]数组数据
			{
				int				inter;
				unsigned char	scale	= *pt++;
				int				elecnt	= *((LPDWORD)pt);
				int*			pint	= (int*)ptidy;
				LPCTSTR			pab;
				LPCTSTR			pae;

				if (_gsp_i_find_arrayarea(pdb, pstrend, &pab, &pae) == false)
				{
					pict->seterror(_ILEM_32, k+1);
					return false;
				}

				pde = pab+1;

				for (int i=0; i<elecnt; i++)
				{
					if (pde >= pae)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}

					if ((pdb=_gsp_strspn(pde, pae, GSP_WHITESPC3)) == NULL)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}
					if ((pde=_gsp_strpbrk(pdb, pae, GSP_WHITESPC3)) == NULL)
						pde = pae;

					if (_gsp_read_int(pdb, scale, &inter) == false)
					{
						pict->seterror(_ILEM_34, k+1, i+1);
						return false;
					}

					*pint++ = inter;
				}

				pt += sizeof(DWORD);
				ptidy = (unsigned char *)pint;
				varsersize[0] += elecnt*(int)sizeof(int);
				pde = pae+1;
				break;
			}
		case	NTP_ARRAY_BOOL:			// bool[]数组数据
			{
				unsigned char	bidx;
				unsigned char	boolen;
				int				elecnt = *((LPDWORD)pt);
				LPCTSTR			pab;
				LPCTSTR			pae;

				if (_gsp_i_find_arrayarea(pdb, pstrend, &pab, &pae) == false)
				{
					pict->seterror(_ILEM_32, k+1);
					return false;
				}

				pde = pab+1;

				for (int i=0; i<elecnt; i++)
				{
					if (pde >= pae)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}

					if ((pdb=_gsp_strspn(pde, pae, GSP_WHITESPC3)) == NULL)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}
					if ((pde=_gsp_strpbrk(pdb, pae, GSP_WHITESPC3)) == NULL)
						pde = pae;

					if (_gsp_read_bool(pdb, pde, &bidx, &boolen) == false)
					{
						pict->seterror(_ILEM_34, k+1, i+1);
						return false;
					}

					*ptidy++ = bidx;
					*ptidy++ = boolen;
				}

				pt += sizeof(DWORD);
				varsersize[0] += elecnt*(int)sizeof(unsigned char)*2;
				pde = pae+1;
				break;
			}
		case	NTP_ARRAY_DOUBLE:		// double[]数组数据
			{
				double			dou;
				int				elecnt	= *((LPDWORD)pt);
				double*			pdou	= (double*)ptidy;
				LPCTSTR			pab;
				LPCTSTR			pae;

				if (_gsp_i_find_arrayarea(pdb, pstrend, &pab, &pae) == false)
				{
					pict->seterror(_ILEM_32, k+1);
					return false;
				}

				pde = pab+1;

				for (int i=0; i<elecnt; i++)
				{
					if (pde >= pae)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}

					if ((pdb=_gsp_strspn(pde, pae, GSP_WHITESPC3)) == NULL)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}
					if ((pde=_gsp_strpbrk(pdb, pae, GSP_WHITESPC3)) == NULL)
						pde = pae;

					if (_gsp_read_double(pdb, &dou) == false)
					{
						pict->seterror(_ILEM_34, i+1);
						return false;
					}

					*pdou++ = dou;
				}

				pt += sizeof(DWORD);
				ptidy = (unsigned char *)pdou;
				varsersize[0] += elecnt*(int)sizeof(double);
				pde = pae+1;
				break;
			}
		case	NTP_ARRAY_STRING:		// string[]数组数据
			{
				LPTSTR	ptb = (LPTSTR)ptidy;
				int		elecnt = *((LPDWORD)pt);
				LPCTSTR	pab;
				LPCTSTR	pae;

				if (_gsp_i_find_arrayarea(pdb, pstrend, &pab, &pae) == false)
				{
					pict->seterror(_ILEM_32, k+1);
					return false;
				}

				pde = pab+1;

				for (int i=0; i<elecnt; i++)
				{
					if (pde >= pae)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}

					if ((pdb=_gsp_strspn(pde, pae, GSP_WHITESPC3)) == NULL)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}
					if (_gsp_read_string(pdb, &pde, pae, ptb, (LPTSTR*)&ptb) == false)
					{
						pict->seterror(_ILEM_34, k+1, i+1);
						return false;
					}
				}

				pt += sizeof(DWORD);
				varsersize[0] += (int)((unsigned char *)ptb-ptidy);
				ptidy = (unsigned char *)ptb;
				pde = pae+1;
				break;
			}
		case	NTP_ARRAY_DATE:			// date[]数组数据
			{
				__int64			inter;
				unsigned char	adtime = *pt++;
				int				elecnt	= *((LPDWORD)pt);
				__int64*		pint	= (__int64*)ptidy;
				LPCTSTR			pab;
				LPCTSTR			pae;

				if (_gsp_i_find_arrayarea(pdb, pstrend, &pab, &pae) == false)
				{
					pict->seterror(_ILEM_32, k+1);
					return false;
				}

				pde = pab+1;

				for (int i=0; i<elecnt; i++)
				{
					if (pde >= pae)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}

					if ((pdb=_gsp_strspn(pde, pae, GSP_WHITESPC3)) == NULL)
					{
						pict->seterror(_ILEM_33, k+1, elecnt, i);
						return false;
					}
					if ((pde=_gsp_strpbrk(pdb, pae, GSP_WHITESPC3)) == NULL)
						pde = pae;

					if (_gsp_read_date(pdb, &pde, pae, adtime, &inter) == false)
					{
						pict->seterror(_ILEM_31, k+1);
						return false;
					}

					*pint++ = inter;
				}

				pt += sizeof(DWORD);
				ptidy = (unsigned char *)pint;
				varsersize[0] += elecnt*(int)sizeof(__int64);
				pde = pae+1;
				break;
			}
		case	NTP_STRUCT:				// 结构中不再允许出现结构变量
		case	NTP_ARRAY_STRUCT:		// 
			{
				pict->seterror(_ILEM_20, k+1);
				return false;
			}
		default:
			_ASSERT(0);
			pict->seterror(_ILEM_4);
			return false;
		}
	}

	*tidytxtend = (LPTSTR)ptidy;

	return true;
}


// 在文本中搜索数组数据的头尾地址
//
// pstrbegin	- IN  - 待搜索文本的开始地址
// pstrend		- IN  - 待搜索文本的结束地址
// pab			- OUT - 返回数组数据的前标识符"("的地址
// pae			- OUT - 返回数组数据的后标识符")"的地址
//
// 返回值：如果成功的在指定文本中找到数组数据标识符“()”，返回true。没找到则返回false。
//
// *注：返回false时，pab和pae指向的变量不会被改写。
//
bool _gsp_i_find_arrayarea(LPCTSTR pstrbegin, LPCTSTR pstrend, LPCTSTR *pab, LPCTSTR *pae)
{
	LPCTSTR pstr	= pstrbegin;
	LPCTSTR pb		= NULL;
	LPCTSTR pe		= NULL;

	// 搜索前缀标识符
	while (pstr=_gsp_strpbrk(pstr, pstrend, CT_ARRYDSNB))
	{
		if (pstr[-1] == CT_ESCP)
		{
			pstr++;
			continue;				// 忽略带转移符的标识符
		}
		else
		{
			pb = pstr;
			break;
		}
	}

	if (pb == NULL) return false;

	// 搜索后缀标识符
	while (pstr=_gsp_strpbrk(pstr, pstrend, CT_ARRYDSNE))
	{
		if (pstr[-1] == CT_ESCP)
		{
			pstr++;
			continue;				// 忽略转移符
		}
		else
		{
			pe = pstr;
			break;
		}
	}

	if (pe == NULL) return false;

	*pab = pb;
	*pae = pe;

	return true;
}


// 在文本中搜索结构数据的头尾地址
//
// pstrbegin	- IN  - 待搜索文本的开始地址
// pstrend		- IN  - 待搜索文本的结束地址
// pab			- OUT - 返回结构数据的前标识符"{"的地址
// pae			- OUT - 返回结构数据的后标识符"}"的地址
//
// 返回值：如果成功的在指定文本中找到结构数据标识符“{}”，返回true。没找到则返回false。
//
// *注：返回false时，pab和pae指向的变量不会被改写。
//
bool _gsp_i_find_structarea(LPCTSTR pstrbegin, LPCTSTR pstrend, LPCTSTR *pab, LPCTSTR *pae)
{
	LPCTSTR pstr	= pstrbegin;
	LPCTSTR pb		= NULL;
	LPCTSTR pe		= NULL;

	// 搜索前缀标识符
	while (pstr=_gsp_strpbrk(pstr, pstrend, CT_STCTDSNB))
	{
		if (pstr[-1] == CT_ESCP)
		{
			pstr++;
			continue;				// 忽略带转移符的标识符
		}
		else
		{
			pb = pstr;
			break;
		}
	}

	if (pb == NULL) return false;

	// 搜索后缀标识符
	while (pstr=_gsp_strpbrk(pstr, pstrend, CT_STCTDSNE))
	{
		if (pstr[-1] == CT_ESCP)
		{
			pstr++;
			continue;				// 忽略转移符
		}
		else
		{
			pe = pstr;
			break;
		}
	}

	if (pe == NULL) return false;

	*pab = pb;
	*pae = pe;

	return true;
}


// 获取str串在pp串数组中的索引
//
// str		- IN - 待判断的字符串
// pp		- IN - 数个字符串的地址（字典）
// cntpp	- IN - 字典中字符串的个数
//
// 返回值：如果成功，返回str在pp中的索引值（0~n），如果pp中没有str，返回-1。
//
// *注：函数对大小写敏感。
//
int _gsp_find_iniparamstr(LPCTSTR str, LPCTSTR *pp, int cntpp)
{
	_ASSERT(str&&pp&&cntpp);

	for (int i=0; i<cntpp; i++)
	{
		if (_tcscmp(str, pp[i]) == 0)
		{
			return i;
		}
	}

	return -1;
}


// 将日期时间值转换为字符串
//
// dt		- IN  - __int64型日期时间值（UTC秒)
// adtime	- IN  - 类型标识
// buff		- OUT - 调用者提供的缓冲区
// buffsize	- IN  - 缓冲区尺寸（字符单位）
//
// 返回值：
//
// *注：
// 
bool _gsp_cnvdt_to_str(__int64 dt, BYTE adtime, LPTSTR buff, size_t buffsize)
{
	struct tm	tms;

	_ASSERT(dt > (__int64)0);

	// 转换为可视日期时间信息
	_localtime64_s(&tms, &dt);

	switch (adtime)
	{
	case	DATETYPE_DATE:			// 只有日期
		_ASSERT(buffsize > 12);
		_tcsftime(buff, buffsize, _T("\"%Y-%m-%d\""), &tms);
		break;
	case	DATETYPE_DATE_TIME:		// 日期和时间
		_ASSERT(buffsize > 21);
		_tcsftime(buff, buffsize, _T("\"%Y-%m-%d %H:%M:%S\""), &tms);
		break;
	default:
		_ASSERT(0);
		return false;
	}

	return true;
}


// 获取结构字节对齐的默认值
//
// 返回值：返回编译器的结构字节对齐值(vc-/Zpn)
//
// *注：函数只会返回1、2、4、8这四个值中的一个，大于8的对齐值会被当做8返回
//
int _gsp_get_sutali_value()
{
	return (int)offsetof(SUTSIZECAL, _end);
}


// 获取结构的各成员的存放偏移量
//
// pvtab		- IN - 结构的变量表数据
// sutmemcnts	- IN - 结构中成员的个数
// poff			- IN - 回写各成员偏移值的ini型数组（由调用者提供，尺寸必须大于等于结构成员个数）
//
// 返回值：返回结构自身占用的字节数，如果预编译数据错误，返回-1。
// 
// *注：pvtab是从变量表首部的尺寸值处开始的
//
int _gsp_get_sutoffset_data(LPCTSTR pvtab, int sutmemcnts, int *poff)
{
	int			*pcnt = (int*)pvtab;
	int			alivalue = _gsp_get_sutali_value();			// 编译器默认的字节对齐值
	int			vtsize, nextpos, tmpali, maxmem;
	LPBYTE		pvt;

	vtsize		= pcnt[0];				// 变量表尺寸（字节单位）
	pvt			= (LPBYTE)&pcnt[1];		// 变量表首地址
	nextpos		= 0;					// 下一个成员的可能的写入位置
	maxmem		= 0;					// 最大成员的类型尺寸

	for (int i=0; i<sutmemcnts; i++)
	{
		switch (pvt[0])
		{
		case	NTP_BYTE:				// byte数据
		case	NTP_INT:				// int数据
		case	NTP_DATE:				// date数据
			tmpali  = min(alivalue,INIDATATYPELENGTH[pvt[0]]);		// 比较默认对齐值与类型尺寸，取小的值
			nextpos = ((nextpos+tmpali-1)/tmpali)*tmpali;			// 计算当前变量的存放偏移
			poff[i] = nextpos;
			nextpos += INIDATATYPELENGTH[pvt[0]];					// 计算下一个变量可能的存放偏移
			if (INIDATATYPELENGTH[pvt[0]] > maxmem)					// 记录结构中最大的成员的类型尺寸
				maxmem = INIDATATYPELENGTH[pvt[0]];
			pvt += sizeof(BYTE)*2;									// 跳转预编译指针
			break;
		case	NTP_BOOL:				// bool数据
		case	NTP_DOUBLE:				// double数据
		case	NTP_STRING:				// string数据
			tmpali  = min(alivalue,INIDATATYPELENGTH[pvt[0]]);
			nextpos = ((nextpos+tmpali-1)/tmpali)*tmpali;
			poff[i] = nextpos;
			nextpos += INIDATATYPELENGTH[pvt[0]];
			if (INIDATATYPELENGTH[pvt[0]] > maxmem)
				maxmem = INIDATATYPELENGTH[pvt[0]];
			pvt++;
			break;
		case	NTP_ARRAY_BYTE:			// byte[]数组数据
		case	NTP_ARRAY_INT:			// int[]数组数据
		case	NTP_ARRAY_DATE:			// date[]数组数据
			tmpali  = min(alivalue,INIDATATYPELENGTH[(int)pvt[0]-NTP_ARRAY_BYTE]);
			nextpos = ((nextpos+tmpali-1)/tmpali)*tmpali;
			poff[i] = nextpos;
			nextpos += INIDATATYPELENGTH[(int)pvt[0]-NTP_ARRAY_BYTE]*(*(int*)&pvt[2]);
			if (INIDATATYPELENGTH[(int)pvt[0]-NTP_ARRAY_BYTE] > maxmem)
				maxmem = INIDATATYPELENGTH[(int)pvt[0]-NTP_ARRAY_BYTE];
			pvt += sizeof(BYTE)*2 + sizeof(int);
			break;
		case	NTP_ARRAY_BOOL:			// bool[]数组数据
		case	NTP_ARRAY_DOUBLE:		// double[]数组数据
		case	NTP_ARRAY_STRING:		// string[]数组数据
			tmpali  = min(alivalue,INIDATATYPELENGTH[(int)pvt[0]-NTP_ARRAY_BYTE]);
			nextpos = ((nextpos+tmpali-1)/tmpali)*tmpali;
			poff[i] = nextpos;
			nextpos += INIDATATYPELENGTH[(int)pvt[0]-NTP_ARRAY_BYTE]*(*(int*)&pvt[1]);
			if (INIDATATYPELENGTH[(int)pvt[0]-NTP_ARRAY_BYTE] > maxmem)
				maxmem = INIDATATYPELENGTH[(int)pvt[0]-NTP_ARRAY_BYTE];
			pvt += sizeof(BYTE) + sizeof(int);
			break;
		default:
			_ASSERT(0);
			return -1;
		}
	}

	// 计数结构体的尺寸
	tmpali = min(alivalue, maxmem);
	nextpos = ((nextpos+tmpali-1)/tmpali)*tmpali;

	return nextpos;
}


// 创建一个结构的实体
//
// pvpt		- IN/OUT - 结构的各个参数
// pdat		- IN     - 结构的变量序列起始地址（不含变量序列尺寸)
//
// 返回值：0-创建成功，-1 - 申请内存失败，-3 - 系统异常
//
// *注：如果函数失败(返回-1、-2)，调用者必须负责释放本函数已经申请的一些内存。
//		结构体内存由调用者负责申请。
//		入口参数pvpt中的各数据必须被调用者填写好。
//
int _gsp_get_sutbody(LPILVT_STRUCT pvpt, LPCTSTR pdat)
{
	LPBYTE				pvt = (LPBYTE)&((int*)pvpt->vartable)[1];	// 跨过变量表尺寸
	LPBYTE				pb  = pvpt->pvalue;
	LPBYTE				pbd = (LPBYTE)pdat;
	LPBYTE				pbtmp;
	int*				pitmp;
	unsigned short*		putmp;
	double*				pdtmp;
	LPTSTR				pstmp;
	__int64*			pi64tmp;
	LPTSTR*				ppstmp;

	int					elecnts, i, k, len, smc;

	_ASSERT(pvpt->pvalue);

	// 初始化结构体内存
	memset((void*)pvpt->pvalue, 0, (size_t)pvpt->sutsize);

	// 对于链表形式的结构，必须忽略结尾的两个变量。它们是库自身添加的指针变量，并没有实际对应的预编译数据
	if (pvpt->memtype == ILSUTMEMTPE_LIST)
		smc = pvpt->sutmemcnt - 2;
	else
		smc = pvpt->sutmemcnt;

	// 读入变量值
	for (i=0; i<smc; i++)
	{
		switch (pvt[0])
		{
		case	NTP_BYTE:				// byte数据
			pb[pvpt->varoffset[i]] = *pbd;
			pvt += sizeof(BYTE)*2;
			pbd++;
			break;
		case	NTP_INT:				// int数据
			*(int*)&pb[pvpt->varoffset[i]] = *(int*)pbd;
			pvt += sizeof(BYTE)*2;
			pbd += sizeof(int);
			break;
		case	NTP_BOOL:				// bool数据
			*(unsigned short *)&pb[pvpt->varoffset[i]] = *(unsigned short *)pbd;
			pvt ++;
			pbd += sizeof(unsigned short);
			break;
		case	NTP_DOUBLE:				// double数据
			*(double*)&pb[pvpt->varoffset[i]] = *(double*)pbd;
			pvt ++;
			pbd += sizeof(double);
			break;
		case	NTP_STRING:				// string数据
			len = *(int*)pbd;
			if ((pstmp = new TCHAR[len+1]) == NULL)
				return -1;
			_tcscpy_s(pstmp, len+1, (LPCTSTR)&(((int*)pbd)[1]));
			*(LPTSTR*)&pb[pvpt->varoffset[i]] = pstmp;
			//
			// 结构中的string变量类型将不保留预编译数据中的串长度值，回写文件时由代码重新计算串长度。
			// 
			pvt++;
			pbd += sizeof(int) + sizeof(TCHAR)*(len+1);
			break;
		case	NTP_DATE:				// date数据
			*(__int64*)&pb[pvpt->varoffset[i]] = *(__int64*)pbd;
			pvt += sizeof(BYTE)*2;
			pbd += sizeof(__int64);
			break;
		case	NTP_ARRAY_BYTE:			// byte[]数组数据
			elecnts = *(int*)&pvt[2];
			pbtmp = &pb[pvpt->varoffset[i]];
			
			for (k=0; k<elecnts; k++)
			{
				pbtmp[k] = pbd[k];
			}

			pvt += sizeof(BYTE)*2 + sizeof(int);
			pbd += sizeof(BYTE)*elecnts;
			break;
		case	NTP_ARRAY_INT:			// int[]数组数据
			elecnts = *(int*)&pvt[2];
			pitmp = (int*)&pb[pvpt->varoffset[i]];
			
			for (k=0; k<elecnts; k++)
			{
				pitmp[k] = ((int*)pbd)[k];
			}

			pvt += sizeof(BYTE)*2 + sizeof(int);
			pbd += sizeof(int)*elecnts;
			break;
		case	NTP_ARRAY_BOOL:			// bool[]数组数据
			elecnts = *(int*)&pvt[1];
			putmp = (unsigned short *)&pb[pvpt->varoffset[i]];
			
			for (k=0; k<elecnts; k++)
			{
				putmp[k] = ((unsigned short *)pbd)[k];
			}

			pvt += sizeof(BYTE) + sizeof(int);
			pbd += sizeof(unsigned short)*elecnts;
			break;
		case	NTP_ARRAY_DOUBLE:		// double[]数组数据
			elecnts = *(int*)&pvt[1];
			pdtmp = (double*)&pb[pvpt->varoffset[i]];
			
			for (k=0; k<elecnts; k++)
			{
				pdtmp[k] = ((double*)pbd)[k];
			}

			pvt += sizeof(BYTE) + sizeof(int);
			pbd += sizeof(double)*elecnts;
			break;
		case	NTP_ARRAY_STRING:		// string[]数组数据
			elecnts = *(int*)&pvt[1];
			ppstmp = (LPTSTR*)&pb[pvpt->varoffset[i]];
			
			for (k=0; k<elecnts; k++)
			{
				len = *(int*)pbd;
				if ((pstmp = new TCHAR[len+1]) == NULL)
					return -1;
				_tcscpy_s(pstmp, len+1, (LPCTSTR)&(((int*)pbd)[1]));
				ppstmp[k] = pstmp;
				pbd += sizeof(int) + sizeof(TCHAR)*(len+1);
			}

			pvt += sizeof(BYTE) + sizeof(int);
			break;
		case	NTP_ARRAY_DATE:			// date[]数组数据
			elecnts = *(int*)&pvt[2];
			pi64tmp = (__int64*)&pb[pvpt->varoffset[i]];
			
			for (k=0; k<elecnts; k++)
			{
				pi64tmp[k] = ((__int64*)pbd)[k];
			}

			pvt += sizeof(BYTE)*2 + sizeof(int);
			pbd += sizeof(__int64)*elecnts;
			break;
		default:
			_ASSERT(0);
			return -3;	// 系统异常
		}
	}

	return 0;
}


// 释放结构体中的内存
// 
// pvpt		- IN - 结构参数
//
// 返回值：0-释放成功，-2 - 系统异常
//
// *注：本函数释放结构体中的外链内存（比如string类型的缓冲区）。
//		函数最后会将结构体内存清空为0内容，但不会释放结构体自身。
//
int _gsp_i_freesutbody(LPILVT_STRUCT pvpt)
{
	LPBYTE				pvt = (LPBYTE)&((int*)pvpt->vartable)[1];	// 跨过变量表尺寸
	LPBYTE				pb  = pvpt->pvalue;
	LPTSTR				pstmp;
	LPTSTR*				ppstmp;
	int					elecnts, i, k, smc;

	// 对于链表形式的结构，必须忽略结尾的两个变量。它们是库自身添加的指针变量，并没有需要释放的外链内存。
	if (pvpt->memtype == ILSUTMEMTPE_LIST)
		smc = pvpt->sutmemcnt - 2;
	else
		smc = pvpt->sutmemcnt;

	for (i=0; i<smc; i++)
	{
		switch (pvt[0])
		{
		case	NTP_BYTE:				// byte数据
		case	NTP_INT:				// int数据
		case	NTP_DATE:				// date数据
			pvt += sizeof(BYTE)*2;
			break;
		case	NTP_BOOL:				// bool数据
		case	NTP_DOUBLE:				// double数据
			pvt ++;
			break;
		case	NTP_STRING:				// string数据
			pstmp = *(LPTSTR*)&pb[pvpt->varoffset[i]];
			if (pstmp)
				delete []pstmp;
			pvt++;
			break;
		case	NTP_ARRAY_BYTE:			// byte[]数组数据
		case	NTP_ARRAY_INT:			// int[]数组数据
		case	NTP_ARRAY_DATE:			// date[]数组数据
			pvt += sizeof(BYTE)*2 + sizeof(int);
			break;
		case	NTP_ARRAY_BOOL:			// bool[]数组数据
		case	NTP_ARRAY_DOUBLE:		// double[]数组数据
			pvt += sizeof(BYTE) + sizeof(int);
			break;
		case	NTP_ARRAY_STRING:		// string[]数组数据
			elecnts = *(int*)&pvt[1];
			ppstmp = (LPTSTR*)&pb[pvpt->varoffset[i]];
			
			for (k=0; k<elecnts; k++)
			{
				if (ppstmp[k])
					delete []ppstmp[k];
			}

			pvt += sizeof(BYTE) + sizeof(int);
			break;
		default:
			_ASSERT(0);
			return -2;	// 系统异常
		}
	}

	// 清除数据
	memset((void*)pvpt->pvalue, 0, (size_t)pvpt->sutsize);

	return 0;
}



