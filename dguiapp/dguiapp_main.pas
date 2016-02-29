unit dguiapp_main;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Memo1: TMemo;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
  protected
    procedure Log(const msg: string);
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

function AttachConsole(dwProcessID: Integer): Boolean; stdcall; external 'kernel32.dll';

const
  ATTACH_PARENT_PROCESS = -1;

procedure TForm1.Log(const msg: string);
begin
  Memo1.Lines.Add(msg);
end;

procedure TForm1.Button3Click(Sender: TObject);
var hStdIn, hStdOut, hStdErr: THandle;
begin
  hStdIn := GetStdHandle(STD_INPUT_HANDLE);
  hStdOut := GetStdHandle(STD_OUTPUT_HANDLE);
  hStdErr := GetStdHandle(STD_ERROR_HANDLE);
  Log('In='+IntToStr(hStdIn)+', out='+IntToStr(hStdOut)+', err='+IntToStr(hStdErr));
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
  if not AttachConsole(ATTACH_PARENT_PROCESS) then
    RaiseLastOsError();
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  Log('Handle: '+IntToStr(TTextRec(Output).Handle));
  writeln('Yo dawg');
  Log('Handle: '+IntToStr(TTextRec(Output).Handle));
end;

procedure TForm1.Button5Click(Sender: TObject);
var hStdOut: THandle;
  str: string;
  written: cardinal;
begin
  str := 'Yo dawg';
  hStdOut := GetStdHandle(STD_OUTPUT_HANDLE);
  if not WriteFile(hStdOut, str[1], Length(str)*SizeOf(char), written, nil) then
    Log('Failed: error '+IntToStr(GetLastError));
end;

procedure TForm1.Button4Click(Sender: TObject);
var s: string;
begin
  readln(s);
  Log('Read: '+s);
end;

end.
