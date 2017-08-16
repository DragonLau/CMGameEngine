#ifndef HGL_WORKFLOW_INCLUDE
#define HGL_WORKFLOW_INCLUDE

#include<hgl/thread/Thread.h>
#include<hgl/thread/SwapData.h>
#include<hgl/type/List.h>
namespace hgl
{
	/**
	 * ���������ֿռ�<br>
	 * ��������һ�ֶԹ����Ķ��߰��Ź�����ƣ��������ڰ�ҵ�������̵߳Ĺ�������<br>
	 * ��������ҪΪÿһ�ֹ���ָ��һ�����߳���������ÿһ�ֹ���ȷֻ��һ������������ںͷַ����ڡ�<br>
	 * �����������ύ����������ڣ������߿����������ط�����ڵķ��亯����

	 ʹ�÷���һ��ÿ�������߳�һ������Ͷ����

		group=new WorkGroup;			//����group

		WorkPost *wp[10];

		for(int i=0;i<MAX_THREADS;i++)
		{
			wp=new WorkPost();			//�����¼�Ͷ����
			wt=new WorkThread(wp);		//���������߳�

			group->Add(wp);				//���Ͷ������group
			group->Add(wt);				//��ӹ����̵߳�group
		}

		group->Start();					//����group���������й����߳�

		for(int i=0;i<0xffff;i++)
		{
			int index=rand()%10;		//���һ���߳�

			for(int j=0;j<rand()%10;j++)
			{
				wp[index]->Post(new Work);			//Ͷ��һ������
			}

			wp[index]->ToWork();
		}

		group->Close();					//�ر�group,�ر����й����߳�


	ʹ�÷���������������̹߳���һ������Ͷ����

		group=new WorkGroup;			//����group

		WorkPost *wp=new WorkPost();
		group->Add(wp);					//���Ͷ������group

		for(int i=0;i<MAX_THREADS;i++)
		{
			wt=new WorkThread(wp);		//���������߳�

			group->Add(wt);				//��ӹ����̵߳�group
		}

		group->Start();					//����group���������й����߳�

		for(int i=0;i<0xffff;i++)
		{
			for(int j=0;j<rand()%10;j++)
			{
				wp->Post(new Work);			//Ͷ��һ������
			}

			wp->ToWork();
		}

		group->Close();					//�ر�group,�ر����й����߳�
	 */ 
	namespace workflow
	{
		/**
		 * ��ڷ�����<br>
		 * Post�Ĺ��������ǽ�������������Ӧ��WorkThread
		 */
		template<typename W> class WorkPost
		{
		protected:
			
			using WorkList=List<W *>;

			SemSwapData<WorkList> work_list;																///<�����б�

		public:

			virtual ~WorkPost()
			{
			}

			virtual void Post(W *w)																			///<Ͷ��һ������
			{
				WorkList &wl=work_list.GetBack();
					wl.Add(w);
				work_list.ReleaseBack();
			}
			
			virtual void Post(W **w,int count)																///<Ͷ��һ������
			{
				WorkList &wl=work_list.GetBack();
					wl.Add(w,count);
				work_list.ReleaseBack();
			}

			virtual void ToWork()																			///<���ѻ��Ĺ����б��͸������߳�
			{
				work_list.ReleaseSem(1);
			}

		public:

			WorkList *WaitWorkList(const double time_out=5)
			{
				if(work_list.WaitSemSwap(time_out))
					return &(work_list.GetFront());

				return(nullptr);
			}
		};//template<typename W> class WorkPost

		/**
		 * �����̣߳�����������������
		 */
		template<typename W> class WorkThread:public Thread
		{
		protected:

			using WorkList=List<W *>;

			WorkPost<W> *work_post;
			WorkList *wl;

			volatile bool exit_work;																///<�˳����

		public:

			WorkThread(WorkPost<W> *wp)
			{
				work_post=wp;
				wl=nullptr;
				exit_work=false;
			}

			virtual ~WorkThread()
			{
			}

			virtual void ProcWork(W *obj)=0;														///<ֱ�Ӵ������Ĵ��麯������ʹ��������ʵ��

			virtual void ExitWork()
			{
				exit_work=true;
			}

			virtual bool Execute() override
			{
				if(!work_post)
					RETURN_FALSE;

				wl=work_post->WaitWorkList();

				if(!wl)
					return(!exit_work);

				W **p=wl->GetData();

				for(int i=0;i<wl->GetCount();i++)
				{
					ProcWork(*p);
					++p;
				}

				return(!exit_work);
			}
		};//template<typename W> class WorkThread:public Thread

		/**
		 * ������<br>
		 * ���ڹ���һ��Ĺ����߳��Լ�Ͷ����<br>
		 * ע������һ�鹤���̹߳���һ��Ͷ������Ҳ����ÿ�������߳���һ��Ͷ���������������ֻΪ����ͳһ����
		 */
		template<typename WP,typename WT> class WorkGroup
		{
			ObjectList<WP> wp_list;														///<Ͷ�����б�
			ObjectList<WT> wt_list;														///<�����߳��б�

		public:

			WorkGroup()
			{
			}

			virtual ~WorkGroup()
			{
				Close();
			}

			virtual bool Add(WP *wp)
			{
				if(!wp)return(false);

				wp_list.Add(wp);
				return(true);
			}

			virtual bool Add(WP **wp,const int count)
			{
				if(!wp)return(false);

				wp_list.Add(wp,count);
				return(true);
			}

			virtual bool Add(WT *wt)
			{
				if(!wt)return(false);

				wt_list.Add(wt);
				return(true);
			}

			virtual bool Add(WT *wt,const int count)
			{
				if(!wt)return(false);

				wt_list.Add(wt,count);
				return(true);
			}

			virtual bool Start()
			{
				int count=wt_list.GetCount();

				if(count<=0)
					RETURN_FALSE;

				for(int i=0;i<count;i++)
					wt_list[i]->Start();

				return(true);
			}

			virtual void Close()
			{
				int count=wt_list.GetCount();

				for(int i=0;i<count;i++)
					wt_list[i]->ExitWork();

				for(int i=0;i<count;i++)
					wt_list[i]->Wait();
			}
		};//template<typename WP,typename WT> class WorkGroup
	}//namespace workflow
}//namespace hgl
#endif//HGL_WORKFLOW_INCLUDE
