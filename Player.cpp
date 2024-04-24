#include "Player.h"
#include <cmath>
#include <vector>
#include <QDebug>
#include <QMessageBox>

vector<string> g_lstAudioDeviceDes;
vector<string> g_lstAudioDeviceName;

Player::Player(void) 
{
	if (!m_pVLC_Inst)
	{
		m_pVLC_Inst = libvlc_new(0, NULL);
		if (!m_pVLC_Inst)
		{
			printf("VLC media player could not start.\nEither the command line options were invalid or no plugins were found");
		}
	}

	libvlc_audio_output_device_t *pDevList = nullptr;
	libvlc_audio_output_t *pOutputList = libvlc_audio_output_list_get(m_pVLC_Inst);
	while (pOutputList)
	{
		//获取设备
		pDevList = libvlc_audio_output_device_list_get(m_pVLC_Inst, pOutputList->psz_name);

		while (pDevList)
		{
			// 找到我想要的那个设备，跳出循环，这里使用i控制，我知道我想要的设备位置
			if (strcmp(pOutputList->psz_name, "mmdevice") == 0)
			{
				g_lstAudioDeviceDes.push_back(std::string(pDevList->psz_description));
				g_lstAudioDeviceName.push_back(std::string(pDevList->psz_device));
			}

			pDevList = pDevList->p_next;
		}
		pOutputList = pOutputList->p_next;
	}
}

Player::~Player(void)
{
	Stop();

	if (m_pVLC_Inst)
	{
		libvlc_release(m_pVLC_Inst);
		m_pVLC_Inst = NULL;
	}
}

bool Player::Play(const std::string &strPath)
{
    if (!m_pVLC_Inst)
    {
        m_pVLC_Inst = libvlc_new(0, NULL);
		if (!m_pVLC_Inst)
		{
			printf("VLC media player could not start.\nEither the command line options were invalid or no plugins were found");
		}
    }

    if(strPath.empty() || ! m_pVLC_Inst)
    {
        return false;
    }

    // 验证地址是网络地址，还是本地地址
    bool bURL = false;
    std::vector<std::string> vctURL;

    vctURL.push_back("http"); 
    vctURL.push_back("https"); 
    vctURL.push_back("ftp");
    vctURL.push_back("rstp");
	vctURL.push_back("rtmp");

    for (unsigned i = 0; i < vctURL.size(); i++)
    {
        // 实际使用请判断大小写
        if (! strPath.compare(0, vctURL[i].size(), vctURL[i]))
        {
            bURL = true;
            break;
        }
    }

    Stop();

    bool bRet = false;

    if (bURL)
    {
        // 示例网络地址
        m = libvlc_media_new_location(m_pVLC_Inst, strPath.c_str());
    } 
    else
    {
        m = libvlc_media_new_path(m_pVLC_Inst, strPath.c_str());
    }

    if (m)
    {
		if (m_pVLC_Player = libvlc_media_player_new_from_media(m))
        {
            libvlc_media_player_set_hwnd(m_pVLC_Player, m_hWnd);
            libvlc_media_player_play(m_pVLC_Player);
			//libvlc_video_set_key_input(m_pVLC_Player, true);
            // 事件管理
            libvlc_event_manager_t *vlc_evt_man = libvlc_media_player_event_manager(m_pVLC_Player);
            bRet = true;
        }

        libvlc_media_release(m);
    }

    return bRet;
}

void Player::Stop()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_stop (m_pVLC_Player);      /* Stop playing */    
        libvlc_media_player_release (m_pVLC_Player);   /* Free the media_player */
        m_pVLC_Player = NULL;
    }
}

void Player::Play()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_play(m_pVLC_Player);
    }
}

void Player::Pause()
{
    if (m_pVLC_Player)
    {
		libvlc_media_player_set_pause(m_pVLC_Player, 1);
    }
}

void Player::Resume()
{
	if (m_pVLC_Player)
	{
		libvlc_media_player_set_pause(m_pVLC_Player, 0);
	}
}

int Player::Volume(int iVol)
{
    if (iVol < 0)
    {
        return -1;
    }

    if (m_pVLC_Player)
    {
        return libvlc_audio_set_volume(m_pVLC_Player,int(iVol * 2)); // 如果放到100，感觉比迅雷的100少了30，所以这里用1.3倍音量
    }
	return -1;
}

void Player::VolumeIncrease()
{
    if (m_pVLC_Player)
    {
        int iVol = libvlc_audio_get_volume(m_pVLC_Player);
        Volume((int)ceil(iVol * 1.1));
    }
}

void Player::VolumeReduce()
{
    if (m_pVLC_Player)
    {
        int iVol = libvlc_audio_get_volume(m_pVLC_Player);
        Volume((int)floor(iVol * 0.9));
    }
}

int Player::GetPos()
{   
    if (m_pVLC_Player)
    {
        return (int)(1000 * libvlc_media_player_get_position(m_pVLC_Player));
    }

    return 0;
}

