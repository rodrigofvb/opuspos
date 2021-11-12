#include "SDK30.h"
#include "linklayer.h"
#include "IP_.h"

#include "Globals.h"
#include "Dialogs.h"

#define __DEFAULT_PIN_CODE 		"0000"

#define __500_MSECONDS__     	50
#define __MAXTIMEOUTNETWORK__  	6000		//60 segundos
#define __MAXTIMEOUTPPP__  		3000		//30 segundos

#define __FALSE__        		0
#define __TRUE__              	1

#define  ABORTED				500

extern struct Confs config;

/*
 * checa se a conexão PPP está ativa
 * retorna 0 se OK e -1 se não OK
 *
 */
int ChecarPPP()
{
	int bContinue = __TRUE__;
	int bReady = __FALSE__;
	int bPPP = __FALSE__;
	int nErr = 0;

	bContinue = gprs_IsNetworkAvailable( __TRUE__, &bReady, &nErr, &bPPP );

	while (bContinue == __TRUE__)
	{
		ttestall(0, __500_MSECONDS__);
	}

	if (bPPP == __TRUE__)
		return 0;
	else
		return -1;
}

int WaitNetworkAvailable(int bGPRS, unsigned int nWaitTimeout, unsigned int nWaitMaxTimeout, int *pError)
{
	unsigned int nWaitTotal = 0;
	int bContinue = __TRUE__;
	int bReady = __FALSE__;
	int bPPP = __FALSE__;
	int cKey;
	int iRet;
	FILE *hKeyboard;

	hKeyboard = fopen("KEYBOARD", "r");

	do
	{
		bContinue = gprs_IsNetworkAvailable( bGPRS, &bReady, pError, &bPPP );
		if( bContinue == __TRUE__  )
		{
			cKey = 0;
			iRet = ttestall( KEYBOARD, nWaitTimeout );
			nWaitTotal += nWaitTimeout;

			if (iRet & KEYBOARD)
				cKey = getchar();

			if (cKey == T_ANN)
			{
				*pError = ABORTED;
				break;
			}
			nWaitTotal += nWaitTimeout;
		}
	} while(( bContinue == __TRUE__ ) && ( nWaitTotal < nWaitMaxTimeout ));

	fclose(hKeyboard);

	return bReady;
}

 // To wait for the PPP link connection.
 int WaitNetworkPPP(unsigned int nWaitTimeout, unsigned int nWaitMaxTimeout)
 {
	unsigned int nWaitTotal = 0;
	int bContinue = __TRUE__;
	int bReady = __FALSE__;
	int bPPP = __FALSE__;

	int nErr = 0;

	do
	{
		bContinue = gprs_IsNetworkAvailable( __TRUE__, &bReady, &nErr, &bPPP );

		if(( bContinue == __TRUE__  ) && ( bPPP == __FALSE__ ))
		{
			ttestall( 0, nWaitTimeout );
			nWaitTotal += nWaitTimeout;
		}
	} while(( bContinue == __TRUE__ ) && ( bPPP == __FALSE__ ) && ( nWaitTotal < nWaitMaxTimeout ));

	return bPPP;
}

int ConnectNetworkGPRS()
{
	int nError;
	FILE *hKeyboard = NULL;
	int num_tentativas = 2;
	int i;

	for (i = 0; i < num_tentativas; i++)
	{
		nError = gprs_enable( config.cfApn );
		if (nError == GPRS_OK)
		{
			if (WaitNetworkAvailable( __TRUE__, __500_MSECONDS__,__MAXTIMEOUTNETWORK__, &nError ) == __TRUE__)
			{
				hKeyboard = fopen("KEYBOARD", "r");
				nError = gprs_connect( T_ANN, config.cfApn, config.cfApnlogin, config.cfApnpwd, (__MAXTIMEOUTPPP__/100) );
				fclose(hKeyboard);

				// Wait for the PPP connection.
				if ((nError == GPRS_OK))
				{
					if (WaitNetworkPPP( __500_MSECONDS__, __MAXTIMEOUTPPP__) == __TRUE__)
					{
						// Connection ok
						break;
					}
					else
					{
						// Connection error
						nError = GPRS_ERR_PPP;
						gprs_stop();
					}
				}
				else
				{
					// Connection error.
					gprs_stop();
				}
			}
		}
		else
		{
			// Connection error.
			gprs_stop();
		}
	}

	return nError;
}

