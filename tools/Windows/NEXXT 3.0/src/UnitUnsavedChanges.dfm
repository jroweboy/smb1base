object FormUnsavedChanges: TFormUnsavedChanges
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'NEXXT: Unsaved changes!'
  ClientHeight = 103
  ClientWidth = 236
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object StaticText1: TStaticText
    Left = 11
    Top = 24
    Width = 216
    Height = 23
    Caption = 'Save changes before quitting?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object Button1: TButton
    Left = 11
    Top = 72
    Width = 60
    Height = 25
    Caption = '&Save'
    ModalResult = 6
    TabOrder = 2
  end
  object Button2: TButton
    Left = 87
    Top = 72
    Width = 60
    Height = 25
    Caption = '&Discard'
    ModalResult = 7
    TabOrder = 3
  end
  object Button3: TButton
    Left = 164
    Top = 72
    Width = 60
    Height = 25
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 1
  end
end
