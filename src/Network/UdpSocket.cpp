//--------------------------------------------------------------------------------------------------
//��������Ϸ������v18��UDPͨ�������Դ�����ļ� UDPSocket.CPP
//--------------------------------------------------------------------------------------------------
#include <hgl/LogInfo.h>
#include <hgl/network/UdpSocket.h>
#include <string.h>

#if HGL_OS != HGL_OS_Windows
#include<netinet/udp.h>
#endif//HGL_OS != HGL_OS_Windows
//--------------------------------------------------------------------------------------------------
namespace hgl
{
	namespace network
	{
		/**
		* ���๹�캯��
		*/
		UDPSocket::UDPSocket()
		{
			ThisSocket=-1;
			bind_port=0;
			tar_addr=new sockaddr_in;
		}

		/**
		* ������������
		*/
		UDPSocket::~UDPSocket()
		{
			CloseSocket();

			delete tar_addr;
		}

		/**
		* ����һ��UDP���ӣ�ʹ��ָ����IP��ַ�Ͷ˿�
		* @param host ��������ʹ�õ�IP��ַ,���host==nullptr���ʾʹ�ñ��������е�ַ
		* @param port ��������������Ķ˿�
		* @return true �����������ɹ�
		* @return false ����������ʧ��
		*/
		bool UDPSocket::Create(const char *host,const uint port)
		{
			if(!CreateSocket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))
				return(false);

			socket_protocols=IPPROTO_UDP;

			if(!BindAddr(ThisSocket,host,port))
			{
				hgl::CloseSocket(ThisSocket);
				return(false);
			}

			bind_port=port;

			SetBlock(false);
			return(true);
		}

		/**
		* ����һ��UDP����
		*/
		bool UDPSocket::Create()
		{
			if((ThisSocket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))<0)
			{
				LOG_ERROR(U16_TEXT("����Socketʧ�ܣ�errno:")+UTF16String(GetLastSocketError()));
				return(false);
			}

			socket_protocols=IPPROTO_UDP;

			SetBlock(false);
			return(true);
		}

		/**
		* �趨��������ʱ���������ݷ���IP��ַ�Ͷ˿ں�
		* @param host ���շ���������IP��ַ
		* @param port ���շ��Ķ˿ں�
		*/
		void UDPSocket::SetSendAddr(const char *host,const uint port)
		{
			FillAddr(tar_addr,host,port);
		}

		/**
		* �趨��������ʱ���������ݷ��ĵ�ַ
		* @param addr ���շ��ĵ�ַ
		*/
		void UDPSocket::SetSendAddr(sockaddr_in &addr)
		{
			memcpy(tar_addr,&addr,sizeof(sockaddr_in));
		}

		/**
		* ����ָ���ĵ�ַ�Ͷ˿ڷ������ݰ�
		* @param data ����ָ��
		* @param size ���ݳ���
		* @return �ѷ��͵������ֽ���
		*/
		int UDPSocket::SendPacket(const void *data,int size)
		{
	#ifdef _DEBUG
			if(ThisSocket==-1)
			{
				LOG_HINT(U16_TEXT("UDPSocketû�е���Create"));
				return(-1);
			}
	#endif//
			return(sendto(ThisSocket,(char *)data,size,0,(sockaddr *)tar_addr,sizeof(sockaddr_in)));
		}

		/**
		* ��ָ����ַ�������ݰ�
		* @param addr �������ݷ��ĵ�ַ
		* @param data ����ָ��
		* @param size ���ݳ���
		* @return �ѷ��͵������ֽ���
		*/
		int UDPSocket::SendPacket(sockaddr_in &addr,const void *data,int size)
		{
	#ifdef _DEBUG
			if(ThisSocket==-1)
			{
				LOG_HINT(U16_TEXT("UDPSocketû�е���Create"));
				return(-1);
			}
	#endif//
			return(sendto(ThisSocket,(char *)data,size,0,(sockaddr *)&addr,sizeof(sockaddr_in)));
		}

		/**
		* �������ⲿ���������ݰ�
		* @param buf �����õ����ݻ�����ָ��
		* @param size ����������
		* @param remote_addr ���ͷ��ĵ�ַ
		* @return ���յ������ݳ���
		*/
		int UDPSocket::RecvPacket(void *buf,int size,sockaddr_in *remote_addr)
		{
	#ifdef _DEBUG
			if(ThisSocket==-1)
			{
				LOG_HINT(U16_TEXT("UDPSocketû�е���Create"));
				return(-1);
			}
	#endif//

			static

	#if HGL_OS == HGL_OS_Windows
			int
	#else
			socklen_t
	#endif//
			sas=sizeof(sockaddr_in);

			return(recvfrom(ThisSocket,(char *)buf,size,0,(sockaddr *)remote_addr,&sas));
		}
	}//namespace network
}//namespace hgl

