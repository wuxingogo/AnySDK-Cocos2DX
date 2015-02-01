#include "HelloWorldScene.h"



USING_NS_CC;
#if ANDROID
#include "AgentManager.h"
#include "PluginChannel.h"
#endif

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    closeItem->setScale(3);
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - 3 * closeItem->getContentSize().width/2 ,
                                origin.y + 3 * closeItem->getContentSize().height/2));
    
    
    auto closeItem1 = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::shareQQ, this));
    closeItem1->setScale(3);
    
    closeItem1->setPosition(Vec2(origin.x + visibleSize.width - 12 * closeItem->getContentSize().width/2 ,
                                origin.y + 3 * closeItem->getContentSize().height/2));
    
    auto closeItem2 = MenuItemImage::create(
                                            "CloseNormal.png",
                                            "CloseSelected.png",
                                            CC_CALLBACK_1(HelloWorld::pay, this));
    closeItem2->setScale(3);
    
    closeItem2->setPosition(Vec2(origin.x + visibleSize.width - 21 * closeItem->getContentSize().width/2 ,
                                 origin.y + 3 * closeItem->getContentSize().height/2));


    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem,closeItem1,closeItem2, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if ANDROID
    PluginChannel::getInstance()->login();
#endif
}

void HelloWorld::shareQQ(cocos2d::Ref * pSender){
    
#if ANDROID
    if (PluginChannel::getInstance()->isLogined()) {
        CCLOG("登陆成功");
        ProtocolShare * proshare = AgentManager::getInstance()->getSharePlugin();
        proshare->setListener();
//        proshare->setResultListener();
        if (proshare == nullptr) {
            CCLOG("分享为空");
        }else{
            CCLOG("分享吧");
            TShareInfo tsi;
            tsi.insert(std::pair<std::string,std::string>("title","ShareSDK是一个神奇的SDK"));
            
            
            
            std::map<std::string, std::string> info;
            // title标题，印象笔记、邮箱、信息、微信、人人网和QQ空间使用
            info["title"] = "ShareSDK是一个神奇的SDK";
            // titleUrl是标题的网络链接，仅在人人网和QQ空间使用
            info["titleUrl"] = "http://sharesdk.cn";
            // site是分享此内容的网站名称，仅在QQ空间使用
            info["site"] = "ShareSDK";
            // siteUrl是分享此内容的网站地址，仅在QQ空间使用
            info["siteUrl"] = "http://sharesdk.cn";
            
            // imagePath是图片的本地路径，Linked-In以外的平台都支持此参数
            info["imagePath"] = "/sdcard/test.png";
            // imageUrl是图片的网络路径，新浪微博，人人网，QQ空间支持此字段
            info["imageUrl"] = "http://www.baidu.com/img/bdlogo.png?tn=63090008_1_hao_pg";
            
            // url仅在微信（包括好友和朋友圈）中使用
            info["url"] ="http://sharesdk.cn";
            
            // text是分享文本，所有平台都需要这个字段
            info["text"] = "ShareSDK支持如微信、新浪微博、腾讯微博等社交平台";
            // comment是我对这条分享的评论，仅在人人网和QQ空间使用
            info["comment"] = "无";
            
            
            proshare->share(info);
        }
    }else{
        CCLOG("登陆失败");
    }
#endif
}

void HelloWorld::pay(cocos2d::Ref * pSender){
#if ANDROID
    PluginChannel::getInstance()->payment();
#endif
}


//#if ANDROID
//extern "C"
//{
//    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_wrapper_nativeInitPlugins(JNIEnv*  env, jobject thiz)
//    {
//        CCLOG("init plugins ");
//    }
//    
//    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_wrapper_nativeUnLoadPlugins(JNIEnv*  env, jobject thiz)
//    {
//        CCLOG("un load plugin");
//    }
//}
//#endif

