#pragma once
#include "uart/ProtocolSender.h"
/*
*此文件由GUI工具生成
*文件功能：用于处理用户的逻辑相应代码
*功能说明：
*========================onButtonClick_XXXX
当页面中的按键按下后系统会调用对应的函数，XXX代表GUI工具里面的[ID值]名称，
如Button1,当返回值为false的时候系统将不再处理这个按键，返回true的时候系统将会继续处理此按键。比如SYS_BACK.
*========================onSlideWindowItemClick_XXXX(int index) 
当页面中存在滑动窗口并且用户点击了滑动窗口的图标后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如slideWindow1;index 代表按下图标的偏移值
*========================onSeekBarChange_XXXX(int progress) 
当页面中存在滑动条并且用户改变了进度后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如SeekBar1;progress 代表当前的进度值
*========================ogetListItemCount_XXXX() 
当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表的总数目,XXX代表GUI工具里面的[ID值]名称，
如List1;返回值为当前列表的总条数
*========================oobtainListItemData_XXXX(ZKListView::ZKListItem *pListItem, int index)
 当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表当前条目下的内容信息,XXX代表GUI工具里面的[ID值]名称，
如List1;pListItem 是贴图中的单条目对象，index是列表总目的偏移量。具体见函数说明
*========================常用接口===============
*LOGD(...)  打印调试信息的接口
*mTextXXXPtr->setText("****") 在控件TextXXX上显示文字****
*mButton1Ptr->setSelected(true); 将控件mButton1设置为选中模式，图片会切换成选中图片，按钮文字会切换为选中后的颜色
*mSeekBarPtr->setProgress(12) 在控件mSeekBar上将进度调整到12
*mListView1Ptr->refreshListView() 让mListView1 重新刷新，当列表数据变化后调用
*mDashbroadView1Ptr->setTargetAngle(120) 在控件mDashbroadView1上指针显示角度调整到120度
*
* 在Eclipse编辑器中  使用 "alt + /"  快捷键可以打开智能提示
*/

#include "net/context.h"
#include "link/context.h"
#include "uart/context.h"
#include "bt/context.h"
#include "media/audio_context.h"
#include "media/media_context.h"
#include "media/music_player.h"
#include "media/media_parser.h"
#include "system/setting.h"
#include "system/fm_emit.h"
#include "system/reverse.h"
#include "manager/LanguageManager.h"
#include "manager/ConfigManager.h"
#include "storage/StoragePreferences.h"
#include "misc/storage.h"
#include "fy/files.hpp"
#include "net/NetManager.h"
#include "os/MountMonitor.h"
#include "system/usb_monitor.h"
#include "tire/tire_parse.h"
#include "sysapp_context.h"
#include "utils/BitmapHelper.h"
#include <base/ui_handler.h>
#include "system/hardware.h"
#include "utils/TimeHelper.h"
#include "mode_observer.h"

#define WIFIMANAGER			NETMANAGER->getWifiManager()

#define QUERY_LINK_AUTH_TIMER	3
#define SWITCH_ADB_TIMER	4
//#define BT_MUSIC_ID3		5
#define MUSIC_ERROR_TIMER	20

static bool _s_need_reopen_linkview;

// 添加页面状态管理
static int _current_page_index = 0;  // 当前页面索引
static bool _is_in_sub_pages = false; // 是否在子页面中
static bool _is_in_reverse_mode = false;      // 倒车状态
static bool _background_resources_loaded = false; // 背景资源加载状态
static bool _is_exiting_reverse = false;      // 新增：标记是否正在从倒车模式退出

/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
	//{1,  1000},
};

bool handle_main_page_back() {  // 页面返回处理函数
    if (mPageWindow1Ptr != NULL) {
        int currentPage = mPageWindow1Ptr->getCurrentPage();
        LOGD("Current page index: %d, _is_in_sub_pages: %s", currentPage, _is_in_sub_pages ? "true" : "false");

        if (_is_in_sub_pages || currentPage != 0) {
			mPageWindow1Ptr->setCurrentPage(0);  // 如果当前在子页面（music/video/photoalbum），返回到第一页
            _current_page_index = 0;
            _is_in_sub_pages = false;
            LOGD("Back to main page (page 0)");
            return true; // 返回true表示已处理返回事件
        } else {
            // 如果当前在第一页，返回false让系统处理（退出当前activity）
            LOGD("At main page, let system handle back");
            return false; // 让系统处理，会调用 EASYUICONTEXT->goBack()
        }
    }
    return false; // 返回false表示未处理，让系统继续处理
}

// 页面切换监听函数
static void onPageChange(int page) {
    _current_page_index = page;
    _is_in_sub_pages = (page != 0);
    LOGD("Page changed to: %d, is_in_sub_pages: %s", page, _is_in_sub_pages ? "true" : "false");
}

