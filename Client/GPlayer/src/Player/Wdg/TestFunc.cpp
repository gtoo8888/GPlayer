#include "global.h"

static uint8 *audio_chunk;
static uint32 audio_len;
static uint8 *audio_pos;
int pcmBuffer_size = 4096;

// 回调函数，音频设备需要更多数据的时候会调用该回调函数
void read_audio_data2(void *udata, Uint8 *stream, int len) {
    SDL_memset(stream, 0, len);
    if (audio_len == 0) return;
    len = (len > audio_len ? audio_len : len);

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

int testSDL() {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    SDL_AudioSpec spec;
    spec.freq = 44100;  // 根据你录制的PCM采样率决定
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;  // 单声道
    spec.silence = 0;
    spec.samples = 1024;
    spec.callback = read_audio_data2;
    spec.userdata = NULL;

    if (SDL_OpenAudio(&spec, NULL) < 0) {
        printf("can't open audio.\n");
        return -1;
    }

    FILE *fp =
        fopen("E:\\Desktop\\languguetest\\Cplusplustest\\3-VisualStudio2017\\0-GPlayer\\test_video\\123.pcm", "rb+");
    if (fp == NULL) {
        printf("cannot open this file\n");
        return -1;
    }
    char *pcmBuffer = (char *)malloc(pcmBuffer_size);

    // 播放
    SDL_PauseAudio(0);

    while (1) {
        if (fread(pcmBuffer, 1, pcmBuffer_size, fp) !=
            pcmBuffer_size) {  // 从文件中读取数据，剩下的就交给音频设备去完成了，它播放完一段数据后会执行回调函数，获取等多的数据
            break;
        }

        audio_chunk = (Uint8 *)pcmBuffer;
        audio_len = pcmBuffer_size;  // 长度为读出数据长度，在read_audio_data中做减法
        audio_pos = audio_chunk;
        LOG_DBG("audio_len:{:d}", audio_len);

        while (audio_len > 0)  // 判断是否播放完毕
            SDL_Delay(1);
    }
    free(pcmBuffer);
    SDL_Quit();

    return 0;
}

void testQMediaPlayer(void) {
    QMediaPlayer *player = new QMediaPlayer();
    QList<QAudio::Role> ans = player->supportedAudioRoles();
    QStringList ans2 = player->supportedMimeTypes();

    QMediaPlayer mediaPlayer;
    // mediaPlayer->setMedia(QUrl::fromLocalFile(
    //     "E:\\Desktop\\languguetest\\Cplusplustest\\3-VisualStudio2017\\0-GPlayer\\test_video\\123.mp3"));
    ans = mediaPlayer.supportedAudioRoles();
    mediaPlayer.play();
    // 等待音频播放完成
    while (mediaPlayer.state() == QMediaPlayer::PlayingState) {
        // 这里可以添加一些其他处理逻辑
        // 注意：在实际应用中，最好使用信号和槽机制来处理媒体状态的变化
    }

    // AVCodecContext* mAudioCodecContext = avcodec_alloc_context3(audioCodec);
    AVCodecContext *mAudioCodecContext = nullptr;
    QMediaPlayer *m_audioPlayer;
    if (!m_audioPlayer) {
        m_audioPlayer = new QMediaPlayer;
        m_audioPlayer->setMedia(QMediaContent());
        // m_audioPlayer->setAudioOutput(QAudioOutput::defaultAudioOutput());
        m_audioPlayer->setVolume(50);  // 根据需要调整音量
    }

    QAudioFormat format;
    format.setSampleRate(mAudioCodecContext->sample_rate);
    format.setChannelCount(mAudioCodecContext->channels);
    format.setSampleSize(16);  // 根据需要调整采样大小
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    // QAudioBuffer buffer(mAudioFrame->data[0], mAudioFrame->linesize[0], format);

    if (m_audioPlayer->state() == QMediaPlayer::StoppedState) {
        // m_audioPlayer->setMedia(QMediaContent(), &buffer);
        m_audioPlayer->play();
    }
}