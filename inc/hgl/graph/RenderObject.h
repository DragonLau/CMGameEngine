#ifndef HGL_GRAPH_RENDER_OBJECT_INCLUDE
#define HGL_GRAPH_RENDER_OBJECT_INCLUDE

namespace hgl
{
	namespace graph
	{
		class Renderable;

		class RenderObject
		{
		protected:

			Renderable *data;

		public:

			RenderObject(Renderable *r)
			{
				data = r;
			}

			virtual ~RenderObject();

			virtual bool Update();																	///<ˢ������
			virtual bool Use();																		///<ʹ���������
		};//class RenderObject
	}//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_RENDER_OBJECT_INCLUDE
