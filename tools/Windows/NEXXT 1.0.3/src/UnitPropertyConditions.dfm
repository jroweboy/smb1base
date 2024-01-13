object FormPropConditions: TFormPropConditions
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Conditions'
  ClientHeight = 135
  ClientWidth = 93
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 3
    Top = 0
    Width = 88
    Height = 85
    Caption = 'Cond.'
    TabOrder = 0
    object chk0: TCheckBox
      Left = 3
      Top = 13
      Width = 73
      Height = 17
      TabStop = False
      Caption = 'Subpal 0'
      TabOrder = 0
      OnClick = chk0Click
      OnMouseEnter = chk0MouseEnter
      OnMouseLeave = chkMapMouseLeave
    end
    object chk1: TCheckBox
      Tag = 1
      Left = 3
      Top = 29
      Width = 73
      Height = 17
      TabStop = False
      Caption = 'Subpal 1'
      TabOrder = 1
      OnClick = chk0Click
      OnMouseEnter = chk0MouseEnter
      OnMouseLeave = chkMapMouseLeave
    end
    object chk2: TCheckBox
      Tag = 2
      Left = 3
      Top = 45
      Width = 73
      Height = 17
      TabStop = False
      Caption = 'Subpal 2'
      TabOrder = 2
      OnClick = chk0Click
      OnMouseEnter = chk0MouseEnter
      OnMouseLeave = chkMapMouseLeave
    end
    object chk3: TCheckBox
      Tag = 3
      Left = 3
      Top = 61
      Width = 73
      Height = 17
      TabStop = False
      Caption = 'Subpal 3'
      TabOrder = 3
      OnClick = chk0Click
      OnMouseEnter = chk0MouseEnter
      OnMouseLeave = chkMapMouseLeave
    end
  end
  object GroupBox2: TGroupBox
    Left = 3
    Top = 84
    Width = 88
    Height = 49
    Caption = 'Show on'
    TabOrder = 1
    object chkTiles: TCheckBox
      Left = 3
      Top = 14
      Width = 73
      Height = 17
      TabStop = False
      Caption = 'Tileset'
      TabOrder = 0
      OnClick = chkTilesClick
      OnMouseEnter = chkTilesMouseEnter
      OnMouseLeave = chkMapMouseLeave
    end
    object chkMap: TCheckBox
      Left = 3
      Top = 29
      Width = 80
      Height = 17
      TabStop = False
      Caption = 'Screen/map'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = chkMapClick
      OnMouseEnter = chkMapMouseEnter
      OnMouseLeave = chkMapMouseLeave
    end
  end
end
