#include "SDK30.h"
#include "FTP_.h"

#include "Dialogs.h"

#define __DATA_SIZE 512

typedef void * RAMDISK_HANDLE;
typedef void * RAMDISK_FILE;

// Types.
//-------------------------------------------------------------------

typedef struct
{
	char m_szName[40];		// Disk name.
	unsigned long m_nSize;	// Disk size.
	int m_bUnload;			// Unload application.

	int m_nFreeSize;

} T__RAMDISK;

// Functions.
//-------------------------------------------------------------------

int RAMDISK_WriteInFile( RAMDISK_HANDLE hDisk, void *hFile, const unsigned char *pucBuffer, unsigned int nSize )
{
	T__RAMDISK* pDisk = (T__RAMDISK*)hDisk;

	if( ( pDisk != NULL ) && ( nSize <= pDisk->m_nSize ) )
	{
		unsigned int nBytes = FS_write(	(unsigned char *) pucBuffer,
										1, nSize,
										(S_FS_FILE*) hFile );

		pDisk->m_nSize -= nBytes;

		if( nBytes == nSize )
		{
			return 0;
		}
	}

	return -1;
}

RAMDISK_HANDLE RAMDISK_Create( const char *szName, unsigned long nSize, unsigned int nMaxFile )
{
    unsigned long nDiskSize = 0;
    S_FS_PARAM_CREATE  DiskParam;
	T__RAMDISK* pDisk = NULL;

    // Create the disk.
    strcpy( DiskParam.Label, szName );

    DiskParam.Mode         = FS_WRITEMANY;
    DiskParam.IdentZone    = FS_WO_ZONE_APPLICATION;
    DiskParam.AccessMode   = FS_WRTMOD;
    DiskParam.NbFichierMax = nMaxFile;

    nDiskSize = FreeSpace() - (512 * 1024);

    if( nDiskSize > (4096 * 1024))
    {
        nDiskSize = 4096 * 1024;
    }

	if( nDiskSize > nSize )
	{
		nDiskSize = nSize;
	}

    if( FS_dskcreate( &DiskParam, &nDiskSize ) == FS_OK )
	{
		pDisk = (T__RAMDISK*)umalloc( sizeof(T__RAMDISK) );
		if( pDisk != NULL )
		{
			strcpy( pDisk->m_szName, szName );

			pDisk->m_nSize     = nDiskSize;
			pDisk->m_bUnload   = 0;
			pDisk->m_nFreeSize = nDiskSize - 512;	// !! VG : 512 bytes allocated
													// for the resumption file.
		}
	}

	return (RAMDISK_HANDLE)pDisk;
}


int RAMDISK_Delete( RAMDISK_HANDLE hDisk )
{
	T__RAMDISK* pDisk = (T__RAMDISK*)hDisk;

	char szDisk[16];
	int  nError = 0;

	if( ( pDisk != NULL ) && ( pDisk->m_nSize != 0 ) )
	{
		// Add the starting '/'
		sprintf( szDisk, "/%s", pDisk->m_szName );

		// Unmount the disk
		nError = FS_unmount( szDisk );
		if( nError == FS_OK )
		{
			// Destroy the disk
			nError = FS_dskkill( szDisk );
			if( nError == FS_OK )
			{
				ufree( pDisk );
				return 0; // Successful !!!
			}
		}
	}

	return (-1);
}


RAMDISK_FILE RAMDISK_OpenFile( RAMDISK_HANDLE hDisk, const char *szName, const char *szMode )
{
    T__RAMDISK* pDisk = (T__RAMDISK*)hDisk;
	S_FS_FILE*  hFile = NULL;

	char szFullPath[64];

	if( memcmp( szName, "./", strlen("./") ) == 0 )
	{
		// In case of file names beginning with "./", skip it !
		szName += strlen( "./" );
	}

	if( pDisk != NULL )
	{
		if( strchr( pDisk->m_szName, '/') == NULL )
		{
			sprintf( szFullPath, "/%s/%s", pDisk->m_szName, szName );
		}
		else
		{
			sprintf( szFullPath, "%s/%s", pDisk->m_szName, szName );
		}

		hFile = FS_open((char*) szFullPath, (char*) szMode );
    }


    return hFile;
}

void RAMDISK_CloseFile( RAMDISK_HANDLE hDisk, RAMDISK_FILE hFile )
{
    FS_close((S_FS_FILE*) hFile );
}


