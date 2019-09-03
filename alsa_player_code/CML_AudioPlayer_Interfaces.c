/******************************************************************************
 *
 *
 *   ALLGO EMBEDDED SYSTEMS CONFIDENTIAL PROPRIETARY
 *
 *    (C) 2019 ALLGO EMBEDDED SYSTEMS PVT. LTD.
 *
 *   FILENAME        - CML_AudioPlayer_Interfaces.c
 *
 *   COMPILER        - gcc 5.4.0
 *
 ******************************************************************************
 *  CHANGE HISTORY
 *   ---------------------------------------------------------------------------
 *   DATE           REVISION      		AUTHOR                  COMMENTS
 *   ---------------------------------------------------------------------------
 *   26/08/2019     1.0             Ankit Raj,Roshni         Initial Version
 *   					  	        Taj,Aishwarya M
 *  
 ********************************************************************************
 *   DESCRIPTION
 *   This file contains all audio player related API implimentation.
 *   
 ******************************************************************************/
#include"CML_types.h"
#include"CML_AudioPlayer_types.h"

void (*fpsetstate)(CML_playerstate ecurstate);

void psetstate(CML_playerstate ecurstate)
{	
	switch(ecurstate)
	{
	 case STOP_STATE:ssetcurstate.icurstate=101;break;
	 case PLAY_STATE:ssetcurstate.icurstate=102;break;
	 case PAUSED_STATE:ssetcurstate.icurstate=103;break;
	}
}

CML_RetType cml_audioplayer_init(UINT32 iNoOfargs,CML_CHAR *pacargslist[])
{   
    CML_RetType eret;
    CML_UINT32 ishuffleflag; 
    if(ishuffleflag=cml_checkinput(iNoOfargs,pacargslist)<0)
         printf("");
    if(cml_myplaylist(iNoOfargs,pacargslist)<0)
        printf("ERROR CREATING PLAYLIST");
    if(1==ishuffleflag)
    {
        if(cml_shuffle()<0)
            printf("ERROR IN SHUFFLE");
    }
    fpsetstate=psetstate;
    sCurFile.icurfileindex = 0;
   if(pthread_create(&sthreadid.playbacktid,NULL,cml_playbackthread,
   													(void*)args)<0)
       printf("Thread creation failed");
   return TBW;
}

CML_RetType cml_audioplayer_play()
{
    if(101 == ssetcurstate.icurstate)
    {
       if(pthread_create(&sthreadid.playbacktid,NULL,cml_playbackthread,
       												(void*)args)<0)
           printf("Thread creation failed");
    
    }

    return TBW;
}

CML_RetType cml_audioplayer_pause_resume()
{
     if(ssetcurstate.icurstate == 102)
			{
				if(snd_pcm_pause(salsaconfig.pcm_handle, 1) < 0)
				{ 
					printf("ERROR while pause");
					return 0;
			    	}
				printf("song paused\n");
				fpsetstate(PAUSED_STATE);
				
			}
     else if(ssetcurstate.icurstate == 103)
			{
				if(snd_pcm_pause( salsaconfig.pcm_handle, 0) < 0)
			{
				printf("ERROR while resume");
				return 0;
		    	}
			printf("song resumed\n");
			fpsetstate(PLAY_STATE);
			}
     else
            {
                printf("can't pause in STOP STATE");
            }
     return TBW;
 }

CML_RetType cml_audioplayer_volume_control(CML_INT64 volume)
{
    CML_INT64 min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    snd_mixer_elem_t* elem;
    const  CML_CHAR *card = "default";
    const CML_CHAR *selem_name = "Master";
    snd_mixer_open(&svconfig.handle, 0);
    snd_mixer_attach(svconfig.handle, card);
    snd_mixer_selem_register(svconfig.handle, NULL, NULL);
    snd_mixer_load(svconfig.handle);

    snd_mixer_selem_id_alloca(&svconfig.sid);
    snd_mixer_selem_id_set_index(svconfig.sid, 0);
    snd_mixer_selem_id_set_name(svconfig.sid, selem_name);
    snd_mixer_find_selem(svconfig.handle, svconfig.sid);

    snd_mixer_selem_get_playback_volume_range(svconfig.elem, &svconfig.min, &svconfig.max);
    snd_mixer_selem_set_playback_volume_all(svconfig.elem, volume * svconfig.max / 100);

    snd_mixer_close(svconfig.handle);
}

