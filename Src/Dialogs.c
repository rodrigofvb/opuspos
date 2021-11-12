#include "SDK30.H"

#include "Logo.h"
#include "Conn.h"

/*
 * emite um sinal sonoro (bip)
 */
void Bip()
{
	Beep(10, 4, 5, BEEP_ON|BEEP_WAIT|BEEP_OFF);
}

/*
 * limpa a tela do terminal
 */
void LimpaTela()
{
	_clrscr();
	PaintGraphics();
}

/* 	mensagem na tela sem janela - substitui o printf
 * 	centroX - centraliza na horizontal
 * 	centroY - centraliza na vertical
 */
void DisplaySimpleMessage(char *msg, bool centroX, bool centroY)
{
	int MAXX, MAXY;
	int tamstr;
	int x, y;
	int hY, hX;

	int bPolicy = _MEDIUM_;
	int btype = _PROPORTIONNEL_;

	GetScreenSize(&MAXY, &MAXX);

	if (!centroX)
	{
		x = 0;
	}
	else
	{
		tamstr = _SizeofExtendedString (msg, bPolicy, btype);
		x = (int)(MAXX / 2)-(int)(tamstr / 2);
	}

	if (!centroY)
	{
		y = 0;
	}
	else
	{
		y = (int)(MAXY/ 2)-(int)(GetPoliceSize() / 2);

		if (IsHeader())
		{
			if (GetHeaderSize(&hY, &hX)==0)
				y += (int) (hY / 2);
			else
				y -= (int) (hY / 2);
		}
	}

	CreateGraphics( bPolicy );
	_clrscr();
	_DrawExtendedString(x, y, msg, _OFF_, _MEDIUM_, _PROPORTIONNEL_);
	PaintGraphics();
}

/* 	mensagem na tela sem janela em 2 linhas - substitui o printf
 *  - linha1 (entrada) = string primeira linha
 *  - linha2 (entrada) = string segunda linha
 * 	- centroX (entrada) = centraliza na horizontal (aceita TRUE OR FALSE)
 * 	- centroY (entrada) = centraliza na vertical (aceita TRUE OR FALSE)
 */
void DisplaySimpleMessage2lines(char *linha1, char *linha2, bool centroX, bool centroY)
{
	int MAXX, MAXY;
	int tamstr;
	int x1, x2, y;
	int hY, hX;

	int bPolicy = _MEDIUM_;
	int btype = _PROPORTIONNEL_;

	GetScreenSize(&MAXY, &MAXX);

	if (!centroX)
	{
		x1 = 0;
		x2 = 0;
	}
	else
	{
		tamstr = _SizeofExtendedString (linha1, bPolicy, btype);
		x1 = (int)(MAXX / 2)-(int)(tamstr / 2);

		tamstr = _SizeofExtendedString (linha2, bPolicy, btype);
		x2 = (int)(MAXX / 2)-(int)(tamstr / 2);
	}

	if (!centroY)
	{
		y = 0;
	}
	else
	{
		y = (int)(MAXY/ 2)-(int)(GetPoliceSize() / 2);

		if (IsHeader())
		{
			if (GetHeaderSize(&hY, &hX)==0)
				y += (int) (hY / 2);
			else
				y -= (int) (hY / 2);
		}
	}

	CreateGraphics( bPolicy );
	_clrscr();
	_DrawExtendedString(x1, y-6, linha1, _OFF_, _MEDIUM_, _PROPORTIONNEL_);
	_DrawExtendedString(x2, y+6, linha2, _OFF_, _MEDIUM_, _PROPORTIONNEL_);
	PaintGraphics();
}

/*
 *  mensagem na tela com janela
 *  - message (entrada) = mensagem
 *  - title (entrada) = título
 *  - timeout (entrada) = tempo de exibição
 */