void Player::SeekTo(float iPos)
{
    if (iPos < 0 || iPos > 1)
    {
        return;
    }

    if (m_pVLC_Player)
    {
        libvlc_media_player_set_position(m_pVLC_Player, iPos);
    }
}

void Player::SeekForward()
{
    // 一次快退5秒
    if (m_pVLC_Player)
    {
        libvlc_time_t i_time = libvlc_media_player_get_time(m_pVLC_Player) + 5000;

        if (i_time > GetTotalTime())
        {
            i_time = GetTotalTime();
        }

        libvlc_media_player_set_time(m_pVLC_Player, i_time);
    }
}

void Player::SeekBackward()
{
    if (m_pVLC_Player)
    {
        libvlc_time_t i_time = libvlc_media_player_get_time(m_pVLC_Player) - 5000;

        if (i_time < 0)
        {
            i_time = 0;
        }

        libvlc_media_player_set_time(m_pVLC_Player, i_time);
    }
}

void Player::SeekForward(ENUM_FORWARD_TYPE nX)
{
	if (nullptr == m_pVLC_Player)
		return;

	long nXX = 1000;
	switch(nX)
	{
	case ENUM_FORWARD_X1:
		return;
		nXX = 1000 * 0;
		break;
	case ENUM_FORWARD_X2:
		nXX = 1000 * 1;
		break;
	case ENUM_FORWARD_X4:
		nXX = 1000 * 3;
		break;
	case ENUM_FORWARD_X8:
		nXX = 1000 * 7;
		break;
	case ENUM_FORWARD_X16:
		nXX = 1000 * 15;
		break;
	}

	libvlc_time_t i_time = libvlc_media_player_get_time(m_pVLC_Player) + nXX;

	if (i_time > GetTotalTime())
	{
		i_time = GetTotalTime();
	}

	libvlc_media_player_set_time(m_pVLC_Player, i_time);
}

void Player::SeekBackward(ENUM_REVERSE_TYPE nX)
{
	if (nullptr == m_pVLC_Player)
		return;

	long nXX = 1000;
	switch(nX)
	{
	case ENUM_REVERSE_X1:
		nXX = 1000 * 2;
		break;
	case ENUM_REVERSE_X2:
		nXX = 1000 * 3;
		break;
	case ENUM_REVERSE_X4:
		nXX = 1000 * 5;
		break;
	case ENUM_REVERSE_X8:
		nXX = 1000 * 9;
		break;
	case ENUM_REVERSE_X16:
		nXX = 1000 * 16;
		break;
	}
	if (m_pVLC_Player)
	{
		libvlc_time_t i_time = libvlc_media_player_get_time(m_pVLC_Player) - nXX;

		if (i_time < 0)
		{
			i_time = 0;
		}

		libvlc_media_player_set_time(m_pVLC_Player, i_time);
	}
}

int Player::SnapShoot(const char *psz_filepath)
{
	if (NULL != m_pVLC_Player && psz_filepath != "")
		return libvlc_video_take_snapshot(m_pVLC_Player, 0, psz_filepath, 0, 0);
	else
		return -1;
}

int Player::GetSubtitleCount()
{
	if (! m_pVLC_Player)
		return 0;

	int nCnt = libvlc_video_get_spu(m_pVLC_Player);
	return nCnt;
}

void Player::GetSubtitles(libvlc_track_description_t* subtitles)
{
	if (!m)
		return;

	// 解析媒体实例
	libvlc_media_parse(m);
	// 获取媒体文件的播放长度,  返回 ms
	libvlc_time_t duration = libvlc_media_get_duration(m);
	char *str = libvlc_media_get_meta(m, libvlc_meta_Description);
	qDebug() << str;
	// 此处是获取媒体包含多个的视频和音频轨以及其他类型的轨道信息
	libvlc_media_track_info_t *media_tracks_info = NULL;
	libvlc_media_track_t **media_tracks = NULL;
	int trackCount = libvlc_media_get_tracks_info(m, &media_tracks_info);
	unsigned tracksNt = libvlc_media_tracks_get(m, &media_tracks);
	// 释放内存
	libvlc_media_tracks_release(media_tracks, tracksNt);
	SetSubtitile(true);
	// 把打开的媒体文件设置给播放器
	//libvlc_media_player_set_media(m_pVLC_Player, m);
}

void Player::SetSubtitile(bool open)
{
	if (!m_pVLC_Player)
		return;

	if (open)
	{
		int nTrack = libvlc_video_get_spu(m_pVLC_Player);
		qDebug() << "current video subtitle track = "<<nTrack << "\n";
		libvlc_video_set_spu(m_pVLC_Player, 1);
		//libvlc_media_player_set_video_title_display(m_pVLC_Player, libvlc_position_t::libvlc_position_center, 300);
	}
	else
	{
		libvlc_video_set_spu(m_pVLC_Player, 0);
		//libvlc_media_player_set_video_title_display(m_pVLC_Player, libvlc_position_t::libvlc_position_disable, 300);
	}
}

