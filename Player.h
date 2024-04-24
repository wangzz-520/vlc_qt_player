#ifndef __Player_H__
#define __Player_H__
#include <iostream>
#include <Windows.h>
#include <vlc/vlc.h>
#include "Global.h"

struct libvlc_instance_t;
struct libvlc_media_player_t;
struct libvlc_event_t;
struct libvlc_track_description_t;

using namespace zz;
using namespace std;

class Player
{
public:
    Player(void);
    ~Player(void);

public:
	// 播放路径为strPath的文件
	bool Play(const std::string &strPath);
	// 播放
    void Play();
	// 暂停
    void Pause();
	// 停止
    void Stop();
	// 继续播放
	void Resume();					
	// 音量设置为iVol
    int Volume(int iVol);
	// 音量增大
    void VolumeIncrease();
	// 音量减小 
    void VolumeReduce();
	// 跳到指定位置iPos
    void SeekTo(float iPos); 
	// 快进
    void SeekForward();
	// 快进x1, x2, x4, x8, x16
	void SeekForward(ENUM_FORWARD_TYPE);
	// 快退
    void SeekBackward();
	// 快退x1, x2, x4, x8, x16
	void SeekBackward(ENUM_REVERSE_TYPE);
	//截图
	int  SnapShoot(const char *psz_filepath);
	//字幕个数
	int GetSubtitleCount();
	//获取字幕
	void GetSubtitles(libvlc_track_description_t* subtitles);
	//设置字幕
	void SetSubtitile(bool open);
	// 设置视频显示的窗口句柄
	void SetHWND(void *hwnd);
	// 获取视频显示的窗口句柄
	void *GetHWND();
	// 文件是否打开
    bool IsOpen(); 
	// 文件是否正在播放
    bool IsPlaying();
	// 获取文件当前播放的位置
    int GetPos();
	// 获取总时间
	__int64 GetTotalTime();
	// 获取时间
	__int64 GetTime();
	// 获取音量
    int GetVolume();
	//设置缩放
	void setScale(PLAYER_VIDEO_SCALE_TYPE);
	//设置宽高比
	void setRadio(PLAYER_VIDEO_RADIO_TYPE);
	//设置裁剪
	void setCrop(PLAYER_VIDEO_CROP_TYPE);
	//设置音频设备
	void setAudioDev(std::string dev);
	//设置立体声模式
	void setAudioModule(int index);
	//设置播放速度
	void setPlaySpeed(float speed);
	//添加字幕文件
	void addSubTitle(const char *filePath);

private:
	// VLC实例
	libvlc_instance_t *m_pVLC_Inst = nullptr;
	// VLC播放器
	libvlc_media_player_t *m_pVLC_Player = nullptr;
	//媒体
	libvlc_media_t *m = nullptr;
	// 视频显示的窗口句柄
	void *m_hWnd = nullptr;
};

#endif