void ShowDialogBox(char *message, char *title, int timeout)
{
	int prec;
	int MAXX;
	int MAXY;

	prec = StateHeader(0);
	GetScreenSize(&MAXY, &MAXX);

	SetFont( _SMALL_ );
	_DrawWindow( 3, 11, (MAXX-4), (MAXY-1-12), 1, (unsigned char*)title );
	_DrawExtendedString((int)(MAXX / 2)-((int)(strlen(message) / 2) * 6), 20,
			message, _OFF_, _MEDIUM_, _PROPORTIONNEL_);
	PaintGraphics();

	ttestall(0, timeout * 100);

	StateHeader(prec);
}

/*
 * interrompe a execução e exibe diálogo na tela pedindo ao usuário
 * para pressionar qualquer tecla para continuar
 * - timeout (entrada) = tempo do diálogo até continuar a
 * execução do programa, 0 = infinito.
 */
void PressioneTecla(int timeout)
{
	FILE *hKeyb = fopen("KEYBOARD", "r");
	DisplaySimpleMessage2lines ("Pressione qualquer", "tecla para continuar", TRUE, TRUE);
	ttestall(KEYBOARD, timeout);
	fclose(hKeyb);
}

/* Diálogo que pede ao usuário para digitar um número inteiro
 * titulo (entrada) - título da caixa de diálogo
 * tamanho_max (entrada) - o tamanho máximo de caracteres numéricos
 * val_init (entrada) - valor inicial, se houver
 * val_init (saída) - retorno do valor digitado
 * retorna CR_ENTRY_OK se o valor for OK
 */
int PegaInteiro(char *titulo, int tamanho_max, char *val_init)
{
	unsigned char cr;
	StructList Menu;
	int MAXX;
	int MAXY;
	int prec;
	TAB_ENTRY_STRING NumEntry;
	ENTRY_BUFFER buf_entry;

	prec = StateHeader(0);
	GetScreenSize(&MAXY, &MAXX);

	Menu.MyWindow.left         = 0;
	Menu.MyWindow.top         = 0;
	Menu.MyWindow.rigth        = (MAXX-1);
	Menu.MyWindow.bottom       = (MAXY-1);
	Menu.MyWindow.nblines      = 4;
	Menu.MyWindow.fontsize     = _MEDIUM_;
	Menu.MyWindow.type         = _PROPORTIONNEL_;
	Menu.MyWindow.font         = 0;
	Menu.MyWindow.shortcommand = _OFF_;
	Menu.MyWindow.selected     = _ON_;
	Menu.MyWindow.offset       = 0;
	Menu.MyWindow.thickness    = 1	;
	Menu.MyWindow.border       = _ON_;
	Menu.MyWindow.popup        = _NOPOPUP_;
	Menu.MyWindow.first        = 0;
	Menu.MyWindow.time_out     = 60;
	Menu.MyWindow.current      = 0;
	Menu.MyWindow.title        = (byte *) titulo;

	Menu.tab[0] = NULL;
	Menu.tab[1] = (byte *) val_init;
	Menu.tab[2] = NULL;
	Menu.tab[3] = NULL;

	NumEntry.mask     = 0;
	NumEntry.time_out = 4*MINUTE;
	NumEntry.line     = 25;
	NumEntry.column   = 5;
	NumEntry.echo     = ECHO_NORMAL;
	NumEntry.nb_max   = tamanho_max;
	NumEntry.nb_min   = 1;

	G_Numerical_Entry (&Menu, &NumEntry);

	ttestall(ENTRY, 0);

	cr = Get_Entry(&buf_entry);

	if (cr==CR_ENTRY_OK)
	{
		strcpy(val_init, (char *)buf_entry.d_entry);
	}
	else if (cr==CR_ENTRY_NOK)
	{
		// Incorrect Entry
	}
	StateHeader(prec);

	return cr;
}

