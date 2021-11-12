#include "SDK30.H"
#include "IP_.h"
#include "FTP_.h"

#include "Globals.h"
#include "Dialogs.h"
#include "FTP.h"
#include "Conn.h"

extern struct Confs config;

/*
 * Retorna o modelo do dispositivo
 * - modelo (saída): descrição do modelo
 */
void PegarModelo(char *modelo)
{
	unsigned char product;
	unsigned char range;

	range = PSQ_Get_product_type(&product);

	switch(product)
	{
		case TYPE_TERMINAL_SMART :
			strcpy(modelo, "EFTsmart");
			break;
		case TYPE_TERMINAL_EFT30F :
			strcpy(modelo, "EFT30F");
			break;
		case TYPE_TERMINAL_EFT30P :
			strcpy(modelo, "EFT30plus");
			break;
		case TYPE_TERMINAL_TWIN30 :
			switch(range)
			{
				case TYPE_TERMINAL_TWIN30  :
					strcpy(modelo, "TWIN30");
					break;
				case TYPE_TERMINAL_TWIN33  :
					strcpy(modelo, "TWIN33");
					break;
				case TYPE_TERMINAL_TWIN32  :
					strcpy(modelo, "TWIN32");
					break;
				case TYPE_TERMINAL_TWIN31  :
					strcpy(modelo, "TWIN31");
					break;
				default :
					strcpy(modelo, "TWIN30");
					break;
			}
			break;
		case TYPE_TERMINAL_EFT930 :
			switch(range)
			{
				case TYPE_TERMINAL_EFT930W :
					strcpy(modelo, "EFT930W");
					break;
				case TYPE_TERMINAL_EFT930P :
					strcpy(modelo, "EFT930P");
					break;
				case TYPE_TERMINAL_EFT930G :
					strcpy(modelo, "EFT930G");
					break;
				case TYPE_TERMINAL_EFT930B :
					strcpy(modelo, "EFT930B");
					break;
				case TYPE_TERMINAL_EFT930S :
					strcpy(modelo, "EFT930S");
					break;
			}
			break;
		case TYPE_TERMINAL_ML30 :
			switch(range)
			{
				case TYPE_TERMINAL_ML30 :
					strcpy(modelo, "ML30");
					break;
				case TYPE_TERMINAL_SPM :
					strcpy(modelo, "SPM");
					break;
				case TYPE_TERMINAL_IMP350 :
					strcpy(modelo, "IMP350");
					break;
				case TYPE_TERMINAL_IPP320 :
					strcpy(modelo, "IPP320");
					break;
				case TYPE_TERMINAL_IPP350 :
					strcpy(modelo, "IPP350");
					break;
				default :
					strcpy(modelo, "ML30");
					break;
			}
			break;
		case TYPE_TERMINAL_CAD30 :
			switch(range)
			{
				case TYPE_TERMINAL_CAD30 :
					strcpy(modelo, "CAD30");
					break;
				case TYPE_TERMINAL_IUN150 :
					strcpy(modelo, "iUC150");
					break;
				case TYPE_TERMINAL_IUN180 :
					strcpy(modelo, "iUC180");
					break;
				case TYPE_TERMINAL_IUN230 :
					strcpy(modelo, "iUC230");
					break;
				default :
					strcpy(modelo, "CAD30");
					break;
			}
			break;
		case TYPE_TERMINAL_MR40 :
			strcpy(modelo, "MR40");
			break;
		case TYPE_TERMINAL_X07 :
			switch(range)
			{
				case TYPE_TERMINAL_E532 :
					strcpy(modelo, "E532");
					break;
				case TYPE_TERMINAL_IWL350 :
					strcpy(modelo, "IWL350");
					break;
				case TYPE_TERMINAL_IWL280 :
					strcpy(modelo, "IWL280");
					break;
				case TYPE_TERMINAL_IWL220 :
					strcpy(modelo, "IWL220");
					break;
				case TYPE_TERMINAL_IWL250 :
					strcpy(modelo, "IWL250");
					break;
				case TYPE_TERMINAL_ICT220 :
					strcpy(modelo, "ICT220");
					break;
				case TYPE_TERMINAL_ICT250 :
					strcpy(modelo, "ICT250");
					break;
				case TYPE_TERMINAL_ICT280 :
					strcpy(modelo, "ICT280");
					break;
				case TYPE_TERMINAL_ISC350 :
					strcpy(modelo, "ISC350");
					break;
				case TYPE_TERMINAL_ISC250 :
					strcpy(modelo, "ISC250");
					break;
				default :
					strcpy(modelo, "ICT220");
					break;
		  }
		  break;
		default :
			strcpy(modelo, "EFT30F");
			break;
	}
}

