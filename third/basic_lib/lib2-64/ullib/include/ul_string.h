/**
 * @file
 * @brief �ַ����ַ�������
 */
#ifndef _UL_MODULE_STRING_
#define _UL_MODULE_STRING_

extern int UL_CHAR_SPACE[];
extern char legal_char_set[];
extern char url_eng_set[];
extern char legal_word_set[];

/**
 *  �жϸ������ַ��Ƿ�Ϊ�ո�\n
 *  �ַ�����ŵ�dststr�У�srcstr���ֲ���
 *  
 *  @param[in]  ch �������ַ�
 *  @return �����жϽ��
 *  - 0 ��ʾ���ǿո�
 *  - 1 ��ʾ�ǿո�
 *  @note ע��space�ַ�����'\0',���뺯��isspace��ͬ
 */
#define ul_isspace(ch) UL_CHAR_SPACE[(unsigned char)(ch)]

/**
 *  ���ַ����еĴ�д��ĸת��ΪСд��ĸ
 *  
 *  @param[in]  pstr ��ת�����ַ���
 *  @param[out] pstr ת������ַ���
 *  @return ���ش�����
 *  - 0 ��ʾת��ʧ��
 *  - 1 ��ʾת���ɹ�
 *  @note �������ת��gbk�ַ���,gbk���ʿ��ܱ��޸�,����,�������ĸ���ul_trans2lower
 */
int ul_tolowerstr_singlebuf(unsigned char *pstr);

/**
 *  ����д��ĸת��ΪСд��ĸ���Ǵ�д��ĸ�򷵻���ĸ����
 *  
 *  @param[in]  srcchar ��ת�����ַ�
 *  @param[out] ��
 *  @return ����ת������ַ�
 */
int ul_tolower(unsigned char srcchar);

/**
 *  ���ַ����еĴ�д��ĸת��ΪСд������ת�����\n
 *  �ַ�����ŵ�dststr�У�srcstr���ֲ���
 *  
 *  @param[in]  srcstr ��ת�����ַ���
 *  @param[in]  dststr ���ת������ַ����Ļ�����
 *  @param[out] dststr ת������ַ���
 *  @return ���ش�����
 *  - 0 ��ʾת��ʧ��
 *  - 1 ��ʾת���ɹ�
 *  @note ע��dststr�Ĵ�С>=strlen(srcstr)+1
 */
int ul_tolowerstr(unsigned char *dststr, unsigned char *srcstr);

/**
 *  ���ַ�����ȡ����һ�����ʣ���������һ�����ʵ�λ��
 *  
 *  @param[in]  ps_src    Դ�ַ���
 *  @param[in]  ps_result �õ��ĵ���
 *  @param[in]  charset   ת��ʹ�õľ���
 *  @param[out] ��
 *  @return ��һ�����ʵ�λ��
 */
char *ul_sgetw(char *ps_src, char *ps_result, char *charset);

/**
 *  ���ַ�����ȡ����һ�����ʣ���������һ�����ʵ�λ�ã�֧��˫�ֽڱ���
 *  
 *  @param[in]  ps_src    Դ�ַ���
 *  @param[in]  ps_result �õ��ĵ���
 *  @param[in]  charset   ת��ʹ�õľ���
 *  @param[out] ��
 *  @return ��һ���ַ�����λ��
 */
char *ul_sgetw_ana(char *ps_src, char *ps_result, char *charset);


/**
 *  �����ִ�Сд�ַ����Ƚ�(����mapʵ��)
 *  
 *  @param[in]  s1 �ַ���1 
 *  @param[in]  s2 �ַ���2
 *  @param[in]  n  ���Ƚϳ���
 *  @param[out] ��
 *  @return �Ƿ����
 * - 1 ���
 * - 0 �����
 */
int ul_strncasecmp(const char *s1, const char *s2, size_t n);

/**
 * @brief 	The strlcpy() function copies up to size - 1 characters 
 * 			from the NUL-terminated string src to dst, NULL-terminating the result.
 *
 * @param [out] dst   : char*	Ŀ�껺����
 * @param [int] src   : const char*	src Դ������
 * @param [in] siz   : size_t Ŀ�껺�����Ĵ�С
 * @return  size_t ����Ӧ���������ַ������ȣ�ע����ܻᳬ����������С
 * @retval   
 * @see 
 * @author xiaowei
 * @date 2007/12/03 21:20:35
**/
size_t ul_strlcpy(char *dst, const char *src, size_t siz);

/**
 * @brief  	appends the NUL-terminated string src to the end of dst.  
 * 			It will append at most size - strlen(dst) - 1 bytes, NUL-terminating the result.
 *
 * @param [out] dst   : char*	destination buffer
 * @param [in] src   : const char*		
 * @param [in] siz   : size_t	
 * @return  size_t ����Ӧ���������ַ������ȣ�ע����ܻᳬ����������С
 * @retval   
 * @see 
 * @author xiaowei
 * @date 2007/12/03 21:28:37
**/
size_t ul_strlcat(char *dst, const char *src, size_t siz);

#endif
