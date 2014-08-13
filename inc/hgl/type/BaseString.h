#ifndef HGL_TYPE_BASE_STRING_INCLUDE
#define HGL_TYPE_BASE_STRING_INCLUDE

#include<hgl/type/StringInstance.h>
#include<hgl/CompOperator.h>
#include<hgl/Str.h>
#include<hgl/VectorMath.h>
namespace hgl
{
	/**
	* �ַ�������
	*/
	template<typename T> class BaseString															///�ַ�������
	{
	protected:

		typedef BaseString<T>			SelfClass;
		typedef StringInstance<T>		InstClass;
		typedef SharedPtr<InstClass>	SharedClass;

		SharedClass data;																			///<�ַ�������ʵ��

	public:

		BaseString()HGL_DEFAULT_MEMFUNC;

		BaseString(const T *str)
		{
			Set(str);
		}

		/**
		* ����һ��Cָ�����ַ������õ�ǰ�ַ�������
		* @param str �ַ������ݣ���len<0������£�����0Ϊ��β
		* @param len �ַ������ȣ����str��0Ϊ��β������Ϊ��ֵ���������Զ����㳤��
		* @param one_instance �Ƿ������һ��ʵ��������ǽ�������������Ƕ���ֱ��ʹ�ô�ֵ������delete[]�ͷ�
		*/
		BaseString(const T *str,int len,bool one_instance=false)
		{
			Set(str,len,one_instance);
		}

		BaseString(const InstClass &si)
		{
			if((&si)==nullptr)return;

			data=si;
		}

		BaseString(const SelfClass &bs)
		{
			if((&bs)==nullptr)return;

			Set(bs);
		}

		BaseString(const T ch)
		{
			Set(&ch,1,false);
		}

#define BASE_STRING_NUMBER_CONSTRUCT(type,func)	BaseString(const type num)	\
		{	\
			Set(func(new T[8*sizeof(type)],8*sizeof(type),num),-1,true);	\
		}

//		BASE_STRING_NUMBER_CONSTRUCT(size_t,utos);
		BASE_STRING_NUMBER_CONSTRUCT(int,	itos);
		BASE_STRING_NUMBER_CONSTRUCT(uint,	utos);
		BASE_STRING_NUMBER_CONSTRUCT(int64,	itos);
		BASE_STRING_NUMBER_CONSTRUCT(uint64,utos);

		BASE_STRING_NUMBER_CONSTRUCT(float,	ftos);
		BASE_STRING_NUMBER_CONSTRUCT(double,ftos);

#undef BASE_STRING_NUMBER_CONSTRUCT

		BaseString(const int *value,int N)
		{
			const int size=N*sizeof(int)*8;
			int len;

			T tmp_str[size];

			itos(tmp_str,size,*value);
			++value;

			for(int i=1;i<N;i++)
			{
				strcat(tmp_str,(const T)(','));
				len=strlen(tmp_str);
				itos(tmp_str+len,size-len,*value);
				++value;
			}

			Set(tmp_str);
		}

		BaseString(const float *value,int N)
		{
			const int size=N*sizeof(float)*16;
			int len;

			T tmp_str[size];

			ftos(tmp_str,size,*value);
			++value;

			for(int i=1;i<N;i++)
			{
				len=strlen(tmp_str);
				tmp_str[len++]=',';
				ftos(tmp_str+len,size-len,*value);
				++value;
			}

			Set(tmp_str);
		}

		BaseString(const Vector2f &v,int n=2):BaseString((const float *)&v,2){}
		BaseString(const Vector3f &v,int n=3):BaseString((const float *)&v,3){}
		BaseString(const Vector4f &v,int n=4):BaseString((const float *)&v,4){}

		virtual ~BaseString()
		{
		}

		const T GetEndChar()const																	///<ȡ�õ�ǰ�ַ������һ���ַ�
		{
			if(!this)return(0);
			return(data.valid()?data->GetEndChar():0);
		}

		const int Length()const																		///<��ǰ�ַ�������
		{
			if(!this)return(0);
			return(data.valid()?data->GetLength():0);
		}

		const bool IsEmpty()const																	///<��ǰ�ַ����Ƿ�յ�
		{
			if(!this)return(true);
			return(data.valid()?data->GetLength()<=0:true);
		}

		/**
		* ȡ��һ��C�����ַ���ָ��,ʧ�ܷ���NULL
		*/
		T *c_str()const
		{
			if(!this)return(nullptr);
			return(data.valid()?data->c_str():nullptr);
		}

		/**
		* �ҵ�ָ���ַ���������һ��C�����ַ���ָ��
		*/
		T *strchr(T ch)const
		{
			if(!this)return(nullptr);
			return(data.valid()?data->c_str()+FindChar(ch):nullptr);
		}

		/**
		* ��ĩβ�����ַ���������һ��C�����ַ���ָ��
		*/
		T *strrchr(T ch)const
		{
			if(!this)return(nullptr);
			return(data.valid()?data->c_str()+FindRightChar(ch):nullptr);
		}

		/**
		* ����һ��Cָ�����ַ������õ�ǰ�ַ�������
		* @param str �ַ������ݣ���len<0������£�����0Ϊ��β
		* @param len �ַ������ȣ����str��0Ϊ��β������Ϊ��ֵ���������Զ����㳤��
		* @param one_instance �Ƿ������һ��ʵ��������ǽ�������������Ƕ���ֱ��ʹ�ô�ֵ������delete[]�ͷ�
		*/
		void Set(const T *str,int len=-1,bool one_instance=false)
		{
			if(!str||!*str||!len)		//len=-1Ϊ�Լ��,Ϊ0������
			{
				Clear();
				return;
			}

			data=new InstClass(str,len,one_instance);
		}

		void Strcpy(const T *str,int len=-1,bool one=false)
		{
			Set(str,len,one);
		}

		/**
		* ���õ�ǰ�ַ���������
		* @param bs ������Դ�ַ���
		*/
		void Set(const SelfClass &bs)
		{
			if((&bs)==nullptr)
			{
				Clear();
				return;
			}

			data=bs.data;
		}

		/**
		* ���õ�ǰ�ַ��е�����
		*/
		void Set(const InstClass &si)
		{
			if((&si)==nullptr)
			{
				Clear();
				return;
			}

			data=si;
		}

		/**
		* ���õ�ǰ�ַ����е�����
		*/
		void Set(const SharedClass &spsi)
		{
			if((&spsi)==nullptr)
			{
				Clear();
				return;
			}

			data=spsi;
		}

		/**
		* �����ַ���������
		*/
		bool Set(const SelfClass &bs,int start,int count)
		{
			if(count<=0)return(false);

			if((&bs)==nullptr)
				return(false);

			data=bs.data->CreateCopy(start,count);
			return(true);
		}

		/**
		* �Ͽ�������BaseString���õ����������һ�����е�ʵ��
		*/
		bool Unlink()
		{
			if(!data.valid())
				return(false);

			if(data.only())		//�Լ�����
				return(true);

			data=data->CreateCopy();
			return(true);
		}

		/**
		* ȡָ�����������ַ�
		* @param n ����ƫ��
		* @param ch �ַ���ű���
		* @return �Ƿ��ȡ�ɹ�
		*/
		bool GetChar(int n,T &ch)const
		{
			if(n<0)return(false);

			if(!data.valid())
				return(false);

			return data->GetChar(n,ch);
		}

		/**
		* �趨ָ�����������ַ�
		* @param n ����ƫ��
		* @param ch �ַ�
		* @return �Ƿ����óɹ�
		*/
		bool SetChar(int n,const T ch)
		{
			if(n<0)return(false);

			if(!Unlink())
				return(false);

			return data->SetChar(n,ch);
		}

		/**
		* ��ָ��ƫ�ƴ�����ָ�����ȵ��ַ���
		* @param pos Ҫ�����λ��
		* @param str Ҫ������ַ���
		* @param len Ҫ������ַ�����,���Ϊ-1���Զ�����ַ�������
		*/
		bool Insert(int pos,const T *str,int len=-1)
		{
			if(len==0)return(false);

			if(data.valid()&&Unlink())
			{
				if(len<0)
					len=strlen(str);

				return data->Insert(pos,str,len);
			}
			else
			{
				Set(str,len);
				return(true);
			}
		}

		/**
		* ��ָ��ƫ�ƴ������ַ���
		* @param pos Ҫ�����λ��
		* @param str Ҫ������ַ���
		*/
		bool Insert(int pos,const SelfClass &str)
		{
			if((&str)==nullptr)
				return(false);

			return Insert(pos,str.c_str(),str.Length());
		}

		bool Strcat(const T *str,int len)
		{
			if(!str||!*str||len==0)return(false);

			return Insert(Length(),str,len);
		}

		/**
		* ׷��һ���ַ�������ǰ�ַ�������
		*/
		bool Strcat(const SelfClass &bs)
		{
			if((&bs)==nullptr)
				return(false);

			return Insert(Length(),bs);
		}

		/**
		* ��ָ��λ��ɾ��ָ�����ַ�
		* @param pos Ҫɾ������ʼλ��
		* @param num Ҫɾ�����ַ�����
		* @return �Ƿ�ɹ�
		*/
		bool Delete(int pos,int num)
		{
			if(pos<0||num<=0)return(false);

			if(data.valid()&&Unlink())
				return data->Delete(pos,num);

			return(false);
		}

		/**
		* ������е��ַ�������
		*/
		void Clear()
		{
			data.unref();
		}

		/**
		* ��һ���ַ������бȽ�
		* @param bs �Ƚ��ַ���
		* @return <0 ����С
		* @return 0 ��ͬ
		* @return >0 �����
		*/
		int Comp(const SelfClass &bs)const
		{
			if(!data.valid())
				return(bs.Length());

			if(bs.Length()<=0)
				return Length();

			return data->Comp(bs.data.get());
		}

		/**
		* ��һ���ַ������бȽ�
		* @param bs �Ƚ��ַ���
		* @return <0 ����С
		* @return 0 ��ͬ
		* @return >0 �����
		*/
		int Comp(const T *str)const
		{
			if(!data.valid())
			{
				if(!str)
					return 0;
				else
					return *str;
			}

			return data->Comp(str);
		}

		/**
		* ��һ���ַ������бȽ�
		* @param pos ��ʼλ��
		* @param bs �Ƚ��ַ���
		* @return <0 ����С
		* @return 0 ��ͬ
		* @return >0 �����
		*/
		int Comp(const int pos,const SelfClass &bs)const
		{
			if(!data.valid())
				return(bs.Length());

			if(bs.Length()<=0)
				return Length();

			return data->Comp(pos,bs.data.get());
		}

		/**
		* ��һ���ַ������бȽ�
		* @param bs �Ƚ��ַ���
		* @return <0 ����С
		* @return 0 ��ͬ
		* @return >0 �����
		*/
		int Comp(const int pos,const T *str)const
		{
			if(!data.valid())
			{
				if(!str)
					return 0;
				else
					return *str;
			}

			return data->Comp(pos,str);
		}

		/**
		* ����һ���ַ������бȽ�,Ӣ�Ĳ����ִ�Сд
		* @param bs �Ƚ��ַ���
		* @return <0 ����С
		* @return 0 ��ͬ
		* @return >0 �����
		*/
		int CaseComp(const SelfClass &bs)const
		{
			if(!data.valid())
				return(bs.Length());

			if(bs.Length()<=0)
				return 1;

			return data->CaseComp(bs.data->c_str());
		}

		/**
		* ����һ���ַ������бȽ�,Ӣ�Ĳ����ִ�Сд
		* @param bs �Ƚ��ַ���
		* @return <0 ����С
		* @return 0 ��ͬ
		* @return >0 �����
		*/
		int CaseComp(const T *str)const
		{
			if(!data.valid())
			{
				if(!str)
					return 0;

				return *str;
			}

			return data->CaseComp(str);
		}

		/**
		* ����һ���ַ����Ƚ�ָ�ֳ��ȵ��ַ�
		* @param bs �Ƚ��ַ���
		* @param num �Ƚ�����
		* @return <0 ����С
		* @return 0 ��ͬ
		* @return >0 �����
		*/
		int Comp(const SelfClass &bs,const int num)const
		{
			if(!data.valid())
			{
				if(num<=0)return 0;

				return bs.Length();
			}

			if(bs.Length<=0)
				return 1;

			return data->Comp(bs.data->c_str(),num);
		}

		/**
		* ����һ���ַ����Ƚ�ָ�ֳ��ȵ��ַ�
		* @param str �Ƚ��ַ���
		* @param num �Ƚ�����
		* @return <0 ����С
		* @return 0 ��ͬ
		* @return >0 �����
		*/
		int Comp(const T *str,const int num)const
		{
			if(!data.valid())
			{
				if(!str||num<=0)
					return 0;

				return *str;
			}

			return data->Comp(str,num);
		}

		/**
		 * ����һ���ַ����Ƚ�ָ�ֳ��ȵ��ַ�,Ӣ�Ĳ����ִ�Сд
		 * @param bs �Ƚ��ַ���
		 * @param num �Ƚ�����
		 * @return <0 ����С
		 * @return 0 ��ͬ
		 * @return >0 �����
		 */
		int CaseComp(const SelfClass &bs,const int num)const
		{
			if(!data.valid())
			{
				if(num<=0)return 0;

				return bs.Length();
			}

			if(bs.Length<=0)
				return 1;

			return data->CaseComp(bs.data->c_str(),num);
		}

		/**
		 * ����һ���ַ����Ƚ�ָ�ֳ��ȵ��ַ�,Ӣ�Ĳ����ִ�Сд
		 * @param str �Ƚ��ַ���
		 * @param num �Ƚ�����
		 * @return <0 ����С
		 * @return 0 ��ͬ
		 * @return >0 �����
		 */
		int CaseComp(const T *str,const int num)const
		{
			if(!data.valid())
			{
				if(!str||num<=0)
					return 0;

				return *str;
			}

			return data->CaseComp(str,num);
		}
	public:

		bool ToBool(bool &result)const																///<�������е��ַ���ת���ɲ�����ֵ������
		{
			return data.valid()?stob(data->c_str(),result):false;
		}

		template<typename I>
		bool ToInt(I &result)const																	///<�������е��ַ���ת����������ֵ������
		{
			return data.valid()?etof(data->c_str(),result):false;
		}

		template<typename U>
		bool ToUint(U &result)const																	///<�������е��ַ���ת����������ֵ������
		{
			return data.valid()?etof(data->c_str(),result):false;
		}

		template<typename F>
		bool ToFloat(F &result)const																///<�������е��ַ���ת���ɸ�����ֵ������
		{
			return data.valid()?etof(data->c_str(),result):false;
		}

		/**
		* ����ǰ�ַ���ȫ��תΪСд
		* @return ת����ĵ�ǰ�ַ���
		*/
		SelfClass &LowerCase()																		    ///<�������е���ĸȫ��תΪСд
		{
			if(data.valid()&&Unlink())
				tolower(data->c_str());

			return(*this);
		}

		/**
		* ����ǰ�ַ���ȫ��תΪСд
		* @return ת������ַ���
		*/
		SelfClass &ToLowerCase()																		///<�������е���ĸȫ��תΪСд
		{
			if(!data.valid())
				return SelfClass();

			return SelfClass(data->c_str()).LowerString();
		}

		/**
		* ����ǰ�ַ���ȫ��תΪ��д
		* @return ת����ĵ�ǰ�ַ���
		*/
		SelfClass &UpperCase()																		///<�������е���ĸȫ��תΪ��д
		{
			if(data.valid()&&Unlink())
				toupper(data->c_str());

			return(*this);
		}

		/**
		* ����ǰ�ַ���ȫ��ת��Ϊ��д
		* @return ת������ַ���
		*/
		SelfClass &ToUpperCase()																		///<�������е���ĸȫ��תΪ��д
		{
			if(!data.valid())
				return SelfClass();

			return SelfClass(data->c_str()).UpperString();
		}

		/**
		* ��䵱ǰ�ַ����Ĳ�������Ϊָ���ַ�
		* @param ch Ҫ�����ַ�
		* @param start ������ʼλ��
		* @param len ���ĸ���
		* @return �Ƿ�ɹ�
		*/
		bool FillChar(const T ch,int start=0,int len=-1)
		{
			if(!data.valid())
				return(false);

			if(start<0||data->GetLength()<start+len)
				return(false);

			if(Unlink())
			{
				hgl_set(data->c_str()+start,ch,len);
				return(true);
			}

			return(false);
		}

	protected:

		typedef T *(*ConvFunc)(const T *,int &);

		bool StrConv(ConvFunc conv)
		{
			if(!data.valid()||data->GetLength()<=0)
				return(false);

			int new_len=data->GetLength();

			T *new_str=conv(data->c_str(),new_len);

			if(new_len>0)
			{
				Set(new_str,new_len,true);
				return(true);
			}
			else
			{
				Clear();
				return(false);
			}
		}

	public:

		bool TrimLeft(){return StrConv(trimleft);}													///<ɾ���ַ���ǰ�˵Ŀո񡢻��еȲ������ַ���
		bool TrimRight(){return StrConv(trimright);}												///<ɾ���ַ�����˵Ŀո񡢻��еȲ������ַ���
		bool Trim(){return StrConv(trim);}															///<ɾ���ַ������˵Ŀո񡢻��еȲ������ַ���

		bool TrimLeft(int n){return Delete(0,n);}													///<ɾ���ַ���ǰ�˵�ָ�����ַ�
		bool TrimRight(int n){return Unlink()?data->TrimRight(n):false;}							///<ɾ���ַ�����˵�ָ�����ַ�

		bool ClipLeft(int n){return Unlink()?data->ClipLeft(n):false;}								///<��ȡ�ַ���ǰ�˵�ָ�����ַ�,��ͬTrimRight(lengths-n))
		bool ClipRight(int n){return Delete(0,Length()-n);}											///<��ȡ�ַ�����˵�ָ�����ַ�,��ͬTrimLeft(length-n)
		bool Clip(int pos,int num)																	///<��ָ��λ��ɾ��ָ�����ַ�
		{
			if(!Unlink())
				return(false);

			return data->Clip(pos,num);
		}

		/**
		 * ���ַ�����ȡָ���Ӵ�Ϊ�µ�����
		 * @param start ��ʼ�ַ�����
		 * @param n �ַ�����
		 * @return �ɷ�ɹ�
		 */
		bool SubString(int start,int n)																///<ȡ�ַ���ָ���ε��ַ�
		{
			if(!Unlink())
				return(false);

			return data->SubString(start,n);
		}

		/**
		 * ���ַ�����ȡָ���Ӵ�Ϊ�µ�����
		 * @param sc �µ��ַ���
		 * @param start ��ʼ�ַ�����
		 * @param n �ַ�����
		 * @return �ɷ�ɹ�
		 */
		bool SubString(SelfClass &sc,int start,int n)																///<ȡ�ַ���ָ���ε��ַ�
		{
			if(Length()<start+n)
				return(false);

			sc.Set(data->c_str()+start,n);
			return(true);
		}

		bool Resize(int n)
		{
			if(!data.valid())
				return(false);

			if(n==0)
			{
				Clear();
				return(true);
			}

			return data->Resize(n);
		}

		int StatChar(const T ch)const{return data.valid()?StatChar(data->c_str(),ch):-1;}			///<ͳ���ַ�����ĳ���ַ��ĸ���
		int StatLine()const{return data.valid()?StatLine(data->c_str()):-1;}						///<ͳ���ַ�������

		int FindChar(int pos,const T ch)const														///<���ص�ǰ�ַ�����ָ���ַ���ʼ������(��������)
		{
			if(!data.valid())
				return(-1);

			const T *result=hgl::strchr(data->c_str()+pos,ch);

			if(result)
				return result-(data->c_str()+pos);

			return(-1);
		}

		int FindChar(const T ch)const{return FindChar(0,ch);}										///<���ص�ǰ�ַ�����ָ���ַ���ʼ������(��������)
		int FindRightChar(const T ch)const															///<���ص�ǰ�ַ�����ָ���ַ���ʼ������(��������)
		{
			if(!data.valid())
				return(-1);

			const T *result=hgl::strrchr(data->c_str(),ch);

			if(result)
				return result-(data->c_str());

			return(-1);
		}

		/**
		* �������ַ����ڣ�����ָ���ַ���
		* @param str Ҫ���ҵ��ַ���
		* @param start �ӵڼ����ַ���ʼ���ң�Ĭ��0
		* @return ָ���Ӵ����ڵ�ƫ��
		* @return -1 ����
		*/
		int FindString(const SelfClass &str,int start=0)const										///<���ص�ǰ�ַ�����ָ���Ӵ���ʼ������
		{
			if(!data.valid())
				return(-1);

			if(str.Length()<=0)
				return(-1);

			if(start<0||start>data->GetLength()-str.Length())
				return(-1);

			const T *result=strstr(data->c_str()+start,str.c_str());

			if(result)
				return result-(data->c_str());

			return(-1);
		}

		/**
		* �������ַ����ڣ����ָ���ַ���
		* @param sub Ҫ������ַ���
		* @return �ܼ�����ĸ���
		* @return -1 ����
		*/
		int ClearString(const SelfClass &sub)													///<�����ǰ�ַ�����ָ���Ӵ�
		{
			if(!Unlink())
				return(-1);

			if(sub.Length()<=0)
				return(false);

			T *sp=data->c_str();
			T *tp=sub.c_str();
			T *p;
			int len=data->GetLength();
			int sub_len=sub.Length;
			int count=0;

			while((p=strstr(sp,tp)))
			{
				len-=(p-sp)+sub_len;
				sp=p;
				memcpy(p,p+sub_len,len*sizeof(T));
				count++;
			}

			return(count);
		}

		/**
		* ��ָ��λ��д���ַ���
		* @param pos ��ʼд���λ��
		* @param str Ҫд����ַ���
		* @return �Ƿ�ɹ�
		*/
		bool WriteString(int pos,const SelfClass &str)
		{
			if(!Unlink())
				return(false);

			if(str.Length()<=0)
				return(false);

			return data->Write(pos,str);
		}

		/**
		* �滻��ǰ�ַ�����ָ���ַ�����һ���ַ�
		* @param sch Ҫ�滻���ַ�
		* @param tch �滻����ַ�
		* @return �ܼ��滻����
		* @return <0 ����
		*/
		int Replace(const T tch,const T sch)														///<�滻�ַ�
		{
			if(!Unlink())
				return(-1);

			return replace(data->c_str(),tch,sch);
		}

	public:	//����������

		operator const InstClass &()
		{
			return data;
		}

		const T &operator [](int index)
		{
			if(data.valid())
				if(index>=0&&index<data->GetLength())
					return *(data->c_str()+index);

			const static T zero_char=0;

			return zero_char;	//this is error
		}

		operator 		T *()		{return c_str();}
		operator const	T *()const	{return c_str();}

		SelfClass &operator =	(const T *str		 ){Set(str);return(*this);}
		SelfClass &operator	=	(const SelfClass &str){Set(str);return(*this);}
		SelfClass &operator	+=	(const SelfClass &str){Strcat(str);return(*this);}
		SelfClass &operator	<<	(const SelfClass &str){return(operator+=(str));}

		static SelfClass ComboString(const T *str1,int len1,const T *str2,int len2)
		{
			if(!str1||len1<=0)
			{
				if(!str2||len2<=0)
					return(SelfClass());

				return SelfClass(str2,len2);
			}
			else
			{
				if(!str2||len2<=0)
					return(SelfClass(str1,len1));
			}

			const int new_len=len1+len2;

			T *ms=new T[new_len+1];

			memcpy(ms,		str1,len1*sizeof(T));
			memcpy(ms+len1,	str2,len2*sizeof(T));

			ms[new_len]=0;

			return(SelfClass(ms,new_len,true));
		}

		SelfClass  operator +	(const SelfClass &str) const
		{
			if(str.Length()<=0)		//����Է�Ϊ��
				return(*this);

			if(!data.valid())		//����ҷ�Ϊ��
				return(str);

			return ComboString(data->c_str(),data->GetLength(),str.c_str(),str.Length());
		}

		SelfClass	operator +	(const T ch) const
		{
			if(!data.valid())
				return(SelfClass(ch));

			return ComboString(data->c_str(),data->GetLength(),&ch,1);
		}

		SelfClass	operator +	(const T *str) const
		{
			if(!data.valid())
				return(SelfClass(str));

			return ComboString(data->c_str(),data->GetLength(),str,strlen(str));
		}

#define BASE_STRING_NUMBER_OPERATOR_ADD(type,func)	SelfClass	operator +	(const type &num) const	\
		{	\
			SharedPtr<T> vstr=func(new T[8*sizeof(type)],8*sizeof(type),num);	\
			\
			return operator+(vstr->data);	\
		}

		//BASE_STRING_NUMBER_OPERATOR_ADD(size_t,	utos);
		BASE_STRING_NUMBER_OPERATOR_ADD(int,	itos);
		BASE_STRING_NUMBER_OPERATOR_ADD(uint,	utos);
		BASE_STRING_NUMBER_OPERATOR_ADD(int64,	itos);
		BASE_STRING_NUMBER_OPERATOR_ADD(uint64,	utos);

		BASE_STRING_NUMBER_OPERATOR_ADD(float,	ftos);
		BASE_STRING_NUMBER_OPERATOR_ADD(double,	dtos);

#undef BASE_STRING_NUMBER_OPERATOR_ADD

		CompOperator(const T *,Comp);
		CompOperator(const SelfClass &,Comp);
	};//template<typename T> class BaseString

