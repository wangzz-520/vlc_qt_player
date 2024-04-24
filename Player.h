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
	// ����·��ΪstrPath���ļ�
	bool Play(const std::string &strPath);
	// ����
    void Play();
	// ��ͣ
    void Pause();
	// ֹͣ
    void Stop();
	// ��������
	void Resume();					
	// ��������ΪiVol
    int Volume(int iVol);
	// ��������
    void VolumeIncrease();
	// ������С 
    void VolumeReduce();
	// ����ָ��λ��iPos
    void SeekTo(float iPos); 
	// ���
    void SeekForward();
	// ���x1, x2, x4, x8, x16
	void SeekForward(ENUM_FORWARD_TYPE);
	// ����
    void SeekBackward();
	// ����x1, x2, x4, x8, x16
	void SeekBackward(ENUM_REVERSE_TYPE);
	//��ͼ
	int  SnapShoot(const char *psz_filepath);
	//��Ļ����
	int GetSubtitleCount();
	//��ȡ��Ļ
	void GetSubtitles(libvlc_track_description_t* subtitles);
	//������Ļ
	void SetSubtitile(bool open);
	// ������Ƶ��ʾ�Ĵ��ھ��
	void SetHWND(void *hwnd);
	// ��ȡ��Ƶ��ʾ�Ĵ��ھ��
	void *GetHWND();
	// �ļ��Ƿ��
    bool IsOpen(); 
	// �ļ��Ƿ����ڲ���
    bool IsPlaying();
	// ��ȡ�ļ���ǰ���ŵ�λ��
    int GetPos();
	// ��ȡ��ʱ��
	__int64 GetTotalTime();
	// ��ȡʱ��
	__int64 GetTime();
	// ��ȡ����
    int GetVolume();
	//��������
	void setScale(PLAYER_VIDEO_SCALE_TYPE);
	//���ÿ�߱�
	void setRadio(PLAYER_VIDEO_RADIO_TYPE);
	//���òü�
	void setCrop(PLAYER_VIDEO_CROP_TYPE);
	//������Ƶ�豸
	void setAudioDev(std::string dev);
	//����������ģʽ
	void setAudioModule(int index);
	//���ò����ٶ�
	void setPlaySpeed(float speed);
	//�����Ļ�ļ�
	void addSubTitle(const char *filePath);

private:
	// VLCʵ��
	libvlc_instance_t *m_pVLC_Inst = nullptr;
	// VLC������
	libvlc_media_player_t *m_pVLC_Player = nullptr;
	//ý��
	libvlc_media_t *m = nullptr;
	// ��Ƶ��ʾ�Ĵ��ھ��
	void *m_hWnd = nullptr;
};

#endif