static void entry_lylink_ftu() {
	if (!sys::reverse_does_enter_status()) {
		EASYUICONTEXT->openActivity("lylinkviewActivity");
		_s_need_reopen_linkview = false;
	} else {
		LOGD("Is reverse status !!!\n");
		lk::video_stop();
		_s_need_reopen_linkview = true;
	}
}

// **内存优化：优化背景资源清理函数**
static void cleanup_background_resources() {
    if (_background_resources_loaded) {
        LOGD("[main2] Cleaning up background resources");

        if (mPageWindow1Ptr) {
            mPageWindow1Ptr->setBackgroundBmp(NULL);
        }

        _background_resources_loaded = false;
    }
}

static void set_back_pic() {
	// **内存优化：避免重复加载背景资源**
	if (_background_resources_loaded) {
		return;
	}

	bitmap_t *bg1_bmp = NULL;
	BitmapHelper::loadBitmapFromFile(bg1_bmp, CONFIGMANAGER->getResFilePath("/HomePage/car_home_wallpaper_first.jpg").c_str(), 3);

	if (mPageWindow1Ptr) {
		mPageWindow1Ptr->setBackgroundBmp(bg1_bmp);
	}

	_background_resources_loaded = true;
}


// **新增：倒车前窗口状态记录结构**
struct WindowState {
    bool window3_visible;
    bool window4_visible;
};

// **新增：添加窗口状态管理变量**
static WindowState _reverse_backup_window_state = {false, false};
static bool _has_backup_window_state = false;

void set_activity_transition_flag(bool flag) {
    LOGD("[main2] Activity transition flag set to: %s", flag ? "true" : "false");
}

// **新增：记录当前窗口显示状态的函数**
static void backup_current_window_state() {
    if (mWindow3Ptr) {
        _reverse_backup_window_state.window3_visible = mWindow3Ptr->isVisible();
    }
    if (mWindow4Ptr) {
        _reverse_backup_window_state.window4_visible = mWindow4Ptr->isVisible();
    }
    _has_backup_window_state = true;

    LOGD("[main2] Backup window state - Window3: %s, Window4: %s",
         _reverse_backup_window_state.window3_visible ? "visible" : "hidden",
         _reverse_backup_window_state.window4_visible ? "visible" : "hidden");
}

// **新增：隐藏所有窗口的函数**
static void hide_all_windows() {
    LOGD("[main2] Hiding all windows before reverse exit");
    if (mWindow3Ptr) {
        mWindow3Ptr->hideWnd();
    }
    if (mWindow4Ptr) {
        mWindow4Ptr->hideWnd();
    }
}

// **新增：恢复倒车前窗口状态的函数**
static void restore_backup_window_state() {
    if (!_has_backup_window_state) {
        LOGD("[main2] No backup window state to restore");
        return;
    }

    LOGD("[main2] Restoring backup window state - Window3: %s, Window4: %s",
         _reverse_backup_window_state.window3_visible ? "show" : "keep hidden",
         _reverse_backup_window_state.window4_visible ? "show" : "keep hidden");

    if (_reverse_backup_window_state.window3_visible && mWindow3Ptr) {
        mWindow3Ptr->showWnd();
    }
    if (_reverse_backup_window_state.window4_visible && mWindow4Ptr) {
        mWindow4Ptr->showWnd();
    }

    // 清除备份状态
    _has_backup_window_state = false;
}


static void parser() {

}

static void update_main_music_time() {

}

static void _update_music_info() {

}

static void _update_music_progress() {

}


namespace { // 加个匿名作用域，防止多个源文件定义相同类名，运行时冲突
 // 实现自己的监听接口
 class MyPageChangeListener : public ZKPageWindow::IPageChangeListener {
 public:
     virtual void onPageChange(ZKPageWindow *pPageWindow, int page) {
         LOGD("当前切换到第%d页", page);
         ::onPageChange(page);  // 调用我们的页面切换处理函数
     }
 };

 static MyPageChangeListener _s_page_change_listener;
 }

namespace{
class AppSlidePageChangeListener : public ZKSlideWindow::ISlidePageChangeListener {
protected:
	virtual void onSlidePageChange(ZKSlideWindow *pSlideWindow, int page) {
		mStatusRadioGroupPtr->setCheckedID((page == 0) ? ID_MAIN2_RadioButton0 : ID_MAIN2_RadioButton1);
	}
};
}
static AppSlidePageChangeListener _s_app_slide_page_change_listener;

/**
 * 当界面构造时触发
 */
