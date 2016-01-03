#ifndef HGL_NETWORK_IP_INCLUDE
#define HGL_NETWORK_IP_INCLUDE

#include<hgl/type/DataType.h>
#include<hgl/type/List.h>
#include<hgl/type/BaseString.h>
#include<hgl/Str.h>

#if HGL_OS == HGL_OS_Windows
    #include<winsock2.h>
    #include<ws2tcpip.h>

    #if SOMAXCONN == 5
    #error Please use <winsock2.h>
    #endif//

    typedef int socklen_t;
    typedef ULONG in_addr_t;

    #define GetLastSocketError() WSAGetLastError()
#else
    #include<errno.h>
    #include<sys/types.h>
    #include<sys/ioctl.h>
    #include<sys/socket.h>
    #include<unistd.h>
    #include<netdb.h>
    #include<arpa/inet.h>
    #include<netinet/in.h>

    #define GetLastSocketError() (errno)

    #if HGL_OS == HGL_OS_Linux
        #include<sys/sendfile.h>

        inline int sendfile(int tfd,int sfd,size_t size)
        {
            return sendfile(tfd,sfd,nullptr,size);
        }
    #endif//HGL_OS == HGL_OS_Linux

    #if HGL_OS == HGL_OS_FreeBSD
        #include<sys/uio.h>

        inline int sendfile(int tfd,int sfd,size_t size)
        {
            return sendfile(tfd,sfd,0,size,nullptr,nullptr,0);
        }
    #endif//HGL_OS == HGL_OS_FreeBSD
#endif//HGL_OS == HGL_OS_Windows

namespace hgl
{
	namespace network
	{
		template<int FAMILY,typename InAddr, typename SockAddr, typename SockAddrIn>
		struct ip_tool
		{
		private:

			static void AddAddrToList(List<InAddr> &addr_list, const SockAddrIn *sai);

			static int GetIPList(const char *addr_string, List<InAddr> &addr_list,int socktype,int protocol)
			{
				struct addrinfo hints, *answer, *ptr;

				hgl_zero(hints);
				hints.ai_family = FAMILY;
                hints.ai_socktype=socktype;
                hints.ai_protocol=protocol;

				if (getaddrinfo(addr_string, nullptr, &hints, &answer))			//�˺������Windows 2003/Vista
					return(-1);

				int count = 0;
				for (ptr = answer; ptr; ptr = ptr->ai_next)
				{
					AddAddrToList(addr_list,(SockAddrIn *)(ptr->ai_addr));
					++count;
				}

				freeaddrinfo(answer);
				return(count);
			}

		public:

			/**
			* ȡ�ñ���IP��ַ
			* @param addr_list ����IP��ַ�б�
			* @return ����IP��ַ����,-1��ʾʧ��
			*/
			static int	GetLocalIP(List<InAddr> &addr_list)											///<ȡ�ñ���IP��ַ�б�
			{
				return GetIPList("localhost", addr_list);
			}

			/**
			* ��һ������ת����IP(IP��ʽ)
			* @param domain ����
			* @param addr_list IP��ַ�б�
            * @param socktype Socket����(����ΪSOCK_STREAM��SOCK_DGRAM��SOCK_RAW��SOCK_RDM��SOCK_SEQPACKET��ֵ),Ĭ��Ϊ�������͡�
            * @param protocol Э������(����ΪIPPROTO_TCP��IPPROTO_UDP��IPPROTO_SCTP),Ĭ��Ϊ�������͡�
			* @return IP��ַ����,-1��ʾʧ��
			*/
			static int	Domain2IP(const UTF8String &domain,List<InAddr> &addr_list,int socktype=0,int protocol=0)					///<ת��������IP��ַ
			{
				if (name.Length() <= 0)return(-1);

				return GetIPList(name.c_str(),addr_list,socktype,protocol);
			}

			/**
			* ��ָ������/IP��port����ǰsocket
			* @param ThisSocket Ҫָ���ĵ�ǰsocket
			* @param addr ָ����ip/port
			* @param reuse �Ƿ��õ�ǰ��ַ,Ĭ��Ϊ1
			* @return �Ƿ�ɹ�
			*/
			static bool	BindAddr(int ThisSocket,const SockAddrIn &addr,int reuse=1)
			{
				const int val = reuse;

#if HGL_OS == HGL_OS_Windows
				setsockopt(ThisSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&val, sizeof(BOOL));		//win�µ�BOOL����Ҳ��int������Ψһ����ֻ����val�Ĵ�������
#else
				setsockopt(ThisSocket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
#endif//HGL_OS == HGL_OS_Windows

				if (bind(ThisSocket, (SockAddr *)&addr, sizeof(SockAddrIn)))
				{
					LOG_ERROR(OS_TEXT("Bind Socket Error! errno: ") + OSString(GetLastSocketError()));
					std::cerr << "Bind Socket Error! errno: " << GetLastSocketError() << std::endl;
					return(false);
				}

				return(true);
			}

			static bool FillAddr(SockAddrIn *addr, const UTF8String &addr_string, ushort port,int socktype,int protocol)		///<��ָ���������ַ��䵽sockaddr_in�ṹ��
			{
                struct addrinfo hints, *answer, *ptr;

                hgl_zero(hints);
                hints.ai_family = FAMILY;
                hints.ai_socktype=socktype;
                hints.ai_protocol=protocol;

                if (getaddrinfo(addr_string, nullptr, &hints, &answer))         //�˺������Windows 2003/Vista
                    RETURN_FALSE;

                memcpy(addr,answer->ai_addr,sizeof(SockAddrIn));
                freeaddrinfo(answer);
                return(true);
			}

			static bool	BindAddr(int ThisSocket, const char *name, int port)
			{
				SockAddrIn addr;

				if (!FillAddr(&addr, name, port))return(false);

				return BindAddr(ThisSocket, addr);
			}

			/**
			* ��乩������ʹ�õ������ַ��ṹ
			*/
			static void FillAddrByAny(SockAddrIn &addr, ushort port);
		};

		using ipv4 = ip_tool<AF_INET,	struct in_addr,	struct sockaddr,	struct sockaddr_in	>;
		using ipv6 = ip_tool<AF_INET6,	struct in6_addr,struct sockaddr6,	struct sockaddr_in6	>;

		template<> void ipv4::AddAddrToList(List<in_addr> &addr_list, const sockaddr_in *sai)
		{
			addr_list.Add(sai->sin_addr);
		}

		template<> void ipv6::AddAddrToList(List<in6_addr> &addr_list, const sockaddr_in6 *sai)
		{
			addr_list.Add(sai->sin6_addr);
		}

		template<> void ipv4::FillAddrByAny(struct sockaddr_in &addr, ushort port)
		{
			hgl_zero(addr);

			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
		}

		template<> void ipv6::FillAddrByAny(struct sockaddr_in6 &addr, ushort port)
		{
			hgl_zero(addr);

			addr.sin6_family = AF_INET6;
			addr.sin6_port = htons(port);
		}
	}//namespace network
}//namespace hgl
#endif//HGL_NETWORK_IP_TOOL_INCLUDE