int PegaAlfa(char *titulo, char *val_init)
{
	int retour;
	BUFFER_SAISIE   	buf_saisie;
	unsigned char   	alfa[50];
	StructList      	Menu;
	unsigned char   	cr;
	TAB_ENTRY_STRING 	t_alfa;

	int MAXX,MAXY,i;

	GetScreenSize(&MAXY,&MAXX);

	CreateGraphics(_MEDIUM_);

	memset( &Menu, 0, sizeof(Menu) );

	Menu.MyWindow.left			= 0;
	Menu.MyWindow.top			= 0;
	Menu.MyWindow.rigth			= (MAXX-1);
	Menu.MyWindow.bottom		= (MAXY-1);
	Menu.MyWindow.fontsize		= GetDefaultFont();
	Menu.MyWindow.type			= _PROPORTIONNEL_;
	Menu.MyWindow.font			= 0;
	Menu.MyWindow.shortcommand	= _ON_;
	Menu.MyWindow.thickness		= 1;
	Menu.MyWindow.border		= _ON_;
	Menu.MyWindow.popup			= _NOPOPUP_;
	Menu.MyWindow.first			= 0;
	Menu.MyWindow.time_out		= 60;
	Menu.MyWindow.current		= 0;
	Menu.MyWindow.correct		= _OFF_;
	Menu.MyWindow.offset		= 0;
	Menu.MyWindow.nblines		= 3;
	Menu.MyWindow.selected		= _OFF_;

	for (i=0;i<NB_ELT_LISTE;i++)
	  Menu.tab[i] = (byte *) PT_NULL;

	Menu.MyWindow.title = (byte *) titulo;

	t_alfa.mask     = MASK_ANN;
	t_alfa.time_out = 4*MINUTE;
	t_alfa.line     = 25;
	t_alfa.column   = 5;
	t_alfa.echo     = ECHO_NORMAL;
	t_alfa.nb_max   = 22;
	t_alfa.nb_min   = 1;

	strcpy((char *)alfa, val_init);
	Menu.tab[1] = (unsigned char *) alfa;
	G_Extended_entry((StructListe *) &Menu,(TAB_ENTRY_STRING *) &t_alfa);

	ttestall(SAISIE, 0);

	cr = Read_Saisie(&buf_saisie);
	switch (cr)
	{
		case  CR_SAISIE_OK :
			buf_saisie.donnees[buf_saisie.nombre]='\0';
			retour = INIT_OK;
			break;
		case CR_SAISIE_VAL :
			retour = INIT_VAL;
			break;
		case CR_SAISIE_NOK :
			retour = INIT_KO;
			break;
		default :
			retour = ANNUL;
			break;
		case CR_SAISIE_FCT :
		case CR_SAISIE_SK1 :
			retour = INIT_SK1;
			break;
	}

	if (retour==INIT_OK)
	{
		strcpy((char *)val_init,(char *)buf_saisie.donnees);
	}
	return (retour);
}

/* abre um diálogo pedindo a senha
 * titulo (entrada) = título da caixa
 * senha (saída) = saída da senha
 * retorna 0 se OK, ou negativo se não OK
 */
int PegaSenha(char *titulo, char *senha)
{
	unsigned char cr;
	StructList Menu;
	int MAXX;
	int MAXY;
	int prec;
	TAB_ENTRY_STRING NumEntry;
	ENTRY_BUFFER buf_entry;

	prec = StateHeader(0);
	GetScreenSize(&MAXY, &MAXX);

	Menu.MyWindow.left         = 2;
	Menu.MyWindow.top          = 22;
	Menu.MyWindow.rigth        = (MAXX-2-1);
	Menu.MyWindow.bottom       = (MAXY-4-1);
	Menu.MyWindow.nblines      = 4;
	Menu.MyWindow.fontsize     = _MEDIUM_;
	Menu.MyWindow.type         = _PROPORTIONNEL_;
	Menu.MyWindow.font         = 0;
	Menu.MyWindow.shortcommand = _OFF_;
	Menu.MyWindow.selected     = _ON_;
	Menu.MyWindow.offset       = 0;
	Menu.MyWindow.thickness    = 1;
	Menu.MyWindow.border       = _ON_;
	Menu.MyWindow.popup        = _NOPOPUP_;
	Menu.MyWindow.first        = 0;
	Menu.MyWindow.time_out     = 60;
	Menu.MyWindow.current      = 0;
	Menu.MyWindow.title        = NULL;

	Menu.tab[0] = (byte *) titulo;
	Menu.tab[1] = NULL;
	Menu.tab[2] = NULL;
	Menu.tab[3] = NULL;

	NumEntry.mask     = 0;
	NumEntry.time_out = 60;
	NumEntry.line     = 40;
	NumEntry.column   = 10;
	NumEntry.echo     = ECHO_SPECIAL;
	NumEntry.nb_max   = 10;
	NumEntry.nb_min   = 4;

	G_Numerical_Entry (&Menu, &NumEntry);

	ttestall(ENTRY, 0);

	cr = Get_Entry(&buf_entry);

	if (cr==CR_ENTRY_OK)
	{
		// For validation
		strcpy(senha, (char *)buf_entry.d_entry);
	}

	StateHeader(prec);

	return cr;
}