	//������������value+str�����
	//�����еĵ���������str+value�����

	template<typename V,typename T> BaseString<T> operator + (const V &value,const BaseString<T> &str)
	{
		if(str.Length()<=0)
			return BaseString<T>(value);

		return BaseString<T>(value)+str;
	}

	typedef BaseString<char>		UTF8String;
	typedef BaseString<char>		AnsiString;
	typedef BaseString<u16char>		UTF16String;
	typedef BaseString<os_char>		OSString;
	typedef BaseString<char32_t>	UTF32String;
	typedef BaseString<wchar_t>		WideString;

	/**
	 * ���ۼӵķ�ʽΪһ���ַ��������һ��hash��
	 */
	template<typename T,int HASH_MAX> uint StringFastHash(const BaseString<T> &str)
	{
		const T *p=str.c_str();
		int c=str.Length();

		uint result=0;

		while(c-->0)
			result+=*p++;

		return(result%HASH_MAX);
	}

	/**
	 * ��һ��ԭʼ���ݣ�ת����һ��16������ֵ��ɵ��ַ���
	 * @param value Ҫת����ԭʼ����
	 * @return ת���õ��ַ���
	 * @see HexToString
	 */
	template<typename T,typename I> BaseString<T> ToHexString(const I &value)
	{
		T str[(sizeof(I)<<1)+1];

		ToUpperHexStr(str,value);

		return BaseString<T>(str);
	}