/*
 * Retorna o iccid do dispositivo
 * - iccid (saída): descrição do iccid
 */
void PegarICCID(char *iccid)
{
	T_DGPRS_GET_INFORMATION info;
	FILE *gprs = NULL;

	gprs = stdperif(( char *)"DGPRS", NULL );

	if( gprs != NULL )
	{
		fioctl( DGPRS_FIOCTL_GET_INFORMATION, &info, gprs );
	}

	strcpy(iccid, info.simIccId);
}

/*
 * Retorna o serial do dispositivo
 * - serial (saída): descrição do serial
 */
void PegarSerial(char *numserial)
{
	char serial[65];
	char *p1 = NULL;
	char *p2 = NULL;

	memset(serial, 0, sizeof(serial));

	PSQ_Give_Serial_Number(serial);

	//copia os caracteres somente até o primeiro espaço vazio
	p1 = &serial[0];
	p2 = p1;

	while (*p2 != ' ' && p2 != '\0')
		p2++;

	strncpy(numserial, serial, p2 - p1);
}

void AtualizarSistema()
{
	RAMDISK_HANDLE hDisk = NULL;
	FTP_HANDLE hFTP = 0;
	char host_ip[16];
	int nNbFiles;
	int Ativado = -1;
	int ret = 0;

	unsigned long MAX_DISK = 2097152; 	//2 MB

	char szLogin[] = "usuario";
	char szPassword[] = "3096";
	char szDirectory[] = "opuspos";
	int nMaxFiles = 10;

	char *aFilenames[nMaxFiles];

	char terminal = PSQ_Get_hardware_type();

	DisplaySimpleMessage( "Conectando...", TRUE, TRUE );

	PegaIPServidor(host_ip, FALSE);

	hFTP = FTP_Connect( szLogin, szPassword, host_ip, 21, 1 );

	if( hFTP == NULL )
	{
		FTP_Disconnect( hFTP );
		PegaIPServidor(host_ip, TRUE);
		hFTP = FTP_Connect( szLogin, szPassword, host_ip, 21, 1 );
	}

	if( hFTP != NULL )
	{
		DisplaySimpleMessage( "Lendo pasta...", TRUE, TRUE );

		if( FTP_ChangeDir( hFTP, szDirectory ) == 0 )
		{
			if (terminal == TYPE_TERMINAL_X07)
			{
				ret = FTP_ChangeDir( hFTP, "ict220" );
			}
			else if (terminal == TYPE_TERMINAL_EFT930)
			{
				ret = FTP_ChangeDir( hFTP, "eft930" );
			}

			if( ret == 0 )
			{
				nNbFiles = iFtp_get_file_list( hFTP, aFilenames, nMaxFiles);

				if ( nNbFiles > 0 )
				{
					hDisk = RAMDISK_Create( "TEMP", MAX_DISK, nNbFiles );

					if( hDisk != NULL )
					{
						if ( iFtp_download_files( hFTP, hDisk, aFilenames, nNbFiles ) == 0 )
						{
							DisplaySimpleMessage2lines( "Download OK", "Ativando software..." , TRUE, TRUE );

							Ativado = RAMDISK_Activate( hDisk );
						}
						else
						{
							DisplaySimpleMessage2lines( "Erro ao baixar", "arquivos", TRUE, TRUE);
							ttestall( 0, 200 );
						}

						RAMDISK_Delete( hDisk );
					}
					else
					{
						DisplaySimpleMessage2lines( "Erro", "Sem memoria", TRUE, TRUE);
						ttestall( 0, 200 );
					}
				}
				else
				{
					DisplaySimpleMessage2lines( "Erro: Arquivos ", "nao encontrados", TRUE, TRUE);
					ttestall( 0, 200 );
				}
			}
			else
			{
				DisplaySimpleMessage2lines( "Erro: Pasta nao", "encontrada", TRUE, TRUE);
				ttestall( 0, 200 );
			}
		}
		else
		{
			DisplaySimpleMessage2lines( "Erro: Pasta nao", "encontrada", TRUE, TRUE);
			ttestall( 0, 200 );
		}
	}
	else
	{
		DisplaySimpleMessage( "Erro de conexao", TRUE, TRUE);
		ttestall( 0, 200 );
	}
	FTP_Disconnect( hFTP );

	if( Ativado == 0 )
	{
		DisplaySimpleMessage2lines( "Sistema atualizado", "Reiniciando...", TRUE, TRUE);
		ttestall( 0, 200 );
		OEM_exit(0); 	//reiniciar o terminal
	}
	else
	{
		DisplaySimpleMessage( "Nao atualizado", TRUE, TRUE);
		ttestall( 0, 200 );
	}
}
