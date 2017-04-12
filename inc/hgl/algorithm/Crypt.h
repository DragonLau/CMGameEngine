#ifndef HGL_ALGORITHM_CRYPT_INCLUDE
#define HGL_ALGORITHM_CRYPT_INCLUDE

namespace hgl   //У��/�����㷨
{
    namespace algorithm
    {
        void OverflowEncrypt(void *, void *, int, void *, int);			        ///<�������(��ȼ���)
        void OverflowDecrypt(void *, void *, int, void *, int);			        ///<�������

        void AesEncrypt(uint8 data[16], uint8 *key, int keysize);               ///<AES����
        void AesDecrypt(uint8 data[16], uint8 *key, int keysize);               ///<AES����

        void RC4Encrypt(uint8 *data, int datasize, uint8 *key, int keysize);    ///<RC4����
    }//namespace algorithm
}//namespace hgl
#endif//HGL_ALGORITHM_CRYPT_INCLUDE