static void onUI_init(){

	    // **内存优化：初始化状态变量**
	    _is_in_reverse_mode = false;
	    _background_resources_loaded = false;
	    _is_exiting_reverse = false;  // 初始化新增的状态变量

	    mStatusRadioGroupPtr->setCheckedID(ID_MAIN2_RadioButton0);
	    mpage3RadioGroupPtr->setCheckedID(ID_MAIN2_RadioButton3);


	    // 注册页面切换监听器
	    if (mPageWindow1Ptr != NULL) {
	        mPageWindow1Ptr->setPageChangeListener(&_s_page_change_listener);
	    }

}

/**
 * 当切换到该界面时触发
 */
static void onUI_intent(const Intent *intentPtr) {
    if (intentPtr != NULL) {
        //TODO
    }
}

/*
 * 当界面显示时触发
 */
static void onUI_show() {
	mode::set_switch_mode(E_SWITCH_MODE_NULL);
	    LOGD("[main2] Transitioning setup");
	    _is_in_reverse_mode = false;

	    int curPos = -1;

	    // 先隐藏窗口
	    backup_current_window_state();
	    hide_all_windows();

	    // 设置背景和恢复窗口
	    set_back_pic();
	    restore_backup_window_state();
	    LOGD("[main2] Background and windows restored");
}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {

}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
	if (mPageWindow1Ptr != NULL) {
		mPageWindow1Ptr->setPageChangeListener(NULL);
	}
}

/**
 * 串口数据回调接口
 */
static void onProtocolDataUpdate(const SProtocolData &data) {

}

/**
 * 定时器触发函数
 * 不建议在此函数中写耗时操作，否则将影响UI刷新
 * 参数： id
 *         当前所触发定时器的id，与注册时的id相同
 * 返回值: true
 *             继续运行当前定时器
 *         false
 *             停止运行当前定时器
 */
static bool onUI_Timer(int id){
	switch (id) {
	case 0: {
		unsigned long freeram = 0;
	}
		break;
	case 1: {

	}
		break;
	case QUERY_LINK_AUTH_TIMER:		// 查询互联授权状态
		lk::query_is_authorized();
		return false;
	case SWITCH_ADB_TIMER: {
		if (strcmp("UpgradeActivity", EASYUICONTEXT->currentAppName()) == 0) {
			app::hide_topbar();
		}
		if (sys::setting::is_usb_adb_enabled()) {
			if (sys::get_usb_mode() != E_USB_MODE_DEVICE) {
				sys::change_usb_mode(E_USB_MODE_DEVICE);
			}
		} else {
			sys::set_usb_config(E_USB_MODE_HOST);
		}

		// unmute
		uart::set_amplifier_mute(1);
	}
		return false;
	case MUSIC_ERROR_TIMER:
		media::music_next(true);
		return false;
    case base::TIMER_UI_HANDLER:
      return base::UiHandler::onTimer();
      break;
	default:
		break;
	}
    return true;
}

/**
 * 有新的触摸事件时触发
 * 参数：ev
 *         新的触摸事件
 * 返回值：true
 *            表示该触摸事件在此被拦截，系统不再将此触摸事件传递到控件上
 *         false
 *            触摸事件将继续传递到控件上
 */


static void open_linkhelp_activity(link_mode_e mode) {
	Intent *i = new Intent;
	i->putExtra("link_mode", fy::format("%d", mode));
	LOGD("[main2] choose link mode %s\n", sys::setting::get_link_mode_str(mode));
	EASYUICONTEXT->openActivity("linkhelpActivity", i);
}

static void _change_link_app(link_mode_e mode) {
	switch (mode) {
	case E_LINK_MODE_HICAR:
	case E_LINK_MODE_ANDROIDAUTO:
	case E_LINK_MODE_CARPLAY:
		if (net::get_mode() != E_NET_MODE_AP) { net::change_mode(E_NET_MODE_AP);}
		break;
	case E_LINK_MODE_AIRPLAY:
		if (net::get_mode() != E_NET_MODE_AP) { net::change_mode(E_NET_MODE_AP); }
		break;
	case E_LINK_MODE_CARLIFE:
		if (net::get_mode() != E_NET_MODE_WIFI) { net::change_mode(E_NET_MODE_WIFI); }
		break;
	case E_LINK_MODE_MIRACAST:
	case E_LINK_MODE_LYLINK:
		if (net::get_mode() != E_NET_MODE_P2P) { net::change_mode(E_NET_MODE_P2P); }
		break;
	default:
		break;
	}
	open_linkhelp_activity(mode);
}

