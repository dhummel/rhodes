#include "generated/cpp/NativeMenubarBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"
#include "rubyext/WebView.h"

#ifdef RHODES_EMULATOR
#define RHO_APPS_DIR ""
#else
#define RHO_APPS_DIR "apps/"
#endif

extern "C" void rho_sys_app_exit();

namespace rho 
{

#ifdef OS_ANDROID

void rho_impl_setNativeMenu(const Vector<String>& menu);
String rho_impl_getNativeMenu();

#endif

using namespace apiGenerator;
using namespace common;

class CNativeMenubarSingletonImpl : public CNativeMenubarSingletonBase
{

virtual void getMainMenu(rho::apiGenerator::CMethodResult& oResult)
{
#ifdef OS_ANDROID
    oResult.setJSON(rho_impl_getNativeMenu());
#else
    rho::Vector< Hashtable<String, String> > arRes;
    RHODESAPP().getAppMenu().getMenuItemsEx(arRes);

    oResult.set(arRes);
#endif
}

virtual void setMainMenu( const rho::Vector<rho::String>& mainMenu, rho::apiGenerator::CMethodResult& oResult)
{
#ifdef OS_ANDROID
    rho_impl_setNativeMenu(mainMenu);
#else
    RHODESAPP().getAppMenu().setAppMenuJSONItems(mainMenu);
    
#if defined (_WIN32_WCE) && !defined (OS_PLATFORM_MOTCE)
    rho_webview_update_menu(1);
#endif

#endif
}

virtual void getExtraMenu(rho::apiGenerator::CMethodResult& oResult)
{
    rho::Vector< Hashtable<String, String> > arRes;
    RHODESAPP().getAppMenu().getMenuItemsEx(arRes, true);

    oResult.set(arRes);
}

virtual void setExtraMenu( const rho::Vector<rho::String>& extraMenu, rho::apiGenerator::CMethodResult& oResult)
{
    RHODESAPP().getAppMenu().setAppMenuJSONItems(extraMenu, true);
}

virtual void getMainButton(rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String, String> hashRes;
    RHODESAPP().getAppMenu().getMenuButtonEx(hashRes);
    oResult.set(hashRes);
}

virtual void setMainButton( const rho::Hashtable<rho::String, rho::String>& mainButton, rho::apiGenerator::CMethodResult& oResult)
{
    rho::Hashtable<rho::String, rho::String>::const_iterator it = mainButton.begin();

    if ( it != mainButton.end() )
    {
        RHODESAPP().getAppMenu().setRightButton(mainButton);
    }

#if defined (_WIN32_WCE) && !defined (OS_PLATFORM_MOTCE)
    rho_webview_update_menu(1);
#endif
}

virtual void getExtraButton(rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<String, String> hashRes;
    RHODESAPP().getAppMenu().getMenuButtonEx(hashRes, true);
    oResult.set(hashRes);
}

virtual void setExtraButton( const rho::Hashtable<rho::String, rho::String>& extraButton, rho::apiGenerator::CMethodResult& oResult)
{
    const rho::Hashtable<rho::String, rho::String>::const_iterator it = extraButton.begin();
    if ( it != extraButton.end() )
    {
        RHODESAPP().getAppMenu().setLeftButton(extraButton);
    }

#if defined (_WIN32_WCE) && !defined (OS_PLATFORM_MOTCE)
    rho_webview_update_menu(1);
#endif
}

/* --------------------------------------------------------------------- */

virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor)
{
}

virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor)
{
}

};

////////////////////////////////////////////////////////////////////////

class CNativeMenubarFactory: public CNativeMenubarFactoryBase
{
public:
    ~CNativeMenubarFactory(){}

    INativeMenubarSingleton* createModuleSingleton()
    { 
        return new CNativeMenubarSingletonImpl(); 
    }
};

} //rho

extern "C" void Init_NativeMenuBar()
{
    rho::CNativeMenubarFactory::setInstance( new rho::CNativeMenubarFactory() );
    rho::Init_NativeMenubar_API();
}