/* abre um diálogo solicitando entrada do endereço IP
 * já faz a validação do IP
 * titulo (entrada) = titulo da caixa
 * val_init (entrada e saída) = entrada do valor inicial e saída do valor digitado
 */
int PegaEnderecoIP(char titulo[20], char *val_init)
{
	unsigned char cr;
	StructList Menu;
	int MAXX;
	int MAXY;
	TAB_ENTRY_ALPHA AlphanumEntry;
	ENTRY_BUFFER buf_entry;

	GetScreenSize(&MAXY, &MAXX);

	Menu.MyWindow.left         	= 0;
	Menu.MyWindow.top         	= 0;
	Menu.MyWindow.rigth        	= (MAXX-1);
	Menu.MyWindow.bottom       	= (MAXY-1);
	Menu.MyWindow.nblines      	= 4;
	Menu.MyWindow.fontsize		= _MEDIUM_;
	Menu.MyWindow.type			= _PROPORTIONNEL_;
	Menu.MyWindow.font         	= 0;
	Menu.MyWindow.shortcommand 	= _OFF_;
	Menu.MyWindow.selected    	= _ON_;
	Menu.MyWindow.offset       	= 0;
	Menu.MyWindow.thickness    	= 1	;
	Menu.MyWindow.border       	= _ON_;
	Menu.MyWindow.popup        	= _NOPOPUP_;
	Menu.MyWindow.first        	= 0;
	Menu.MyWindow.time_out     	= 60;
	Menu.MyWindow.current      	= 0;
	Menu.MyWindow.title        	= (byte *) titulo;

	Menu.tab[0] = NULL;
	Menu.tab[1] = (byte *) val_init;
	Menu.tab[2] = NULL;
	Menu.tab[3] = NULL;

	AlphanumEntry.mask     = MASK_ANN;
	AlphanumEntry.time_out = 4*MINUTE;
	AlphanumEntry.line     = 25;
	AlphanumEntry.column   = 5;
	AlphanumEntry.echo     = ECHO_NORMAL;
	AlphanumEntry.nb_max   = 15;
	AlphanumEntry.nb_min   = 7;
	AlphanumEntry.tab_caracteres = (byte *) "0123456789."; 	//caracteres permitidos
	G_Alphanumerical_Entry(&Menu, &AlphanumEntry);

	ttestall(ENTRY, 0);

	cr = Get_Entry(&buf_entry);

	if (cr==CR_ENTRY_OK)
	{
		buf_entry.d_entry[buf_entry.d_len] = '\0';

		if (Validate_ip4((char *)buf_entry.d_entry))
		{
			strcpy((char *)val_init,(char *)buf_entry.d_entry);
		}
		else
		{
			ShowDialogBox("Endereco invalido", NULL, 2);
		}
	}
	else if (cr==CR_ENTRY_NOK)
	{
		ShowDialogBox("Endereco invalido", NULL, 2);
	}

	return cr;
}

/* abre um diálogo para digitação do MSISDN
 * - dia (saída) = Saída do número sem a máscara
 * retorna 0 se OK, -1 se houver erro
 */
