/***********************************************
/gen auto by zuitools
***********************************************/
#include "main2Activity.h"

/*TAG:GlobalVariable全局变量*/
static ZKTextView* mTextView3Ptr;
static ZKWindow* mWindow1Ptr;
static ZKWindow* mbtTipsWindowPtr;
static ZKTextView* mTextView6Ptr;
static ZKWindow* mWindow5Ptr;
static ZKWindow* mlinkTipsWindowPtr;
static ZKButton* maudiooutputButtonPtr;
static ZKRadioGroup* mpage3RadioGroupPtr;
static ZKTextView* mTextView7Ptr;
static ZKWindow* mWindow4Ptr;
static ZKButton* msettingPage2ButtonPtr;
static ZKButton* malbumPage2ButtonPtr;
static ZKButton* mvideoPage2ButtonPtr;
static ZKButton* mmusicPage2ButtonPtr;
static ZKButton* mbluetoothPage2ButtonPtr;
static ZKButton* mmiracastPage2ButtonPtr;
static ZKButton* maicastPage2ButtonPtr;
static ZKButton* mairplayPage2ButtonPtr;
static ZKButton* mandroidautoPage2ButtonPtr;
static ZKButton* mcarplayPage2ButtonPtr;
static ZKRadioGroup* mStatusRadioGroupPtr;
static ZKTextView* mTextView5Ptr;
static ZKWindow* mWindow3Ptr;
static ZKPageWindow* mPageWindow1Ptr;
static main2Activity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(main2Activity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/main2Logic.cc"

/***********/
typedef struct {
    int id;
    const char *pApp;
} SAppInfo;

/**
 *点击跳转window
 */
static SAppInfo sAppInfoTab[] = {
//  { ID_MAIN_TEXT, "TextViewActivity" },
};

/***************/
typedef bool (*ButtonCallback)(ZKButton *pButton);
/**
 * button onclick表
 */
typedef struct {
    int id;
    ButtonCallback callback;
}S_ButtonCallback;

/*TAG:ButtonCallbackTab按键映射表*/
static S_ButtonCallback sButtonCallbackTab[] = {
    ID_MAIN2_audiooutputButton, onButtonClick_audiooutputButton,
    ID_MAIN2_settingPage2Button, onButtonClick_settingPage2Button,
    ID_MAIN2_albumPage2Button, onButtonClick_albumPage2Button,
    ID_MAIN2_videoPage2Button, onButtonClick_videoPage2Button,
    ID_MAIN2_musicPage2Button, onButtonClick_musicPage2Button,
    ID_MAIN2_bluetoothPage2Button, onButtonClick_bluetoothPage2Button,
    ID_MAIN2_miracastPage2Button, onButtonClick_miracastPage2Button,
    ID_MAIN2_aicastPage2Button, onButtonClick_aicastPage2Button,
    ID_MAIN2_airplayPage2Button, onButtonClick_airplayPage2Button,
    ID_MAIN2_androidautoPage2Button, onButtonClick_androidautoPage2Button,
    ID_MAIN2_carplayPage2Button, onButtonClick_carplayPage2Button,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
};


typedef int (*ListViewGetItemCountCallback)(const ZKListView *pListView);
typedef void (*ListViewobtainListItemDataCallback)(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index);
typedef void (*ListViewonItemClickCallback)(ZKListView *pListView, int index, int id);
typedef struct {
    int id;
    ListViewGetItemCountCallback getListItemCountCallback;
    ListViewobtainListItemDataCallback obtainListItemDataCallback;
    ListViewonItemClickCallback onItemClickCallback;
}S_ListViewFunctionsCallback;
/*TAG:ListViewFunctionsCallback*/
static S_ListViewFunctionsCallback SListViewFunctionsCallbackTab[] = {
};


typedef void (*SlideWindowItemClickCallback)(ZKSlideWindow *pSlideWindow, int index);
typedef struct {
    int id;
    SlideWindowItemClickCallback onSlideItemClickCallback;
}S_SlideWindowItemClickCallback;
/*TAG:SlideWindowFunctionsCallbackTab*/
static S_SlideWindowItemClickCallback SSlideWindowItemClickCallbackTab[] = {
};


typedef void (*EditTextInputCallback)(const std::string &text);
typedef struct {
    int id;
    EditTextInputCallback onEditTextChangedCallback;
}S_EditTextInputCallback;
/*TAG:EditTextInputCallback*/
static S_EditTextInputCallback SEditTextInputCallbackTab[] = {
};

typedef void (*VideoViewCallback)(ZKVideoView *pVideoView, int msg);
typedef struct {
    int id; //VideoView ID
    bool loop; // 是否是轮播类型
    int defaultvolume;//轮播类型时,默认视频音量
    VideoViewCallback onVideoViewCallback;
}S_VideoViewCallback;
/*TAG:VideoViewCallback*/
static S_VideoViewCallback SVideoViewCallbackTab[] = {
};


typedef void (*RadioGroupCallback)(ZKRadioGroup*, int);
typedef struct {
  int id;
  RadioGroupCallback onCheckedChanged;
}S_RadioGroupCallback;
/*TAG:RadioGroupCallbackTab*/
static S_RadioGroupCallback SRadioGroupCallbackTab[] = {
    ID_MAIN2_page3RadioGroup, onCheckedChanged_page3RadioGroup,
    ID_MAIN2_StatusRadioGroup, onCheckedChanged_StatusRadioGroup,
};

main2Activity::main2Activity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

main2Activity::~main2Activity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    mTextView3Ptr = NULL;
    mWindow1Ptr = NULL;
    mbtTipsWindowPtr = NULL;
    mTextView6Ptr = NULL;
    mWindow5Ptr = NULL;
    mlinkTipsWindowPtr = NULL;
    maudiooutputButtonPtr = NULL;
    mpage3RadioGroupPtr = NULL;
    mTextView7Ptr = NULL;
    mWindow4Ptr = NULL;
    msettingPage2ButtonPtr = NULL;
    malbumPage2ButtonPtr = NULL;
    mvideoPage2ButtonPtr = NULL;
    mmusicPage2ButtonPtr = NULL;
    mbluetoothPage2ButtonPtr = NULL;
    mmiracastPage2ButtonPtr = NULL;
    maicastPage2ButtonPtr = NULL;
    mairplayPage2ButtonPtr = NULL;
    mandroidautoPage2ButtonPtr = NULL;
    mcarplayPage2ButtonPtr = NULL;
    mStatusRadioGroupPtr = NULL;
    mTextView5Ptr = NULL;
    mWindow3Ptr = NULL;
    mPageWindow1Ptr = NULL;
}

