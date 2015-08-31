#ifndef HGL_GRAPH_RENDER_OBJECT_INCLUDE
#define HGL_GRAPH_RENDER_OBJECT_INCLUDE

namespace hgl
{
	namespace graph
	{
		class Renderable;

        enum RenderMode
        {
            rmNone=0,

            rmNormal,                                       ///<������Ⱦ
            rmDepth,                                        ///<�����������Ⱦ
            rmShadowMapsDepth,                              ///<ShadowMaps���������Ⱦ
            rmOQDepth,                                      ///<OQ��ѯ�����������Ⱦ
            rmOQShadowMapsDepth,                            ///<ShadowMaps��OQ��ѯ���������Ⱦ
            rmDeferredShadowMaps,                           ///<�ӳ���Ӱ��Ⱦ
            rmDeferredLight,                                ///<�ӳٹ�����Ⱦ
            rmDeferredDecal,                                ///<�ӳ�������Ⱦ
            rmDeferredFinal,                                ///<�ӳ���Ⱦ���ջ��

            rmEnd
        };//

		class RenderObject
		{
		protected:

			Renderable *data;

            struct RenderItem
            {
                uint        vao;
                int *       location;                       ///<shader�󶨱�����ַ
                RenderState state;                          ///<��Ⱦ״̬
            };

            Map<GLSL *,RenderItem> render_items;            ///<��Ⱦ��

		public:

			RenderObject(Renderable *r)
			{
				data = r;
			}

			virtual ~RenderObject();

			virtual bool Add(GLSL *,);																///<ˢ������
			virtual bool Use();																		///<ʹ���������
		};//class RenderObject
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_OBJECT_INCLUDE