int RAMDISK_Activate( RAMDISK_HANDLE hDisk )
{
	T__RAMDISK* pDisk = (T__RAMDISK*)hDisk;

	if( pDisk != NULL )
	{
		if( SoftwareActivate( pDisk->m_szName ) == 0 )
		{
			return 0;
		}
	}

	return -1;
}

/*
 * Funções
 */

// Download a file.
int iFtp_download_file ( FTP_HANDLE hFTP, RAMDISK_HANDLE hDisk, char *szFilename )
{
	unsigned char pucBuffer[__DATA_SIZE];
	int nRet = -1;
	int nReadBytes = 0, nTotalBytes = 0;
	int h, w, tamSaida;
	char saida[20];

	GetScreenSize(&h, &w);

	// Get the file to transfer.
	if( FTP_Get( hFTP, szFilename ) == 0 )
	{
	   // Open and create a file in the RAM disk. In append mode
	   RAMDISK_FILE hFile = RAMDISK_OpenFile( hDisk, szFilename, "a");
	   if( hFile != 0 )
	   {
		  nRet = 0;
		  do
		  {
			  // Read bytes of the file from the data channel.
			  nReadBytes = FTP_GetData( hFTP, (char *)pucBuffer, __DATA_SIZE );
			  if( nReadBytes >= 0 )
			  {
				  // Write the read files in the RAM file.
				  if( RAMDISK_WriteInFile( hDisk, hFile, pucBuffer, nReadBytes ) != 0 )
				  {
					  // Write Error.
					  nRet = -1;
				  }

				  nTotalBytes += nReadBytes;

				  //imprime total de bytes na tela
				  memset(saida, 0, sizeof(saida));
				  sprintf(saida, "  %d bytes", nTotalBytes);
				  tamSaida = _SizeofExtendedString(saida, _NORMALE_, _PROPORTIONNEL_);
				  _DrawExtendedString(w-tamSaida, h-10, saida, _OFF_, _NORMALE_, _PROPORTIONNEL_);
				  PaintGraphics();
			  }
		  } while(( nReadBytes > 0 ) && ( nRet == 0 ));

		  // Close the RAM file
		  RAMDISK_CloseFile( hDisk, hFile );
	   }

	   // Close FTP data channel.
	   FTP_CloseData( hFTP );
	}
	else
	{
		// File error. Can not access remote file.
		//To complete...
	}
	return nRet;
}

int iFtp_download_files( FTP_HANDLE hFTP, RAMDISK_HANDLE hDisk, char **aFilenames, int nNbFiles )
{
	char cBuf1[60];
	char cBuf2[60];

	int i = 0;
	int bContinue = 1;

	FTP_SetType( hFTP, __TYPE_IMAGE );

	while(( bContinue == 1 ) && ( i < nNbFiles ))
	{
		sprintf( cBuf1, "Baixando arquivo %d/%d",  i+1, nNbFiles );
		sprintf( cBuf2, "%s",  aFilenames[i] );
		DisplaySimpleMessage2lines( cBuf1, cBuf2, TRUE, TRUE );

		if( iFtp_download_file( hFTP, hDisk, aFilenames[i] ) == 0 )
		{
			// Next File.
			i++;
		}
		else
		{
			bContinue = 0;
		}
	}

	if( bContinue == 1 )
	{
		// Success !
		return 0;
	}

	return -1;
}


int iFtp_get_file_list( FTP_HANDLE hFTP, char **aFilenames, int nMaxFile )
{
	int bContinue = 1;
	int nNbFiles = 0;

	// List the content of the remote directory.
	// Just get the name, not the full description of the files.

	DisplaySimpleMessage( "Carregando Lista....", TRUE, TRUE );

	if( FTP_GetList( hFTP, "." ) == 0 )
	{
	while( bContinue == 1 )
	{
	   // Read next item of the list.
	   const char *szFilename = FTP_GetListItem( hFTP );

	   if( szFilename != NULL)
	   {
			if( nNbFiles < nMaxFile )
			{
				 // Duplicate Name. And store it in the filename array.
				 aFilenames[nNbFiles] = (char *)
				 umalloc( strlen ( szFilename ) + 1 );
				 strcpy( aFilenames[nNbFiles], szFilename );
				 nNbFiles++;
			}
			else
			{
				// Error : two many files.
				DisplaySimpleMessage( "Erro: muitos arquivos", TRUE, TRUE );
			}
	   }
	   else
	   {
			// End of the list.
			bContinue = 0;
	   }
	}

	// Close the data channel.
	FTP_CloseData( hFTP );
	}
	// Return the number of items.
	return nNbFiles;
}