const char* main2Activity::getAppName() const{
	return "main2.ftu";
}

//TAG:onCreate
void main2Activity::onCreate() {
	Activity::onCreate();
    mTextView3Ptr = (ZKTextView*)findControlByID(ID_MAIN2_TextView3);
    mWindow1Ptr = (ZKWindow*)findControlByID(ID_MAIN2_Window1);
    mbtTipsWindowPtr = (ZKWindow*)findControlByID(ID_MAIN2_btTipsWindow);
    mTextView6Ptr = (ZKTextView*)findControlByID(ID_MAIN2_TextView6);
    mWindow5Ptr = (ZKWindow*)findControlByID(ID_MAIN2_Window5);
    mlinkTipsWindowPtr = (ZKWindow*)findControlByID(ID_MAIN2_linkTipsWindow);
    maudiooutputButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_audiooutputButton);
    mpage3RadioGroupPtr = (ZKRadioGroup*)findControlByID(ID_MAIN2_page3RadioGroup);if(mpage3RadioGroupPtr!= NULL){mpage3RadioGroupPtr->setCheckedChangeListener(this);}
    mTextView7Ptr = (ZKTextView*)findControlByID(ID_MAIN2_TextView7);
    mWindow4Ptr = (ZKWindow*)findControlByID(ID_MAIN2_Window4);
    msettingPage2ButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_settingPage2Button);
    malbumPage2ButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_albumPage2Button);
    mvideoPage2ButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_videoPage2Button);
    mmusicPage2ButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_musicPage2Button);
    mbluetoothPage2ButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_bluetoothPage2Button);
    mmiracastPage2ButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_miracastPage2Button);
    maicastPage2ButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_aicastPage2Button);
    mairplayPage2ButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_airplayPage2Button);
    mandroidautoPage2ButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_androidautoPage2Button);
    mcarplayPage2ButtonPtr = (ZKButton*)findControlByID(ID_MAIN2_carplayPage2Button);
    mStatusRadioGroupPtr = (ZKRadioGroup*)findControlByID(ID_MAIN2_StatusRadioGroup);if(mStatusRadioGroupPtr!= NULL){mStatusRadioGroupPtr->setCheckedChangeListener(this);}
    mTextView5Ptr = (ZKTextView*)findControlByID(ID_MAIN2_TextView5);
    mWindow3Ptr = (ZKWindow*)findControlByID(ID_MAIN2_Window3);
    mPageWindow1Ptr = (ZKPageWindow*)findControlByID(ID_MAIN2_PageWindow1);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void main2Activity::onClick(ZKBase *pBase) {
	//TODO: add widget onClik code 
    int buttonTablen = sizeof(sButtonCallbackTab) / sizeof(S_ButtonCallback);
    for (int i = 0; i < buttonTablen; ++i) {
        if (sButtonCallbackTab[i].id == pBase->getID()) {
            if (sButtonCallbackTab[i].callback((ZKButton*)pBase)) {
            	return;
            }
            break;
        }
    }


    int len = sizeof(sAppInfoTab) / sizeof(sAppInfoTab[0]);
    for (int i = 0; i < len; ++i) {
        if (sAppInfoTab[i].id == pBase->getID()) {
            EASYUICONTEXT->openActivity(sAppInfoTab[i].pApp);
            return;
        }
    }

	Activity::onClick(pBase);
}

void main2Activity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void main2Activity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void main2Activity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool main2Activity::onTimer(int id) {
	return onUI_Timer(id);
}

void main2Activity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int main2Activity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void main2Activity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void main2Activity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void main2Activity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool main2Activity::onTouchEvent(const MotionEvent &ev) {
    return onmain2ActivityTouchEvent(ev);
}