CML_VOID *cml_playbackthread(CML_VOID *args)
{
    CML_UINT32 pcm, tmp, dir;	
	CML_INT32 buff_size, loops;
    
    sfiledesc.ifiledesc = open(samyplaylist[sCurFile.icurfileindex].pfilepath , O_RDONLY);   
	
    cml_extractheaderinfo();
	
    /* Open the PCM device in playback mode */
	if (pcm = snd_pcm_open(&salsaconfig.pcm_handle, PCM_DEVICE,
					SND_PCM_STREAM_PLAYBACK, 0) < 0) 
		printf("ERROR: Can't open \"%s\" PCM device. %s\n",
					PCM_DEVICE, snd_strerror(pcm));

	/* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&salsaconfig.params);

	snd_pcm_hw_params_any(salsaconfig.pcm_handle, salsaconfig.params);

	/* Set parameters */
	if (pcm = snd_pcm_hw_params_set_access(salsaconfig.pcm_handle, salsaconfig.params,
					SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
		printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_format(salsaconfig.pcm_handle, salsaconfig.params,
						SND_PCM_FORMAT_S16_LE) < 0) 
		printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_channels(salsaconfig.pcm_handle, salsaconfig.params, swaveheaderinfo.uwchannel) < 0) 
		printf("ERROR: Can't set swaveheaderinfo.uwchannel number. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_rate_near(salsaconfig.pcm_handle, salsaconfig.params, &swaveheaderinfo.isamplerate, 0) < 0) 
		printf("ERROR: Can't set swaveheaderinfo.isamplerate. %s\n", snd_strerror(pcm));

	/* Write parameters */
	if (pcm = snd_pcm_hw_params(salsaconfig.pcm_handle, salsaconfig.params) < 0)
		printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

	/* Resume information */
	printf("PCM name: '%s'\n", snd_pcm_name(salsaconfig.pcm_handle));

	printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(salsaconfig.pcm_handle)));

	snd_pcm_hw_params_get_channels(salsaconfig.params, &tmp);
	printf("swaveheaderinfo.uwchannel: %i ", tmp);

	if (tmp == 1)
		printf("(mono)\n");
	else if (tmp == 2)
		printf("(stereo)\n");

	snd_pcm_hw_params_get_rate(salsaconfig.params, &tmp, 0);
	printf("swaveheaderinfo.isamplerate: %d bps\n", tmp);

	printf("swaveheaderinfo.iduration: %d\n", swaveheaderinfo.iduration);	

	/* Allocate buffer to hold single period */
	snd_pcm_hw_params_get_period_size(salsaconfig.params, &salsaconfig.frames, 0);

	buff_size = salsaconfig.frames * swaveheaderinfo.uwchannel * 2 /* 2 -> sample size */;
	salsaconfig.buffer = (char *) malloc(buff_size);

	snd_pcm_hw_params_get_period_time(salsaconfig.params, &tmp, NULL);
    fpsetstate(PLAY_STATE);

	for (loops = (swaveheaderinfo.iduration * 1000000) / tmp; loops > 0; loops--) {

		if (pcm = read(sfiledesc.ifiledesc, salsaconfig.buffer, buff_size) == 0) {
			printf("Early end of file.\n");
			return 0;
		}

		if (pcm = snd_pcm_writei(salsaconfig.pcm_handle, salsaconfig.buffer, salsaconfig.frames) == -EPIPE) {
			printf("XRUN.\n");
			snd_pcm_prepare(salsaconfig.pcm_handle);
		} else if (pcm < 0) {
			printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
		}

	}

	/*snd_pcm_drain(salsaconfig.pcm_handle);
	snd_pcm_close(salsaconfig.pcm_handle);
	free(salsaconfig.buffer);*/

    cml_audioplayer_play_next_file();

}

CML_RetType cml_audioplayer_play_next_file()
{
    if(101 == ssetcurstate.icurstate)
    {
        printf("Can't play next in stop state\n");
    }
    snd_pcm_drop(salsaconfig.pcm_handle);
    pthread_cancel(sthreadid.playbacktid);
    if(sCurFile.icurfileindex == sCurFile.icount)
    {
        printf("Last file of the playlist");
    }
    sCurFile.icurfileindex += 1;

    pthread_create(&sthreadid.playbacktid,NULL,cml_playbackthread,NULL);

}


CML_RetType cml_audioplayer_play_prev_file() 
{
     if(101 == ssetcurstate.icurstate)
    {
        printf("Can't play next in stop state\n");
    }
    pthread_cancel(sthreadid.playbacktid);
    snd_pcm_drop(salsaconfig.pcm_handle);
    if(0 == sCurFile.icurfileindex )
    {
        printf("First file of the playlist");
    }
    
    sCurFile.icurfileindex -= 1;

    pthread_create(&sthreadid.playbacktid,NULL,cml_playbackthread,NULL);

}

CML_RetType cml_audioplayer_playback_stop()
{
    fpsetstate(STOP_STATE);
    snd_pcm_drain(salsaconfig.pcm_handle);
    snd_pcm_drop(salsaconfig.pcm_handle);
    pthread_cancel(sthreadid.playbacktid);
}
CML_RetType cml_audioplayer_seek_back()
{
    pthread_cancel(sthreadid.playback_tid);
	snd_pcm_drop(salsaconfig.pcm_handle);
	snd_pcm_prepare(salsaconfig.pcm_handle);
	seconds -= 10;
	lseek64(sfiledesc.ifiledesc, swavheaderinfo.ibyterate*(seconds) , SEEK_SET);
	pthread_create(&sthreadid.playback_tid, NULL, playback_thread, NULL);
	
}

CML_RetType cml_audioplayer_seek_forward()
{
    pthread_cancel(sthreadid.playback_tid);
	//pthread_cancel(timer_tid);
	snd_pcm_drop(salsaconfig.pcm_handle);
	snd_pcm_prepare(salsaconfig.pcm_handle);
	seconds += 10;
	lseek64(sfiledesc.ifiledesc, swavheaderinfo.ibyterate*(seconds) , SEEK_SET);
	pthread_create(&sthreadid.playback_tid, NULL, playback_thread, NULL);
	//pthread_create(&timer_tid, NULL, timer_display, NULL);
}


CML_RetType cml_audioplayer_seek_to(CML_UINT32 sec )
{
    pthread_cancel(sthreadid.playback_tid);
	//pthread_cancel(timer_tid);
	snd_pcm_drop(salsaconfig.pcm_handle);
	snd_pcm_prepare(salsaconfig.pcm_handle);
	seconds = sec;
	lseek64(sfiledesc.ifiledesc, swavheaderinfo.ibyterate*(seconds) , SEEK_SET);
	pthread_create(&salsaconfig.playback_tid, NULL, playback_thread, NULL);
	//pthread_create(&timer_tid, NULL, timer_display, NULL);
}

CML_RetType cml_display()
{
    //tbw
}
#endif/*CML AudioPlayer Interfaces*/
