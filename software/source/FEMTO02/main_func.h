#ifndef _AVR_MAIN_FUNC_H_
#define _AVR_MAIN_FUNC_H_

void ess_mute(void);
void phase_ess(void);
void volume_set(void);
void init_vol_dn(unsigned char data);
void init_vol(unsigned char data);
void audio_level_sp_down(void);
void audio_level_sp_up(void);
void audio_level_down(void);
void audio_level_up(void);
void channel_change(void);
void channel_up(void);
void channel_down(void);
void ess_filter(void);
void femto_function(void);

#endif // _AVR_MAIN_FUNC_H_