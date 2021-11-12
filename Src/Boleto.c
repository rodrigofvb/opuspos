#include "SDK30.H"

#include "BoletoBMP.h"
#include "FontesBMP.h"
#include "Codbarras25BMP.h"
#include "HTTP.h"
#include "Globals.h"
#include "Dialogs.h"

struct Confs config;

//retorna posicao y do próximo caracter na vertical
int ImprimeCaracVert(const unsigned char *bmp, int xpos, int ypos, int grande)
{
	unsigned long w, h;
	unsigned short bc;

	GetBmpInfos((byte*)bmp, &w, &h, &bc);

	ypos -= (int)h;

	_PrintBmpXY(xpos, ypos, (byte*) bmp, _OFF_);

	if (!grande)
		ypos -= pxls_por_letra;
	else
		ypos -= pxls_por_letra_grande;

	return ypos;
}

/* imprime o [texto] na vertical
 * maxcarac - quantidade máxima de caracteres
 * xpos - posicao horizontal,
 * ypos - posicao de início na vertical (menor = mais acima, maior = mais abaixo)
 * grande - fonte grande = TRUE e fonte pequena = FALSE
 */
void ImprimeVertical(char *texto, int maxcarac, int xpos, int ypos, int grande)
{
	int i = 0;

	while (i < maxcarac && texto[i] != '\0')
	{
		int c = texto[i];

		if (!grande)
		{
			//fonte pequena
			switch(c)
			{
				//Associação com o código da tabela ASCII
				case 36: ypos = ImprimeCaracVert(f_cifrao, xpos, ypos, grande); break;
				case 37: ypos = ImprimeCaracVert(f_perc, xpos, ypos, grande); break;
				case 44: ypos = ImprimeCaracVert(f_virgula, xpos, ypos, grande); break;
				case 45: ypos = ImprimeCaracVert(f_hifen, xpos, ypos, grande); break;
				case 46: ypos = ImprimeCaracVert(f_ponto, xpos, ypos, grande); break;
				case 47: ypos = ImprimeCaracVert(f_barra, xpos, ypos, grande); break;
				case 48: ypos = ImprimeCaracVert(f_0, xpos, ypos, grande); break;
				case 49: ypos = ImprimeCaracVert(f_1, xpos, ypos, grande); break;
				case 50: ypos = ImprimeCaracVert(f_2, xpos, ypos, grande); break;
				case 51: ypos = ImprimeCaracVert(f_3, xpos, ypos, grande); break;
				case 52: ypos = ImprimeCaracVert(f_4, xpos, ypos, grande); break;
				case 53: ypos = ImprimeCaracVert(f_5, xpos, ypos, grande); break;
				case 54: ypos = ImprimeCaracVert(f_6, xpos, ypos, grande); break;
				case 55: ypos = ImprimeCaracVert(f_7, xpos, ypos, grande); break;
				case 56: ypos = ImprimeCaracVert(f_8, xpos, ypos, grande); break;
				case 57: ypos = ImprimeCaracVert(f_9, xpos, ypos, grande); break;
				case 65: ypos = ImprimeCaracVert(f_A, xpos, ypos, grande); break;
				case 66: ypos = ImprimeCaracVert(f_B, xpos, ypos, grande); break;
				case 67: ypos = ImprimeCaracVert(f_C, xpos, ypos, grande); break;
				case 68: ypos = ImprimeCaracVert(f_D, xpos, ypos, grande); break;
				case 69: ypos = ImprimeCaracVert(f_E, xpos, ypos, grande); break;
				case 70: ypos = ImprimeCaracVert(f_F, xpos, ypos, grande); break;
				case 71: ypos = ImprimeCaracVert(f_G, xpos, ypos, grande); break;
				case 72: ypos = ImprimeCaracVert(f_H, xpos, ypos, grande); break;
				case 73: ypos = ImprimeCaracVert(f_I, xpos, ypos, grande); break;
				case 74: ypos = ImprimeCaracVert(f_J, xpos, ypos, grande); break;
				case 75: ypos = ImprimeCaracVert(f_K, xpos, ypos, grande); break;
				case 76: ypos = ImprimeCaracVert(f_L, xpos, ypos, grande); break;
				case 77: ypos = ImprimeCaracVert(f_M, xpos, ypos, grande); break;
				case 78: ypos = ImprimeCaracVert(f_N, xpos, ypos, grande); break;
				case 79: ypos = ImprimeCaracVert(f_O, xpos, ypos, grande); break;
				case 80: ypos = ImprimeCaracVert(f_P, xpos, ypos, grande); break;
				case 81: ypos = ImprimeCaracVert(f_Q, xpos, ypos, grande); break;
				case 82: ypos = ImprimeCaracVert(f_R, xpos, ypos, grande); break;
				case 83: ypos = ImprimeCaracVert(f_S, xpos, ypos, grande); break;
				case 84: ypos = ImprimeCaracVert(f_T, xpos, ypos, grande); break;
				case 85: ypos = ImprimeCaracVert(f_U, xpos, ypos, grande); break;
				case 86: ypos = ImprimeCaracVert(f_V, xpos, ypos, grande); break;
				case 87: ypos = ImprimeCaracVert(f_W, xpos, ypos, grande); break;
				case 88: ypos = ImprimeCaracVert(f_X, xpos, ypos, grande); break;
				case 89: ypos = ImprimeCaracVert(f_Y, xpos, ypos, grande); break;
				case 90: ypos = ImprimeCaracVert(f_Z, xpos, ypos, grande); break;
				default:
					ypos -= pxls_por_espaco;	//espaço em branco
			}
		}
		else
		{
			//fonte grande
			switch(c)
			{
				//Associação com o código da tabela ASCII
				case 45: ypos = ImprimeCaracVert(fg_hifen, xpos, ypos, grande); break;
				case 46: ypos = ImprimeCaracVert(fg_ponto, xpos, ypos, grande); break;
				case 48: ypos = ImprimeCaracVert(fg_0, xpos, ypos, grande); break;
				case 49: ypos = ImprimeCaracVert(fg_1, xpos, ypos, grande); break;
				case 50: ypos = ImprimeCaracVert(fg_2, xpos, ypos, grande); break;
				case 51: ypos = ImprimeCaracVert(fg_3, xpos, ypos, grande); break;
				case 52: ypos = ImprimeCaracVert(fg_4, xpos, ypos, grande); break;
				case 53: ypos = ImprimeCaracVert(fg_5, xpos, ypos, grande); break;
				case 54: ypos = ImprimeCaracVert(fg_6, xpos, ypos, grande); break;
				case 55: ypos = ImprimeCaracVert(fg_7, xpos, ypos, grande); break;
				case 56: ypos = ImprimeCaracVert(fg_8, xpos, ypos, grande); break;
				case 57: ypos = ImprimeCaracVert(fg_9, xpos, ypos, grande); break;
				default:
					ypos -= pxls_por_espaco_grande;	//espaço em branco
			}
		}

		i++;
	}
}

