//protótipos em FTP.c
typedef void * RAMDISK_HANDLE;
typedef void * RAMDISK_FILE;

RAMDISK_HANDLE RAMDISK_Create( const char *szName, unsigned long nSize, unsigned int nMaxFile );
int RAMDISK_Delete( RAMDISK_HANDLE hDisk );
int RAMDISK_Activate( RAMDISK_HANDLE hDisk );

int iFtp_download_file ( FTP_HANDLE hFTP, RAMDISK_HANDLE hDisk, char *szFilename );
int iFtp_download_files( FTP_HANDLE hFTP, RAMDISK_HANDLE hDisk, char **aFilenames, int nNbFiles );
int iFtp_get_file_list( FTP_HANDLE hFTP, char **aFilenames, int nMaxFile );