static void open_link_activity(link_mode_e mode) {
	LYLINK_TYPE_E link_type = lk::get_lylink_type();
	switch(mode) {
	case E_LINK_MODE_CARPLAY:
		if ((link_type == LINK_TYPE_WIFICP) || (link_type == LINK_TYPE_USBCP)) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_ANDROIDAUTO:
		if ((link_type == LINK_TYPE_WIFIAUTO) || (link_type == LINK_TYPE_USBAUTO)) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_CARLIFE:
		if ((link_type == LINK_TYPE_WIFILIFE) || (link_type == LINK_TYPE_USBLIFE)) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_HICAR:
		if ((link_type == LINK_TYPE_WIFIHICAR) || (link_type == LINK_TYPE_USBHICAR)) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_MIRACAST:
		if (link_type == LINK_TYPE_MIRACAST) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_LYLINK:
		if (link_type == LINK_TYPE_WIFILY) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_AIRPLAY:
		if (link_type == LINK_TYPE_AIRPLAY) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	default:
		break;
	}
	if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return;
	}

	if (mode == E_LINK_MODE_AIRPLAY || mode == E_LINK_MODE_LYLINK || mode ==E_LINK_MODE_MIRACAST) {
		if (bt::is_on()) {
			bt::power_off();
		}
	} else {
		if (!bt::is_on()) {
			bt::power_on();
		}
	}

	open_linkhelp_activity(mode);
//	_change_link_app(mode);
}

static bool onmain2ActivityTouchEvent(const MotionEvent &ev) {
	LayoutPosition pos = EASYUICONTEXT->getNaviBar()->getPosition();
	if (pos.mTop != -pos.mHeight) {	return false; } 	// 导航栏下滑了
	switch (ev.mActionStatus) {
	case MotionEvent::E_ACTION_DOWN:	//触摸按下
		break;
	case MotionEvent::E_ACTION_MOVE:	//触摸滑动
		break;
	case MotionEvent::E_ACTION_UP:  	//触摸抬起
		break;
	default:
		break;
	}
	return false;
}
static void onCheckedChanged_StatusRadioGroup(ZKRadioGroup* pRadioGroup, int checkedID) {
    LOGD(" RadioGroup StatusRadioGroup checked %d", checkedID);
}

static bool onButtonClick_carplayPage2Button(ZKButton *pButton) {
    LOGD(" ButtonClick carplayButton !!!\n");
    open_link_activity(E_LINK_MODE_CARPLAY);
    return false;
}

static bool onButtonClick_androidautoPage2Button(ZKButton *pButton) {
    LOGD(" ButtonClick androidautoButton !!!\n");
    open_link_activity(E_LINK_MODE_ANDROIDAUTO);
    return false;
}

static bool onButtonClick_airplayPage2Button(ZKButton *pButton) {
    LOGD(" ButtonClick airpalyButton !!!\n");
    open_link_activity(E_LINK_MODE_AIRPLAY);
    return false;
}

static bool onButtonClick_aicastPage2Button(ZKButton *pButton) {
    open_link_activity(E_LINK_MODE_LYLINK);
    return false;
}

static bool onButtonClick_miracastPage2Button(ZKButton *pButton) {
    LOGD(" ButtonClick miracastPage2Button !!!\n");
    open_link_activity(E_LINK_MODE_MIRACAST);
    return false;
}

static bool onButtonClick_bluetoothPage2Button(ZKButton *pButton) {
    LOGD(" ButtonClick bluetoothPage2Button !!!\n");
    if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return false;
	}
	EASYUICONTEXT->openActivity("btsettingActivity");
    return false;
}

static bool onButtonClick_musicPage2Button(ZKButton *pButton) {
    LOGD(" ButtonClick musicPage2Button !!!\n");
    if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return false;
	}
	EASYUICONTEXT->openActivity("musicActivity");
    return false;
}

static bool onButtonClick_videoPage2Button(ZKButton *pButton) {
    LOGD(" ButtonClick videoPage2Button !!!\n");
    if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return false;
	}
	EASYUICONTEXT->openActivity("videoActivity");
    return false;
}

static bool onButtonClick_albumPage2Button(ZKButton *pButton) {
    LOGD(" ButtonClick albumPage2Button !!!\n");
    if (lk::is_connected()) {
			mlinkTipsWindowPtr->showWnd();
			return false;
		}

	EASYUICONTEXT->openActivity("PhotoAlbumActivity");
    return false;
}

static bool onButtonClick_settingPage2Button(ZKButton *pButton) {
    LOGD(" ButtonClick settingButton !!!\n");
    EASYUICONTEXT->openActivity("setshowActivity");
    return false;
}


static void onCheckedChanged_page3RadioGroup(ZKRadioGroup* pRadioGroup, int checkedID) {
    LOGD(" RadioGroup page3RadioGroup checked %d", checkedID);
}

static bool onButtonClick_audiooutputButton(ZKButton *pButton) {
    LOGD(" ButtonClick volumeoutputButton !!!\n");
    // 这个按钮在原函数中没有对应的Activity，可能需要添加具体逻辑
    EASYUICONTEXT->openActivity("FMemitActivity");
    return false;
}
