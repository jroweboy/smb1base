object FormSelect2subpalSets: TFormSelect2subpalSets
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Select'
  ClientHeight = 122
  ClientWidth = 120
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMode = pmAuto
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btnOK: TButton
    Left = 63
    Top = 98
    Width = 56
    Height = 22
    Caption = 'OK'
    TabOrder = 0
    OnClick = btnOKClick
  end
  object BtnCancel: TButton
    Left = 4
    Top = 98
    Width = 56
    Height = 22
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = BtnCancelClick
  end
  object GroupBox1: TGroupBox
    Left = 3
    Top = 3
    Width = 116
    Height = 46
    Caption = '1st set (ppu bg)'
    TabOrder = 2
    object btn1a: TSpeedButton
      Left = 5
      Top = 17
      Width = 23
      Height = 22
      GroupIndex = 1
      Down = True
      Caption = 'A'
    end
    object btn1b: TSpeedButton
      Tag = 1
      Left = 33
      Top = 17
      Width = 23
      Height = 22
      GroupIndex = 1
      Caption = 'B'
    end
    object btn1c: TSpeedButton
      Tag = 2
      Left = 60
      Top = 17
      Width = 23
      Height = 22
      GroupIndex = 3
      Caption = 'C'
    end
    object btn1d: TSpeedButton
      Tag = 3
      Left = 87
      Top = 17
      Width = 23
      Height = 22
      GroupIndex = 4
      Caption = 'D'
    end
  end
  object GroupBox2: TGroupBox
    Left = 3
    Top = 48
    Width = 116
    Height = 46
    Caption = '2nd set (ppu sprites)'
    TabOrder = 3
    object btn2a: TSpeedButton
      Left = 5
      Top = 17
      Width = 23
      Height = 22
      GroupIndex = 1
      Caption = 'A'
    end
    object btn2b: TSpeedButton
      Tag = 1
      Left = 33
      Top = 17
      Width = 23
      Height = 22
      GroupIndex = 1
      Down = True
      Caption = 'B'
    end
    object btn2c: TSpeedButton
      Tag = 2
      Left = 60
      Top = 17
      Width = 23
      Height = 22
      GroupIndex = 3
      Caption = 'C'
    end
    object btn2d: TSpeedButton
      Tag = 3
      Left = 87
      Top = 17
      Width = 23
      Height = 22
      GroupIndex = 4
      Caption = 'D'
    end
  end
end