int ConnectNetworkGPRS_LL()
{
	unsigned int nWaitTotal = 0;
	unsigned int nWaitTimeout = __500_MSECONDS__;
	unsigned int nWaitMaxTimeout = __MAXTIMEOUTNETWORK__;

	int nError;
	int nStatus;
	int cKey = 0;
	int iRet;
	FILE *hKeyboard;

	hKeyboard = fopen("KEYBOARD", "r");

	// Start the GPRS/3G network.
	LL_GPRS_Start( __DEFAULT_PIN_CODE, config.cfApn );

	while ((( nError = LL_Network_GetStatus( LL_PHYSICAL_V_GPRS, &nStatus )) == LL_ERROR_NETWORK_NOT_READY )
			  && ( nWaitTotal < nWaitMaxTimeout ))
	{
		if( nStatus == LL_STATUS_GPRS_AVAILABLE )
		{
			// The GPRS/3G network is available. Perform the connection.
			nError = LL_GPRS_Connect( config.cfApn, config.cfApnlogin, config.cfApnpwd, __MAXTIMEOUTPPP__ );
		}
		else
		{
			cKey = 0;
			iRet = ttestall( KEYBOARD, nWaitTimeout );
			nWaitTotal += nWaitTimeout;

			if (iRet & KEYBOARD)
				cKey = getchar();

			if (cKey == T_ANN)
			{
				LL_GPRS_Stop();
				nError = ABORTED;
				break;
			}
		}
	}

	fclose(hKeyboard);

	// Display the status.
	switch( nError )
	{
	 case LL_ERROR_OK:
		 upload_diagnostic_txt("\nGPRS Connected !");
		 break;

	 case LL_ERROR_NETWORK_NOT_SUPPORTED:
		 upload_diagnostic_txt("\nGPRS Network Not supported !");
		 break;

	 default:
	 {
		 switch( nStatus )
		 {
		 case LL_STATUS_GPRS_DISCONNECTED:
			 upload_diagnostic_txt("\nGPRS Disconnected !");
			 break;
		 case LL_STATUS_GPRS_ERROR_NO_SIM:
			 upload_diagnostic_txt("\nNO SIM !");
			 break;
		 case LL_STATUS_GPRS_ERROR_SIM_LOCK:
			 upload_diagnostic_txt("\nSIM Lock!");
			 break;
		 case LL_STATUS_GPRS_ERROR_BAD_PIN:
			 upload_diagnostic_txt("\nBad PIN!");
			 break;
		 case LL_STATUS_GPRS_ERROR_NO_PIN:
			 upload_diagnostic_txt("\nNO PIN!");
			 break;
		 case LL_STATUS_GPRS_ERROR_PPP:
			 upload_diagnostic_txt("\nPPP Error!");
			 break;
		 case LL_STATUS_GPRS_ERROR_UNKNOWN:
			 upload_diagnostic_txt("\nUnknown Error!");
			 break;
		 case LL_STATUS_GPRS_CONNECTING:
			 upload_diagnostic_txt("\nConnecting...!");
			 break;
		 case LL_STATUS_GPRS_AVAILABLE:
			 upload_diagnostic_txt("\nGPRS available...!");
			 break;
		 case LL_STATUS_GPRS_CONNECTING_PPP:
			 upload_diagnostic_txt("\nConnecting PPP...!");
			 break;
		 case LL_STATUS_GPRS_CONNECTED:
			 upload_diagnostic_txt("\nConnected!");
			 break;
		 default:
			 upload_diagnostic_txt("\nUnknown network status !");
			 break;
		 }
	 }
	 break;
	}

	return nError;
}

void AbrirGPRS()
{
	//-- Abrir GPRS
	int ret;

    DisplaySimpleMessage("Procurando Rede", TRUE, TRUE);

    ret = ConnectNetworkGPRS();

    switch (ret)
    {
    	case GPRS_OK:
       		DisplaySimpleMessage("Rede conectada", TRUE, TRUE);
       		break;

    	case GPRS_ERR_PPP:
    		DisplaySimpleMessage("APN nao conectada", TRUE, TRUE);
    		break;

    	case GPRS_ERR_NO_SIM:
    		DisplaySimpleMessage("Sim card ausente", TRUE, TRUE);
    		break;

    	case ABORTED:
    	    DisplaySimpleMessage("Cancelado", TRUE, TRUE);
    	    break;

       	default:
       		DisplaySimpleMessage("Nao conectado", TRUE, TRUE);
    }
    ttestall(0, 100);
}