void main2Activity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void main2Activity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void main2Activity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
    int tablen = sizeof(SVideoViewCallbackTab) / sizeof(S_VideoViewCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SVideoViewCallbackTab[i].id == pVideoView->getID()) {
        	if (SVideoViewCallbackTab[i].loop) {
                //循环播放
        		videoLoopPlayback(pVideoView, msg, i);
        	} else if (SVideoViewCallbackTab[i].onVideoViewCallback != NULL){
        	    SVideoViewCallbackTab[i].onVideoViewCallback(pVideoView, msg);
        	}
            break;
        }
    }
}

void main2Activity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

	switch (msg) {
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_STARTED:
		LOGD("ZKVideoView::E_MSGTYPE_VIDEO_PLAY_STARTED\n");
    if (callbackTabIndex >= (sizeof(SVideoViewCallbackTab)/sizeof(S_VideoViewCallback))) {
      break;
    }
		pVideoView->setVolume(SVideoViewCallbackTab[callbackTabIndex].defaultvolume / 10.0);
		mVideoLoopErrorCount = 0;
		break;
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_ERROR:
		/**错误处理 */
		++mVideoLoopErrorCount;
		if (mVideoLoopErrorCount > 100) {
			LOGD("video loop error counts > 100, quit loop playback !");
            break;
		} //不用break, 继续尝试播放下一个
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED:
		LOGD("ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED\n");
        std::vector<std::string> videolist;
        std::string fileName(getAppName());
        if (fileName.size() < 4) {
             LOGD("getAppName size < 4, ignore!");
             break;
        }
        fileName = fileName.substr(0, fileName.length() - 4) + "_video_list.txt";
        fileName = "/mnt/extsd/" + fileName;
        if (!parseVideoFileList(fileName.c_str(), videolist)) {
            LOGD("parseVideoFileList failed !");
		    break;
        }
		if (pVideoView && !videolist.empty()) {
			mVideoLoopIndex = (mVideoLoopIndex + 1) % videolist.size();
			pVideoView->play(videolist[mVideoLoopIndex].c_str());
		}
		break;
	}
}

void main2Activity::startVideoLoopPlayback() {
    int tablen = sizeof(SVideoViewCallbackTab) / sizeof(S_VideoViewCallback);
    for (int i = 0; i < tablen; ++i) {
    	if (SVideoViewCallbackTab[i].loop) {
    		ZKVideoView* videoView = (ZKVideoView*)findControlByID(SVideoViewCallbackTab[i].id);
    		if (!videoView) {
    			return;
    		}
    		//循环播放
    		videoLoopPlayback(videoView, ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED, i);
    		return;
    	}
    }
}

void main2Activity::stopVideoLoopPlayback() {
    int tablen = sizeof(SVideoViewCallbackTab) / sizeof(S_VideoViewCallback);
    for (int i = 0; i < tablen; ++i) {
    	if (SVideoViewCallbackTab[i].loop) {
    		ZKVideoView* videoView = (ZKVideoView*)findControlByID(SVideoViewCallbackTab[i].id);
    		if (!videoView) {
    			return;
    		}
    		if (videoView->isPlaying()) {
    		    videoView->stop();
    		}
    		return;
    	}
    }
}

bool main2Activity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
	mediaFileList.clear();
	if (NULL == pFileListPath || 0 == strlen(pFileListPath)) {
        LOGD("video file list is null!");
		return false;
	}

	ifstream is(pFileListPath, ios_base::in);
	if (!is.is_open()) {
		LOGD("cann't open file %s \n", pFileListPath);
		return false;
	}
	char tmp[1024] = {0};
	while (is.getline(tmp, sizeof(tmp))) {
		string str = tmp;
		removeCharFromString(str, '\"');
		removeCharFromString(str, '\r');
		removeCharFromString(str, '\n');
		if (str.size() > 1) {
     		mediaFileList.push_back(str.c_str());
		}
	}
  LOGD("(f:%s, l:%d) parse fileList[%s], get [%d]files", __FUNCTION__,
      __LINE__, pFileListPath, int(mediaFileList.size()));
  for (std::vector<string>::size_type i = 0; i < mediaFileList.size(); i++) {
    LOGD("file[%d]:[%s]", int(i), mediaFileList[i].c_str());
  }
	is.close();

	return true;
}

int main2Activity::removeCharFromString(string& nString, char c) {
    string::size_type   pos;
    while(1) {
        pos = nString.find(c);
        if(pos != string::npos) {
            nString.erase(pos, 1);
        } else {
            break;
        }
    }
    return (int)nString.size();
}

void main2Activity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void main2Activity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void main2Activity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}
void main2Activity::onCheckedChanged(ZKRadioGroup* pRadioGroup, int checkedID) {
    int tablen = sizeof(SRadioGroupCallbackTab) / sizeof(S_RadioGroupCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SRadioGroupCallbackTab[i].id == pRadioGroup->getID()) {
        	SRadioGroupCallbackTab[i].onCheckedChanged(pRadioGroup, checkedID);
            break;
        }
    }
}