void ImprimeCodBarras25(char *Codbarras, int maxcarac, int xpos, int ypos)
{
	unsigned long w_fp, h_fp;
	unsigned long w_gp, h_gp;
	unsigned long w_fb, h_fb;
	unsigned long w_gb, h_gb;
	unsigned short bc;
	int i, j, cp, cb;

	if (maxcarac%2 != 0) return;

	char barcode[10][5] = {
		{'0','0','1','1','0'},
		{'1','0','0','0','1'},
		{'0','1','0','0','1'},
		{'1','1','0','0','0'},
		{'0','0','1','0','1'},
		{'1','0','1','0','0'},
		{'0','1','1','0','0'},
		{'0','0','0','1','1'},
		{'1','0','0','1','0'},
		{'0','1','0','1','0'},
	};

	GetBmpInfos((byte*)barra_fina_p, &w_fp, &h_fp, &bc);
	GetBmpInfos((byte*)barra_grossa_p, &w_gp, &h_gp, &bc);
	GetBmpInfos((byte*)barra_fina_b, &w_fb, &h_fb, &bc);
	GetBmpInfos((byte*)barra_grossa_b, &w_gb, &h_gb, &bc);

	//sequencia inicial - barra fina preta, fina branca, fina preta, fina branca
	ypos -= h_fp; _PrintBmpXY(xpos, ypos, (byte*) barra_fina_p, _OFF_);
	ypos -= h_fb; _PrintBmpXY(xpos, ypos, (byte*) barra_fina_b, _OFF_);
	ypos -= h_fp; _PrintBmpXY(xpos, ypos, (byte*) barra_fina_p, _OFF_);
	ypos -= h_fb; _PrintBmpXY(xpos, ypos, (byte*) barra_fina_b, _OFF_);

	i = 0;

	while(i < maxcarac-1)
	{
		//conversão char para int
		cp = Codbarras[i] - '0';
		cb = Codbarras[i+1] - '0';

		for(j = 0; j < 5; j++)
		{
			//imprime barra preta
			if (barcode[cp][j]=='0')
			{
				ypos -= h_fp;
				_PrintBmpXY(xpos, ypos, (byte*) barra_fina_p, _OFF_);
			}
			else
			{
				ypos -= h_gp;
				_PrintBmpXY(xpos, ypos, (byte*) barra_grossa_p, _OFF_);
			}

			//imprime barra branca
			if (barcode[cb][j]=='0')
			{
				ypos -= h_fb;
				_PrintBmpXY(xpos, ypos, (byte*) barra_fina_b, _OFF_);
			}
			else
			{
				ypos -= h_gb;
				_PrintBmpXY(xpos, ypos, (byte*) barra_grossa_b, _OFF_);
			}
		}

		i += 2;
	}

	//sequencia final - barra grossa preta, fina branca, fina preta
	ypos -= h_gp; _PrintBmpXY(xpos, ypos, (byte*) barra_grossa_p, _OFF_);
	ypos -= h_fb; _PrintBmpXY(xpos, ypos, (byte*) barra_fina_b, _OFF_);
	ypos -= h_fp; _PrintBmpXY(xpos, ypos, (byte*) barra_fina_p, _OFF_);
}