int PegaData(char *diadisp)
{
	int MAXX, MAXY;
	int iIdx;
	int iRet;
	int ret = 0;
	int tamstr;

	char cKey;
	char tcBuf[8];

	FILE *hKeyboard;

	char mask = '_';
	char titulo[] = "Digite a Data";
	int bPolicy = _MEDIUM_;
	int btype = _PROPORTIONNEL_;

	GetScreenSize(&MAXY, &MAXX);

	hKeyboard = fopen("KEYBOARD", "r");

lblInicio:
	iIdx = 0;

	memset(tcBuf, mask, sizeof(tcBuf));

	sprintf(diadisp, "%c%c/%c%c/%c%c%c%c",
			tcBuf[0], tcBuf[1], tcBuf[2], tcBuf[3], tcBuf[4], tcBuf[5] ,tcBuf[6], tcBuf[7]);

	_clrscr();
	//imprime o título
	tamstr = _SizeofExtendedString(titulo, bPolicy, btype);
	_DrawExtendedString((int)(MAXX/2)-(int)(tamstr/2), 0, titulo, _OFF_, bPolicy, btype);
	//imprime a máscara
	_DrawExtendedString(30, 30, diadisp, _OFF_, bPolicy, btype);
	PaintGraphics();

	while(1)
	{
		cKey = 0;
		iRet = ttestall(KEYBOARD, 0);
		if (iRet & KEYBOARD)
			cKey = getchar();

		switch (cKey)
		{
			case T_ANN:
				// Assign red key
				Bip();
				if (iIdx > 0)
					goto lblInicio;
				else
					goto lblKO;
				break;
			case T_VAL:
				// Assign green key
				if (iIdx == 8)
				{
					Bip();
					//TODO validar data inserida
					goto lblOK;
				}
				break;
			case T_CORR:
				// Assign yellow key
				if (iIdx > 0)
				{
					Bip();
					iIdx--;
					tcBuf[iIdx] = mask;
				}
				break;
			case T_F:
				// Assign to function key
				break;
			case F1:
				// Assign F1
				break;
			case T_NUM1: case T_NUM2: case T_NUM3: case T_NUM4: case T_NUM5:
			case T_NUM6: case T_NUM7: case T_NUM8: case T_NUM9: case T_NUM0:
				if (iIdx < 11)
				{
					Bip();
					tcBuf[iIdx] = cKey;
					iIdx++;
				}
				break;
		}

		memset(diadisp, 0, sizeof(diadisp));

		//mapeamento do telefone no visor
		sprintf(diadisp, "%c%c/%c%c/%c%c%c%c",
					tcBuf[0], tcBuf[1], tcBuf[2], tcBuf[3], tcBuf[4], tcBuf[5] ,tcBuf[6], tcBuf[7]);

		_DrawExtendedString( 30, 30, diadisp, _OFF_, bPolicy, btype);
		PaintGraphics();
	}
lblKO:
	ret = -1;
	diadisp = NULL;
	goto lblEnd;
lblOK:
	ret = 0;
	goto lblEnd;
lblEnd:
	fclose(hKeyboard);
	return ret;
}

/* abre um diálogo para digitação do MSISDN
 * - operadora (entrada) = Nome da Operadora
 * - msisdn (saída) = Saída do número sem a máscara
 * - m_msisdn (saída) = Saída do número com a máscara
 * retorna 0 se OK, -1 se houver erro
 */
