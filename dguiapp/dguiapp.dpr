program dguiapp;

uses
  Vcl.Forms,
  dguiapp_main in 'dguiapp_main.pas' {Form1};

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