	/**
	 * ��һ����ֵת��Ϊһ����16���Ʊ�ʾ���ַ���<br>
	 * ͬToHexString�������ڣ�HexToString����ֵ��С����
	 * @param value Ҫת������ֵ
	 * @see ToHexString
	 */
	template<typename T,typename I> BaseString<T> HexToString(const I &value)
	{
		T str[(sizeof(I)<<1)+1];

		htos(str,sizeof(I)<<1,value);

		return BaseString<T>(str);
	}

	template<typename T,int POINTER_SIZE> struct PointerToHexStringConvert;

	template<typename T> struct PointerToHexStringConvert<T,4>
	{
		static BaseString<T> Conv(const void *ptr)
		{
			return HexToString<T,uint32>(reinterpret_cast<const uint32 >(ptr));
		}
	};

	template<typename T> struct PointerToHexStringConvert<T,8>
	{
		static BaseString<T> Conv(const void *ptr)
		{
			return HexToString<T,uint64>(reinterpret_cast<const uint64 >(ptr));
		}
	};

	/**
	 * ��һ��ָ��ת����һ��16�����ַ���
	 */
	template<typename T> BaseString<T> PointerToHexString(const void *value)
	{
		return PointerToHexStringConvert<T,sizeof(void *)>::Conv(value);
	}

	inline BaseString<os_char> PointerToHexOSString(const void *value)
	{
		return PointerToHexString<os_char>(value);
	}

	inline BaseString<char> PointerToHexUTF8String(const void *value)
	{
		return PointerToHexString<char>(value);
	}

	inline BaseString<char16_t> PointerToHexUTF16String(const void *value)
	{
		return PointerToHexString<char16_t>(value);
	}
}//namespace hgl
#endif//HGL_TYPE_BASE_STRING_INCLUDE
