#ifndef HGL_ROOT_FLOW_CONTROL_INCLUDE
#define HGL_ROOT_FLOW_CONTROL_INCLUDE

#include<hgl/platform/InputDevice.h>
#include<hgl/object/FlowControl.h>
namespace hgl
{
    /**
    * ͼ��Ӧ�ø����̿�����
    */
    class RootFlowControl:public FlowControl
    {
    public: //�¼�

        using FlowControl::FlowControl;
        ~RootFlowControl();

        virtual void OnClose        ();                                                             ///<���ڹر��¼�
        virtual void OnResize       (int,int);                                                      ///<���ڳߴ�����¼�
        virtual void OnRotate       (int);                                                          ///<��Ļ��ת�¼�

        virtual bool OnMouseMove    (int,int);                                                      ///<����ƶ��¼�
        virtual bool OnMouseWheel   (int,int);                                                      ///<�������¼�

        virtual bool OnMouseDown    (MouseButton);                                                  ///<��갴�������¼�
        virtual bool OnMouseUp      (MouseButton);                                                  ///<��갴�������¼�
        virtual bool OnMouseRepeat  (MouseButton);                                                  ///<��갴���Զ������¼�(������ס���Ų������¼�)

        virtual bool OnKeyDown      (KeyboardButton);                                               ///<���������¼�
        virtual bool OnKeyUp        (KeyboardButton);                                               ///<���������¼�
        virtual bool OnKeyRepeat    (KeyboardButton);                                               ///<�����Զ������¼�(������ס���Ų������¼�)

        virtual bool OnChar         (os_char);                                                      ///<�ַ�����
    };//class RootFlowControl
}//namespace hgl
#endif//HGL_ROOT_FLOW_CONTROL_INCLUDE
