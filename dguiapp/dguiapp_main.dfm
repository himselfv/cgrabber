object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 329
  ClientWidth = 248
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    248
    329)
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 8
    Top = 70
    Width = 232
    Height = 25
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Print to console'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 8
    Top = 39
    Width = 232
    Height = 25
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Find and attach console'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Memo1: TMemo
    Left = 8
    Top = 163
    Width = 232
    Height = 158
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 2
  end
  object Button3: TButton
    Left = 8
    Top = 8
    Width = 232
    Height = 25
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Print handles'
    TabOrder = 3
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 8
    Top = 132
    Width = 232
    Height = 25
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Read from console'
    TabOrder = 4
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 8
    Top = 101
    Width = 232
    Height = 25
    Anchors = [akLeft, akTop, akRight]
    Caption = 'WriteFile to console'
    TabOrder = 5
    OnClick = Button5Click
  end
end
