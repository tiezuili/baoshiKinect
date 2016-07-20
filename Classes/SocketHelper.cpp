//
//  SocketHelper.cpp
//  KinectGame
//
//  Created by ArcherPeng on 15/3/9.
//
//

#include "SocketHelper.h"
#include "KinectHelper.h"
SocketHelper * SocketHelper::sh = nullptr;
void SocketHelper::connect()
{
    //����socket
    this->s_client.Init();
    bool res= this->s_client.Create(AF_INET, SOCK_STREAM,0);
    std::cout << "client socket create"<<res<<std::endl;
    //���ӷ�����
    res=this->s_client.Connect("127.0.0.1", 9000);
    std::cout << "client socket connect"<<res<<std::endl;
    if(res==1)
    {//�����߳�
//        pthread_t tid;
//        bool ok= pthread_create(&tid, NULL, getMessage, NULL);
//        std::cout << "�߳��������"<<ok<<endl;
        auto t = std::thread(&SocketHelper::receive, this);
        t.detach();
    }

}
void SocketHelper::receive()
{
    this->isConnected = true;
    std::cout<<"�߳������ɹ�"<<endl;
    char buf[1024*64];
    while (this->s_client.Recv(buf, sizeof(buf)))
    {
//        CCLOG("Error �� %d",this->s_client.GetError());
        //���շ�������Ϣ
        
                //s_client.Clean();
        for (int i = 0 ; i<1024*64; ++i)
        {
            if (buf[i]=='\n')
            {
                buf[i]='\0';
            }
        }
//        std::cout << "recv --> "<<buf<<std::endl;
        std::string str = buf;
        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,str]
        {
        KinectHelper::getInstance()->analyzeAndDispatchKinectData(std::string(str));
        });
    }
    //�Ͽ�������
    this->s_client.Close();
    this->isConnected = false;
}
void SocketHelper::send(std::string str)
{
    const char * cStr = str.c_str();

    this->s_client.Send(cStr,strlen(cStr)+1);
}