//
//  PluginChannel.cpp
//  TestAnySDK
//
//  Created by ly on 15-1-28.
//
//
#if ANDROID

#include <jni.h>
#include <android/log.h>
#include "PluginJniHelper.h"
#include "PluginChannel.h"
#include "platform/CCCommon.h"
#include "cocos2d.h"

using namespace anysdk::framework;
using namespace cocos2d;

#define  LOG_TAG    "PluginChannel"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,__VA_ARGS__);
extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_wrapper_nativeInitPlugins(JNIEnv*  env, jobject thiz)
    {
        PluginChannel::getInstance()->loadPlugins();
    }
}
PluginChannel* PluginChannel::_pInstance = NULL;

PluginChannel::PluginChannel()
{
    
    
}

PluginChannel::~PluginChannel()
{
    destroy();
    unloadPlugins();
}

PluginChannel* PluginChannel::getInstance()
{
    if (_pInstance == NULL) {
        _pInstance = new PluginChannel();
    }
    return _pInstance;
}

void PluginChannel::purge()
{
    if (_pInstance)
    {
        delete _pInstance;
        _pInstance = NULL;
    }
}


void PluginChannel::loadPlugins()
{
    LOGD("Load plugins invoked");
    /**
     * appKey、appSecret、privateKey不能使用Sample中的值，需要从打包工具中游戏管理界面获取，替换
     * oauthLoginServer参数是游戏服务提供的用来做登陆验证转发的接口地址。
     */
    std::string oauthLoginServer = "http://oauth.anysdk.com/api/OauthLoginDemo/Login.php";
    std::string appKey = "A777E057-CB6B-03E5-3094-42E6E705867A";
    std::string appSecret = "f40754ea8d56e27ed8ca1c299ff80797";
    std::string privateKey = "CDDD0391977CE71C4E814D37C089E1EB";
    
    _agent = AgentManager::getInstance();
    _agent->init(appKey,appSecret,privateKey,oauthLoginServer);
    
    //使用框架中代理类进行插件初始化
    _agent->loadALLPlugin();
    
    //对用户系统设置监听类
    _pluginUser = _agent->getUserPlugin();
    if(_pluginUser)
    {
        _pluginUser->setDebugMode(true);
        _pluginUser->setActionListener(this);
    }
    
    //对支付系统设置监听类
    LOGD("Load plugins invoked");
    _pluginsIAPMap  = _agent->getIAPPlugin();
    std::map<std::string , ProtocolIAP*>::iterator iter;
    for(iter = _pluginsIAPMap->begin(); iter != _pluginsIAPMap->end(); iter++)
    {
        (iter->second)->setDebugMode(true);
       	(iter->second)->setResultListener(this);
    }
    
}

void PluginChannel::unloadPlugins()
{
    LOGD("Unload plugins invoked");
    _agent->unloadALLPlugin();
}


void PluginChannel::payment()
{
    if(_pluginsIAPMap)
    {
        std::map<std::string, std::string> productInfo;
        productInfo["Product_Price"] = "1";
        productInfo["Product_Id"] = "1";
        productInfo["Product_Name"] = "豌豆荚测试a1";
        productInfo["Server_Id"] = "13";
        productInfo["Product_Count"] = "1";
        productInfo["Role_Id"] = "1";
        productInfo["Role_Name"] = "1";
        productInfo["Role_Grade"] = "1";
        productInfo["Role_Balance"] = "1";
        
        std::map<std::string , ProtocolIAP*>::iterator it = _pluginsIAPMap->begin();
        if(_pluginsIAPMap->size() == 1)
        {
            
            (it->second)->setDebugMode(true);
            (it->second)->payForProduct(productInfo);
        }
        else if(_pluginsIAPMap->size() > 1)
        {
            
            //多支付，游戏开发商自己处理相关UI及界面
        }
    }
    
}

void PluginChannel::onPayResult(PayResultCode ret, const char* msg, TProductInfo info)
{
    std::string temp = "fail";
    switch(ret)
    {
        case kPaySuccess://支付成功回调
            temp = "Success";
            MessageBox("Succeed","Payment");
            break;
        case kPayFail://支付失败回调
            MessageBox("Fail","Payment");
            break;
        case kPayCancel://支付取消回调
            MessageBox("Cancel","Payment");
            break;
        case kPayNetworkError://支付超时回调
            MessageBox("NetworkError","Payment" );
            break;
        case kPayProductionInforIncomplete://支付信息提供不完全回调
            MessageBox("ProductionInforIncomplete","Payment"  );
            break;
            /**
             * 新增加:正在进行中回调
             * 支付过程中若SDK没有回调结果，就认为支付正在进行中
             * 游戏开发商可让玩家去判断是否需要等待，若不等待则进行下一次的支付
             */
        case kPayNowPaying:
            break;
        default:
            break;
    }
    
    
}
void PluginChannel::login()
{
    if(_pluginUser)
    {
        _pluginUser->login();
    }
}

bool PluginChannel::isLogined()
{
    if(_pluginUser)
    {
        return _pluginUser->isLogined();
    }
    return false;
}

void PluginChannel::onActionResult(ProtocolUser* pPlugin, UserActionResultCode code, const char* msg)
{
    LOGD("onActionResult%d%s",code,msg);
    switch(code)
    {
        case kInitSuccess://初始化SDK成功回调
            break;
        case kInitFail://初始化SDK失败回调
            break;
        case kLoginSuccess://登陆成功回调
            break;
            
        case kLoginCancel://登陆取消回调
            MessageBox("Cancel","Login");
            break;
        case kLoginNetworkError://登陆网络出错回调
        case kLoginFail://登陆失败回调
            MessageBox("Fail","Login");
            break;
        case kLogoutSuccess://登出成功回调
            break;
        case kLogoutFail://登出失败回调
            break;
        case kPlatformEnter://平台中心进入回调
            break;
        case kPlatformBack://平台中心退出回调
            break;
        case kPausePage://暂停界面回调
            break;
        case kExitPage://退出游戏回调
            purge();
            Director::getInstance()->end();
            break;
        case kAntiAddictionQuery://防沉迷查询回调
            break;
        case kRealNameRegister://实名注册回调
            break;
        case kAccountSwitchSuccess://切换账号成功回调
            break;
        case kAccountSwitchFail://切换账号失败回调
            break;
        default:
            break;
    }
    
}

std::string PluginChannel::getChannelId()
{
    return _agent->getChannelId();
}

std::string PluginChannel::getUserId()
{
    if(_pluginUser)
    {
        return _pluginUser->getUserID();
    }
}

void PluginChannel::destroy()
{
    if(_pluginUser && _pluginUser->isFunctionSupported("destroy"))
    {
        _pluginUser->callFuncWithParam("destroy",NULL);
    }
}

void PluginChannel::exit()
{
    if(_pluginUser && _pluginUser->isFunctionSupported("exit"))
    {
        _pluginUser->callFuncWithParam("exit",NULL);
    }
}
#endif