int PegaMSISDN(char *operadora, char *msisdn, char *f_msisdn)
{
	int MAXX, MAXY;
	int iIdx;
	int iRet;
	int ret = 0;
	int tamstr;

	char cKey;
	char tcBuf[12];
	char teldisp[16];

	FILE *hKeyboard;

	char mask = '_';
	char titulo[] = "Numero do Telefone";
	int bPolicy = _MEDIUM_;
	int btype = _PROPORTIONNEL_;

	GetScreenSize(&MAXY, &MAXX);

	hKeyboard = fopen("KEYBOARD", "r");

lblInicio:
	iIdx = 0;

	memset(tcBuf, mask, sizeof(tcBuf));
	tcBuf[11] = '\0';

	sprintf(teldisp, "(%c%c) %c%c%c%c%c-%c%c%c%c",
			tcBuf[0], tcBuf[1], tcBuf[2], tcBuf[3], tcBuf[4], tcBuf[5] ,tcBuf[6],
			tcBuf[7], tcBuf[8], tcBuf[9], tcBuf[10]);

	_clrscr();
	//imprime o título
	tamstr = _SizeofExtendedString(titulo, bPolicy, btype);
	_DrawExtendedString((int)(MAXX/2)-(int)(tamstr/2), 0, titulo, _OFF_, bPolicy, btype);
	//imprime a operadora
	tamstr = _SizeofExtendedString(operadora, bPolicy, btype);
	_DrawExtendedString((int)(MAXX/2)-(int)(tamstr/2), 12, operadora, _OFF_, bPolicy, btype);
	//imprime a máscara
	_DrawExtendedString(20, 30, teldisp, _OFF_, bPolicy, btype);
	PaintGraphics();

	while(1)
	{
		cKey = 0;
		iRet = ttestall(KEYBOARD, 0);
		if (iRet & KEYBOARD)
			cKey = getchar();

		switch (cKey)
		{
			case T_ANN:
				// Assign red key
				Bip();
				if (iIdx > 0)
					goto lblInicio;
				else
					goto lblKO;
				break;
			case T_VAL:
				// Assign green key
				if (iIdx == 11)
				{
					Bip();
					goto lblOK;
				}
				break;
			case T_CORR:
				// Assign yellow key
				if (iIdx > 0)
				{
					Bip();
					iIdx--;
					tcBuf[iIdx] = mask;
				}
				break;
			case T_F:
				// Assign to function key
				break;
			case F1:
				// Assign F1
				break;
			case T_NUM1: case T_NUM2: case T_NUM3: case T_NUM4: case T_NUM5:
			case T_NUM6: case T_NUM7: case T_NUM8: case T_NUM9: case T_NUM0:
				if (iIdx < 11)
				{
					Bip();
					tcBuf[iIdx] = cKey;
					iIdx++;
				}
				break;
		}

		memset(teldisp, 0, sizeof(teldisp));

		//mapeamento do telefone no visor
		sprintf(teldisp, "(%c%c) %c%c%c%c%c-%c%c%c%c",
				tcBuf[0], tcBuf[1], tcBuf[2], tcBuf[3], tcBuf[4], tcBuf[5] ,tcBuf[6],
				tcBuf[7], tcBuf[8], tcBuf[9], tcBuf[10]);

		_DrawExtendedString( 20, 30, teldisp, _OFF_, bPolicy, btype);
		PaintGraphics();
	}
lblKO:
	ret = -1;
	msisdn = NULL;
	f_msisdn = NULL;
	goto lblEnd;
lblOK:
	ret = 0;
	strcpy(msisdn, tcBuf);
	strcpy(f_msisdn, teldisp);
	goto lblEnd;
lblEnd:
	fclose(hKeyboard);
	return ret;
}

void DisplayLogo()
{
	int MAXX, MAXY;
	int x, y;

	GetScreenSize(&MAXY, &MAXX);

	x = (int)(MAXX/2)-(int)(__LogoBMP_LARG__/2);
	y = (int)(MAXY/2)-(int)(__LogoBMP_ALT__/2);

	if (IsHeader())
	{
		int hY, hX;
		if (GetHeaderSize(&hY, &hX)==0)
			y += (int) (hY/2);
		else
			y -= (int) (hY/2);
	}

	clrscr();
	DisplayBitmap (x, y, (unsigned char*)__LogoBMP__, 8, _OFF_);
	PaintGraphics();
}

void PrintSimpleMessage(char *msg)
{
	FILE *hPrn;
	hPrn = fopen("PRINTER","w");

	pprintf("\x0F");
	pprintf(msg);
	pprintf("\x12");

	ttestall(PRINTER, 30*100);
	fclose(hPrn);
}
