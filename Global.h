﻿#ifndef GLOBAL_H 
#define GLOBAL_H

#include <string>
#include <vector>

#define DELAY_10_DAY									(1*60*60*24*10)
#define DELAY_1_DAY										(1*60*60*24)
#define HORIZONTAL_SCROLLBAR_TYPE		0		//横向滚动条
#define VERTICAL_SCROLLBAR_TYPE				1		//纵向滚动条

using namespace std;

namespace zz
{
	typedef enum {
		PLAYER_MENU_OPEN_URL,
		PLAYER_MENU_OPEN_FILE,
		PLAYER_MENU_OPEN_MORE_FILE,
		PLAYER_MENU_OPEN_ADD_SUBTITLE,
		PLAYER_MENU_OPEN_FULLSCREEN,
		PLAYER_MENU_OPEN_EXITFULLSCREEN,
		PLAYER_MENU_OPEN_FRONT,
		PLAYER_MENU_OPEN_NEVERFRONT,
		PLAYER_MENU_OPEN_PLAYSTART,
		PLAYER_MENU_OPEN_PLAYSTOP,
		PLAYER_MENU_OPEN_AUDIO,
		PLAYER_MENU_OPEN_VIDEO,
		PLAYER_MENU_OPEN_SNAP
	}PLAYER_MENU_TYPE;

	typedef enum {
		PLAYER_VIDEO_SCALE,
		PLAYER_VIDEO_RADIO,
		PLAYER_VIDEO_CROP
	}PLAYER_VIDEO_TYPE;

	typedef enum {
		PLAYER_SCALE_1_4,
		PLAYER_SCALE_1_2,
		PLAYER_SCALE_1_1,
		PLAYER_SCALE_2_1
	}PLAYER_VIDEO_SCALE_TYPE;

	typedef enum {
		PLAYER_RADIO_DEFULT,
		PLAYER_RADIO_16_9,
		PLAYER_RADIO_4_3,
		PLAYER_RADIO_1_1,
		PLAYER_RADIO_16_10,
		PLAYER_RADIO_2_21_1,
		PLAYER_RADIO_2_35_1,
		PLAYER_RADIO_2_38_1,
		PLAYER_RADIO_5_4
	}PLAYER_VIDEO_RADIO_TYPE;

	typedef enum {
		PLAYER_CROP_DEFULT,
		PLAYER_CROP_16_10,
		PLAYER_CROP_16_9,
		PLAYER_CROP_4_3,
		PLAYER_CROP_1_85_1,
		PLAYER_CROP_2_21_1,
		PLAYER_CROP_2_35_1,
		PLAYER_CROP_2_39_1,
		PLAYER_CROP_5_3,
		PLAYER_CROP_5_4,
		PLAYER_CROP_1_1
	}PLAYER_VIDEO_CROP_TYPE;

	typedef enum {
		AudioChannel_Error = -1,
		AudioChannel_Stereo = 1,
		AudioChannel_RStereo = 2,
		AudioChannel_Left = 3,
		AudioChannel_Right = 4,
		AudioChannel_Dolbys = 5
	} PLAYER_AUDIO_TYPE;

	typedef enum {
		ENUM_PLAYSTATUS_PLAYING = 0,	//正在播放
		ENUM_PLAYSTATUS_PAUSE,			//暂停播放
		ENUM_PLAYSTATUS_STOP			//停止播放
	}ENUM_PLAYSTATUS_TYPE;

	typedef enum {
		ENUM_FORWARD_X1 = 0,
		ENUM_FORWARD_X2,
		ENUM_FORWARD_X4,
		ENUM_FORWARD_X8,
		ENUM_FORWARD_X16
	} ENUM_FORWARD_TYPE;

	typedef enum {
		ENUM_REVERSE_X1 = 0,
		ENUM_REVERSE_X2,
		ENUM_REVERSE_X4,
		ENUM_REVERSE_X8,
		ENUM_REVERSE_X16
	} ENUM_REVERSE_TYPE;

	typedef enum {
		MODULE_PAGE_PLAY = 0,	//播放界面
		MODULE_PAGE_NETPAGE		//我的网页
	}MODULE_PAGE_ENUM;

	typedef enum {
		ENUM_TOOLBUTTON_IDLE = 0,
		ENUM_TOOLBUTTON_SELECT
	}ENUM_TOOLBUTTON_STYLE_STATE;

	typedef struct {
		std::string videoId;
		std::string videoName;
		std::string videoFilePath;
		int videoPlayTimeStamp;
		std::string setId;
	}VIDEO_TREE_ITEM;


	typedef enum {
		PLAYACTIVE_OPEN_URL,
		PLAYACTIVE_OPEN_DIR,
	}PLAYACTIVE_OPENMENU_TYPE;


}

extern  std::vector<std::string> g_lstAudioDeviceDes;
extern  std::vector<std::string> g_lstAudioDeviceName;

#endif // GLOBAL_H