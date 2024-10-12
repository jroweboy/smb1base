object FormWarning: TFormWarning
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Warning'
  ClientHeight = 211
  ClientWidth = 394
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 3
    Top = 0
    Width = 389
    Height = 182
    TabOrder = 0
    object StaticText1: TStaticText
      Left = 5
      Top = 10
      Width = 378
      Height = 170
      AutoSize = False
      Caption = 'StaticText1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -10
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object CheckBox1: TCheckBox
    Left = 8
    Top = 190
    Width = 103
    Height = 17
    Caption = 'Don'#39't show again'
    TabOrder = 1
  end
  object Button1: TButton
    Left = 346
    Top = 186
    Width = 45
    Height = 22
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 2
  end
  object Button2: TButton
    Left = 298
    Top = 186
    Width = 45
    Height = 22
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
    WordWrap = True
  end
end