void Player::SetHWND(void *hwnd)
{
	m_hWnd = hwnd;
}

void *Player::GetHWND()
{
    return m_hWnd;
}

bool Player::IsOpen()
{
    return NULL != m_pVLC_Player;
}

bool Player::IsPlaying()
{
    if (m_pVLC_Player)
    {
        return (1 == libvlc_media_player_is_playing(m_pVLC_Player));
    }

    return false;
}

__int64 Player::GetTotalTime()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_get_length(m_pVLC_Player);
    }

    return 0;
}

__int64 Player::GetTime()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_get_time(m_pVLC_Player);
    }

    return 0;
}

int Player::GetVolume()
{
    if (m_pVLC_Player)
    {
        return libvlc_audio_get_volume(m_pVLC_Player);
    }

    return 0;
}

void Player::setScale(PLAYER_VIDEO_SCALE_TYPE type)
{
	if (m_pVLC_Player)
	{
		if (type == PLAYER_SCALE_1_4)
			libvlc_video_set_scale(m_pVLC_Player, 0.25);
		else if (type == PLAYER_SCALE_1_2)
			libvlc_video_set_scale(m_pVLC_Player, 0.5);
		else if (type == PLAYER_SCALE_1_1)
			libvlc_video_set_scale(m_pVLC_Player, 1);
		else if (type == PLAYER_SCALE_2_1)
			libvlc_video_set_scale(m_pVLC_Player, 2);
	}
}

void Player::setRadio(PLAYER_VIDEO_RADIO_TYPE type)
{
	if (m_pVLC_Player)
	{
		if (type == PLAYER_RADIO_DEFULT)
			libvlc_video_set_aspect_ratio(m_pVLC_Player, "16:9");
		else if (type == PLAYER_RADIO_16_9)
			libvlc_video_set_aspect_ratio(m_pVLC_Player, "16:9");
		else if (type == PLAYER_RADIO_4_3)
			libvlc_video_set_aspect_ratio(m_pVLC_Player, "4:3");
		else if (type == PLAYER_RADIO_1_1)
			libvlc_video_set_aspect_ratio(m_pVLC_Player, "1:1");
		else if (type == PLAYER_RADIO_16_10)
			libvlc_video_set_aspect_ratio(m_pVLC_Player, "16:10");
		else if (type == PLAYER_RADIO_2_21_1)
			libvlc_video_set_aspect_ratio(m_pVLC_Player, "221:100");
		else if (type == PLAYER_RADIO_2_35_1)
			libvlc_video_set_aspect_ratio(m_pVLC_Player, "235:100");
		else if (type == PLAYER_RADIO_2_38_1)
			libvlc_video_set_aspect_ratio(m_pVLC_Player, "238:100");
		else if (type == PLAYER_RADIO_5_4)
			libvlc_video_set_aspect_ratio(m_pVLC_Player, "5:4");
	}

}

void Player::setCrop(PLAYER_VIDEO_CROP_TYPE type)
{
	if (m_pVLC_Player)
	{
		if (type == PLAYER_CROP_DEFULT)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "16:9");
		else if (type == PLAYER_CROP_16_10)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "16:10");
		else if (type == PLAYER_CROP_16_9)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "16:9");
		else if (type == PLAYER_CROP_4_3)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "4:3");
		else if (type == PLAYER_CROP_1_85_1)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "185:100");
		else if (type == PLAYER_CROP_2_21_1)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "221:100");
		else if (type == PLAYER_CROP_2_35_1)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "235:100");
		else if (type == PLAYER_CROP_2_39_1)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "239:100");
		else if (type == PLAYER_CROP_5_3)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "5:3");
		else if (type == PLAYER_CROP_5_4)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "5:4");
		else if (type == PLAYER_CROP_1_1)
			libvlc_video_set_crop_geometry(m_pVLC_Player, "1:1");
	}

}

void Player::setAudioDev(std::string dev)
{
	if (!m_pVLC_Player)
		return;

	//指定设备输出pDevList->psz_device
	libvlc_audio_output_device_set(m_pVLC_Player, NULL, dev.c_str());
}

void Player::setAudioModule(int index)
{
	if (!m_pVLC_Player)
		return;

	libvlc_audio_output_channel_t channel = (libvlc_audio_output_channel_t)index;
	int cc = libvlc_audio_set_channel(m_pVLC_Player, channel);
}

void Player::setPlaySpeed(float speed)
{
	if (m_pVLC_Player)
		libvlc_media_player_set_rate(m_pVLC_Player, speed);
}

void Player::addSubTitle(const char *filePath)
{
	if (!m_pVLC_Player)
		return;

	 libvlc_video_set_subtitle_file(m_pVLC_Player, filePath);
}
