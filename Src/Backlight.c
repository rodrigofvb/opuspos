#include "SDK30.H"

#include "Globals.h"
#include "File.h"

extern struct Confs config;

void DefinirLuzFundo()
{
	char cKey;
    char tcDisplay[18+1];
    int iIdxInicio = 100;
	int iIdx;
	int iRet;
	FILE *hKeyboard = NULL;
	int iIdxMod;

	cKey = '%';

	HWCNF_SetBacklightLevelEx (BACKLIGHT_DEVICE_ALL, BACKLIGHT_TARGET_ALL, iIdxInicio);

	if (config.cfBacklight[0] != 0)
	{
		iIdxInicio = atoi(config.cfBacklight);
		HWCNF_SetBacklightLevelEx (BACKLIGHT_DEVICE_ALL, BACKLIGHT_TARGET_ALL, iIdxInicio);
	}

	iIdx = iIdxInicio;

	iIdxMod = iIdx % 10;
	if (iIdxMod > 0)
		iIdx += (10 - iIdxMod); //arrendonda p/ múltiplo de 10 se não for

	sprintf (tcDisplay, "Luz de Fundo: %3d%c", iIdx, cKey);

	_clrscr();
	_DrawExtendedString(20, 5, tcDisplay, _OFF_, _MEDIUM_, _PROPORTIONNEL_);
	_DrawExtendedString(1, 20, "- seta p/ abaixo diminui", _OFF_, _POLICE4x7_, _PROPORTIONNEL_);
	_DrawExtendedString(1, 30, "- seta p/ cima aumenta", _OFF_, _POLICE4x7_, _PROPORTIONNEL_);
	_DrawExtendedString(1, 40, "- botao verde salva", _OFF_, _POLICE4x7_, _PROPORTIONNEL_);
	PaintGraphics();

	hKeyboard = fopen("KEYBOARD", "r");

	while(1)
	{
		cKey = 0;
		iRet = ttestall(KEYBOARD, 0);
		if (iRet & KEYBOARD)
			cKey = getchar();

        if (cKey==T_ANN)	//Botão Vermelho
        {
        	HWCNF_SetBacklightLevelEx (BACKLIGHT_DEVICE_ALL, BACKLIGHT_TARGET_ALL, iIdxInicio);
        	break;
        }

        if (cKey==T_VAL)	//Botão Verde
        {
        	sprintf(config.cfBacklight, "%d", iIdx);
        	SalvarConfigs();
        	break;
        }

        if ((cKey==T_SKBAS) || (cKey==T_SK2)) 	// Key DOWN
		{
        	iIdx -=10;

			if (iIdx < 0)
				iIdx = 0;

			HWCNF_SetBacklightLevelEx (BACKLIGHT_DEVICE_ALL, BACKLIGHT_TARGET_ALL, iIdx);
		}

        if((cKey==T_SKHAUT) || (cKey==T_SK3)) 	// Key UP
		{
			iIdx +=10;

			if (iIdx > 100)
				iIdx = 100;

			HWCNF_SetBacklightLevelEx (BACKLIGHT_DEVICE_ALL, BACKLIGHT_TARGET_ALL, iIdx);
		}

		cKey='%';

		memset(tcDisplay, 0, sizeof(tcDisplay));
		sprintf (tcDisplay, "Luz de Fundo: %3d%c", iIdx, cKey);

		_clrscr();
		_DrawExtendedString(20, 5, tcDisplay, _OFF_, _MEDIUM_, _PROPORTIONNEL_);
		_DrawExtendedString(1, 20, "- seta p/ abaixo diminui", _OFF_, _POLICE4x7_, _PROPORTIONNEL_);
		_DrawExtendedString(1, 30, "- seta p/ cima aumenta", _OFF_, _POLICE4x7_, _PROPORTIONNEL_);
		_DrawExtendedString(1, 40, "- botao verde salva", _OFF_, _POLICE4x7_, _PROPORTIONNEL_);
		PaintGraphics();
    }

	fclose(hKeyboard);
}
