//
//  PluginChannel.h
//  TestAnySDK
//
//  Created by ly on 15-1-28.
//
//

#ifndef __TestAnySDK__PluginChannel__
#define __TestAnySDK__PluginChannel__

#include "AgentManager.h"
#include "string"
using namespace std;
using namespace anysdk::framework;

class PluginChannel: public PayResultListener,public UserActionListener
{
public:
    static PluginChannel* getInstance();
    static void purge();
    
    //初始化插件
    void loadPlugins();
    
    //卸载插件
    void unloadPlugins();
    
    //登录函数
    void login();
    
    //获取登录状态
    bool isLogined();
    
    //用户系统回调
    virtual void onActionResult(ProtocolUser* pPlugin, UserActionResultCode code, const char* msg);
    
    //获取用户唯一标识符，进行游戏端登陆
    std::string getUserId();
    
    //支付
    void payment();
    
   	//支付回调函数
    virtual void onPayResult(PayResultCode ret, const char* msg, TProductInfo info);
    
    //获取渠道id
    std::string getChannelId();
    
    //sdk退出
    void destroy();
    
    //渠道退出界面调用
    void exit();
    
private:
    PluginChannel();
    virtual ~ PluginChannel();
    static PluginChannel* _pInstance;
    AgentManager* _agent;
    ProtocolUser* _pluginUser;
    std::map<std::string , ProtocolIAP*>* _pluginsIAPMap;
    
};


#endif /* defined(__TestAnySDK__PluginChannel__) */
