#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#define IDC_MAIN_BUTTON	101			// Button identifier
#define IDC_MAIN_EDIT	102			// Edit box identifier

int inhash(char inp[]){ /* generates hash value based on input password */
    int hashed1=0;
    int i,leng=strlen(inp);
    for(i=0;i<leng&&inp[i]!='\n';i++)
        hashed1=(hashed1+8976*(int)inp[i])%20380739;
    return(hashed1%1000);
}
int get_file_size(char  filename[]) /* path to file , returns the number of character's in the text file */
{
    FILE *p_file = NULL;
    p_file = fopen(filename,"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}

int endec(char filename[],char psdw[])/*to encode or  decode the file based on password*/
{
    FILE *f;
     int hashed=0,ed=0;
     int inphash=inhash(psdw);//generates hash based on given password
     int savd=inphash;
    f=fopen(filename,"rb");/*used rb because file has to be read in binary because XOR'ed file may contain EOF in between which may cause unexpected error like termination in between */
    if(f==NULL)
        return 202;//202 for read faliure

    char read=fgetc(f);
    if(read=='~')//file has header ~? if it is encrypted otherwise not
    {
        read=fgetc(f);
        if(read=='?')
        { ed=1;
         fscanf(f,"%d\n",&hashed);
         if(hashed!=inphash)
            {printf("wrong pswd");
             fclose(f);
             return 404;//returns 404 in case of wrong password ie hash mismatch
            }
        }
    }
    if(ed==0)
    {fclose(f);
     f=fopen(filename,"rb");
    }


    char temps[999999];
    long int crs=0,j=0;
    int filesz=get_file_size(filename);
    if(ed==1)
        filesz=filesz-4;
        while(1){

        inphash=inphash+2;
        char ca=(char)inphash;
        char ca2=fgetc(f);
        printf("%c\n",ca2);
        if(crs>=filesz)
            break;
        temps[crs]= ca2 ^ ca;//XORing
        crs++;

            }
    fclose(f);

    FILE *fp;
    fp=fopen(filename,"wb");
    if(ed==0)
    {
        fprintf(fp,"~?%d\n",savd);
        fprintf(fp,"");
    }
    for(j=0;j<crs;j++){
            putc (temps[j] , fp);
    printf("%c",temps[j]);
    }
    fclose(fp);
    return (ed==0?20:10);
}


LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
HWND hEdit;
char szFileName[1000];

LRESULT CALLBACK WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int )
{

	WNDCLASSEX wClass;
	ZeroMemory(&wClass,sizeof(WNDCLASSEX));
	wClass.cbClsExtra=NULL;
	wClass.cbSize=sizeof(WNDCLASSEX);
	wClass.cbWndExtra=NULL;
	wClass.hbrBackground=(HBRUSH)COLOR_WINDOW;
	wClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wClass.hIcon=LoadIcon(NULL,IDI_WINLOGO);

	wClass.hIconSm=LoadIcon (NULL, IDI_WINLOGO);
	wClass.hInstance=hInst;
	   wClass.style = CS_DBLCLKS;
	wClass.lpfnWndProc=(WNDPROC)WinProc;
	wClass.lpszClassName="Window Class";
	wClass.lpszMenuName=NULL;
	wClass.style=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;


	if(!RegisterClassEx(&wClass))
	{
		int nResult=GetLastError();
		MessageBox(NULL,
			"Window class creation failed\r\n",
			"Window Class Failed",
			MB_ICONERROR);
	}

	HWND hWnd=CreateWindowEx(NULL,
			"Window Class",
			"INPUT PASSWORD",
			WS_OVERLAPPEDWINDOW,
            500,
			200,
			280,
			280,
			HWND_DESKTOP,
			NULL,
			hInst,
			NULL);

	if(!hWnd)
	{
		int nResult=GetLastError();

		MessageBox(NULL,
			"Window creation failed\r\n",
			"Window Creation Failed",
			MB_ICONERROR);
	}
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";
    int msgboxID=MessageBox(NULL, "                 Welcome to XOR encryption \n click ok to select a file to encrypt or decrypt \n NOTE : this is for recreational purposes only ", TEXT("XOR ENCRYPTOR"), MB_OKCANCEL | MB_DEFBUTTON2);
    switch(msgboxID){
    case IDCANCEL:{
    PostQuitMessage(0);
    return 0;
    exit(1);}
    }
    GetOpenFileName(&ofn);
    ShowWindow(hWnd,1);
    UpdateWindow(hWnd);
	MSG msg;
	ZeroMemory(&msg,sizeof(MSG));
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WinProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
		{
			   hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD|WS_VISIBLE|
				ES_CENTER|ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_CENTER,
				50,
				100,
				150,
				30,
				hWnd,
				(HMENU)IDC_MAIN_EDIT,
				GetModuleHandle(NULL),
				NULL);
			HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEdit,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
			SendMessage(hEdit,
				WM_SETTEXT,
				NULL,
				(LPARAM)"ENTER PASSWORD HERE");

			// Create a push button
			HWND hWndButton=CreateWindowEx(NULL,
				"BUTTON",
				"OK",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				70,
				150,
				100,
				24,
				hWnd,
				(HMENU)IDC_MAIN_BUTTON,
				GetModuleHandle(NULL),
				NULL);
			SendMessage(hWndButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
		}
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
            {
				case IDC_MAIN_BUTTON:
				{
					char buffer[256];
					SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
                        int q=endec(szFileName,buffer);
                        if(q!=404)
                        {
                            if(q==202){
                            MessageBox(NULL, "Unable to Open File ", TEXT("XOR ENCRYPTOR :by Randomizer "), MB_ICONERROR);
                            PostQuitMessage(0);
                            }
                            if(q==20)
                            MessageBox(NULL, "Your file has been encrypted", TEXT("XOR ENCRYPTOR "), MB_OK);
                            if(q==10){
                            MessageBox(NULL, "       Your file has been Decrypted\n thanks for using XOR ENCRYPTOR ", TEXT("XOR ENCRYPTOR: by Randomizer"), MB_OK);

                            }

                            PostQuitMessage(0);

                        }
                        MessageBox(NULL, "           The password You entered is invalid \n please try again with a correct password or try restarting the application", TEXT("XOR ENCRYPTOR"), MB_ICONINFORMATION);

				}
				break;
			}
			break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd,msg,wParam,lParam);
}
