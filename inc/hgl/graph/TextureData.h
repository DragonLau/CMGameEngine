#ifndef HGL_GRAPH_TEXTURE_DATA_INCLUDE
#define HGL_GRAPH_TEXTURE_DATA_INCLUDE

#include<hgl/graph/TextureFormat.h>
namespace hgl
{
    namespace graph
    {
        /**
        * ��������
        */
        struct TextureData
        {
            const TextureFormat   * source_format;                  ///<Դʼ�����ʽ

            uint                    video_format;                   ///<�Դ��ʽ

            bool                    gen_mipmaps;                    ///<�Ƿ����mipmaps

            void *                  bitmap;                         ///<λͼ����

            uint                    bitmap_bytes;                   ///<λͼ�����ֽ���

        public:

            uint GetVideoFormat()const
            {
                return(video_format?video_format:source_format->video_format);
            }
        };//struct TextureData

        struct Texture1DData :public TextureData
        {
            uint length;

        public:

            Texture1DData()
            {
                hgl_zero(this, sizeof(Texture1DData));
            }
        };//struct Texture1DData:public TextureData

        struct Texture2DData :public TextureData
        {
            uint width, height;

        public:

            Texture2DData()
            {
                hgl_zero(this, sizeof(Texture2DData));
            }
        };

        struct Texture1DArrayData :public TextureData
        {
            uint length, count;

        public:

            Texture1DArrayData()
            {
                hgl_zero(this, sizeof(Texture1DArrayData));
            }
        };

        struct Texture2DArrayData :public TextureData
        {
            uint width, height, layer;

        public:

            Texture2DArrayData()
            {
                hgl_zero(this, sizeof(Texture2DArrayData));
            }
        };

        struct Texture3DData :public TextureData
        {
            uint width, height, depth;

        public:

            Texture3DData()
            {
                hgl_zero(this, sizeof(Texture3DData));
            }
        };
    }//namespace graph
}//namespace hgl
#endif//HGL_GRAPH_TEXTURE_DATA_INCLUDE