bool ImprimeBoleto(Tboleto *boleto)
{
	unsigned long w, tamquadro, tamlogo;
	unsigned short bc;
	unsigned char Status;
	FILE *hPrn;
	int ret = TRUE;
	int r;

	char agencia_conta[sizeof(boleto->agencia) + sizeof(boleto->conta) + 1];
	char linhadigitalfmt[55], codbarras[45];

	sprintf(agencia_conta,"%s/%s", boleto->agencia, boleto->conta);

	if (strlen(boleto->linhadigitavel)==47)
	{
		sprintf(linhadigitalfmt,
			"%c%c%c%c%c.%c%c%c%c%c %c%c%c%c%c.%c%c%c%c%c%c %c%c%c%c%c.%c%c%c%c%c%c %c %c%c%c%c%c%c%c%c%c%c%c%c%c%c",
			boleto->linhadigitavel[0], boleto->linhadigitavel[1], boleto->linhadigitavel[2], boleto->linhadigitavel[3],
			boleto->linhadigitavel[4], boleto->linhadigitavel[5], boleto->linhadigitavel[6], boleto->linhadigitavel[7],
			boleto->linhadigitavel[8], boleto->linhadigitavel[9], boleto->linhadigitavel[10], boleto->linhadigitavel[11],
			boleto->linhadigitavel[12], boleto->linhadigitavel[13], boleto->linhadigitavel[14], boleto->linhadigitavel[15],
			boleto->linhadigitavel[16], boleto->linhadigitavel[17], boleto->linhadigitavel[18], boleto->linhadigitavel[19],
			boleto->linhadigitavel[20], boleto->linhadigitavel[21], boleto->linhadigitavel[22], boleto->linhadigitavel[23],
			boleto->linhadigitavel[24], boleto->linhadigitavel[25], boleto->linhadigitavel[26], boleto->linhadigitavel[27],
			boleto->linhadigitavel[28], boleto->linhadigitavel[29], boleto->linhadigitavel[30], boleto->linhadigitavel[31],
			boleto->linhadigitavel[32], boleto->linhadigitavel[33], boleto->linhadigitavel[34], boleto->linhadigitavel[35],
			boleto->linhadigitavel[36], boleto->linhadigitavel[37], boleto->linhadigitavel[38], boleto->linhadigitavel[39],
			boleto->linhadigitavel[40], boleto->linhadigitavel[41], boleto->linhadigitavel[42], boleto->linhadigitavel[43],
			boleto->linhadigitavel[44], boleto->linhadigitavel[45], boleto->linhadigitavel[46]);

		sprintf(codbarras,
			"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
			boleto->linhadigitavel[0], boleto->linhadigitavel[1], boleto->linhadigitavel[2], boleto->linhadigitavel[3],
			boleto->linhadigitavel[32], boleto->linhadigitavel[33], boleto->linhadigitavel[34], boleto->linhadigitavel[35],
			boleto->linhadigitavel[36],boleto->linhadigitavel[37], boleto->linhadigitavel[38], boleto->linhadigitavel[39],
			boleto->linhadigitavel[40], boleto->linhadigitavel[41], boleto->linhadigitavel[42], boleto->linhadigitavel[43],
			boleto->linhadigitavel[44], boleto->linhadigitavel[45], boleto->linhadigitavel[46], boleto->linhadigitavel[4],
			boleto->linhadigitavel[5], boleto->linhadigitavel[6], boleto->linhadigitavel[7], boleto->linhadigitavel[8],
			boleto->linhadigitavel[10], boleto->linhadigitavel[11], boleto->linhadigitavel[12], boleto->linhadigitavel[13],
			boleto->linhadigitavel[14], boleto->linhadigitavel[15], boleto->linhadigitavel[16], boleto->linhadigitavel[17],
			boleto->linhadigitavel[18], boleto->linhadigitavel[19], boleto->linhadigitavel[21], boleto->linhadigitavel[22],
			boleto->linhadigitavel[23], boleto->linhadigitavel[24], boleto->linhadigitavel[25], boleto->linhadigitavel[26],

			boleto->linhadigitavel[27], boleto->linhadigitavel[28], boleto->linhadigitavel[29], boleto->linhadigitavel[30]);
	}

	hPrn = fopen("PRINTER","w");

	GetBmpInfos((byte*)__boleto__, &w, &tamquadro, &bc); 	//boleto

	DefPrinterMargins(0, 0);

lblInicioImpressao:
	DisplaySimpleMessage ("Aguarde, Imprimindo...", TRUE, TRUE);

	InitPrinterBitmap(tamquadro);

	//imprime o boleto vazio
	r = _PrintBmpXY(0, 0, (byte*)__boleto__, _OFF_);
	if (r != BMP_OK) goto lblErr;

	//logo do banco
	if (strcmp(boleto->nomebanco, "ITAU")==0)
	{
		GetBmpInfos((byte*)__LogoItau__, &w, &tamlogo, &bc);

		r = _PrintBmpXY(10, tamquadro-tamlogo-20, 	(byte*) __LogoItau__, _OFF_);
		if (r != BMP_OK) goto lblErr;
		r = _PrintBmpXY(5, 	1560-tamlogo, 			(byte*) __LogoItau__, _OFF_);
		if (r != BMP_OK) goto lblErr;

		ImprimeVertical(boleto->codbanco, 	5, 		25, 	tamquadro-tamlogo-60, TRUE);
	}
	else
	{
		ImprimeVertical(boleto->codbanco, 	5, 		10, 	tamquadro-20, TRUE);
		ImprimeVertical(boleto->nomebanco, 	10, 	25, 	1542, TRUE);
		ImprimeVertical(boleto->nomebanco, 	10, 	45, 	tamquadro-20, TRUE);
	}
	//recibo do sacado
	ImprimeVertical(boleto->sacado, 		33, 	120, 	tamquadro-20, FALSE);
	ImprimeVertical(boleto->cedente, 		33, 	160, 	tamquadro-20, FALSE);
	ImprimeVertical(agencia_conta, 			33, 	199, 	tamquadro-20, FALSE);
	ImprimeVertical(boleto->nossonumero, 	33, 	238, 	tamquadro-20, FALSE);
	ImprimeVertical(boleto->datavenc, 		33, 	280, 	tamquadro-20, FALSE);
	ImprimeVertical(boleto->valor, 			33, 	321, 	tamquadro-20, FALSE);
	//ficha de compensação
	ImprimeVertical(boleto->codbanco, 		5, 		25, 	1280, TRUE);
	ImprimeVertical(linhadigitalfmt, 		54, 	25, 	1152, TRUE);
	ImprimeVertical(boleto->cedente, 		60, 	82, 	1550, FALSE);
	ImprimeVertical(boleto->dataproc, 		10, 	120, 	1550, FALSE);
	ImprimeVertical(boleto->documento, 		10, 	120, 	1180, FALSE);
	ImprimeVertical("DM", 					2, 		120, 	912, FALSE);
	ImprimeVertical("N", 					1, 		120, 	718, FALSE);
	ImprimeVertical(boleto->carteira, 		3, 		162, 	1210, FALSE);
	ImprimeVertical("R$", 					2, 		162, 	902, FALSE);
	ImprimeVertical(boleto->datavenc, 		10, 	82, 	423, FALSE);
	ImprimeVertical(boleto->dataproc, 		10, 	120, 	571, FALSE);
	ImprimeVertical(boleto->sacado, 		50, 	200, 	852, FALSE);
	ImprimeVertical(boleto->sacadoend, 		50, 	220, 	852, FALSE);
	ImprimeVertical(boleto->sacadocep, 		50, 	240, 	852, FALSE);
	ImprimeVertical(agencia_conta, 			16, 	82, 	211, FALSE);
	ImprimeVertical(boleto->nossonumero, 	16, 	120, 	211, FALSE);
	ImprimeVertical(boleto->valor, 			15, 	162, 	211, FALSE);
	ImprimeVertical(boleto->instrucao1, 	70, 	200, 	1550, FALSE);
	ImprimeVertical(boleto->instrucao2, 	70, 	220, 	1550, FALSE);
	ImprimeVertical(boleto->instrucao3, 	70, 	240, 	1550, FALSE);

	//imprime código de barras
	ImprimeCodBarras25(codbarras, 44, 272, 1560);

	PaintPrinterGraphics();

	pprintf ("\n\n\n\n\n\n\n");
	ttestall(PRINTER, 0);

	//tratamento de falta de papel ou erro da impressora
	status(hPrn, &Status);

	if ((Status & (PRINT_ERROR | PAPER_OUT)) != 0)
	{
		if ((Status & PAPER_OUT) != 0)
		{
			DisplaySimpleMessage ("Favor colocar papel", TRUE, TRUE);

			while ((Status & PAPER_OUT) != 0)
			{
				ttestall(0, 100);
				status(hPrn, &Status);
			}

			PressioneTecla(0);

			goto lblInicioImpressao;
		}

		if ((Status & PRINT_ERROR) != 0)
		{
			DisplaySimpleMessage ("Falha na impressora", TRUE, TRUE);
		}

		ret = FALSE;
	}
	fclose(hPrn);
	goto lblEnd;
lblErr:
	DisplaySimpleMessage ("Erro na impressao", TRUE, TRUE);
	ret = FALSE;
	goto lblEnd;
lblEnd:
	return ret;
}

void ImprimeBMP()
{
	doubleword uiMode;
	S_FS_FILE *pxFile=NULL;
	unsigned char *pcBMP;
	int iLenData;

	FS_mount ("/HOST", &uiMode);
	pxFile = FS_open ("/HOST/OPUSLOGO.BMP", "r");
	iLenData = FS_length(pxFile);

	pcBMP = umalloc(iLenData);

	FS_read(pcBMP, 1, iLenData, pxFile);
	FS_unmount("/HOST");

	//--

	FILE *hPrn;
	hPrn = fopen("PRINTER","w");
	int ret;

	InitPrinterBitmap(50);

	ret = _PrintBmpXY(0, 0, (byte*) pcBMP, _OFF_);

	PaintPrinterGraphics();

	pprintf ("\n\n\n\n\n\n\n");
	ttestall(0, 5*100);

	fclose(hPrn);

	ufree(pcBMP);
}

/*
 * Baixa os dados dos boletos pendentes e os imprime
 * retorna 0 se OK, ou -1 para Erro de conexão
 * tipo (entrada): "AUTO" (impressão automática) ou "MANUAL" (impressão manual)
 */
int ImprimeBoletos(char *tipo)
{
	int linha = 0;
	int ret = 0;
	int i, j;

	char msg[50];
	char buffer[2048];
	char bufimp[10];
	char recstr[50][500];	//máx. 50 boletos
	char *token;

	Tboleto dadosboleto;

	memset(msg, 0, sizeof(msg));
	memset(buffer, 0, sizeof(buffer));

	sprintf(msg, "4636boletos|%s|%s", config.cfCod, tipo);

	i = _EnviaSocket(msg, strlen(msg), buffer, sizeof(buffer));

	if (i > 0)
	{
		memset(recstr, 0 , sizeof(recstr));

		//separa o buffer em linhas
		token = strtok(buffer, "\r\n");
		while (token != NULL)
		{
			memset(recstr[linha], 0, sizeof(recstr[linha]));
			strcpy(recstr[linha], token);
			linha++;

			token = strtok(NULL, "\r\n");
		}

		//separa as linhas em campos
		for (j = 0; j < linha; j++)
		{
			memset(&dadosboleto, 0, sizeof dadosboleto);
			token = strtok(recstr[j], "|");

			if (token != NULL)
			{
				strcpy(dadosboleto.nomebanco, token);
				token = strtok(NULL, "|");

				if (token != NULL)
				{
					strcpy(dadosboleto.codbanco, token);
					token = strtok(NULL, "|");

					if (token != NULL)
					{
						strcpy(dadosboleto.linhadigitavel, token);
						token = strtok(NULL, "|");

						if (token != NULL)
						{
							strcpy(dadosboleto.cedente, token);
							token = strtok(NULL, "|");

							if (token != NULL)
							{
								strcpy(dadosboleto.sacado, token);
								token = strtok(NULL, "|");

								if (token != NULL)
								{
									strcpy(dadosboleto.sacadoend, token);
									token = strtok(NULL, "|");

									if (token != NULL)
									{
										strcpy(dadosboleto.sacadocep, token);
										token = strtok(NULL, "|");

										if (token != NULL)
										{
											strcpy(dadosboleto.agencia, token);
											token = strtok(NULL, "|");

											if (token != NULL)
											{
												strcpy(dadosboleto.conta, token);
												token = strtok(NULL, "|");

												if (token != NULL)
												{
													strcpy(dadosboleto.carteira, token);
													token = strtok(NULL, "|");

													if (token != NULL)
													{
														strcpy(dadosboleto.documento, token);
														token = strtok(NULL, "|");

														if (token != NULL)
														{
															strcpy(dadosboleto.nossonumero, token);
															token = strtok(NULL, "|");

															if (token != NULL)
															{
																strcpy(dadosboleto.datavenc, token);
																token = strtok(NULL, "|");

																if (token != NULL)
																{
																	strcpy(dadosboleto.dataproc, token);
																	token = strtok(NULL, "|");

																	if (token != NULL)
																	{
																		strcpy(dadosboleto.valor, token);
																		token = strtok(NULL, "|");

																		if (token != NULL)
																		{
																			strcpy(dadosboleto.instrucao1, token);
																			token = strtok(NULL, "|");

																			if (token != NULL)
																			{
																				strcpy(dadosboleto.instrucao2, token);
																				token = strtok(NULL, "|");

																				if (token != NULL)
																				{
																					strcpy(dadosboleto.instrucao3, token);
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

			//imprimir o boleto e envia confirmação da operação para o servidor
			if (ImprimeBoleto(&dadosboleto))
			{
				memset(msg, 0, sizeof(msg));
				memset(bufimp, 0, sizeof(bufimp));

				sprintf(msg, "4636impresso|%s|%s|%s", config.cfCod, dadosboleto.documento, tipo);

				i = _EnviaSocket(msg, strlen(msg), bufimp, sizeof(bufimp));

				if (i > 0)
				{
					if (strstr(bufimp, "OK") == NULL)
						ret = -1;
				}
				else
					ret = -1;
			}

			if (j < linha - 1)
				PressioneTecla(0);
		}
	}
	else
		ret = -1;

	return ret;
}
